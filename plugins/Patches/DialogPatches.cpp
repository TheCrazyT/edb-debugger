#include "DialogPatches.h"
#include "ui_DialogPatches.h"

DialogPatches::DialogPatches(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPatches)
{
    ui->setupUi(this);
}

DialogPatches::~DialogPatches()
{
    delete ui;
}
