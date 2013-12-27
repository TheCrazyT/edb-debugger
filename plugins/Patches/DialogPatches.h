#ifndef DIALOGPATCHES_H
#define DIALOGPATCHES_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class DialogPatches;
}

class DialogPatches : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPatches(QWidget *parent = 0);
    ~DialogPatches();
    
private slots:

    void updateList();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_clicked();

private:
    virtual void showEvent(QShowEvent *event);

private:
    Ui::DialogPatches *ui;
};

#endif // DIALOGPATCHES_H
