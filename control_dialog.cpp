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
    Set_comboBoxMeasMode();
    Set_PTravelDistance();
    Set_PTravelVelocity();
    Set_PicSave();
    Set_comboBoxPulseRate();
    Set_comboBoxRun();
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

void Control_Dialog::ConvertToMetric( void ){
    ui->DensitySpinBox->setMinimum( MAT_DENSITY_MIN * MAT_DENSITY_IMP_TO_MET );
    ui->DensitySpinBox->setMaximum( MAT_DENSITY_MAX * MAT_DENSITY_IMP_TO_MET );
    ui->horizontalSliderDensity->setMinimum( MAT_DENSITY_MIN * MAT_DENSITY_IMP_TO_MET );
    ui->horizontalSliderDensity->setMaximum( MAT_DENSITY_MAX * MAT_DENSITY_IMP_TO_MET );

    ui->PVelocitySpinBox->setMaximum( MAT_TRAVEL_VEL_MAX
                                      *  MAT_TRAVEL_VEL_IMP_TO_MET );
    ui->PVelocitySpinBox->setMinimum( MAT_TRAVEL_VEL_MIN
                                      *  MAT_TRAVEL_VEL_IMP_TO_MET );
    ui->horizontalSliderPVelocity->setMaximum( MAT_TRAVEL_VEL_MAX
                                               *  MAT_TRAVEL_VEL_IMP_TO_MET );
    ui->horizontalSliderPVelocity->setMinimum( MAT_TRAVEL_VEL_MIN
                                               *  MAT_TRAVEL_VEL_IMP_TO_MET );

    ui->PDistanceSpinBox->setMaximum( MAT_TRAVEL_DIST_MAX
                                      * MAT_TRAVEL_DIST_IMP_TO_MET );
    ui->PDistanceSpinBox->setMinimum( MAT_TRAVEL_DIST_MIN
                                      * MAT_TRAVEL_DIST_IMP_TO_MET );
    ui->horizontalSliderPDistance->setMaximum( MAT_TRAVEL_DIST_MAX
                                               * MAT_TRAVEL_DIST_IMP_TO_MET );
    ui->horizontalSliderPDistance->setMinimum( MAT_TRAVEL_DIST_MIN
                                               * MAT_TRAVEL_DIST_IMP_TO_MET );
}

int Control_Dialog::CreateValue(QByteArray *working, bool *ok){

    int result = -1;
    *ok = false;

    if( working->indexOf( 0xff) != -1){
        QByteArray value = working->left( working->indexOf( 0xff));
        result = value.toInt( ok );
        *working = working->right( working->size() - value.size() - 1);
        *ok = true;
    }
    return( result );
}


/******************************************************************************

  Function: DataSet::AmpGain IntToAmpGain( char data_in )
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
DataSet::AmpGain Control_Dialog::IntToAmpGain( int data_in, bool* ok ){

    DataSet::AmpGain return_amp_gain = DataSet::Gain_1;
    *ok = false;    

    if(data_in >= AMPLIFIER_GAIN_1_SETTING && data_in <= AMPLIFIER_GAIN_500_SETTING ){
        switch (data_in){
        case AMPLIFIER_GAIN_500_SETTING:
            return_amp_gain = DataSet::Gain_500;
            *ok = true;
            break;
        case AMPLIFIER_GAIN_250_SETTING:
            return_amp_gain = DataSet::Gain_250;
            *ok = true;
            break;
        case AMPLIFIER_GAIN_100_SETTING:
            return_amp_gain = DataSet::Gain_100;
            *ok = true;
            break;
        case AMPLIFIER_GAIN_50_SETTING:
            return_amp_gain = DataSet::Gain_50;
            *ok = true;
            break;
        case AMPLIFIER_GAIN_25_SETTING:
            return_amp_gain = DataSet::Gain_25;
            *ok = true;
            break;
        case AMPLIFIER_GAIN_10_SETTING:
            return_amp_gain = DataSet::Gain_10;
            *ok = true;
            break;
        case AMPLIFIER_GAIN_5_SETTING:
            return_amp_gain = DataSet::Gain_5;
            *ok = true;
            break;
        default:
            return_amp_gain = DataSet::Gain_1;
            *ok = true;

        }
        ui->comboBoxAmpGain->setCurrentIndex( data_in );
    }

    return(return_amp_gain);
}

/******************************************************************************

  Function: DataSet::Rate IntToCaptureRate( char data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:
#define PICTURE_RATE_250MHZ             0
#define PICTURE_RATE_500MHZ             1
#define PICTURE_RATE_1000MHZ            2
#define PICTURE_RATE_2000MHZ            3
******************************************************************************/
DataSet::Rate Control_Dialog::IntToCaptureRate( int data_in, bool* ok ){

    DataSet::Rate return_rate = DataSet::RATE_2000KHZ;
    *ok = false;

    switch (data_in){
    case PICTURE_RATE_2000MHZ:
        return_rate = DataSet::RATE_2000KHZ;
        *ok = true;
        break;
    case PICTURE_RATE_1000MHZ:
        return_rate = DataSet::RATE_2000KHZ;
        *ok = true;
        break;
    case PICTURE_RATE_500MHZ:
        return_rate = DataSet::RATE_500KHZ;
        *ok = true;
        break;
    default:
        return_rate = DataSet::RATE_250KHZ;
        *ok = true;
    }
    ui->comboBoxCaptureRate->setCurrentIndex( data_in );
    return( return_rate );
}

