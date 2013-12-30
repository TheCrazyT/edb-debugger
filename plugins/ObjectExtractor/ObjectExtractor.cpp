#include "ObjectExtractor.h"
#include "DialogObjectExtractor.h"
#include "SessionObjectWriter.h"
#include "edb.h"
#include <QMenu>
#include <QList>
#include <QVariant>
#include "constants.h"

typedef QList<QVariant> QVarList;

ObjectExtractor::ObjectExtractor() : menu_(0),dialog_(0) {
    dialog_ = new DialogObjectExtractor(edb::v1::debugger_ui);
}
ObjectExtractor::~ObjectExtractor(){
    delete dialog_;
}

//------------------------------------------------------------------------------
// Name: private_init
// Desc:
//------------------------------------------------------------------------------
void ObjectExtractor::private_init() {
}

//------------------------------------------------------------------------------
// Name: menu
// Desc:
//------------------------------------------------------------------------------
QMenu *ObjectExtractor::menu(QWidget *parent) {

    Q_ASSERT(parent);

    if(!menu_) {
        menu_ = new QMenu(tr("ObjectExtractor"), parent);
        menu_->addAction (tr("&ObjectExtractor"), this, SLOT(show_menu()), QKeySequence(tr("Ctrl+O")));
    }

    return menu_;
}

QList<QAction *> ObjectExtractor::data_context_menu(){
    QMenu* dataMenu = new QMenu(tr("ObjectExtractor"),edb::v1::debugger_ui);
    QList<QAction *>* actionList = new QList<QAction *>();
    QAction* action = dataMenu->addAction (tr("&Add to ObjectExtractor"), this, SLOT(show_menu(edb::v1::get_selected_data_address())));
    *actionList << action;
    return *actionList;
}

//------------------------------------------------------------------------------
// Name: show_menu
// Desc:
//------------------------------------------------------------------------------
void ObjectExtractor::show_menu() {
    dialog_->show();
}

//------------------------------------------------------------------------------
// Name: show_menu
// Desc:
//------------------------------------------------------------------------------
void ObjectExtractor::show_menu(edb::address_t addr) {
    if(!dialog_) {
        dialog_ = new DialogObjectExtractor(edb::v1::debugger_ui);
    }

    dialog_->show();
    static_cast<DialogObjectExtractor*>(dialog_)->newObjVar(addr);
}


//------------------------------------------------------------------------------
// Name: getSessionIdentifier
// Desc:
//------------------------------------------------------------------------------
QString* ObjectExtractor::getSessionIdentifier() const{
    QString* result = new QString("ObjectExtractorV1");
    return result;
}

//------------------------------------------------------------------------------
// Name: serializeSessionObject
// Desc:
//------------------------------------------------------------------------------
void ObjectExtractor::serializeSessionObject(QDataStream* stream) const{
    QVarList varList=QVarList();
    varList.append(QVariant(dialog_->objectTree->count()));
    for(int i=0;i<dialog_->objectTree->count();i++){
        QHash<qint32,void*> subTree = *(dialog_->objectTree->find(i).value());
        qDebug() << subTree;
        if((subTree[TREE_NAME]==NULL) || (subTree[TREE_ADDRESS]==NULL)){
            qDebug() << "Problem while writing session objects for ObjectExtractor!";
            return;
        }
        QString treeName = *(QString*)(subTree[TREE_NAME]);
        quint64 treeAddr = *(quint64*)(subTree[TREE_ADDRESS]);
        varList.append(QVariant(ROOT));
        varList.append(QVariant(treeName));
        varList.append(QVariant(treeAddr));
        qDebug() << "serialize ROOT:" << treeName << "," << treeAddr;
        qDebug() << subTree;
        parseTree(&varList,&subTree);
    }

    SessionObjectWriter<QVarList>(getSessionIdentifier(),varList) >> *stream;
}

//------------------------------------------------------------------------------
// Name: deserializeSessionObject
// Desc:
//------------------------------------------------------------------------------
void ObjectExtractor::deserializeSessionObject(QDataStream* stream) const{
    SessionObjectWriter<QVarList>* obj = new SessionObjectWriter<QVarList>(getSessionIdentifier(),QVarList());
    *obj << *stream;

    dialog_->cleanUp(true);
    QVarList varList = obj->getObjects();
    int i=0;
    for(quint8 idx=0;idx<obj->getObjects().count();){
        QHash<qint32,void*>* subTree = new QHash<qint32,void*>();
        dialog_->objectTree->insert(i++,subTree);
        deserializeTree(&idx,&varList,subTree,NULL);
    }

}

void ObjectExtractor::deserializeTree(quint8* idx,QVarList* varList,QHash<qint32,void*>* subTree,QTreeWidgetItem* parent=NULL) const{
    int count = varList->at((*idx)++).value<int>();

    for(int i=0;i<count;i++){
        int treeType = varList->at((*idx)++).value<int>();
        QString treeName = varList->at((*idx)++).value<QString>();
        quint64 treeAddr = varList->at((*idx)++).value<quint64>();

        if(treeType == ROOT){
            qDebug() << "deserialize ROOT:" << treeName << "," << treeAddr;
            QHash<qint32,void*>* subTree2 = new QHash<qint32,void*>();
            subTree2->insert(TREE_TYPE,new int(treeType));
            subTree2->insert(TREE_NAME,new QString(treeName));
            subTree2->insert(TREE_ADDRESS,new quint64(treeAddr));
            subTree->insert(i, subTree2);
            QTreeWidgetItem* newParent = dialog_->create(treeType,&treeName,treeAddr,parent);
            deserializeTree(idx,varList,subTree2,newParent);
        }else if(treeType == POINTER){
            qDebug() << "deserialize POINTER:" << treeName << "," << treeAddr;
            QTreeWidgetItem* newParent = dialog_->create(treeType,&treeName,treeAddr,parent);
            QHash<qint32,void*>* subTree2 = new QHash<qint32,void*>();
            deserializeTree(idx,varList,subTree2,newParent);
        }else{
            qDebug() << "deserialize value:" << treeName;
            dialog_->create(treeType,&treeName,treeAddr,parent);
        }
    }
}

void ObjectExtractor::parseTree(QVarList* varList,QHash<qint32,void*>* subTree) const{
    quint8 countMetaData;
    Q_FOREACH(qint32 key, subTree->keys()){
        if(key < 0){
            countMetaData++;
        }
    }

    varList->append(QVariant(subTree->count()-countMetaData));

    Q_FOREACH(qint32 key, subTree->keys()){
        if(key < 0){
            continue;
        }
        QHash<qint32,void*> subTree2 = *(QHash<qint32,void*>*)subTree->find(key).value();
        quint8 treeType = *(quint8*)(subTree2[TREE_TYPE]);
        varList->append(QVariant(treeType));
        if(treeType == POINTER){
            QString treeName = *(QString*)(subTree2[TREE_NAME]);
            quint64 treeAddr = *(quint64*)(subTree2[TREE_ADDRESS]);
            varList->append(QVariant(treeName));
            varList->append(QVariant(treeAddr));
            qDebug() << "serialize POINTER:" << treeName << "," << treeAddr;
            qDebug() << subTree2;
            parseTree(varList,&subTree2);
        }else{
            QString treeName = *(QString*)(subTree2[TREE_NAME]);
            qDebug() << "serialize value:" << treeName;
            qDebug() << subTree2;
            varList->append(QVariant(treeName));
            varList->append(QVariant(0));
        }
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ObjectExtractor, ObjectExtractor)
#endif
