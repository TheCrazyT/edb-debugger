#ifndef IPLUGINSESSION_H
#define IPLUGINSESSION_H
#include "SessionObject.h"
#include <QDataStream>
#include <QString>

class IPluginSession {
public:
    virtual ~IPluginSession() {}
    virtual QString* getSessionIdentifier() const{return 0;}
    virtual void serializeSessionObject(QDataStream* stream) const{}
    virtual void deserializeSessionObject(QDataStream* stream) const{}
};

Q_DECLARE_INTERFACE(IPluginSession, "edb.IPluginSession/1.0")
#endif // IPLUGINSESSION_H
