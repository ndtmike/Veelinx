#ifndef CONTROL_DIALOG_H
#define CONTROL_DIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "inst_data.h"
//#include "remotecontrol.h"
//******************************************************************************
//  Definitions
//******************************************************************************
#define AMPLIFIER_GAIN_1_SETTING        0
#define AMPLIFIER_GAIN_5_SETTING        1
#define AMPLIFIER_GAIN_10_SETTING       2
#define AMPLIFIER_GAIN_25_SETTING       3
#define AMPLIFIER_GAIN_50_SETTING       4
#define AMPLIFIER_GAIN_100_SETTING      5
#define AMPLIFIER_GAIN_250_SETTING      6
#define AMPLIFIER_GAIN_500_SETTING      7
#define CALC_METHOD_ARB_MU              2
#define CALC_METHOD_DERIVED_MU          1
#define CALC_METHOD_SIMPLE_E            0
#define CYCLE_TIME_MAX                  10
#define CYCLE_TIME_MIN                  2
#define MAT_DENSITY_MAX                 500
#define MAT_DENSITY_MAX_MET             8010
#define MAT_DENSITY_MET_TO_IMP          0.062428
#define MAT_DENSITY_IMP_TO_MET          16
#define MAT_DENSITY_MIN                 50
#define MAT_DENSITY_MIN_MET             801
#define MAT_TRAVEL_DIST_MAX             600
#define MAT_TRAVEL_DIST_MAX_MET         15.240000
#define MAT_TRAVEL_DIST_MET_TO_IMP      39.370079
#define MAT_TRAVEL_DIST_IMP_TO_MET      254 / 10000
#define MAT_TRAVEL_DIST_MIN             0.1
#define MAT_TRAVEL_DIST_MIN_MET         0.002540
#define MAT_TRAVEL_VEL_MAX              40000
#define MAT_TRAVEL_VEL_MAX_MET          12191
#define MAT_TRAVEL_VEL_MET_TO_IMP       3.280851
#define MAT_TRAVEL_VEL_IMP_TO_MET       3047 / 10000
#define MAT_TRAVEL_VEL_MIN              1000
#define MAT_TRAVEL_VEL_MIN_MET          304
#define PICTURE_RATE_250MHZ             0
#define PICTURE_RATE_500MHZ             1
#define PICTURE_RATE_1000MHZ            2
#define PICTURE_RATE_2000MHZ            3
#define PULSE_CALC_DISTANCE             0
#define PULSE_CALC_VELOCITY             1
#define PULSES_PER_SEQ_1                1
#define PULSES_PER_SEQ_3                3
#define PULSES_PER_SEQ_10               10
#define REVIEW_TEST_NUM_MAX             1800
#define REVIEW_TEST_NUM_MIN             1
#define TEST_EXEC_NUM_MAX               1800
#define TEST_EXEC_NUM_MIN               1
#define UNITS_IMPERIAL                  0
#define UNITS_METRIC                    1

#define REMOTE_CTRL_HEADER              0x5A
#define REMOTE_CTRL_FOOTER              0xA3
#define REMOTE_CTRL_MSG_SIZE            5

#define MSG_CODE_PULSE_FREQ             0x00
#define MSG_CODE_PULSE_CYCLE_TIME       0x01
#define MSG_CODE_SAVE_TEST              0x02
#define MSG_CODE_DISPLAY_SIG            0x03
#define MSG_CODE_MSMT_MODE              0x04
#define MSG_CODE_DISTANCE               0x05
#define MSG_CODE_VELOCITY               0x06
#define MSG_CODE_RUN_A_TEST             0x07
#define MSG_CODE_AMP_GAIN               0x10
#define MSG_CODE_PICTURE_RATE           0x11
#define MSG_CODE_PULSER_VOLTAGE         0x12
#define MSG_CODE_HIVOLT                 0x01
#define MSG_CODE_LOVOLT                 0x00
#define MSG_CODE_WAVE_TYPE              0x20
#define MSG_CODE_COMPRESSION            0x00
#define MSG_CODE_SHEAR                  0x01
#define MSG_CODE_DENSITY                0x21
#define MSG_CODE_E_MU_CALC_METHOD       0x22
#define MSG_CODE_REVIEW_TEST_NUM        0x30
#define MSG_CODE_INITIAL_SETTINGS       0x40
#define MSG_CODE_WATCHDOG_RESET         0x50
#define MSG_CODE_FILL                   0xFF

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

    QByteArray BufferAmpGain;
    QByteArray BufferCalc;
    QByteArray BufferCaptureRate;
    QByteArray BufferDataSave;
    QByteArray BufferEMethod;
    QByteArray BufferPicSave;
    QByteArray BufferPulseRate;
    QByteArray BufferVolt;
    QByteArray BufferWaveType;

    bool CheckAmpGain;
    bool CheckCalc;
    bool CheckCaptureRate;
    bool CheckDataSave;
    bool CheckEMethod;
    bool CheckFormChange;
    bool CheckPicSave;
    bool CheckPulseRate;
    bool CheckVolt;
    bool CheckWaveType;

