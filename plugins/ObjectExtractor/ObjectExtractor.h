#ifndef OBJECTEXTRACTOR_H
#define OBJECTEXTRACTOR_H

#include "IPlugin.h"


class QMenu;
class QDialog;
class ObjectExtractor : public QObject, public IPlugin
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
    virtual ~ObjectExtractor();
    ObjectExtractor();

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

#endif // OBJECTEXTRACTOR_H
