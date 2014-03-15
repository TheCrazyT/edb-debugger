#ifndef MAIN_H
#define MAIN_H
#include "edb.h"
#include "Debugger.h"
namespace mainNsp{
    int start_debugger(edb::pid_t attach_pid, const QString &program, const QList<QByteArray> &programArgs);
    void load_translations();
    void load_plugins(const QString &directory);
    void usage();
    int start(int argc, char *argv[]);
}
#endif // MAIN_H
