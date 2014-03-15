#include "MainThreadObject.h"
#include "main.h"
#include "Tests.h"
#include "Symbol.h"
#include "ISymbolManager.h"
#include "Configuration.h"
#include <QDebug>
#include <QThread>
#include <QApplication>
#include <QTest>
namespace MTO{
MainThreadObject* MainThreadObject::instance;

void MainThreadObject::run()
{
    qDebug() << "run" << this << "(threadid=" << QThread::currentThread() << ")";
    qDebug() << "mt:" << MainThreadObject::getInstance();
    int argc=3;
    char **argv = new char*[2];

    if(qApp->argc()>0){
        argv[0] = qApp->argv()[0];
    }
    argv[1] = "--run";
    argv[2] = Test::testApp;

    sleep(1);
    qDebug() << "Starting ...(threadid=" << QThread::currentThread() << ")";
    mainNsp::start(argc,argv);
    qDebug() << "start finished";
}

MainThreadObject::MainThreadObject(){
    instance = this;
}

MainThreadObject* MainThreadObject::getInstance(){
    return instance;
}

void MainThreadObject::sendFinishedInit(){
    qDebug() << "sendFinishedInit" << this << "(threadid=" << QThread::currentThread() << ")";
    emit finishedInit();
}

void MainThreadObject::getEntryPoint()
{
    qDebug() << "MainThreadObject::getEntryPoint(threadid=" << QThread::currentThread() << ")";
    if(QThread::currentThread() != Test::Tests::mainThread){
        qDebug() << "mainThread:" << Test::Tests::mainThread;
        qFatal( "Thread mismatch");
    }
    edb::address_t entryPoint = 0;
    Symbol::pointer sym = edb::v1::symbol_manager().find(QString("TestApp::main"));
    if(!sym) {
        sym = edb::v1::symbol_manager().find(QString("main"));
    }

    if(sym) {
        entryPoint = sym->address;
    } else if(edb::v1::config().find_main) {
        qDebug() << "no sym found, using locate_main_function.";
        entryPoint = edb::v1::locate_main_function();
    }
    qDebug() << "getEntryPointSig,getEntryPointFinishedSig";
    emit getEntryPointSig(entryPoint);
    emit getEntryPointFinishedSig();
}
MainThreadObject::~MainThreadObject() {
    qDebug() << "MainThreadObject destruct";
}

}
