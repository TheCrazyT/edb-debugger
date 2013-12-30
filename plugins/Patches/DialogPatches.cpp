#include "DialogPatches.h"
#include "ui_DialogPatches.h"
#include "edb.h"
#include "IPatch.h"
#include "Patch.h"
#include "IDebuggerCore.h"
#include "qmenu.h"
#include "ELF64.h"
#include "symbols.h"
#include "elf/elf_header.h"
#include "elf/elf_shdr.h"
#include "elf_binary.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QtGui/QFileDialog>

DialogPatches::DialogPatches(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPatches)
{
    ui->setupUi(this);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

DialogPatches::~DialogPatches()
{
    delete ui;
}

//------------------------------------------------------------------------------
// Name: showEvent
// Desc:
//------------------------------------------------------------------------------
void DialogPatches::showEvent(QShowEvent *) {
    updateList();
}

//------------------------------------------------------------------------------
// Name: updateList
// Desc:
//------------------------------------------------------------------------------
void DialogPatches::updateList() {
    IDebuggerCore::PatchList* patches = edb::v1::debugger_core->get_code_patches();
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        ui->tableWidget->removeRow(0);
    }
    ui->tableWidget->setColumnWidth(0,150);
    Q_FOREACH(const IPatch::pointer &patch, *patches) {
        const int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row,0,new QTableWidgetItem(edb::v1::format_pointer(patch->getAddress())));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(patch->isActive()?"true":"false"));
    }
}

void DialogPatches::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    IDebuggerCore::PatchList* patches = edb::v1::debugger_core->get_code_patches();
    int row = item->row();
    edb::address_t address = patches->values().at(row)->getAddress();
    edb::v1::jump_to_address(address);
}

void DialogPatches::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);

    if(ui->tableWidget->selectedItems().size()==0){
        return;
    }
    QMenu myMenu;


    QAction* disable = NULL;
    QAction* enable = NULL;
    QAction* remove = NULL;
    const int row = ui->tableWidget->selectedItems().at(0)->row();
    IDebuggerCore::PatchList* patches = edb::v1::debugger_core->get_code_patches();
    IPatch::pointer patch = patches->values().at(row);
    if(patch->isActive()){
        disable = myMenu.addAction("&Disable");
    }else{
        enable = myMenu.addAction("&Enable");
    }
    remove = myMenu.addAction("&Remove");
    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem == disable)
    {
        patch.staticCast<Patch>()->disable();
    }
    if (selectedItem == enable)
    {
        patch.staticCast<Patch>()->enable();
    }
    if (selectedItem == remove)
    {
        patch.staticCast<Patch>()->disable();
        edb::address_t key = patches->keys().at(row);
        qDebug("removing %i patches",  patches->remove(key));
        qDebug("%i patches left",patches->count());
    }
    updateList();
}

void DialogPatches::on_pushButton_clicked()
{
    bool patched = false;
    if(edb::v1::debugger_core) {
        if(const edb::pid_t pid = edb::v1::debugger_core->pid()) {
            const QString exe            = edb::v1::debugger_core->process_exe(pid);
            const QList<Module> modules  = edb::v1::debugger_core->loaded_modules();
            Q_FOREACH(const Module &m, modules) {
                if(m.name.compare(exe) == 0) {
                    QFile file(exe);
                    if(file.open(QIODevice::ReadOnly)) {
                        QByteArray byteArray = file.readAll();
                        void * data = byteArray.data();
                        const plugin::binary_info::elf64_header *const header64 = (plugin::binary_info::elf64_header *)(data);
                        if((std::memcmp(header64->e_ident, ELFMAG, SELFMAG) == 0) && (header64->e_ident[EI_CLASS] == ELFCLASS64)){
                            QString patchName = QFileDialog::getSaveFileName(this, "Save patched file", "", "");
                            QFile patch(patchName);
                            IDebuggerCore::PatchList* patches = edb::v1::debugger_core->get_code_patches();
                            plugin::binary_info::elf64_shdr* shdr = (plugin::binary_info::elf64_shdr *)(data + header64->e_shoff);
                            for(int i=0;i<header64->e_shnum;i++) {
                                Q_FOREACH(const IPatch::pointer &patch, *patches) {
                                    if((shdr->sh_addr!=0) && (patch->getAddress() > shdr->sh_addr) && (patch->getAddress() < shdr->sh_addr + shdr->sh_size)){
                                        quint64 offset = shdr->sh_offset + patch->getAddress() - shdr->sh_addr;
                                        qDebug("Section size: %x",shdr->sh_size);
                                        qDebug("Section addr: %x",shdr->sh_addr);
                                        qDebug("Section offset: %x",shdr->sh_offset);
                                        qDebug("Patch offset: %x",offset);
                                        memcpy(data + offset,patch->getBytes(),patch->getSize());
                                    }
                                }
                                shdr++;
                            }
                            patch.open(QIODevice::WriteOnly);
                            patch.write((const char*)data,byteArray.count());
                            patch.close();
                            patched = true;
                        }
                        const plugin::binary_info::elf32_header *const header32 = (plugin::binary_info::elf32_header *)(data);
                        if((std::memcmp(header32->e_ident, ELFMAG, SELFMAG) == 0) && (header32->e_ident[EI_CLASS] == ELFCLASS64)){
                            QString patchName = QFileDialog::getSaveFileName(this, "Save patched file", "", "");
                            QFile patch(patchName);
                            IDebuggerCore::PatchList* patches = edb::v1::debugger_core->get_code_patches();
                            plugin::binary_info::elf32_shdr* shdr = (plugin::binary_info::elf32_shdr *)(data + header64->e_shoff);
                            for(int i=0;i<header32->e_shnum;i++) {
                                Q_FOREACH(const IPatch::pointer &patch, *patches) {
                                    if((shdr->sh_addr!=0) && (patch->getAddress() > shdr->sh_addr) && (patch->getAddress() < shdr->sh_addr + shdr->sh_size)){
                                        quint64 offset = shdr->sh_offset + patch->getAddress() - shdr->sh_addr;
                                        qDebug("Section size: %x",shdr->sh_size);
                                        qDebug("Section addr: %x",shdr->sh_addr);
                                        qDebug("Section offset: %x",shdr->sh_offset);
                                        qDebug("Patch offset: %x",offset);
                                        memcpy(data + offset,patch->getBytes(),patch->getSize());
                                    }
                                }
                                shdr++;
                            }
                            patch.open(QIODevice::WriteOnly);
                            patch.write((const char*)data,byteArray.count());
                            patch.close();
                            patched = true;
                        }
                        file.close();
                    }
                }
            }
        }
    }
    if(!patched){
        QMessageBox::critical(this,
                         QString("Error"),
                         QString("unable to patch file"),
                         QMessageBox::Ok);
    }
}
