#ifndef OBJECTEXTRACTOR_H
#define OBJECTEXTRACTOR_H

#include "DialogObjectExtractor.h"
#include "IPlugin.h"
#include "IPluginSession.h"
#include "edb.h"
#include <QMenu>
#include <QHash>
#include <QTreeWidgetItem>
#include <QDialog>

typedef QList<QVariant> QVarList;


class ObjectExtractor : public QObject, public IPlugin, public IPluginSession
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
    Q_INTERFACES(IPluginSession)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "edb.IPlugin/1.0")
#endif
    Q_CLASSINFO("author", "CrazyT")
    Q_CLASSINFO("url", "")

private:
    virtual void private_init();
    virtual void parseTree(QVarList* varList,QHash<qint32,void*>* subTree) const;
    virtual void deserializeTree(quint8* idx,QVarList* varList,QHash<qint32,void*>* subTree,QTreeWidgetItem* parent) const;

public:
    virtual ~ObjectExtractor();
    ObjectExtractor();

public:
    virtual QMenu* menu(QWidget *parent = 0);
    virtual QList<QAction *> data_context_menu();

public:
    virtual void serializeSessionObject(QDataStream* stream) const;
    virtual void deserializeSessionObject(QDataStream* stream) const;
    QString* getSessionIdentifier() const;

public Q_SLOTS:
    void show_menu();
    void show_menu(edb::address_t addr);
    void show_dialog();

private:
    DialogObjectExtractor* dialog_;
    QMenu* menu_;

signals:

public slots:

};

#endif // OBJECTEXTRACTOR_H
