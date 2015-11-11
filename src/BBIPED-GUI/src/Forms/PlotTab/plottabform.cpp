#include "plottabform.h"
#include "ui_plottabform.h"

PlotTabForm::PlotTabForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotTabForm)
{
    ui->setupUi(this);
}

PlotTabForm::~PlotTabForm()
{
    delete ui;
}
