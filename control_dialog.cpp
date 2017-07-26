#include "control_dialog.h"
#include "ui_control_dialog.h"

Control_Dialog::Control_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Control_Dialog)
{
    ui->setupUi(this);

    Set_comboBoxAmpGain();
    Set_comboBoxCalc();
    Set_comboBoxCaptureRate();
    Set_CycleTime();
    Set_DataSave();
    Set_Density();
    Set_comboBoxEMethod();
    Set_MaterialTravelDistance();
    Set_MaterialTravelVelocity();
    Set_PicSave();
    Set_comboBoxPulseRate();
    Set_comboBoxWaveType();
    Set_comboBoxUnits();
    Set_Voltage();

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

Control_Dialog::~Control_Dialog()
{
    delete ui;
}
/******************************************************************************

  Function: DataSet::AmpGain CharToAmpGain( char data_in )
  Description:
  ============
  This routine initializes the amplifier gain

  data_in coded as follows:
    #define AMPLIFIER_GAIN_1_SETTING        0
    #define AMPLIFIER_GAIN_5_SETTING        1
    #define AMPLIFIER_GAIN_10_SETTING       2
    #define AMPLIFIER_GAIN_25_SETTING       3
    #define AMPLIFIER_GAIN_50_SETTING       4
    #define AMPLIFIER_GAIN_100_SETTING      5
    #define AMPLIFIER_GAIN_250_SETTING      6
    #define AMPLIFIER_GAIN_500_SETTING      7
******************************************************************************/
DataSet::AmpGain Control_Dialog::InitAmpGain( char data_in ){

    DataSet::AmpGain return_amp_gain;

    if (data_in == AMPLIFIER_GAIN_500_SETTING)
    {
        return_amp_gain = DataSet::Gain_500;
    }
    else if (data_in == AMPLIFIER_GAIN_250_SETTING)
    {
        return_amp_gain = DataSet::Gain_250;
    }
    else if (data_in == AMPLIFIER_GAIN_100_SETTING)
    {
        return_amp_gain = DataSet::Gain_100;
    }
    else if (data_in == AMPLIFIER_GAIN_50_SETTING)
    {
        return_amp_gain = DataSet::Gain_50;
    }
    else if (data_in == AMPLIFIER_GAIN_25_SETTING)
    {
        return_amp_gain = DataSet::Gain_25;
    }
    else if (data_in == AMPLIFIER_GAIN_10_SETTING)
    {
        return_amp_gain = DataSet::Gain_10;;
    }
    else if (data_in == AMPLIFIER_GAIN_5_SETTING)
    {
        return_amp_gain = DataSet::Gain_5;;
    }
    else
    {
        return_amp_gain = DataSet::Gain_1;;
    }

    return(return_amp_gain);
}

/******************************************************************************

  Function: DataSet::Calc CharToCalc( char data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:
#define PULSE_CALC_DISTANCE             0
#define PULSE_CALC_VELOCITY             1
******************************************************************************/
DataSet::Calc Control_Dialog::CharToCalc( char data_in )
{

}

/******************************************************************************

  Function: DataSet::Rate CharToCaptureRate( char data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:
#define PICTURE_RATE_250MHZ             0
#define PICTURE_RATE_500MHZ             1
#define PICTURE_RATE_1000MHZ            2
#define PICTURE_RATE_2000MHZ            3
******************************************************************************/
DataSet::Rate Control_Dialog::CharToCaptureRate( char data_in ){

    DataSet::Rate return_rate;

    if (data_in == PICTURE_RATE_2000MHZ)
    {
        return_rate = DataSet::RATE_2000KHZ;
    }
    else if (data_in == PICTURE_RATE_1000MHZ)
    {
        return_rate = DataSet::RATE_2000KHZ;
    }
    else if (data_in == PICTURE_RATE_500MHZ)
    {
        return_rate = DataSet::RATE_500KHZ;
    }
    else
    {
        return_rate = DataSet::RATE_250KHZ;
    }
    return( return_rate );
}


/******************************************************************************

  Function: DataSet::Rate CharToCycleTime( char data_in_hi, char data_in_lo )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:
 #define PICTURE_RATE_250MHZ             0
 #define PICTURE_RATE_500MHZ             1
 #define PICTURE_RATE_1000MHZ            2
 #define PICTURE_RATE_2000MHZ            3
******************************************************************************/
unsigned Control_Dialog::CharToCycleTime( char data_in_hi, char data_in_lo )
{
    unsigned returntime;

    if ((data_in > CYCLE_TIME_MIN) && (data_in < CYCLE_TIME_MAX))
    {
      returntime = data_in;
    }
    else
    {
      returntime = CYCLE_TIME_MIN;
    }

    return( returntime );
}

/******************************************************************************

  Function: DataSet::Rate CharToDataSave( char data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:

******************************************************************************/
unsigned Control_Dialog::CharToCycleTime( char data_in_hi, char data_in_lo )
{

}

/******************************************************************************

  Function: unsigned CharToDensity( char data_in_hi, char data_in_lo )

  Description:
  ============
  This routine initializes the material density

  data_in coded as follows:
    #define MAT_DENSITY_MAX                 500
    #define MAT_DENSITY_MIN                 50
******************************************************************************/
unsigned Control_Dialog::CharToDensity( char data_in_hi, char data_in_lo ){

    unsigned return_density;

    // Initialize the material density
    return_density = (((data_in_hi) & 0xFF00) >> 8) + ((data_in_lo) & 0x00FF);
    if (return_density < MAT_DENSITY_MIN)
    {
        return_density = MAT_DENSITY_MIN;
    }
    else if (return_density > MAT_DENSITY_MAX)
    {
        return_density = MAT_DENSITY_MAX;
    }

    return( return_density );
}

/******************************************************************************

  Function: DataSet::EMethod CharToEMethod( int data_in )

  Description:
  ============
  This routine initializes the material density

  data_in coded as follows:
#define CALC_METHOD_ARB_MU              2
#define CALC_METHOD_DERIVED_MU          1
#define CALC_METHOD_SIMPLE_E            0
******************************************************************************/
DataSet::EMethod Control_Dialog::CharToEMethod(int data_in){

    DataSet::EMethod return_e;

    if (data_in == CALC_METHOD_ARB_MU)
    {
        return_e = DataSet::ArbMu;
    }
    else if (data_in == CALC_METHOD_DERIVED_MU)
    {
        return_e = DataSet::DerivedMu;
    }
    else
    {
        return_e = DataSet::SimpleE;
    }

    return(return_e);
}
//******************************************************************************
//
//  Function: unsigned CharToMaterialTravelDistance( int data_in_hi, int data_in_lo)
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
//  #define MAT_TRAVEL_DIST_MAX             600
//  #define MAT_TRAVEL_DIST_MIN             0.1
//******************************************************************************
unsigned RemoteControl::CharToMaterialTravelDistance( int data_in_hi, int data_in_lo)
{
    unsigned return_distance;

    return_distance = (((data_in_hi) & 0xFF00) >> 8) + ((data_in_lo) & 0x00FF);
    if (return_distance < MAT_TRAVEL_DIST_MIN)
    {
        return_distance = MAT_TRAVEL_DIST_MIN;
    }
    else if (return_distance > MAT_TRAVEL_DIST_MAX)
    {
        return_distance = MAT_TRAVEL_DIST_MAX;
    }

    return(return_distance);
}

//******************************************************************************
//
//  Function: unsigned CharToMaterialTravelVelocity( int data_in_hi, int data_in_lo)
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
//  #define MAT_TRAVEL_VEL_MAX              40000
//  #define MAT_TRAVEL_VEL_MIN              1000
//******************************************************************************
unsigned RemoteControl::InitMaterialTravelVelocity( int data_in_hi, int data_in_lo)
{
    unsigned return_velocity;

    return_velocity = (((data_in_hi) & 0xFF00) >> 8) + ((data_in_lo) & 0x00FF);
    if (return_velocity < MAT_TRAVEL_DIST_MIN)
    {
        return_velocity = MAT_TRAVEL_VEL_MIN;
    }
    else if (return_velocity > MAT_TRAVEL_VEL_MAX)
    {
        return_velocity = MAT_TRAVEL_VEL_MAX;
    }

    return(return_velocity);
}

/******************************************************************************

  Function: DataSet::Rate CharToDataSave( char data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:

******************************************************************************/
unsigned Control_Dialog::CharToPicSave( char data_in_hi )
{

}
/******************************************************************************
    Function: DataSet::Pulse CharToPulse(int data_in)

    Description:
    ============
    This routine initializes the pulse rate.  Mostly used to simplify the
    InitializeRemoteControlForm Function

    data_in coded as follows:
    #define PULSES_PER_SEQ_1                1
    #define PULSES_PER_SEQ_3                3
   #define PULSES_PER_SEQ_10               10
******************************************************************************/
DataSet::Pulse RemoteControl::CharToPulse(int data_in)
{
    DataSet::Pulse return_pulse;

    // Initialize the pulses per sequence
    if (data_in == PULSES_PER_SEQ_3)
    {
      return_pulse = DataSet::PulsePerSeq_3;
    }
    else if (data_in == PULSES_PER_SEQ_10)
    {
      return_pulse = DataSet::PulsePerSeq_10;
    }
    else
    {
      return_pulse = DataSet::PulsePerSeq_1;
    }

    return(return_pulse);
}

/******************************************************************************

  Function: DataSet::Rate CharToDataSave( char data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:

******************************************************************************/
DataSet::Wave Control_Dialog::CharToWaveType( char data_in_hi )
{

}

/******************************************************************************

  Function: DataSet::Rate CharToDataSave( char data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:

******************************************************************************/
 DataSet::Units Control_Dialog::CharToUnits( char data_in_hi )
{

}

/******************************************************************************

  Function: DataSet::Rate CharToDataSave( char data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:

******************************************************************************/
DataSet::Voltage Control_Dialog::CharToVoltage( char data_in_hi )
{

}
/******************************************************************************

  Function: Set_comboBoxAmpGain

  Description:
  ============
  Set Items in AmpGain comboBox

******************************************************************************/
void Control_Dialog::Set_comboBoxAmpGain(){

    ui->comboBoxAmpGain->addItem("1x Gain");
    ui->comboBoxAmpGain->addItem("5x Gain");
    ui->comboBoxAmpGain->addItem("10x Gain");
    ui->comboBoxAmpGain->addItem("25x Gain");
    ui->comboBoxAmpGain->addItem("50x Gain");
    ui->comboBoxAmpGain->addItem("100x Gain");
    ui->comboBoxAmpGain->addItem("250x Gain");
    ui->comboBoxAmpGain->addItem("500x Gain");

}

void Control_Dialog::Set_comboBoxCalc()
{
    ui->comboBoxCalcVar->addItem("Velocity");
    ui->comboBoxCalcVar->addItem("Distance");
}

void Control_Dialog::Set_comboBoxCaptureRate()
{
    ui->comboBoxCaptureRate->addItem("250 kHz");
    ui->comboBoxCaptureRate->addItem("500 kHz");
    ui->comboBoxCaptureRate->addItem("1.0 MHz");
    ui->comboBoxCaptureRate->addItem("2.0 MHz");
}

void Control_Dialog::Set_CycleTime()
{
    connect(ui->CycleTimeSpinBox, SIGNAL(valueChanged(int)),
            ui->horizontalSliderCycleTime,SLOT(setValue(int)));
    connect(ui->horizontalSliderCycleTime, SIGNAL(valueChanged(int)),
            ui->CycleTimeSpinBox, SLOT(setValue(int)));
}

void Control_Dialog::Set_DataSave()
{
    ui->comboBoxDataSave->addItem("Yes");
    ui->comboBoxDataSave->addItem("No");
}

void Control_Dialog::Set_Density()
{
    connect(ui->DensitySpinBox, SIGNAL(valueChanged(int)),
            ui->horizontalSliderDensity,SLOT(setValue(int)));
    connect(ui->horizontalSliderDensity, SIGNAL(valueChanged(int)),
            ui->DensitySpinBox, SLOT(setValue(int)));
}

void Control_Dialog::Set_comboBoxEMethod()
{
    ui->comboBoxEMethod->addItem("Arb. Mu");
    ui->comboBoxEMethod->addItem("Derived Mu");
    ui->comboBoxEMethod->addItem("Simple E");
}

void Control_Dialog::Set_MaterialTravelDistance()
{
    connect(ui->DistanceSpinBox, SIGNAL(valueChanged(int)),
            ui->horizontalSliderDistance,SLOT(setValue(int)));
    connect(ui->horizontalSliderDistance, SIGNAL(valueChanged(int)),
            ui->DistanceSpinBox, SLOT(setValue(int)));
}

void Control_Dialog::Set_MaterialTravelVelocity()
{
    connect(ui->VelocitySpinBox, SIGNAL(valueChanged(int)),
            ui->horizontalSliderVelocity,SLOT(setValue(int)));
    connect(ui->horizontalSliderVelocity, SIGNAL(valueChanged(int)),
            ui->VelocitySpinBox, SLOT(setValue(int)));
}

void Control_Dialog::Set_PicSave()
{
    ui->comboBoxPicSave->addItem("Yes");
    ui->comboBoxPicSave->addItem("No");
}

void Control_Dialog::Set_comboBoxPulseRate()
{
    ui->comboBoxPulseRate->addItem("1");
    ui->comboBoxPulseRate->addItem("3");
    ui->comboBoxPulseRate->addItem("10");
}

void Control_Dialog::Set_comboBoxWaveType()
{
    ui->comboBoxWaveType->addItem("P Wave");
    ui->comboBoxWaveType->addItem("S Wave");
}

void Control_Dialog::Set_Voltage()
{
    ui->comboBoxVoltage->addItem("Hi");
    ui->comboBoxVoltage->addItem("Low");
}

void Control_Dialog::Set_comboBoxUnits()
{
    ui->comboBoxUnits->addItem("Standard");
    ui->comboBoxUnits->addItem("Metric");
}

DataSet::AmpGain Control_Dialog::Ret_comboBoxAmpGain()
{
    int intvar;
    DataSet::AmpGain retampgain;
    intvar = ui->comboBoxAmpGain->currentIndex();

    switch (intvar) {
    case 0:
        retampgain = DataSet::Gain_1;
        break;
    case 1:
        retampgain = DataSet::Gain_5;
        break;
    case 2:
        retampgain = DataSet::Gain_10;
        break;
    case 3:
        retampgain = DataSet::Gain_25;
        break;
    case 4:
        retampgain = DataSet::Gain_50;
        break;
    case 5:
        retampgain = DataSet::Gain_100;
        break;
    case 6:
        retampgain = DataSet::Gain_250;
        break;
    case 7:
        retampgain = DataSet::Gain_500;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("AmpGain %1").arg(intvar));
        break;
    }
    return(retampgain);
}

