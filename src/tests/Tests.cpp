#include "main.h"
#include "IPlugin.h"
#include "IBinary.h"
#include "ISymbolManager.h"
#include "Debugger.h"
#include "Symbol.h"
#include "Tests.h"
#include "State.h"
#include "MainThreadObject.h"
#include "Configuration.h"
#include "IDebuggerCore.h"

#include <QtTest/QtTest>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QtGui>
#include <QMainWindow>

#if defined(TEST_BUILD)
namespace Test{

    QThread* Tests::mainThread;

    void Tests::start(){
        if(!testStarted){
            testStarted = true;
            qDebug() << "start";
            Tests* tcp = this;
            exit(QTest::qExec(tcp, qApp->argc(), qApp->argv()));
        }
    }

    void Tests::getEntryPoint(edb::address_t addr){
        qDebug() << "getEntryPoint" << addr;
        entryPoint = addr;
        loop = 0;
    }

    edb::address_t Tests::getEntryPoint(){
        qDebug() << "getEntryPoint";
        qDebug() << "(threadid=" << QThread::currentThread() << ")";

        emit getEntryPointSig();
        qDebug() << "sleeping till entrypoint changes ...";
        if(loop != 0){
            loop->exec();
        }else{
            qFatal("loop = 0");
        }
        qDebug() << "sleeping done(entrypoint=" << entryPoint << ")";
        return entryPoint;
    }

    void Tests::initTestCase()
    {
        qRegisterMetaType<edb::address_t>("edb::address_t");
        loop = new QEventLoop();
        loop2 = new QEventLoop();
        MTO::MainThreadObject* mt = new MTO::MainThreadObject();
        mt->moveToThread(mainThread);
        if(!QObject::connect(this, SIGNAL(getEntryPointSig()), mt, SLOT(getEntryPoint())))qFatal("connection failed 3");
        if(!QObject::connect(mt, SIGNAL(getEntryPointFinishedSig()), loop, SLOT(quit())))qFatal("connection failed 4");
        if(!QObject::connect(mt, SIGNAL(getEntryPointSig(edb::address_t)), this, SLOT(getEntryPoint(edb::address_t))))qFatal("connection failed 5");

        qDebug() << "TestThread:" << QThread::currentThread();
        entryPoint = 0;
        qDebug() << "Start initTestCase";
        QString file = QString("./sessions/TestApp.edb");
        if(QFile::exists(file)){
            qDebug() << "Deleted: " << file;
            QFile::remove(file);
        }
    }

    void Tests::singleStep()
    {

        qDebug() << "singleStep";
        QAction* action;
        //loop2->exec();
        edb::address_t entryPoint = getEntryPoint();
        qDebug() << "Current addr:" << entryPoint;

        QWidgetList allToplevelWidgets = QApplication::topLevelWidgets();
        foreach (QWidget *w, allToplevelWidgets) {
             if (w->inherits("QMainWindow")) {
                 mainWindow = (Debugger*)w;
             }
        }

        qDebug() << "single step";
        action = mainWindow->findChild<QAction*>(QString("action_Step_Into"));
        action->trigger();
        qDebug() << "after single step";

        sleep(1);

        State state;
        edb::v1::debugger_core->get_state(&state);
        if(entryPoint == state.instruction_pointer()){
            qFatal("single step failed!(%x,%x)",entryPoint,state.instruction_pointer());
        }
        sleep(1);
    }
    void Tests::restart(){
        qDebug() << "restart";
        mainWindow->findChild<QAction*>(QString("action_Restart"));
        qDebug() << "End restart";

    }
    void Tests::cleanupTestCase(){
        qDebug() << "cleanupTestCase ...";
        QAction* action;
        sleep(1);
        qDebug() << "Closing ...";
        action = mainWindow->findChild<QAction*>(QString("actionE_xit"));
        action->trigger();
        sleep(1);
        mainThread->terminate();
    }

}

int main(int argc, char *argv[])
{
    qDebug() << "main";
    Test::Tests tc;
    tc.mainThread = QThread::currentThread();
    QThread* testThread = new QThread();
    tc.moveToThread(testThread);
    QObject::connect(testThread,SIGNAL(started()),&tc,SLOT(start()));
    testThread->start();

    qDebug() << "run" << "(threadid=" << QThread::currentThread() << ")";
    int argc2=3;
    char **argv2 = new char*[2];

    if(qApp->argc()>0){
        argv2[0] = qApp->argv()[0];
    }
    argv2[1] = "--run";
    argv2[2] = Test::testApp;

    sleep(1);
    qDebug() << "Starting ...(threadid=" << QThread::currentThread() << ")";
    mainNsp::start(argc2,argv2);
    qDebug() << "start finished";
}

//QTEST_MAIN(Test::Tests)
//QTEST_APPLESS_MAIN(Test::Tests)
#include "Tests.moc"
#endif
