#ifndef MAINTHREADOBJECT_H
#define MAINTHREADOBJECT_H
#include "edb.h"
#include <QObject>

namespace MTO{
class MainThreadObject;

    class MainThreadObject : public QObject
    {
         Q_OBJECT
     private:
        static MainThreadObject* instance;

     public:
        static MainThreadObject* getInstance();
        MainThreadObject();
        ~MainThreadObject();
        void sendFinishedInit();
     signals:
         void finishedInit();
     public slots:
         void run();
         void getEntryPoint();
     signals:
         void getEntryPointSig(edb::address_t addr);
         void getEntryPointFinishedSig();
    };
}
#endif // MAINTHREADOBJECT_H