DataSet::Calc Control_Dialog::Ret_comboBoxCalc()
{
    int intvar;
    DataSet::Calc retcalc;
    intvar = ui->comboBoxCalcVar->currentIndex();
    switch (intvar) {
    case 0:
        retcalc = DataSet::Vel;
        break;
    case 1:
        retcalc = DataSet::Dist;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("Calc %1").arg(intvar));
        break;
    }
    return(retcalc);
}

DataSet::Rate Control_Dialog::Ret_comboBoxCaptureRate()
{
    int intvar;
    DataSet::Rate retrate;
    intvar = ui->comboBoxCaptureRate->currentIndex();
    switch (intvar) {
    case 0:
        retrate = DataSet::RATE_250KHZ;
        break;
    case 1:
        retrate = DataSet::RATE_500KHZ;
        break;
    case 2:
        retrate = DataSet::RATE_1000KHZ;
        break;
    case 3:
        retrate = DataSet::RATE_2000KHZ;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("Capture Rate %1").arg(intvar));
        break;
    }
    return(retrate);
}

unsigned Control_Dialog::Ret_CycleTime()
{
    unsigned returnui = ui->CycleTimeSpinBox->value();
    return(returnui);
}

bool Control_Dialog::Ret_DataSave()
{
    bool returnbool;
    int intvar = ui->comboBoxDataSave->currentIndex();
    returnbool = intvar == 0 ? true : false;
    return(returnbool);
}