/******************************************************************************

  Function: DataSet::Rate IntToCycleTime( char data_in_hi, char data_in_lo, bool* ok )
  Description:
  ============

******************************************************************************/
unsigned Control_Dialog::IntToCycleTime( int data_in, bool* ok )
{
    unsigned returntime = 0;

    if( ( data_in >= CYCLE_TIME_MIN ) && ( data_in <= CYCLE_TIME_MAX ) ){
        ui->CycleTimeSpinBox->setValue( data_in );
        if ((data_in >= CYCLE_TIME_MIN) && (data_in <= CYCLE_TIME_MAX)){
            returntime = ( unsigned ) data_in;
        }else{
            returntime = CYCLE_TIME_MIN;
        }
    }
    ui->CycleTimeSpinBox->setValue( returntime );
    *ok = true;
    return( returntime );
}

/******************************************************************************

  Function: DataSet::Calc IntToDataSave( char data_in )
  Description:
  ============
  This routine converts the DataSave Parameter

          1
******************************************************************************/
bool Control_Dialog::IntToDataSave( int data_in, bool* ok )
{
    bool returndatasave = false;
    *ok = false;
    if( data_in == 1 ){
        ui->comboBoxDataSave->setCurrentIndex( DataSave_Index_On() );
        returndatasave = true;
        *ok = true;
    }else  if( data_in == 0 ){
        ui->comboBoxDataSave->setCurrentIndex( DataSave_Index_Off() );
        returndatasave = false;
        *ok = true;
    }
    return( returndatasave );
}

/******************************************************************************

  Function: unsigned IntToDensity( char data_in_hi, char data_in_lo )

  Description:
  ============
  This routine initializes the material density

  data_in coded as follows:
    #define MAT_DENSITY_MAX                 500
    #define MAT_DENSITY_MIN                 50
******************************************************************************/
unsigned Control_Dialog::IntToDensity( char data_in_hi, char data_in_lo, bool* ok ){

    unsigned return_density;
    *ok = true;
    // Initialize the material density
    return_density =  ((int)data_in_hi * 0x0100) + (int)(data_in_lo);//move the hi-byte two places add the low byte

    if ( return_density < MAT_DENSITY_MIN )
    {
        return_density = MAT_DENSITY_MIN;
    }
    else if ( return_density > MAT_DENSITY_MAX )
    {
        return_density = MAT_DENSITY_MAX;
    }
    ui->DensitySpinBox->setValue( return_density );
    return( return_density );
}

/******************************************************************************

  Function: DataSet::EMethod IntToEMethod( int data_in )

  Description:
  ============
  This routine initializes the material density

  data_in coded as follows:
#define CALC_METHOD_ARB_MU              2
#define CALC_METHOD_DERIVED_MU          1
#define CALC_METHOD_SIMPLE_E            0
******************************************************************************/
DataSet::EMethod Control_Dialog::IntToEMethod( int data_in , bool* ok ){

    DataSet::EMethod return_e;
    *ok = false;
    if( data_in >= CALC_METHOD_SIMPLE_E && data_in <= CALC_METHOD_ARB_MU ){
        switch (data_in){
        case CALC_METHOD_ARB_MU:
            return_e = DataSet::ArbMu;
            break;
        case CALC_METHOD_DERIVED_MU:
            return_e = DataSet::DerivedMu;
            break;
        default:
        return_e = DataSet::SimpleE;
        }
        ui->comboBoxEMethod->setCurrentIndex( data_in );
    }
    return(return_e);
}

/******************************************************************************

  Function: DataSet::EMethod IntToMeasMode( int data_in )

  Description:
  ============
  This routine initializes the material density

  data_in coded as follows:

******************************************************************************/
DataSet::MeasMode Control_Dialog::IntToMeasMode( int data_in, bool* ok ){

    DataSet::MeasMode return_measmode;
    *ok = false;

    if( data_in == PULSE_CALC_DISTANCE ){
        ui->comboBoxMeasMode->setCurrentIndex( data_in );
        return_measmode = DataSet::Distance;
        *ok = true;
    }else if( data_in == PULSE_CALC_VELOCITY ){
        ui->comboBoxMeasMode->setCurrentIndex( data_in );
        return_measmode = DataSet::Velocity;
        *ok = true;
    }

    return( return_measmode );
}

