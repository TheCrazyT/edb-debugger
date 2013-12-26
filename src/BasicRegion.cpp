#include "BasicRegion.h"
BasicRegion::BasicRegion()
{
}

BasicRegion::~BasicRegion()
{
}

IRegion* BasicRegion::clone() const{

}

bool BasicRegion::accessible() const{
    return false;
}
bool BasicRegion::readable() const{
    return false;
}
bool BasicRegion::writable() const{
    return false;
}
bool BasicRegion::executable() const{
    return false;
}
edb::address_t BasicRegion::size() const{
    return 0;
}

void BasicRegion::set_permissions(bool read, bool write, bool execute) {}
void BasicRegion::set_start(edb::address_t address){
    start_ = address;
}
void BasicRegion::set_end(edb::address_t address){
    end_ = address;
}
void BasicRegion::set_base(edb::address_t address){
    base_ = address;
}
void BasicRegion::set_name(QString name){
    name_ = name;
}

edb::address_t BasicRegion::start() const{
    return start_;
}
edb::address_t BasicRegion::end() const{
    return end_;
}
edb::address_t BasicRegion::base() const{
    return base_;
}
QString BasicRegion::name() const{
    return name_;
}
IRegion::permissions_t BasicRegion::permissions() const{
}
