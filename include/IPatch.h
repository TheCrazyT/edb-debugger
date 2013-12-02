#ifndef IPATCH_H
#define IPATCH_H

#include "Types.h"
#include <QList>
#include <QSharedPointer>
#include<IRegion.h>

class IPatch{
public:
    typedef QSharedPointer<IPatch> pointer;

public:
    virtual ~IPatch() {}

public:
    virtual bool isActive() const = 0;
    virtual edb::address_t getAddress() const = 0;
    virtual const void *  getBytes() const = 0;
    virtual unsigned int getSize() const = 0;
};

#endif // IPATCH_H
