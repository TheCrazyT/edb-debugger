#include "DialogPatches.h"
#include "ui_DialogPatches.h"
#include "edb.h"
#include "IPatch.h"
#include "Patch.h"
#include "IDebuggerCore.h"
#include "qmenu.h"

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
    IDebuggerCore::PatchList patches = edb::v1::debugger_core->get_code_patches();
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        ui->tableWidget->removeRow(0);
    }
    ui->tableWidget->setColumnWidth(0,150);
    Q_FOREACH(const IPatch::pointer &patch, patches) {
        const int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row,0,new QTableWidgetItem(edb::v1::format_pointer(patch->getAddress())));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(patch->isActive()?"true":"false"));
    }
}

void DialogPatches::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    IDebuggerCore::PatchList patches = edb::v1::debugger_core->get_code_patches();
    int row = item->row();
    edb::address_t address = patches.values().at(row)->getAddress();
    edb::v1::jump_to_address(address);
}

void DialogPatches::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);

    QMenu myMenu;


    QAction* disable = NULL;
    QAction* enable = NULL;
    const int row = ui->tableWidget->selectedItems().at(0)->row();
    IDebuggerCore::PatchList patches = edb::v1::debugger_core->get_code_patches();
    IPatch::pointer patch = patches.values().at(row);
    if(patch->isActive()){
        disable = myMenu.addAction("Disable");
    }else{
        enable = myMenu.addAction("Enable");
    }
    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem == disable)
    {
        patch.staticCast<Patch>()->disable();
        edb::v1::debugger_ui->repaint();
    }
    if (selectedItem == enable)
    {
        patch.staticCast<Patch>()->enable();
        edb::v1::debugger_ui->repaint();
    }
    updateList();
}
