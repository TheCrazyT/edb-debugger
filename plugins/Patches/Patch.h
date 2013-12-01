#ifndef PATCH_H
#define PATCH_H

#include "IPatch.h"
class Patch : public IPatch
{
public:
    Patch(bool active,edb::address_t address,const void * orgBytes,const void * bytes,std::size_t len);
    ~Patch();

    bool isActive() const;
    edb::address_t getAddress() const;
    const void * getBytes() const;
    const void * getOrigBytes() const;
    std::size_t getSize() const;
    void setSize(std::size_t value);
    void enable();
    void disable();

private:
    bool active_;
    edb::address_t address_;
    const void * bytes_;
    const void * origBytes_;
    std::size_t len_;
};

#endif // PATCH_H
