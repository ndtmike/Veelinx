#ifndef CONTROL_DIALOG_H
#define CONTROL_DIALOG_H

#include <QDialog>
#include "inst_data.h"

namespace Ui {
class Control_Dialog;
}

class Control_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Control_Dialog(QWidget *parent = 0);
    ~Control_Dialog();

private slots:
//    void accept();
//    void reject();

private:
    Ui::Control_Dialog *ui;

    void Set_comboBoxAmpGain();
    void Set_comboBoxCalc();
    void Set_comboBoxCaptureRate();
    void Set_comboBoxEMethod();
    void Set_comboBoxPulseRate();
    void Set_comboBoxWaveType();
    void Set_comboBoxUnits();
};

#endif // CONTROL_DIALOG_H
