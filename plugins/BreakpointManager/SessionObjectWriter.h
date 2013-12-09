#ifndef SESSIONOBJECTWRITER_H
#define SESSIONOBJECTWRITER_H

#include "SessionObject.h"
#include "edb.h"

typedef QList<edb::address_t> AddrList;

EDB_EXPORT template<class T>
class SessionObjectWriter:public SessionObject
{
public:
    EDB_EXPORT SessionObjectWriter(QString* identifier, T objects);
    EDB_EXPORT virtual QDataStream& operator>>(QDataStream& ds);
    EDB_EXPORT virtual QDataStream& operator<<(QDataStream& ds);
    EDB_EXPORT virtual T getObjects();
private:
    T objects_;
};

#endif // SESSIONOBJECTWRITER_H
