#include <QtGlobal>
#ifndef IPATCH_H
#define IPATCH_H

class IPatch
{
public:
    IPatch();

    bool isActive();
    edb::address_t getAddress();
    QList<quint8> getBytes();
};

#endif // IPATCH_H
