#include "DialogObjectExtractor.h"
#include "ui_DialogObjectExtractor.h"
#include "qinputdialog.h"
#include "edb.h"
#include "IDebuggerCore.h"
#include "constants.h"
#include <QDebug>
#include <QPointer>
#include <QVariant>
#include <QMap>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QSignalMapper>



DialogObjectExtractor::DialogObjectExtractor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogObjectExtractor)
{
    typeSizes_ = new QHash<int,int>();
    ui->setupUi(this);
    typeList_ = QList<quint8>();
    typeListStr_ = QStringList();
    typeButtons_ = new QList<QPushButton*>();

    QLayout * grid = new QGridLayout();
    typeListStr_ << "NULL terminated String";
    typeListStr_ << "String with length";
    typeListStr_ << "int64";
    typeListStr_ << "int32";
    typeListStr_ << "int16";
    typeListStr_ << "int8";
    typeListStr_ << "uint64";
    typeListStr_ << "uint32";
    typeListStr_ << "uint16";
    typeListStr_ << "uint8";
    typeListStr_ << "custom";
    typeListStr_ << "pointer";
    typeListStr_ << "Array";

    typeList_ << NULL_STRING;
    typeList_ << STRING_WITH_LENGTH;
    typeList_ << INT64;
    typeList_ << INT32;
    typeList_ << INT16;
    typeList_ << INT8;
    typeList_ << UINT64;
    typeList_ << UINT32;
    typeList_ << UINT16;
    typeList_ << UINT8;
    typeList_ << CUSTOM;
    typeList_ << POINTER;
    typeList_ << ARRAY;

    ui->grpCreateElementList->setLayout(grid);

    QSignalMapper *signalMapper = new QSignalMapper(ui->grpCreateElementList);
    quint8 i = 0;
    Q_FOREACH(const QString &s,typeListStr_){
        QPushButton* pb = new QPushButton(s,ui->grpCreateElementList);
        *typeButtons_ << pb;
        pb->connect(pb, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(pb,i++);
        grid->addWidget(pb);
    }

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

    cleanUp();

    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(create(int)));
}


//should be called on every new opened file
void DialogObjectExtractor::cleanUp(bool skipRoot)
{
    ui->treeWidget->clear();
    objectTree = new QHash<quint32,QHash<qint32,void*>*>();

    if(!skipRoot){
        QStringList lst = QStringList();
        lst << "ROOT" << "address";
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget,lst,-1);
        ui->treeWidget->addTopLevelItem(item);

        QHash<qint32,void*>* subObjTree = new QHash<qint32,void*>();
        subObjTree->insert(TREE_NAME,new QString("ROOT"));
        subObjTree->insert(TREE_ADDRESS,new quint64(0));
        subObjTree->insert(TREE_TYPE,new int(ROOT));
        objectTree->insert(0,subObjTree);
    }
}

DialogObjectExtractor::~DialogObjectExtractor()
{
    //delete &objectTree;
    delete ui;
}



void DialogObjectExtractor::on_txtName_returnPressed()
{
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(0);
    item->setText(0,ui->txtName->text());
    QHash<qint32,void*>* subObjTree = objectTree->find(0).value();
    QString str = ui->txtName->text();
    subObjTree->insert(TREE_NAME,new QString(str));
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),ui->txtName->text());
    qDebug() << *objectTree;
    qDebug() << *subObjTree;
}

QTreeWidgetItem* DialogObjectExtractor::create(int type,QString* txt,quint64 addr=0,QTreeWidgetItem* parent=NULL){
    if(parent == NULL){
        parent = ui->treeWidget->topLevelItem(0);
    }

    QStringList lst;
    lst << *txt <<  QString::number(addr,16);
    if(parent != NULL){
        QHash<qint32,void*>* subObjTree = objectTree->find(0).value();
        QList<quint32> indexes = QList<quint32>();
        QTreeWidgetItem* treePath = parent;
        while(treePath->type() != ROOT){
            treePath = treePath->parent();
            if(treePath->type() != ROOT){
                indexes.append(0);
            }else{
                indexes.append(treePath->parent()->indexOfChild(treePath));
            }
        }
        qDebug() << parent->text(0);
        for(int i=indexes.count()-1;i>=0;i--){
            subObjTree = &subObjTree[i];
        }
        if(type != ROOT){
            quint8 newIdx = 0;
            Q_FOREACH(qint32 key, subObjTree->keys()){
                if(key > newIdx){
                    newIdx = key + 1;
                }
            }

            QHash<qint32,void*>* newSubObjTree = new QHash<qint32,void*>();
            subObjTree->insert(newIdx,newSubObjTree);
            subObjTree = newSubObjTree;
        }
        subObjTree->insert(TREE_TYPE,new quint8(type));
        subObjTree->insert(TREE_NAME,new QString(*txt));
        subObjTree->insert(TREE_ADDRESS,new quint64(addr));
    }

    QTreeWidgetItem* item = new QTreeWidgetItem(parent,lst,type);
    if(parent == NULL){
        ui->treeWidget->addTopLevelItem(item);
    }else{
        parent->addChild(item);
    }
    return item;
}

