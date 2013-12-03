#ifndef DIALOGOBJECTEXTRACTOR_H
#define DIALOGOBJECTEXTRACTOR_H

#include <QDialog>

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

private:
    Ui::DialogObjectExtractor *ui;
};

#endif // DIALOGOBJECTEXTRACTOR_H
