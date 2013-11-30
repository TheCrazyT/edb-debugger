#ifndef DIALOGPATCHES_H
#define DIALOGPATCHES_H

#include <QDialog>

namespace Ui {
class DialogPatches;
}

class DialogPatches : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPatches(QWidget *parent = 0);
    ~DialogPatches();
    
private:
    Ui::DialogPatches *ui;
};

#endif // DIALOGPATCHES_H