void DialogObjectExtractor::create(int type){
    QList<QTreeWidgetItem*> sel = ui->treeWidget->selectedItems();
    QTreeWidgetItem* parent = NULL;
    if(sel.size()==0){
        parent = ui->treeWidget->topLevelItem(0);
    }else{
        parent = sel.at(0);
    }

    if(parent == NULL){
        return;
    }

    while((parent->type() != POINTER)&&(parent->type() != -1)){
        parent = parent->parent();
    }

    bool ok = false;
    QString txt = QInputDialog::getText(this, tr("Enter Name"),tr("Name:"), QLineEdit::Normal,"", &ok);
    if(ok) {
        create(type,&txt,0,parent);
    }
    ui->treeWidget->repaint();
    refresh();
}

void DialogObjectExtractor::on_btnDelete_clicked()
{
    QTreeWidgetItem* tli = ui->treeWidget->topLevelItem(0);
    QList<QTreeWidgetItem*> sel = ui->treeWidget->selectedItems();
    if(sel.size() == 0){
        return;
    }
    if(sel.at(0) == tli){
        return;
    }
    tli->removeChild(sel.at(0));
    refresh();
}

void DialogObjectExtractor::on_txtAddress_returnPressed()
{
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(0);
    item->setText(1,ui->txtAddress->text());

    QHash<qint32,void*>* subObjTree = objectTree->find(0).value();
    bool ok = false;
    edb::address_t addr = ui->txtAddress->text().toULongLong(&ok, 16);
    subObjTree->insert(TREE_ADDRESS,new quint64(addr));
    refresh();
}

void DialogObjectExtractor::refresh(QTreeWidgetItem* ti)
{
    bool ok = false;
    QString txt = ti->text(1);
    edb::address_t addr = txt.toULongLong(&ok, 16);

    int size = 0;
    for(int i = 0;i < ti->childCount();i++){
        refresh(ti->child(i));
        size += getTypeSize(addr+size,ti->child(i)->type());
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

quint64 DialogObjectExtractor::getTypeSize(edb::address_t addr,int type){
    return typeSizes_->find(type).value();
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

    bool ok = false;
    QString txt = ti->text(1);
    edb::address_t addr = txt.toULongLong(&ok, 16);
    if(ok && addr!=0){
        refreshTypeButtons(addr);
    }

}

void DialogObjectExtractor::refreshTypeButtons(edb::address_t addr){
    uint size = 128;
    quint8* buf = new quint8[size];

    QTreeWidgetItem* ti = ui->treeWidget->topLevelItem(0);

    uint structSize = 0;
    for(int i = 0;i < ti->childCount();i++){
        refresh(ti->child(i));
        structSize += getTypeSize(addr+structSize,ti->child(i)->type());
    }

    edb::v1::debugger_core->read_bytes(addr+structSize,buf,size);

    QString value = QString();
    for(quint8 x=0;x<typeButtons_->size();x++){
        QPushButton* pb = typeButtons_->at(x);
        value = parseToString(buf,typeList_.at(x));
        pb->setText(typeListStr_.at(x) + "[" + value + "]");
    }
}

void DialogObjectExtractor::on_btnRefresh_clicked()
{
    refresh();
}
void DialogObjectExtractor::timerTimeout()
{
    refresh();
}
void DialogObjectExtractor::newObjVar(edb::address_t addr){
    ui->txtAddress->setText(QString().number(addr));
}

void DialogObjectExtractor::on_chkRefresh_stateChanged(int arg1)
{
    ui->btnRefresh->setEnabled(!ui->chkRefresh->checkState());

    if(ui->chkRefresh->checkState()){
        QTimer* timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
    }
}
