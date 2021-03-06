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

#ifndef SESSIONMANAGER_20090212_H_
#define SESSIONMANAGER_20090212_H_

#include "MemoryRegions.h"
#include "IPlugin.h"
#include "ISessionFile.h"

class QMenu;

class SessionManager : public QObject, public IPlugin, public ISessionFile {
	Q_OBJECT
	Q_INTERFACES(IPlugin)
#if QT_VERSION >= 0x050000
	Q_PLUGIN_METADATA(IID "edb.IPlugin/1.0")
#endif
	Q_CLASSINFO("author", "Evan Teran")
	Q_CLASSINFO("url", "http://www.codef00.com")

public:
	virtual void private_init();

public:
	virtual void save_session(const QString &filename, const QString &executable);
	virtual void load_session(const QString &filename, const QString &executable);
    virtual quint64 updateAddress(quint64 addr);

public:
	virtual QMenu *menu(QWidget *parent = 0);
private:
    MemoryRegions storedRegions_;
};

#endif