//******************************************************************************
//
//  Function: unsigned IntToPTravelDistance( int data_in_hi, int data_in_lo)
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
//  #define MAT_TRAVEL_DIST_MAX             600
//  #define MAT_TRAVEL_DIST_MIN             0.1
//  needs to be chhecked with equipment
//******************************************************************************
unsigned Control_Dialog::IntToPTravelDistance( int data_in_hi, int data_in_lo, bool* ok )
{
    unsigned return_distance;
    return_distance =  (data_in_hi * 0x0100) + (data_in_lo);//move the hi-byte two places add the low byte

    if (return_distance < MAT_TRAVEL_DIST_MIN)
    {
        return_distance = MAT_TRAVEL_DIST_MIN;
    }
    else if (return_distance > MAT_TRAVEL_DIST_MAX)
    {
        return_distance = MAT_TRAVEL_DIST_MAX;
    }
    *ok = true;
    ui->PDistanceSpinBox->setValue( return_distance );

    return(return_distance);
}

//******************************************************************************
//
//  Function: unsigned IntToPTravelVelocity( int data_in_hi, int data_in_lo)
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
//  #define MAT_TRAVEL_VEL_MAX              40000
//  #define MAT_TRAVEL_VEL_MIN              1000
//  needs to be checked with equipment
//******************************************************************************
unsigned Control_Dialog::IntToPTravelVelocity( int data_in_hi, int data_in_lo, bool* ok )
{
    unsigned return_velocity;

    return_velocity =  (data_in_hi * 0x0100) + (data_in_lo);//move the hi-byte two places add the low byte

    if (return_velocity < MAT_TRAVEL_VEL_MIN)
    {
        return_velocity = MAT_TRAVEL_VEL_MIN;
    }
    else if (return_velocity > MAT_TRAVEL_VEL_MAX)
    {
        return_velocity = MAT_TRAVEL_VEL_MAX;
    }
    *ok = true;
    ui->PVelocitySpinBox->setValue( return_velocity );

    return(return_velocity);
}

/******************************************************************************

  Function: DataSet::Rate IntToPicSave( char data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:

******************************************************************************/
bool Control_Dialog::IntToPicSave( int data_in, bool* ok )
{
    bool returnpicsave = false;
    *ok = false;
    if( data_in == 1 ){
        ui->comboBoxPicSave->setCurrentIndex( DataSave_Index_On() ); //use the DataSaveIndex
        returnpicsave = true;
        *ok = true;
    } else if(data_in == 0 ){
        ui->comboBoxPicSave->setCurrentIndex( DataSave_Index_Off() ); //use the DataSaveIndex
        returnpicsave = false;
        *ok = true;
    }
    return( returnpicsave );}

/******************************************************************************
    Function: DataSet::Pulse IntToPulse( int data_in, , bool* ok )

    Description:
    ============
    This routine initializes the pulse rate.  Mostly used to simplify the
    InitializeRemoteControlForm Function

    data_in coded as follows:
    #define PULSES_PER_SEQ_1                1
    #define PULSES_PER_SEQ_3                3
   #define PULSES_PER_SEQ_10               10
******************************************************************************/
DataSet::Pulse Control_Dialog::IntToPulse(int data_in , bool* ok )
{
    DataSet::Pulse return_pulse = DataSet::PulsePerSeq_3;
    if(( data_in == PULSES_PER_SEQ_3 ) || ( data_in == PULSES_PER_SEQ_10 )
            || ( data_in == PULSES_PER_SEQ_1 )){
        *ok = true;
    // Initialize the pulses per sequence
        if (data_in == PULSES_PER_SEQ_3){
            return_pulse = DataSet::PulsePerSeq_3;
            ui->comboBoxPulseRate->setCurrentIndex( Pulse_Index3() );
        } else if (data_in == PULSES_PER_SEQ_10){
            return_pulse = DataSet::PulsePerSeq_10;
            ui->comboBoxPulseRate->setCurrentIndex( Pulse_Index10() );
        }else{
            return_pulse = DataSet::PulsePerSeq_1;
            ui->comboBoxPulseRate->setCurrentIndex( Pulse_Index1() );
        }
    }else{
        *ok = false;
    }

    return( return_pulse );
}

//******************************************************************************
//
//  Function: unsigned IntToReview( int data_in_hi, int data_in_lo)
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
// #define REVIEW_TEST_NUM_MAX             1800
// #define REVIEW_TEST_NUM_MIN             1
//******************************************************************************
unsigned Control_Dialog::IntToReview( int data_in_hi, int data_in_lo, bool* ok )
{
    unsigned return_velocity;

    return_velocity =  (data_in_hi * 0x0100) + (data_in_lo);//move the hi-byte two places add the low byte

    if (return_velocity < REVIEW_TEST_NUM_MIN )
    {
        return_velocity = REVIEW_TEST_NUM_MIN;
    }
    else if (return_velocity > REVIEW_TEST_NUM_MAX )
    {
        return_velocity = REVIEW_TEST_NUM_MAX;
    }
    *ok = true;

    return(return_velocity);
}

