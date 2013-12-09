#ifndef ISESSIONOBJECT_H
#define ISESSIONOBJECT_H
#include <QDataStream>
#include <QString>

class SessionObject {
public:
    SessionObject();
    SessionObject(QString* identifier);
    SessionObject(QString*identifier, void *objects);
    QString* getIdentifier();
    virtual QDataStream& operator<<(QDataStream& ds);
    virtual QDataStream& operator>>(QDataStream& ds);

protected:
    QString* identifier_;
};

#endif // ISESSIONOBJECT_H
