#ifndef SESSIONOBJECTWRITER_H
#define SESSIONOBJECTWRITER_H

#include "SessionObject.h"
#include "IDebuggerCore.h"
#include "edb.h"
#include <QList>
#include <QDebug>

typedef QList<edb::address_t> AddrList;

template<class T>
class SessionObjectWriter:public SessionObject
{
public:
    EDB_EXPORT SessionObjectWriter(QString* identifier, T objects){
        identifier_ = identifier;
        objects_ = objects;
    }
    EDB_EXPORT QDataStream& operator>>(QDataStream& ds){
        qDebug() << *identifier_;
        ds << *identifier_;
        ds << objects_;
    }
    EDB_EXPORT QDataStream& operator<<(QDataStream& ds){
        qDebug() << *identifier_;
        ds >> *identifier_;
        ds >> objects_;
    }
    EDB_EXPORT T getObjects(){
        return objects_;
    }
private:
    T objects_;
};

#endif // SESSIONOBJECTWRITER_H
