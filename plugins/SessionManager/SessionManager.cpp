/*
Copyright (C) 2006 - 2013 Evan Teran
                          eteran@alum.rit.edu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "IPluginSession.h"
#include "SessionManager.h"
#include "edb.h"
#include "SessionObject.h"
#include "IRegion.h"
#include <QtDebug>
#include <QFile>
#include <QDataStream>


//------------------------------------------------------------------------------
// Name: private_init
// Desc:
//------------------------------------------------------------------------------
void SessionManager::private_init() {
	edb::v1::set_session_file_handler(this);
}

//------------------------------------------------------------------------------
// Name: save_session
// Desc:
//------------------------------------------------------------------------------
void SessionManager::save_session(const QString &filename, const QString &executable) {
	qDebug() << "[SessionManager] saving session file:" << filename <<  "for:" << executable;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    QDataStream stream(&file);
    edb::v1::memory_regions() >> stream;
    QHash<QString, QObject *> plugins = edb::v1::plugin_list();
	for(QHash<QString, QObject *>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
		QObject *const o = it.value();
        if(IPluginSession* p = qobject_cast<IPluginSession *>(o)) {
            if(p != 0){
                p->serializeSessionObject(&stream);
            }
        }
	}

    file.close();
}

//------------------------------------------------------------------------------
// Name: load_session
// Desc:
//------------------------------------------------------------------------------
void SessionManager::load_session(const QString &filename, const QString &executable) {
	qDebug() << "[SessionManager] loading session file:" << filename <<  "for:" << executable;
	
    QFile file(filename);

    if(!file.exists()){
        return;
    }
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    storedRegions_ << stream;
    QHash<QString, QObject *> plugins = edb::v1::plugin_list();
    SessionObject* sessObj = new SessionObject();

    while (!file.atEnd()) {
        qint64 pos = stream.device()->pos();
        *sessObj << stream;
        for(QHash<QString, QObject *>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
            IPluginSession* const p = qobject_cast<IPluginSession *>(it.value());
            if(p != 0) {
                    if(p->getSessionIdentifier()->compare(sessObj->getIdentifier()) == 0) {
                        stream.device()->seek(pos);
                        p->deserializeSessionObject(&stream);
                    }
            }
        }
    }
    file.close();
}

//------------------------------------------------------------------------------
// Name: menu
// Desc:
//------------------------------------------------------------------------------
QMenu *SessionManager::menu(QWidget *parent) {
	Q_UNUSED(parent);
	return 0;
}

//------------------------------------------------------------------------------
// Name: updateAddress
// Desc:
//------------------------------------------------------------------------------
quint64 SessionManager::updateAddress(quint64 addr) {
    IRegion::pointer region = storedRegions_.find_region(addr);
    if(region != NULL){
        addr = addr - region->start();
        Q_FOREACH(const IRegion::pointer &r, edb::v1::memory_regions().regions()) {
            if(r->name().compare(region->name()) == 0){
                addr = addr + region->start();
                return addr;
            }
        }
    }
    return addr;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(SessionManager, SessionManager)
#endif
