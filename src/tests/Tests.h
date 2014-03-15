#ifndef TESTS_H
#define TESTS_H
#if defined(TEST_BUILD)
#include "edb.h"
#include "Debugger.h"
#include <QThread>
#include <QEventLoop>
namespace Test{
#if defined(__linux__)
    static char* testApp = "./TestApp";
#else
    static char* testApp = "./TestApp.exe";
#endif
    static QEventLoop* mainLoop;
    //static MainThreadObject* mt;

    class Tests: public QObject
    {
         Q_OBJECT

        signals:
            void getEntryPointSig();
        private:
            edb::address_t getEntryPoint();
            Debugger* mainWindow;
            QEventLoop* loop;
            QEventLoop* loop2;
            bool testStarted;

        private slots:
            void initTestCase();
            void singleStep();
            void restart();
            void cleanupTestCase();
            void getEntryPoint(edb::address_t addr);
            void start();
        private:
            edb::address_t entryPoint;
        public:
            static QThread* mainThread;
    };
}
#endif
#endif // TESTS_H
