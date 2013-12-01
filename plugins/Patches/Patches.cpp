#include "Patches.h"
#include "Patch.h"
#include "DialogPatches.h"
#include "edb.h"
#include <QMenu>

Patches::Patches() : menu_(0),dialog_(0) {
}
Patches::~Patches(){
    delete dialog_;
}

//------------------------------------------------------------------------------
// Name: private_init
// Desc:
//------------------------------------------------------------------------------
void Patches::private_init() {
    edb::v1::set_patches(this);
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
// Name: createPatch
// Desc:
//------------------------------------------------------------------------------
IPatch::pointer Patches::create_patch(edb::address_t address, const void *buf, std::size_t len) const{
    return IPatch::pointer(new Patch(true,address,buf,len));
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
