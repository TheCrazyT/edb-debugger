#ifndef PATCHPLUGIN_H
#define PATCHPLUGIN_H


#include "IPlugin.h"
#include "IPatches.h"
#include "IPatch.h"
#include "Symbol.h"
#include "Types.h"

class QMenu;
class QDialog;
class Patches : public QObject, public IPatches, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "edb.IPlugin/1.0")
#endif
    Q_CLASSINFO("author", "CrazyT")
    Q_CLASSINFO("url", "")

private:
    virtual void private_init();

public:
    virtual ~Patches();
    Patches();

public:
    virtual IPatch::pointer create_patch(edb::address_t address, const void *orgBuf,const void *buf, std::size_t len) const;
public:
    virtual QMenu *menu(QWidget *parent = 0);

public Q_SLOTS:
    void show_menu();
    void show_dialog();

private:
    QDialog *dialog_;
    QMenu * menu_;

signals:
    
public slots:
    
};
#endif // PATCHPLUGIN_H