private slots:
    void EditAmpGain();
    void EditCalc();
    void EditCaptureRate();
    void EditDataSave();
    void EditEMethod();
    void EditPicSave();
    void EditPulseRate();
    void EditVolt();
    void EditWaveType();

private:
    Ui::Control_Dialog *ui;

    int CreateValue( QByteArray* working, bool* ok );
    unsigned ReviewNumber;

    DataSet::Prop Current_Prop;

    void ConvertToMetric( void );

    DataSet::AmpGain IntToAmpGain( int data_in, bool* ok );
    DataSet::Rate IntToCaptureRate( int data_in, bool* ok );
    unsigned IntToCycleTime( int data_in, bool* ok );
    bool IntToDataSave( int data_in, bool* ok );
    unsigned IntToDensity( char data_in_hi, char data_in_lo, bool* ok );
    DataSet::EMethod IntToEMethod( int data_in, bool* ok );
    DataSet::MeasMode IntToMeasMode( int data_in, bool* ok );
    unsigned IntToPTravelDistance( int data_in_hi, int data_in_lo, bool* ok );
    unsigned IntToPTravelVelocity( int data_in_hi, int data_in_lo, bool* ok );
    bool IntToPicSave( int data_in, bool* ok );
    DataSet::Pulse IntToPulse(int data_in, bool* ok );
    unsigned IntToReview( int data_in_hi, int data_in_lo, bool* ok );
    DataSet::Wave IntToWaveType( int data_in, bool* ok );
    DataSet::Units IntToUnits( int data_in, bool* ok );
    DataSet::Voltage IntToVoltage( int data_in, bool* ok );

    DataSet::AmpGain Ret_comboBoxAmpGain();
    DataSet::MeasMode Ret_comboBoxCalc();
    DataSet::Rate Ret_comboBoxCaptureRate();
    unsigned Ret_CycleTime();
    bool Ret_DataSave();
    unsigned Ret_Density();
    DataSet::EMethod Ret_comboBoxEMethod();
    unsigned Ret_PTravelDistance();
    unsigned Ret_PTravelVelocity();
    bool Ret_PicSave();
    DataSet::Pulse Ret_comboBoxPulseRate();
    DataSet::Wave Ret_comboBoxWaveType();
//    DataSet::Units Ret_comboBoxUnits();
    DataSet::Voltage Ret_Voltage();

    void Set_comboBoxAmpGain();
    void Set_comboBoxCalc();
    void Set_comboBoxCaptureRate();
    void Set_CycleTime();
    void Set_DataSave();
    void Set_Density();
    void Set_comboBoxEMethod();
    void Set_PTravelDistance();
    void Set_PTravelVelocity();
    void Set_PicSave();
    void Set_comboBoxPulseRate();
    void Set_comboBoxRun();
    void Set_comboBoxWaveType();
//    void Set_comboBoxUnits();
    void Set_Voltage();

//C++ #define
    static int Pulse_Index1(void) {return(0);}
    static int Pulse_Index3(void) {return(1);}
    static int Pulse_Index10(void) {return(2);}
    static int DataSave_Index_Off(void) {return(0);}
    static int DataSave_Index_On(void) {return(1);}

};

#endif // CONTROL_DIALOG_H
