
unix {
        CONFIG(debug, debug|release) {
		DEFINES    += QT_SHAREDPOINTER_TRACK_POINTERS
		OBJECTS_DIR = $${OUT_PWD}/.debug-shared/obj
		MOC_DIR     = $${OUT_PWD}/.debug-shared/moc
		RCC_DIR     = $${OUT_PWD}/.debug-shared/rcc
		UI_DIR      = $${OUT_PWD}/.debug-shared/uic
	}
	
	CONFIG(release, debug|release) {
		OBJECTS_DIR = $${OUT_PWD}/.release-shared/obj
		MOC_DIR     = $${OUT_PWD}/.release-shared/moc
		RCC_DIR     = $${OUT_PWD}/.release-shared/rcc
		UI_DIR      = $${OUT_PWD}/.release-shared/uic
	}

        CONFIG(TEST){
                OBJECTS_DIR = $${OUT_PWD}/.test-shared/obj
                MOC_DIR     = $${OUT_PWD}/.test-shared/moc
                RCC_DIR     = $${OUT_PWD}/.test-shared/rcc
                UI_DIR      = $${OUT_PWD}/.test-shared/uic
        }
}
