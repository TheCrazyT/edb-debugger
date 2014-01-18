#ifndef DIALOGOBJECTEXTRACTOR_H
#define DIALOGOBJECTEXTRACTOR_H

#include "edb.h"
#include <QDialog>
#include <QString>
#include <QHash>
#include <QTreeWidgetItem>

namespace Ui {
class DialogObjectExtractor;
}

class DialogObjectExtractor : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogObjectExtractor(QWidget *parent = 0);
    ~DialogObjectExtractor();
    void newObjVar(edb::address_t addr);
    void cleanUp(bool skipRoot=false);

private slots:
    void create(int type);
    void on_txtName_returnPressed();
    void on_btnDelete_clicked();
    void on_txtAddress_returnPressed();
    void on_checkBox_stateChanged(int arg1);

    void on_btnRefresh_clicked();

    void on_chkRefresh_stateChanged(int arg1);

private:
    quint64 getTypeSize(edb::address_t addr,int type);
    Ui::DialogObjectExtractor *ui;
    QString parseToString(void * buf,int type);
    void refresh();
    void refresh(QTreeWidgetItem* ti);
    void timerTimeout();
    QHash<int,int>* typeSizes_;
    QStringList typeListStr_;
    QList<quint8> typeList_;
    QList<QPushButton*>* typeButtons_;
    void refreshTypeButtons(edb::address_t addr);

public:
    QTreeWidgetItem* create(int type,QString* text,quint64 addr,QTreeWidgetItem* parent);
    QHash<quint32,QHash<qint32,void*>*>* objectTree;
};

#endif // DIALOGOBJECTEXTRACTOR_H
