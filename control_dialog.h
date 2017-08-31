#ifndef CONTROL_DIALOG_H
#define CONTROL_DIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "inst_data.h"
#include "remotecontrol.h"
/*
#define AMPLIFIER_GAIN_1_SETTING        0
#define AMPLIFIER_GAIN_5_SETTING        1
#define AMPLIFIER_GAIN_10_SETTING       2
#define AMPLIFIER_GAIN_25_SETTING       3
#define AMPLIFIER_GAIN_50_SETTING       4
#define AMPLIFIER_GAIN_100_SETTING      5
#define AMPLIFIER_GAIN_250_SETTING      6
#define AMPLIFIER_GAIN_500_SETTING      7

#define PICTURE_RATE_250MHZ             0
#define PICTURE_RATE_500MHZ             1
#define PICTURE_RATE_1000MHZ            2
#define PICTURE_RATE_2000MHZ            3

#define CYCLE_TIME_MIN 0  // needs to be checked
#define CYCLE_TIME_MAX 10  //needs to be checked
#define MAT_DENSITY_MIN 100 //needs to be checked
#define MAT_DENSITY_MAX 4000 //needs to be checked
*/
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
    bool Set_Control_Dialog( const QByteArray );

private slots:

private:
    Ui::Control_Dialog *ui;

    DataSet::AmpGain CharToAmpGain( char data_in );
    DataSet::Calc CharToCalc( char data_in );
    DataSet::Rate CharToCaptureRate( char data_in );
    unsigned CharToCycleTime( char data_in_hi, char data_in_lo );
    bool CharToDataSave( char data_in );
    unsigned CharToDensity( char data_in_hi, char data_in_lo );
    DataSet::EMethod CharToEMethod( char data_in );
    unsigned CharToMaterialTravelDistance( char data_in_hi, char data_in_lo);
    unsigned CharToMaterialTravelVelocity( char data_in_hi, char data_in_lo);
    bool CharToPicSave( char data_in );
    DataSet::Pulse CharToPulse(char data_in);
    DataSet::Wave CharToWaveType( char data_in );
    DataSet::Units CharToUnits( char data_in );
    DataSet::Voltage CharToVoltage( char data_in );

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

};

#endif // CONTROL_DIALOG_H
