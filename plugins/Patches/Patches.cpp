#include "Patches.h"
#include "Patch.h"
#include "DialogPatches.h"
#include "IDebuggerCore.h"
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
IPatch::pointer Patches::create_patch(edb::address_t address,const void *orgBuf, const void *buf, std::size_t len) const{
    IDebuggerCore::PatchList patches = edb::v1::debugger_core->get_code_patches();

    Q_FOREACH(const IPatch::pointer &p, patches) {
        QSharedPointer<Patch> patch = p.staticCast<Patch>();

        if((address>=patch->getAddress())&&(address<patch->getAddress()+patch->getSize())){
            if(patch->getSize()<len){
                patch->setSize(len);
            }


            quint8 * oldPatchBuf = (quint8 *)patch->getBytes();
            quint8 * newPatchBuf = new quint8[patch->getSize()];
            quint8 * bufp = (quint8 *)buf;
            for(uint i=0;i<patch->getSize();i++){
                if((patch->getAddress()+i>=address)&&(patch->getAddress()+i<address+len)){
                    *newPatchBuf = *bufp;
                    bufp++;
                }else{
                    *newPatchBuf = *oldPatchBuf;
                    oldPatchBuf++;
                }
                newPatchBuf++;
            }

            patches.remove(patch->getAddress());

            return IPatch::pointer(patch);
        }
    }

    return IPatch::pointer(new Patch(true,address,orgBuf,buf,len));
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