/******************************************************************************

  Function: DataSet::Rate IntToWaveType( char data_in, bool *ok )
  Description:
  ============
  This routine initializes the wave type

  data_in coded as follows:

******************************************************************************/
DataSet::Wave Control_Dialog::IntToWaveType( int data_in, bool* ok )
{
    DataSet::Wave return_wave = DataSet::PWave;
    *ok = false;
    if( ( data_in >= 0 ) && ( data_in <= ui->comboBoxWaveType->count() - 1 )){
        ui->comboBoxWaveType->setCurrentIndex( data_in );
        *ok = true;
    // Initialize the pulses per sequence
        return_wave = data_in == 0 ? DataSet::PWave : DataSet::SWave;
    }
    return( return_wave );
}

/******************************************************************************

  Function: DataSet::Rate IntToUnits( char data_in, bool *ok )
  Description:
  ============
  This routine initializes the units of measurement

  data_in coded as follows:

******************************************************************************/
 DataSet::Units Control_Dialog::IntToUnits( int data_in, bool* ok )
{
     DataSet::Units return_units = DataSet::Metric;
     *ok = false;
     if( ( data_in == UNITS_IMPERIAL ) || ( data_in == UNITS_METRIC )){
         ui->comboBoxUnits->setCurrentIndex( data_in );
         *ok = true;
     // Initialize the pulses per sequence
         return_units = data_in == 0 ? DataSet::Imperial : DataSet::Metric;
     }
     return( return_units );
}

/******************************************************************************

  Function: DataSet::Rate IntToVoltage( char data_in )
  Description:
  ============
  This routine initializes the pulser voltage

  data_in coded as follows:

******************************************************************************/
DataSet::Voltage Control_Dialog::IntToVoltage( int data_in, bool* ok )
{
    const int pulse_hi_volt = 128;
    const int pulse_hi_volt_pos = 1;
    const int pulse_lo_volt = 0;
    const int pulse_lo_volt_pos = 0;

    DataSet::Voltage return_voltage = DataSet::Hi;
    *ok = false;
    if( ( data_in == pulse_lo_volt ) || ( data_in == pulse_hi_volt )){
        if(data_in == pulse_hi_volt ){
            ui->comboBoxVoltage->setCurrentIndex( pulse_hi_volt_pos );
            return_voltage = DataSet::Hi;
        }else{
            ui->comboBoxVoltage->setCurrentIndex( pulse_lo_volt_pos );
            return_voltage = DataSet::Low;
        }
        *ok = true;
    }
    return( return_voltage );
}

DataSet::AmpGain Control_Dialog::Ret_comboBoxAmpGain()
{
    int intvar;
    DataSet::AmpGain retampgain;
    intvar = ui->comboBoxAmpGain->currentIndex();


    QByteArray ba500;
    ba500.resize( REMOTE_CTRL_MSG_SIZE );
    ba500[0] = REMOTE_CTRL_HEADER; ba500[1] =  MSG_CODE_AMP_GAIN;
    ba500[2] = AMPLIFIER_GAIN_500_SETTING;
    ba500[3] = MSG_CODE_FILL; ba500[4] = REMOTE_CTRL_FOOTER;
    QByteArray ba250;
    ba250.resize( REMOTE_CTRL_MSG_SIZE );
    ba250[0] = REMOTE_CTRL_HEADER; ba250[1] =  MSG_CODE_AMP_GAIN;
    ba250[2] = AMPLIFIER_GAIN_250_SETTING;
    ba250[3] = MSG_CODE_FILL; ba250[4] = REMOTE_CTRL_FOOTER;
    QByteArray ba100;
    ba100.resize( REMOTE_CTRL_MSG_SIZE );
    ba100[0] = REMOTE_CTRL_HEADER; ba100[1] =  MSG_CODE_AMP_GAIN;
    ba100[2] = AMPLIFIER_GAIN_100_SETTING;
    ba100[3] = MSG_CODE_FILL; ba100[4] = REMOTE_CTRL_FOOTER;
    QByteArray ba50;
    ba50.resize( REMOTE_CTRL_MSG_SIZE );
    ba50[0] = REMOTE_CTRL_HEADER; ba50[1] =  MSG_CODE_AMP_GAIN;
    ba50[2] = AMPLIFIER_GAIN_50_SETTING;
    ba50[3] = MSG_CODE_FILL; ba50[4] = REMOTE_CTRL_FOOTER;
    QByteArray ba25;
    ba25.resize( REMOTE_CTRL_MSG_SIZE );
    ba25[0] = REMOTE_CTRL_HEADER; ba25[1] =  MSG_CODE_AMP_GAIN;
    ba25[2] = AMPLIFIER_GAIN_25_SETTING;
    ba25[3] = MSG_CODE_FILL; ba25[4] = REMOTE_CTRL_FOOTER;
    QByteArray ba10;
    ba10.resize( REMOTE_CTRL_MSG_SIZE);
    ba10[0] = REMOTE_CTRL_HEADER; ba10[1] =  MSG_CODE_AMP_GAIN;
    ba10[2] = AMPLIFIER_GAIN_10_SETTING;
    ba10[3] = MSG_CODE_FILL; ba10[4] = REMOTE_CTRL_FOOTER;
    QByteArray ba5;
    ba5.resize( REMOTE_CTRL_MSG_SIZE );
    ba5[0] = REMOTE_CTRL_HEADER; ba5[1] =  MSG_CODE_AMP_GAIN;
    ba5[2] = AMPLIFIER_GAIN_5_SETTING;
    ba5[3] = MSG_CODE_FILL; ba5[4] = REMOTE_CTRL_FOOTER;
    QByteArray ba1;
    ba1.resize( REMOTE_CTRL_MSG_SIZE );
    ba1[0] = REMOTE_CTRL_HEADER; ba1[1] =  MSG_CODE_AMP_GAIN;
    ba1[2] = AMPLIFIER_GAIN_1_SETTING;
    ba1[3] = MSG_CODE_FILL; ba1[4] = REMOTE_CTRL_FOOTER;

    switch (intvar) {
    case 0:
        retampgain = DataSet::Gain_1;
        BufferAmpGain = ba1;
        break;
    case 1:
        retampgain = DataSet::Gain_5;
        BufferAmpGain = ba5;
        break;
    case 2:
        retampgain = DataSet::Gain_10;
        BufferAmpGain = ba10;
        break;
    case 3:
        retampgain = DataSet::Gain_25;
        BufferAmpGain = ba25;
        break;
    case 4:
        BufferAmpGain = ba50;
        retampgain = DataSet::Gain_50;
        break;
    case 5:
        BufferAmpGain = ba100;
        retampgain = DataSet::Gain_100;
        break;
    case 6:
        BufferAmpGain = ba250;
        retampgain = DataSet::Gain_250;
        break;
    case 7:
        BufferAmpGain = ba500;
        retampgain = DataSet::Gain_500;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("AmpGain %1").arg(intvar));
        break;
    }
    return(retampgain);
}

