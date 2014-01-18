#include "main.h"
#include "IPlugin.h"
#include "IBinary.h"
#include "ISymbolManager.h"
#include "Debugger.h"
#include "Symbol.h"
#include "Configuration.h"

#include <QtTest/QtTest>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QtGui>
#include <QMainWindow>

#if defined(TEST_BUILD)
namespace{
    #if defined(__linux__)
        char* testApp = "./TestApp";
    #else
        char* testApp = "./TestApp.exe";
    #endif

    class MainThread : public QThread
    {
         Q_OBJECT

     protected:
         void run();
    };

    void MainThread::run()
    {
        int argc=3;
        char **argv = new char*[2];

        if(qApp->argc()>0){
            argv[0] = qApp->argv()[0];
        }
        argv[1] = "--run";
        argv[2] = testApp;

        sleep(1);
        qDebug("Starting ...");
        mainNsp::start(argc,argv);
    }


    class Tests: public QObject
    {
         Q_OBJECT
        private slots:
            void initTestCase();
            void test();
            void cleanupTestCase();
        private:
            MainThread *mt;
    };
    void Tests::initTestCase()
    {
        QString file = QString("sessions/TestApp.edb");
        if(QFile::exists(file)){
            qDebug() << "Deleted: " << file;
            QFile::remove(file);
        }

        mt = new MainThread();
        mt->start();
    }
    void Tests::test()
    {
        sleep(2);
        quint32 i = 60;

        edb::address_t entryPoint = 0;
        while((entryPoint==0)&&(i>0)){
            const QString mainSymbol = QFileInfo(testApp).fileName() + "::main";
            const Symbol::pointer sym = edb::v1::symbol_manager().find(mainSymbol);

            if(sym) {
                entryPoint = sym->address;
            } else if(edb::v1::config().find_main) {
                entryPoint = edb::v1::locate_main_function();
            }
            i--;
            sleep(1);
            qDebug() << "Addr == 0 (" << i << ")";
        }
        if(i == 0){
            qFatal("Addr == 0.");
        }
        qDebug() << "Current addr:" << entryPoint;
        /*edb::v1::create_breakpoint(entryPoint);
        sleep(1);
        edb::v1::remove_breakpoint(entryPoint);
        sleep(1);*/
        Debugger* mainWindow;
        QWidgetList allToplevelWidgets = QApplication::topLevelWidgets();
        foreach (QWidget *w, allToplevelWidgets) {
             if (w->inherits("QMainWindow")) {
                 mainWindow = (Debugger*)w;
             }
        }
        //qDebug() << "Triggering restart";
        //mainWindow->on_action_Restart_triggered();
        sleep(1);
        qDebug() << "Closing ...";
        QAction* action = mainWindow->findChild<QAction*>(QString("actionE_xit"));
        action->trigger();

    }
    void Tests::cleanupTestCase(){
        qDebug() << "Cleanup ...";
        mt->terminate();
    }

}

QTEST_MAIN(Tests)
#include "Tests.moc"
#endif
