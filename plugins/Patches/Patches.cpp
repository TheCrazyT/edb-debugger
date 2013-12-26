#include "Patches.h"
#include "Patch.h"
#include "DialogPatches.h"
#include "IDebuggerCore.h"
#include "SessionObjectWriter.h"
#include "edb.h"
#include <QMenu>
#include <QList>
#include <QVariant>
#include <QDataStream>

typedef QList<QVariant> QVarList;


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

QString* Patches::getSessionIdentifier() const{
    QString* result = new QString("PatchesV1");
    return result;
}

void Patches::serializeSessionObject(QDataStream* stream) const{
    IDebuggerCore::PatchList patches = edb::v1::debugger_core->get_code_patches();
    //SessionObjectWriter<IDebuggerCore::PatchList>(getSessionIdentifier(),patches) >> *stream;
    QVarList list = QList<QVariant>();
    list.append(QVariant(patches.keys().size()));
    Q_FOREACH(edb::address_t addr,patches.keys()){
        list.append(QVariant(addr));
    }
    Q_FOREACH(IPatch::pointer patchPointer,patches.values()){
        Patch* patch = (Patch*)patchPointer.data();
        int size= patch->getSize();
        typedef const char BytesBuf[size];
        BytesBuf* bytes;
        BytesBuf* origBytes;
        bytes = (BytesBuf*)patch->getBytes();
        origBytes = (BytesBuf*)patch->getOrigBytes();
        list.append(QVariant(size));
        list.append(QByteArray(origBytes[0],size));
        list.append(QByteArray(bytes[0],size));
    }
    SessionObjectWriter<QVarList>(getSessionIdentifier(), list) >> *stream;

}
void Patches::deserializeSessionObject(QDataStream* stream) const{
    IDebuggerCore::PatchList patches = edb::v1::debugger_core->get_code_patches();

    SessionObjectWriter<QVarList>* obj = new SessionObjectWriter<QVarList>(getSessionIdentifier(),QVarList());
    *obj << *stream;

    patches.clear();

    QList<edb::address_t> addresses = QList<edb::address_t>();
    if(obj->getObjects().size()>0){
        int size =  obj->getObjects().at(0).value<int>();
        for(int i=0;i<size;i++){
            edb::address_t addr = obj->getObjects().at(i+1).value<edb::address_t>();
            addresses.append(addr);
        }
        int x=0;
        for(int i=size;i<size*4;i+=3){
            QVariant v = obj->getObjects().at(i+1);
            int size = v.value<int>();
            QByteArray origBytes = obj->getObjects().at(i+2).value<QByteArray>();
            QByteArray bytes = obj->getObjects().at(i+3).value<QByteArray>();
            edb::v1::debugger_core->create_patch(addresses.at(x),origBytes.data(),bytes.data(),size);
            x++;
        }

    }
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
