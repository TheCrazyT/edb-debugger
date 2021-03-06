#ifndef PATCH_H
#define PATCH_H

#include "IPatch.h"
class Patch : public IPatch
{
public:
    Patch(bool active,edb::address_t address,void * orgBytes,void * bytes,unsigned int len);
    ~Patch();

    bool isActive() const;
    edb::address_t getAddress() const;
    const void * getBytes() const;
    const void * getOrigBytes() const;
    unsigned int getSize() const;
    void setSize(unsigned int value);
    void enable();
    void disable();

private:
    bool active_;
    edb::address_t address_;
    quint8 * bytes_;
    quint8 * origBytes_;
    unsigned int len_;
};

#endif // PATCH_H
