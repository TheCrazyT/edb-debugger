#include "Patch.h"
Patch::Patch(){}

Patch::Patch(bool active,edb::address_t address,const void * bytes,std::size_t len)
{
    active_ = active;
    bytes_ = bytes;
    len_ = len;
    address_ = address;
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
std::size_t Patch::getSize() const{
    return len_;
}