unsigned Control_Dialog::Ret_Density()
{
    unsigned returnui = ui->DensitySpinBox->value();
    return(returnui);
}

DataSet::EMethod Control_Dialog::Ret_comboBoxEMethod()
{
    int intvar;
    DataSet::EMethod retemethod;
    intvar = ui->comboBoxEMethod->currentIndex();
    switch (intvar) {
    case 0:
        retemethod = DataSet::ArbMu;
        break;
    case 1:
        retemethod = DataSet::DerivedMu;
        break;
    case 2:
        retemethod = DataSet::SimpleE;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("EMethod %1").arg(intvar));
        break;
    }
    return(retemethod);
}

unsigned Control_Dialog::Ret_MaterialTravelDistance()
{
    unsigned returnui = ui->DistanceSpinBox->value();
    return(returnui);
}

unsigned Control_Dialog::Ret_MaterialTravelVelocity()
{
    unsigned returnui = ui->VelocitySpinBox->value();
    return(returnui);
}

bool Control_Dialog::Ret_PicSave()
{
    bool returnbool;
    int intvar = ui->comboBoxPicSave->currentIndex();
    returnbool = intvar == 0 ? true : false;
    return(returnbool);
}

DataSet::Pulse Control_Dialog::Ret_comboBoxPulseRate()
{
    int intvar;
    DataSet::Pulse retpulse;
    intvar = ui->comboBoxPulseRate->currentIndex();
    switch (intvar) {
    case 0:
        retpulse = DataSet::PulsePerSeq_1;
        break;
    case 1:
        retpulse = DataSet::PulsePerSeq_3;
        break;
    case 2:
        retpulse = DataSet::PulsePerSeq_10;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("Pulse Rate %1").arg(intvar));
        break;
    }
    return(retpulse);
}

