#include "Patch.h"
#include "edb.h"
#include "IDebuggerCore.h"
#include<QDebug>

Patch::Patch(bool active,edb::address_t address,void * origBytes,void * bytes,unsigned int len)
{
    bytes_ = new quint8[len];
    origBytes_ = new quint8[len];
    memcpy(bytes_,bytes,len);
    memcpy(origBytes_,origBytes,len);

    active_ = active;
    len_ = len;
    address_ = address;

    if(len_>=2){
        qDebug("%x %x %x",bytes_,bytes_[0],bytes_[1]);
        qDebug("%x %x %x",origBytes_,origBytes_[0],origBytes_[1]);
    }
}

Patch::~Patch() {
    delete bytes_;
    delete origBytes_;
}

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
    if(len_>=2){
        qDebug("E %x %x %x",bytes_,bytes_[0],bytes_[1]);
        qDebug("E %x %x %x",origBytes_,origBytes_[0],origBytes_[1]);
    }
}
void Patch::disable(){
    active_ = false;
    QByteArray bytes = QByteArray((char*)origBytes_,len_);
    edb::v1::modify_bytes(address_,len_,bytes,0);
    if(len_>=2){
        qDebug("D %x %x %x",bytes_,bytes_[0],bytes_[1]);
        qDebug("D %x %x %x",origBytes_,origBytes_[0],origBytes_[1]);
    }
}