DataSet::MeasMode Control_Dialog::Ret_comboBoxCalc()
{
    int intvar;
    DataSet::MeasMode retcalc;

    QByteArray bdistance;
    bdistance.resize( REMOTE_CTRL_MSG_SIZE );
    bdistance[0] = REMOTE_CTRL_HEADER; bdistance[1] =  MSG_CODE_DISTANCE;
    bdistance[2] = 0;
    bdistance[3] = MSG_CODE_FILL; bdistance[4] = REMOTE_CTRL_FOOTER;
    QByteArray bvelocity;
    bvelocity.resize( REMOTE_CTRL_MSG_SIZE );
    bvelocity[0] = REMOTE_CTRL_HEADER; bvelocity[1] =  MSG_CODE_VELOCITY;
    bvelocity[2] = 0;
    bvelocity[3] = MSG_CODE_FILL; bvelocity[4] = REMOTE_CTRL_FOOTER;

    intvar = ui->comboBoxCalcVar->currentIndex();
    switch (intvar) {
    case 0:
        BufferCalc = bvelocity;
        retcalc = DataSet::Velocity;
        break;
    case 1:
        BufferCalc = bdistance;
        retcalc = DataSet::Distance;
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
    QByteArray bpr250;
    bpr250.resize( REMOTE_CTRL_MSG_SIZE );
    bpr250[0] = REMOTE_CTRL_HEADER; bpr250[1] =  MSG_CODE_PICTURE_RATE;
    bpr250[2] = PICTURE_RATE_250MHZ;
    bpr250[3] = MSG_CODE_FILL; bpr250[4] = REMOTE_CTRL_FOOTER;
    QByteArray bpr500;
    bpr500.resize( REMOTE_CTRL_MSG_SIZE );
    bpr500[0] = REMOTE_CTRL_HEADER; bpr500[1] =  MSG_CODE_PICTURE_RATE;
    bpr500[2] = PICTURE_RATE_500MHZ;
    bpr500[3] = MSG_CODE_FILL; bpr500[4] = REMOTE_CTRL_FOOTER;
    QByteArray bpr1000;
    bpr1000.resize( REMOTE_CTRL_MSG_SIZE );
    bpr1000[0] = REMOTE_CTRL_HEADER; bpr1000[1] =  MSG_CODE_PICTURE_RATE;
    bpr1000[2] = PICTURE_RATE_1000MHZ;
    bpr1000[3] = MSG_CODE_FILL; bpr1000[4] = REMOTE_CTRL_FOOTER;
    QByteArray bpr2000;
    bpr2000.resize( REMOTE_CTRL_MSG_SIZE );
    bpr2000[0] = REMOTE_CTRL_HEADER; bpr2000[1] =  MSG_CODE_PICTURE_RATE;
    bpr2000[2] = PICTURE_RATE_2000MHZ;
    bpr2000[3] = MSG_CODE_FILL; bpr2000[4] = REMOTE_CTRL_FOOTER;

    intvar = ui->comboBoxCaptureRate->currentIndex();
    switch (intvar) {
    case 0:
        BufferCaptureRate = bpr250;
        retrate = DataSet::RATE_250KHZ;
        break;
    case 1:
        BufferCaptureRate = bpr500;
        retrate = DataSet::RATE_500KHZ;
        break;
    case 2:
        BufferCaptureRate = bpr1000;
        retrate = DataSet::RATE_1000KHZ;
        break;
    case 3:
        BufferCaptureRate = bpr2000;
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
    QByteArray bearbmu;
    bearbmu.resize( REMOTE_CTRL_MSG_SIZE );
    bearbmu[0] = REMOTE_CTRL_HEADER; bearbmu[1] =  MSG_CODE_E_MU_CALC_METHOD;
    bearbmu[2] = CALC_METHOD_ARB_MU;
    bearbmu[3] = MSG_CODE_FILL; bearbmu[4] = REMOTE_CTRL_FOOTER;
    QByteArray bedermu;
    bedermu.resize( REMOTE_CTRL_MSG_SIZE );
    bedermu[0] = REMOTE_CTRL_HEADER; bedermu[1] =  MSG_CODE_E_MU_CALC_METHOD;
    bedermu[2] = CALC_METHOD_DERIVED_MU;
    bedermu[3] = MSG_CODE_FILL; bedermu[4] = REMOTE_CTRL_FOOTER;
    QByteArray bsimple;
    bsimple.resize( REMOTE_CTRL_MSG_SIZE );
    bsimple[0] = REMOTE_CTRL_HEADER; bsimple[1] =  MSG_CODE_E_MU_CALC_METHOD;
    bsimple[2] = CALC_METHOD_SIMPLE_E;
    bsimple[3] = MSG_CODE_FILL; bsimple[4] = REMOTE_CTRL_FOOTER;

    intvar = ui->comboBoxEMethod->currentIndex();
    switch (intvar) {
    case 0:
        BufferEMethod = bearbmu;
        retemethod = DataSet::ArbMu;
        break;
    case 1:
        BufferEMethod = bedermu;
        retemethod = DataSet::DerivedMu;
        break;
    case 2:
        BufferEMethod = bsimple;
        retemethod = DataSet::SimpleE;
        break;
    default:
        QMessageBox::information(this, "Control_Dialog" ,QString("EMethod %1").arg(intvar));
        break;
    }
    return(retemethod);
}

DataSet::Prop Control_Dialog::Return_Control_Dialog()
{
    DataSet::Prop returnprop;

    returnprop.PropAmpGain = Ret_comboBoxAmpGain();
    returnprop.PropCaptureRate = Ret_comboBoxCaptureRate();
    returnprop.PropCycleTime = Ret_CycleTime();
    returnprop.PropDataSave = Ret_DataSave();
    returnprop.PropDensity = Ret_Density();
    returnprop.PropEMethod = Ret_comboBoxEMethod();
    returnprop.PropPTravelDistance = Ret_PTravelDistance();
    returnprop.PropPTravelVelocity = Ret_PTravelVelocity();
    returnprop.PropPicSave = Ret_PicSave();
    returnprop.PropPulseRate = Ret_comboBoxPulseRate();
    returnprop.PropWave = Ret_comboBoxWaveType();
    returnprop.PropUnits = Ret_comboBoxUnits();
    returnprop.PropVoltage = Ret_Voltage();

    return(returnprop);
}

unsigned Control_Dialog::Ret_PTravelDistance()
{
    unsigned returnui = ui->PDistanceSpinBox->value();
    return(returnui);
}

unsigned Control_Dialog::Ret_PTravelVelocity()
{
    unsigned returnui = ui->PVelocitySpinBox->value();
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
    QByteArray bpr1;
    bpr1.resize( REMOTE_CTRL_MSG_SIZE );
    bpr1[0] = REMOTE_CTRL_HEADER; bpr1[1] =  MSG_CODE_PULSE_FREQ;//??
    bpr1[2] = PULSES_PER_SEQ_1;
    bpr1[3] = MSG_CODE_FILL; bpr1[4] = REMOTE_CTRL_FOOTER;
    QByteArray bpr3;
    bpr3.resize( REMOTE_CTRL_MSG_SIZE );
    bpr3[0] = REMOTE_CTRL_HEADER; bpr3[1] =  MSG_CODE_PULSE_FREQ;//??
    bpr3[2] = PULSES_PER_SEQ_3;
    bpr3[3] = MSG_CODE_FILL; bpr3[4] = REMOTE_CTRL_FOOTER;
    QByteArray bpr10;
    bpr10.resize( REMOTE_CTRL_MSG_SIZE );
    bpr10[0] = REMOTE_CTRL_HEADER; bpr10[1] =  MSG_CODE_PULSE_FREQ;//??
    bpr10[2] = PULSES_PER_SEQ_10;
    bpr10[3] = MSG_CODE_FILL; bpr10[4] = REMOTE_CTRL_FOOTER;
    intvar = ui->comboBoxPulseRate->currentIndex();
    switch (intvar) {
    case 0:
        BufferPulseRate = bpr1;
        retpulse = DataSet::PulsePerSeq_1;
        break;
    case 1:
        BufferPulseRate = bpr3;
        retpulse = DataSet::PulsePerSeq_3;
        break;
    case 2:
        BufferPulseRate = bpr10;
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


/******************************************************************************

  Function: Set_comboBoxCalc

  Description:
  ============
  Set Items

******************************************************************************/
void Control_Dialog::Set_comboBoxCalc()
{
    ui->comboBoxCalcVar->addItem("Velocity");
    ui->comboBoxCalcVar->addItem("Distance");
}


/******************************************************************************

  Function: Set_comboBoxCaptureRate

  Description:
  ============
  Set Items
******************************************************************************/
void Control_Dialog::Set_comboBoxCaptureRate()
{
    ui->comboBoxCaptureRate->addItem("250 kHz");
    ui->comboBoxCaptureRate->addItem("500 kHz");
    ui->comboBoxCaptureRate->addItem("1.0 MHz");
    ui->comboBoxCaptureRate->addItem("2.0 MHz");
}

/******************************************************************************

  Function: bool Set_Control_Dialog(const QByteArray)
  Description:
  ============
  Sets the parameter of the control dialog box from the incoming string

  data_in coded as follows:
  "Z@number_of_pulses ÿ repeat_time ÿ test_is_saved ÿ show_the_picture ÿ measurement_mode ÿ
    p_distance_high ÿ p_distance_low ÿ s_distance_high ÿ s_distance_low ÿ
    p_velocity_high ÿ p_velocity_low ÿ s_velocity_high ÿ s_velocity_low ÿ
    run_enable ÿ amp_gain_index ÿ picture_rate_index ÿ pulser_voltage ÿ wave_type ÿ density_hi ÿ density_lo
    e_type ÿ review_number_hi ÿ review_number_lo ÿ display_units"

  Sample string
  Z@10ÿ2ÿ0ÿ1ÿ0ÿ0ÿ0ÿ0ÿ45ÿ0ÿ7ÿ2ÿ128ÿ0ÿ0ÿ50ÿ0ÿ0ÿ1ÿ0ÿ£

******************************************************************************/
bool Control_Dialog::Set_Control_Dialog( const QByteArray header )
{
    bool ok = false;
    int buffer = -1;
    QByteArray working = header.right( header.size() - 2 );

    buffer = CreateValue( &working, &ok );
    if( ok == true){
    Current_Prop.PropPulseRate = IntToPulse( buffer, &ok );
    buffer = CreateValue( &working, &ok );
    if( ok == true ){
        Current_Prop.PropCycleTime = IntToCycleTime( buffer, &ok );
        buffer = CreateValue( &working, &ok );
        if( ok == true ){
            Current_Prop.PropDataSave = IntToDataSave( buffer, &ok  );
            buffer = CreateValue( &working, &ok );
            if( ok == true ){
                Current_Prop.PropPicSave = IntToPicSave( buffer, &ok );
                buffer = CreateValue( &working, &ok );
                if( ok == true ){
                    Current_Prop.PropMeaseMode = IntToMeasMode( buffer, &ok );
                    buffer = CreateValue( &working, &ok );
                    bool ok_low = false;
                    int buffer_low = CreateValue( &working, &ok_low );
                    if( ok == true && ok_low == true ){
                        Current_Prop.PropPTravelDistance =
                            IntToPTravelDistance( buffer, buffer_low, &ok  );
                            buffer = CreateValue( &working, &ok );
                            buffer_low = CreateValue( &working, &ok_low );
                            if( ok == true ){
                                Current_Prop.PropPTravelVelocity =
                                    IntToPTravelVelocity( buffer, buffer_low, &ok  );
                                    buffer = CreateValue( &working, &ok );
                                    if( ok == true ){
                                        Current_Prop.PropRun = (bool)false;
                                        buffer = CreateValue( &working, &ok );
                                        if( ok == true ){
                                            Current_Prop.PropAmpGain =
                                                IntToAmpGain( buffer, &ok );
                                                buffer = CreateValue( &working, &ok );
                                            if( ok == true ){
                                                Current_Prop.PropCaptureRate =
                                                    IntToCaptureRate( buffer, &ok );
                                                buffer = CreateValue( &working, &ok );
                                                if( ok == true ){
                                                    Current_Prop.PropVoltage =
                                                            IntToVoltage( buffer, &ok );
                                                    buffer = CreateValue( &working, &ok );
                                                    if( ok == true ){
                                                        Current_Prop.PropWave =
                                                            IntToWaveType( buffer, &ok );
                                                        buffer = CreateValue( &working, &ok );
                                                        buffer_low = CreateValue( &working, &ok_low );
                                                        if( ok == true ){
                                                            Current_Prop.PropDensity =
                                                                IntToDensity( buffer,
                                                                buffer_low, &ok );
                                                             buffer = CreateValue( &working, &ok );
                                                            if( ok == true ){
                                                                Current_Prop.PropEMethod =
                                                                        IntToEMethod( buffer, &ok );
                                                                buffer = CreateValue( &working, &ok );
                                                                buffer_low = CreateValue( &working, &ok );
                                                                if( ok == true && ok_low ){
                                                                    ReviewNumber = IntToReview( buffer,
                                                                         buffer_low, &ok);
                                                                    buffer = CreateValue( &working, &ok );
                                                                    if( ok == true ){
                                                                        Current_Prop.PropUnits =
                                                                                IntToUnits(buffer, &ok);
                                                                        if(Current_Prop.PropUnits == DataSet::Metric){
                                                                            ConvertToMetric();
                                                                        }

                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                   }
                        }
                    }
                }
            }
        }
    }
    }
    return( ok );
}

void Control_Dialog::Set_CycleTime()
{
    ui->CycleTimeSpinBox->setMinimum( CYCLE_TIME_MIN );
    ui->CycleTimeSpinBox->setMaximum( CYCLE_TIME_MAX );
    ui->horizontalSliderCycleTime->setMinimum( CYCLE_TIME_MIN );
    ui->horizontalSliderCycleTime->setMaximum( CYCLE_TIME_MAX );
    connect(ui->CycleTimeSpinBox, SIGNAL(valueChanged(int)),
            ui->horizontalSliderCycleTime,SLOT(setValue(int)));
    connect(ui->horizontalSliderCycleTime, SIGNAL(valueChanged(int)),
            ui->CycleTimeSpinBox, SLOT(setValue(int)));
}

void Control_Dialog::Set_DataSave()
{
    ui->comboBoxDataSave->addItem("No");
    ui->comboBoxDataSave->addItem("Yes");
}

void Control_Dialog::Set_Density()
{
    ui->DensitySpinBox->setMinimum( MAT_DENSITY_MIN );
    ui->DensitySpinBox->setMaximum( MAT_DENSITY_MAX );
    ui->horizontalSliderDensity->setMinimum( MAT_DENSITY_MIN );
    ui->horizontalSliderDensity->setMaximum( MAT_DENSITY_MAX );
    connect(ui->DensitySpinBox, SIGNAL(valueChanged(int)),
            ui->horizontalSliderDensity,SLOT(setValue(int)));
    connect(ui->horizontalSliderDensity, SIGNAL(valueChanged(int)),
            ui->DensitySpinBox, SLOT(setValue(int)));
}

void Control_Dialog::Set_comboBoxEMethod()
{
    ui->comboBoxEMethod->addItem("Simple E");
    ui->comboBoxEMethod->addItem("Derived Mu");
    ui->comboBoxEMethod->addItem("Arb. Mu");
}

void Control_Dialog::Set_comboBoxMeasMode()
{
    ui->comboBoxMeasMode->addItem("Velocity");
    ui->comboBoxMeasMode->addItem("Distance");
}

void Control_Dialog::Set_PTravelDistance()
{
    ui->PDistanceSpinBox->setMaximum( MAT_TRAVEL_DIST_MAX );
    ui->PDistanceSpinBox->setMinimum( MAT_TRAVEL_DIST_MIN );
    ui->horizontalSliderPDistance->setMaximum( MAT_TRAVEL_DIST_MAX );
    ui->horizontalSliderPDistance->setMinimum( MAT_TRAVEL_DIST_MIN );
    connect(ui->PDistanceSpinBox, SIGNAL(valueChanged(int)),
            ui->horizontalSliderPDistance,SLOT(setValue(int)));
    connect(ui->horizontalSliderPDistance, SIGNAL(valueChanged(int)),
            ui->PDistanceSpinBox, SLOT(setValue(int)));
}

void Control_Dialog::Set_PTravelVelocity()
{
    ui->PVelocitySpinBox->setMaximum( MAT_TRAVEL_VEL_MAX );
    ui->PVelocitySpinBox->setMinimum( MAT_TRAVEL_VEL_MIN );
    ui->horizontalSliderPVelocity->setMaximum( MAT_TRAVEL_VEL_MAX );
    ui->horizontalSliderPVelocity->setMinimum( MAT_TRAVEL_VEL_MIN );
    connect(ui->PVelocitySpinBox, SIGNAL( valueChanged( int )),
            ui->horizontalSliderPVelocity,SLOT( setValue( int )));
    connect(ui->horizontalSliderPVelocity, SIGNAL( valueChanged( int )),
            ui->PVelocitySpinBox, SLOT( setValue( int )));
}

void Control_Dialog::Set_PicSave()
{
    ui->comboBoxPicSave->addItem("No");
    ui->comboBoxPicSave->addItem("Yes");
}

void Control_Dialog::Set_comboBoxPulseRate()
{
    ui->comboBoxPulseRate->addItem("1");
    ui->comboBoxPulseRate->addItem("3");
    ui->comboBoxPulseRate->addItem("10");
}

void Control_Dialog::Set_comboBoxRun()
{
    ui->comboBoxRun->addItem("No");
    ui->comboBoxRun->addItem("Yes");
}

void Control_Dialog::Set_comboBoxWaveType()
{
    ui->comboBoxWaveType->addItem("P Wave");
    ui->comboBoxWaveType->addItem("S Wave");
}

void Control_Dialog::Set_Voltage()
{
    ui->comboBoxVoltage->addItem("Low");
    ui->comboBoxVoltage->addItem("Hi");
}

void Control_Dialog::Set_comboBoxUnits()
{
    ui->comboBoxUnits->addItem("Standard");
    ui->comboBoxUnits->addItem("Metric");
}
