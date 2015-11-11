#ifndef PLOTTABFORM_H
#define PLOTTABFORM_H

#include <QWidget>

namespace Ui {
class PlotTabForm;
}

class PlotTabForm : public QWidget
{
    Q_OBJECT

public:
    explicit PlotTabForm(QWidget *parent = 0);
    ~PlotTabForm();

private:
    Ui::PlotTabForm *ui;
};

#endif // PLOTTABFORM_H
