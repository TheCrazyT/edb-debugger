#include "Patches.h"
#include "DialogPatches.h"
#include "edb.h"
#include <QMenu>

Patches::Patches() : menu_(0),dialog_(0) {
}
Patches::~Patches(){
    delete dialog_;
}


//------------------------------------------------------------------------------
// Name: menu
// Desc:
//------------------------------------------------------------------------------
QMenu *Patches::menu(QWidget *parent) {

    Q_ASSERT(parent);

    if(!menu_) {
        menu_ = new QMenu(tr("Patches"), parent);
        menu_->addAction (tr("&Patches"), this, SLOT(show_menu()), QKeySequence(tr("Ctrl+P")));
    }

    return menu_;
}
//------------------------------------------------------------------------------
// Name: show_menu
// Desc:
//------------------------------------------------------------------------------
void Patches::show_menu() {
    if(!dialog_) {
        dialog_ = new DialogPatches(edb::v1::debugger_ui);
    }

    dialog_->show();
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Patches, Patches)
#endif
