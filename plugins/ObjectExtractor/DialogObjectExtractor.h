#ifndef DIALOGOBJECTEXTRACTOR_H
#define DIALOGOBJECTEXTRACTOR_H

#include "edb.h"
#include <QDialog>
class QString;
class QTreeWidgetItem;

namespace Ui {
class DialogObjectExtractor;
}

class DialogObjectExtractor : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogObjectExtractor(QWidget *parent = 0);
    ~DialogObjectExtractor();
    
private slots:
    void on_txtName_returnPressed();
    void on_btnCreate_clicked();
    void on_btnDelete_clicked();
    void on_txtAddress_returnPressed();
    void on_checkBox_stateChanged(int arg1);

    void on_btnRefresh_clicked();

private:
    Ui::DialogObjectExtractor *ui;
    QString parseToString(void * buf,int type);
    void refresh();
    void refresh(QTreeWidgetItem* ti);
    void timerTimeout();
    QHash<int,int>* typeSizes_;
};

#endif // DIALOGOBJECTEXTRACTOR_H
