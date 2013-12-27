
include(../plugins.pri)


HEADERS += \
    Patches.h \
    DialogPatches.h \
    Patch.h \
    symbols.h ELF32.h ELF64.h PE32.h \
    elf/elf_auxv.h  elf/elf_header.h  elf/elf_nhdr.h  elf/elf_rela.h  elf/elf_shdr.h  elf/elf_syminfo.h  elf/elf_verdaux.h  elf/elf_vernaux.h elf/elf_dyn.h   elf/elf_move.h    elf/elf_phdr.h  elf/elf_rel.h   elf/elf_sym.h   elf/elf_types.h    elf/elf_verdef.h   elf/elf_verneed.h

SOURCES += \
    Patches.cpp \
    DialogPatches.cpp \
    Patch.cpp \
    symbols.cpp ELF32.cpp ELF64.cpp PE32.cpp

FORMS += \
    DialogPatches.ui
