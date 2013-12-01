#include "Patch.h"
#include "edb.h"
#include "IDebuggerCore.h"

Patch::Patch(bool active,edb::address_t address,const void * origBytes,const void * bytes,std::size_t len)
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
std::size_t Patch::getSize() const{
    return len_;
}
void Patch::setSize(std::size_t value){
    len_ = value;
}
void Patch::enable(){
    active_ = true;
    edb::v1::debugger_core->write_bytes(address_,bytes_,len_);
}
void Patch::disable(){
    active_ = false;
    edb::v1::debugger_core->write_bytes(address_,origBytes_,len_);
}
