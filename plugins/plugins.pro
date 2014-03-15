TEMPLATE = subdirs

# Directories
SUBDIRS += \
	Analyzer \
	Assembler \
	BinaryInfo \
	BinarySearcher \
	Bookmarks \
	BreakpointManager \
	CheckVersion \
	DebuggerCore \
	DumpState \
	FunctionFinder \
	HardwareBreakpoints \
	OpcodeSearcher \
	ProcessProperties \
	ROPTool \
	References \
	SessionManager \
	SymbolViewer \
        Patches \
        ObjectExtractor

unix {
	!macx {
		SUBDIRS += HeapAnalyzer
	}
}

CONFIG(TEST){
    message(Compiling Test build)
    CONFIG  += debug
    CONFIG  += qtestlib
    QT += testlib
    SOURCES += tests/Tests.cpp
    TARGET   = tests
    DEFINES += TEST_BUILD
}
