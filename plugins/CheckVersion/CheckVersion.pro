
include(../plugins.pri)

# Input
HEADERS += CheckVersion.h CheckVersionOptionsPage.h
FORMS   += CheckVersionOptionsPage.ui
SOURCES += CheckVersion.cpp CheckVersionOptionsPage.cpp

QT += network

CONFIG(TEST){
    message(Compiling Test build)
    DEFINES += TEST_BUILD
}
