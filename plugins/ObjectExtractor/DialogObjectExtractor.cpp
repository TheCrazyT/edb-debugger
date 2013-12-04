#include "DialogObjectExtractor.h"
#include "ui_DialogObjectExtractor.h"
#include "qinputdialog.h"
#include "edb.h"
#include "IDebuggerCore.h"
#include <QDebug>
#include <QPointer>
#include <QTimer>
#include <QTreeWidgetItem>

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

const int CUSTOM = 20;

const int POINTER = 22;

const int ARRAY = 1000;
const int DYNAMIC_ARRAY = 2000;

DialogObjectExtractor::DialogObjectExtractor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogObjectExtractor)
{
    typeSizes_ = new QHash<int,int>();
    ui->setupUi(this);

    QStringList lst = QStringList();
    lst << "ROOT" << "address";
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget,lst,-1);
    ui->treeWidget->addTopLevelItem(item);

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
    ui->cbType->addItem(QString("custom"),QVariant(CUSTOM));
    ui->cbType->addItem(QString("pointer"),QVariant(POINTER));

    ui->cbType->addItem(QString("Array"),QVariant(ARRAY));

    typeSizes_->insert(NULL_STRING,0);
    typeSizes_->insert(STRING_WITH_LENGTH,0);
    typeSizes_->insert(INT64,8);
    typeSizes_->insert(INT32,4);
    typeSizes_->insert(INT16,2);
    typeSizes_->insert(INT8,1);
    typeSizes_->insert(CUSTOM,0);
    typeSizes_->insert(POINTER,sizeof(std::size_t));

    typeSizes_->insert(ARRAY,0);
    typeSizes_->insert(DYNAMIC_ARRAY,0);
}

DialogObjectExtractor::~DialogObjectExtractor()
{
    delete ui;
}

void DialogObjectExtractor::on_txtName_returnPressed()
{
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(0);
    item->setText(0,ui->txtName->text());
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

    while((parent->type() != POINTER)&&(parent->type() != -1)){
        parent = parent->parent();
    }

    qDebug() << parent->text(0);

    bool ok = false;
    QString txt = QInputDialog::getText(this, tr("Enter Name"),tr("Name:"), QLineEdit::Normal,"", &ok);
    if(ok){
        QStringList lst;
        lst << txt << "";
        int idx = ui->cbType->currentIndex();
        QVariant v = ui->cbType->itemData(idx);
        int type = v.value<int>();

        QTreeWidgetItem* item = new QTreeWidgetItem(parent,lst,type);
        parent->addChild(item);
        ui->treeWidget->repaint();
    }
    refresh();
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
    refresh();
}

void DialogObjectExtractor::on_txtAddress_returnPressed()
{
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(0);
    item->setText(1,ui->txtAddress->text());
    refresh();
}

void DialogObjectExtractor::on_checkBox_stateChanged(int arg1)
{
    ui->btnRefresh->setEnabled(!ui->chkRefresh->checkState());

    if(ui->chkRefresh->checkState()){
        QTimer* timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
    }
}
void DialogObjectExtractor::refresh(QTreeWidgetItem* ti)
{
    bool ok = false;
    QString txt = ti->text(1);
    edb::address_t addr = txt.toULongLong(&ok, 16);

    int size = 0;
    for(int i=0;i < ti->childCount();i++){
        refresh(ti->child(i));
    }

    if(size==0){
        return;
    }

    quint8* buf = new quint8[size];
    edb::v1::debugger_core->read_bytes(addr,buf,size);
    for(int i=0;i < ti->childCount();i++){
        if(ti->child(i)->type() != -1){
            ti->child(i)->setText(1,parseToString(buf,ti->child(i)->type()));
            buf += typeSizes_->find(ti->child(i)->type()).value();
        }
    }
}

QString DialogObjectExtractor::parseToString(void * buf,int type){
    QString result = NULL;
    if(type == INT64){
        qint64* i = (qint64*)buf;
        result = QString::number(*i);
    }
    if(type == INT32){
        qint32* i = (qint32*)buf;
        result = QString::number(*i);
    }
    if(type == INT16){
        qint16* i = (qint16*)buf;
        result = QString::number(*i);
    }
    if(type == INT8){
        qint8* i = (qint8*)buf;
        result = QString::number(*i);
    }
    if(type == UINT64){
        quint64* i = (quint64*)buf;
        result = QString::number(*i);
    }
    if(type == UINT32){
        quint32* i = (quint32*)buf;
        result = QString::number(*i);
    }
    if(type == UINT16){
        quint16* i = (quint16*)buf;
        result = QString::number(*i);
    }
    if(type == UINT8){
        quint8* i = (quint8*)buf;
        result = QString::number(*i);
    }
    if(type == POINTER){
        edb::address_t* addr = (edb::address_t*)buf;
        result = QString::number(*addr,16);
    }
    return result;
}

void DialogObjectExtractor::refresh()
{
    QTreeWidgetItem* ti = ui->treeWidget->topLevelItem(0);
    refresh(ti);
}

void DialogObjectExtractor::on_btnRefresh_clicked()
{
    refresh();
}
void DialogObjectExtractor::timerTimeout()
{
    refresh();
}
