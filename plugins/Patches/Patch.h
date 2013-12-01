#ifndef PATCH_H
#define PATCH_H

#include "IPatch.h"
class Patch : public IPatch
{
public:
    Patch();
    Patch(bool active,edb::address_t address,const void * bytes,std::size_t len);
    ~Patch();

    bool isActive() const;
    edb::address_t getAddress() const;
    const void * getBytes() const;
    std::size_t getSize() const;

private:
    bool active_;
    edb::address_t address_;
    const void * bytes_;
    std::size_t len_;
};

#endif // PATCH_H