DataSet::Wave Control_Dialog::Ret_comboBoxWaveType()
{
    int intvar;
    DataSet::Wave retwave;
    intvar = ui->comboBoxWaveType->currentIndex();
    switch (intvar) {
    case 0:
        retwave = DataSet::PWave;
        break;
    case 1:
        retwave = DataSet::SWave;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("Wave Type %1").arg(intvar));
        break;
    }
    return(retwave);
}

DataSet::Units Control_Dialog::Ret_comboBoxUnits()
{
    int intvar;
    DataSet::Units retunits;
    intvar = ui->comboBoxUnits->currentIndex();
    switch (intvar) {
    case 0:
        retunits = DataSet::Imperial;
        break;
    case 1:
        retunits = DataSet::Metric;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("Units %1").arg(intvar));
        break;
    }
    return(retunits);
}

DataSet::Voltage Control_Dialog::Ret_Voltage()
{
    int intvar;
    DataSet::Voltage retvolt;
    intvar = ui->comboBoxUnits->currentIndex();
    switch (intvar) {
    case 0:
        retvolt = DataSet::Hi;
        break;
    case 1:
        retvolt = DataSet::Low;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("Units %1").arg(intvar));
        break;
    }
    return(retvolt);
}

DataSet::Prop Control_Dialog::Return_Control_Dialog()
{
    DataSet::Prop returnprop;

    returnprop.PropAmpGain = Ret_comboBoxAmpGain();
    returnprop.PropCalc = Ret_comboBoxCalc();
    returnprop.PropCaptureRate = Ret_comboBoxCaptureRate();
    returnprop.PropCycleTime = Ret_CycleTime();
    returnprop.PropDataSave = Ret_DataSave();
    returnprop.PropDensity = Ret_Density();
    returnprop.PropEMethod = Ret_comboBoxEMethod();
    returnprop.PropMaterialTravelDistance = Ret_MaterialTravelDistance();
    returnprop.PropMaterialTravelVelocity = Ret_MaterialTravelVelocity();
    returnprop.PropPicSave = Ret_PicSave();
    returnprop.PropPulseRate = Ret_comboBoxPulseRate();
    returnprop.PropWave = Ret_comboBoxWaveType();
    returnprop.PropUnits = Ret_comboBoxUnits();
    returnprop.PropVoltage = Ret_Voltage();

   return(returnprop);
}
