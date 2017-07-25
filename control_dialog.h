 #ifndef CONTROL_DIALOG_H
#define CONTROL_DIALOG_H

#include <QDialog>
#include <QMessageBox>
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

    DataSet::Prop Return_Control_Dialog();

private slots:

private:
    Ui::Control_Dialog *ui;

    void Set_comboBoxAmpGain();
    void Set_comboBoxCalc();
    void Set_comboBoxCaptureRate();
    void Set_CycleTime();
    void Set_DataSave();
    void Set_Density();
    void Set_comboBoxEMethod();
    void Set_MaterialTravelDistance();
    void Set_MaterialTravelVelocity();
    void Set_PicSave();
    void Set_comboBoxPulseRate();
    void Set_comboBoxWaveType();
    void Set_comboBoxUnits();
    void Set_Voltage();

    DataSet::AmpGain Ret_comboBoxAmpGain();
    DataSet::Calc Ret_comboBoxCalc();
    DataSet::Rate Ret_comboBoxCaptureRate();
    unsigned Ret_CycleTime();
    bool Ret_DataSave();
    unsigned Ret_Density();
    DataSet::EMethod Ret_comboBoxEMethod();
    unsigned Ret_MaterialTravelDistance();
    unsigned Ret_MaterialTravelVelocity();
    bool Ret_PicSave();
    DataSet::Pulse Ret_comboBoxPulseRate();
    DataSet::Wave Ret_comboBoxWaveType();
    DataSet::Units Ret_comboBoxUnits();
    DataSet::Voltage Ret_Voltage();
};

#endif // CONTROL_DIALOG_H
