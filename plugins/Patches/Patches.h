#ifndef PATCHPLUGIN_H
#define PATCHPLUGIN_H


#include "IPlugin.h"

class QMenu;
class QDialog;
class Patches : public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "edb.IPlugin/1.0")
#endif
    Q_CLASSINFO("author", "CrazyT")
    Q_CLASSINFO("url", "")


public:
    Patches();
    virtual ~Patches();

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
