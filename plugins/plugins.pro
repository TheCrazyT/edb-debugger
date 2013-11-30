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
        Patches

unix {
	!macx {
		SUBDIRS += HeapAnalyzer
	}
}

