#ifndef BASICREGION_H
#define BASICREGION_H

#include "IRegion.h"

class BasicRegion : public IRegion
{
public:
    BasicRegion();

public:
    typedef QSharedPointer<IRegion> pointer;
    typedef quint32                 permissions_t;

public:
    virtual ~BasicRegion();

public:
    virtual IRegion *clone() const;

public:
    virtual bool accessible() const;
    virtual bool readable() const;
    virtual bool writable() const;
    virtual bool executable() const;
    virtual edb::address_t size() const;

public:
    virtual void set_permissions(bool read, bool write, bool execute) ;
    virtual void set_start(edb::address_t address);
    virtual void set_end(edb::address_t address);
    virtual void set_base(edb::address_t address);
    virtual void set_name(QString name);

public:
    virtual edb::address_t start() const;
    virtual edb::address_t end() const; // NOTE: is the address of one past the last byte of the region
    virtual edb::address_t base() const;
    virtual QString name() const;
    virtual permissions_t permissions() const;

private:
    edb::address_t start_;
    edb::address_t end_;
    edb::address_t base_;
    QString name_;
};

#endif // BASICREGION_H
