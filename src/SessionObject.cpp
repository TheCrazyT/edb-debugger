#include "SessionObject.h"
#include<QDebug>
#include<QString>
SessionObject::SessionObject(){

}

SessionObject::SessionObject(QString* identifier){
    identifier_ = identifier;
}



QString* SessionObject::getIdentifier(){
    return identifier_;
}

QDataStream& SessionObject::operator<<(QDataStream& ds){
    identifier_ = new QString();
    ds >> *identifier_;
}
QDataStream& SessionObject::operator>>(QDataStream& ds){
    qDebug()<<"WTF";
}
