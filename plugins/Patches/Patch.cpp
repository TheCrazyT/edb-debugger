#include "Patch.h"
#include "edb.h"
#include "IDebuggerCore.h"

Patch::Patch(bool active,edb::address_t address,const void * origBytes,const void * bytes,unsigned int len)
{
    active_ = active;
    bytes_ = bytes;
    len_ = len;
    address_ = address;
    origBytes_ = origBytes;
}

Patch::~Patch() {}

bool Patch::isActive() const{
    return active_;
}
edb::address_t Patch::getAddress() const{
    return address_;
}
const void * Patch::getBytes() const{
    return bytes_;
}
const void * Patch::getOrigBytes() const{
    return origBytes_;
}
unsigned int Patch::getSize() const{
    return len_;
}
void Patch::setSize(unsigned int value){
    len_ = value;
}
void Patch::enable(){
    active_ = true;
    QByteArray bytes = QByteArray((char*)bytes_,len_);
    edb::v1::modify_bytes(address_,len_,bytes,0);
}
void Patch::disable(){
    active_ = false;
    QByteArray bytes = QByteArray((char*)origBytes_,len_);
    edb::v1::modify_bytes(address_,len_,bytes,0);
}
