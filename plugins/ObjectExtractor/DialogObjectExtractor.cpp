#include "DialogObjectExtractor.h"
#include "ui_DialogObjectExtractor.h"
#include "qinputdialog.h"
#include <QDebug>

const int NULL_STRING = 0;
const int STRING_WITH_LENGTH = 1;
const int INT64 = 2;
const int INT32 = 3;
const int INT16 = 4;
const int INT8 = 5;
const int UINT64 = 6;
const int UINT32 = 7;
const int UINT16 = 8;
const int UINT8 = 9;

DialogObjectExtractor::DialogObjectExtractor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogObjectExtractor)
{
    ui->setupUi(this);
    ui->cbType->addItem(QString("NULL terminated String"),QVariant(NULL_STRING));
    ui->cbType->addItem(QString("String with length"),QVariant(STRING_WITH_LENGTH));
    ui->cbType->addItem(QString("int64"),QVariant(INT64));
    ui->cbType->addItem(QString("int32"),QVariant(INT32));
    ui->cbType->addItem(QString("int16"),QVariant(INT16));
    ui->cbType->addItem(QString("int8"),QVariant(INT8));
    ui->cbType->addItem(QString("uint64"),QVariant(UINT64));
    ui->cbType->addItem(QString("uint32"),QVariant(UINT32));
    ui->cbType->addItem(QString("uint16"),QVariant(UINT16));
    ui->cbType->addItem(QString("uint8"),QVariant(UINT8));
}

DialogObjectExtractor::~DialogObjectExtractor()
{
    delete ui;
}

void DialogObjectExtractor::on_txtName_returnPressed()
{
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(0);
    item->setText(1,ui->txtAddress->text());
}

void DialogObjectExtractor::on_btnCreate_clicked()
{
    QList<QTreeWidgetItem*> sel = ui->treeWidget->selectedItems();
    QTreeWidgetItem* parent = NULL;
    if(sel.size()==0){
        parent = ui->treeWidget->topLevelItem(0);
    }else{
        parent = sel.at(0);
    }

    if(parent==NULL){
        return;
    }

    qDebug() << parent->text(0);

    bool ok = false;
    QString txt = QInputDialog::getText(this, tr("Enter Name"),tr("Name:"), QLineEdit::Normal,"", &ok);
    if(ok){
        QStringList lst;
        lst << txt << "";
        QTreeWidgetItem* item = new QTreeWidgetItem(parent,lst);
        parent->addChild(item);
        ui->treeWidget->repaint();
    }
}

void DialogObjectExtractor::on_btnDelete_clicked()
{
    QTreeWidgetItem* tli = ui->treeWidget->topLevelItem(0);
    QList<QTreeWidgetItem*> sel = ui->treeWidget->selectedItems();
    if(sel.size()==0){
        return;
    }
    if(sel.at(0)==tli){
        return;
    }
    tli->removeChild(sel.at(0));
}

void DialogObjectExtractor::on_txtAddress_returnPressed()
{
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(0);
    item->setText(1,ui->txtAddress->text());
}
