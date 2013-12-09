#include "SessionObjectWriter.h"
#include "IDebuggerCore.h"
#include "edb.h"
#include <QList>
#include <QDebug>
template<class T>
SessionObjectWriter<T>::SessionObjectWriter(QString* identifier, T objects){
    identifier_ = identifier;
    objects_ = objects;
}

template<class T>
QDataStream& SessionObjectWriter<T>::operator>>(QDataStream& ds){
    qDebug() << *identifier_;
    ds << *identifier_;
    ds << objects_;
}
template<class T>
QDataStream& SessionObjectWriter<T>::operator<<(QDataStream& ds){
    qDebug() << *identifier_;
    ds >> *identifier_;
    ds >> objects_;
}
template<class T>
T SessionObjectWriter<T>::getObjects(){
    return objects_;
}

template class SessionObjectWriter<AddrList>;
