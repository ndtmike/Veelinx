#include "remotecontrol.h"

RemoteControl::RemoteControl(QWidget *parent) : QWidget(parent)
{

}

//******************************************************************************
//
//  Function: RemoteControl::GetData()
//
//  Description:
//  ============
//  Gets data from the V-Meter Unit
//
//******************************************************************************
void RemoteControl::GetData(void){
     unsigned char    buffer[6] = {0x5A, 0x40, 0xFF, 0xFF, 0xA3};
     // Send initial settings message and retrieve received message
   /*  initTime = TRUE;
     comBufferPtr = 0;
     comReceivedData = false;
     Com_OpenComPort();
     Com_MarkStartTime();
     SioPuts(comPort, buffer, 5);
     while (Com_CheckForRxTimeOut() == false || comReceivedData == true)
     {
       Com_CheckForRxData('Z', 5);
     }
   */
     // Convert the received data into decimal values
   /*  if ((comRxMsg[0] == 0x5A) && (comRxMsg[1] == 0x40))
     {
       setPtr = 2;
     }
     else
     {
       setPtr = 0;
     }
     for (point = 0; ((point < 256) && (setPtr < 3000)); point++)
     {
       initData[point] = ConvertHex2Dec('ÿ', &comRxMsg[0]);
       setPtr++;                                                         // Skip over the separator character at the end of each data point
     }
   */

}

//******************************************************************************
//
//  Function: TRemoteControlForm::InitializeRemoteControlForm()
//
//  Description:
//  ============
//  This routine initializes the parameters for the remote control form.
//
//******************************************************************************
void RemoteControl::InitializeRemoteControlForm(void)
{
    int initData[1000] = {0};
    DataSet::Prop current_prop;

    char comRxMsg[2];
  // Initialize the parameters using the received data
  if ((comRxMsg[0] == 0x5A) && (comRxMsg[1] == 0x40))
  {
    current_prop.PropPulseRate = InitPulse( initData[0]);
    current_prop.PropCycleTime = InitPulseCycleTime( initData[1]);
    current_prop.PropDataSave = initData[2] == 1 ? true : false;
    current_prop.PropPicSave = initData[3] == 1 ? true : false;
    current_prop.PropCalc = initData[4] == PULSE_CALC_VELOCITY ? DataSet::Vel : DataSet::Dist;
    current_prop.PropMaterialTravelDistance = InitMaterialTravelDistance(initData[5], initData[6]);
    current_prop.PropMaterialTravelVelocity = InitMaterialTravelVelocity(initData[7], initData[8]);
    current_prop.PropAmpGain = InitAmpGain( initData[10]);
    current_prop.PropCaptureRate = InitCaptureRate(initData[11]);
    current_prop.PropVoltage = initData[12] == 128 ? DataSet::Hi : DataSet::Low;
    current_prop.PropWave = initData[13] == 1 ? DataSet::SWave : DataSet::PWave;
    current_prop.PropDensity = InitPropDensity( initData[14], initData[15]);
    current_prop.PropEMethod = InitPropCalc( initData[16]);
    current_prop.PropUnits = initData[19] == UNITS_METRIC ? DataSet::Metric : DataSet::Imperial;
    current_prop.PropMaterialTravelVelocity = current_prop.PropUnits == DataSet::Metric ?
                current_prop.PropMaterialTravelVelocity * MAT_TRAVEL_VEL_MET_TO_IMP:
                current_prop.PropMaterialTravelVelocity;
    current_prop.PropMaterialTravelDistance = current_prop.PropUnits == DataSet::Metric ?
                current_prop.PropMaterialTravelDistance * MAT_TRAVEL_DIST_MET_TO_IMP:
                current_prop.PropMaterialTravelDistance;

    // Initialize the review data set number
/*    reviewTestNum = (((initData[17]) & 0xFF00) >> 8) + ((initData[18]) & 0x00FF);
    if (reviewTestNum < REVIEW_TEST_NUM_MIN)
    {
      reviewTestNum = REVIEW_TEST_NUM_MIN;
    }
    else if (reviewTestNum > REVIEW_TEST_NUM_MAX)
    {
      reviewTestNum = REVIEW_TEST_NUM_MAX;
    }
    EditReviewTestNumber->Text = EditReviewTestNumber->Text.sprintf("%i", reviewTestNum);
*/
  }
  else
  {
      current_prop.PropPulseRate = DataSet::PulsePerSeq_3;
      current_prop.PropCycleTime = CYCLE_TIME_MIN;
      current_prop.PropDataSave = false;
      current_prop.PropPicSave = false;
      current_prop.PropCalc = DataSet::Vel ;
      current_prop.PropMaterialTravelDistance = MAT_TRAVEL_DIST_MIN;
      current_prop.PropMaterialTravelVelocity = MAT_TRAVEL_VEL_MIN;
      current_prop.PropAmpGain = DataSet::Gain_25;
      current_prop.PropCaptureRate = DataSet::RATE_500KHZ;
      current_prop.PropVoltage = DataSet::Hi;
      current_prop.PropWave = DataSet::PWave;
      current_prop.PropDensity = MAT_DENSITY_MIN;
      current_prop.PropEMethod = DataSet::ArbMu;
      current_prop.PropUnits = DataSet::Imperial;
  }
}

/******************************************************************************

  Function: unsigned InitAmpGain( int data_in )
  Description:
  ============
  This routine initializes the amplifier gain

  data_in coded as follows:
    #define AMPLIFIER_GAIN_1_SETTING        0
    #define AMPLIFIER_GAIN_5_SETTING
    #define AMPLIFIER_GAIN_10_SETTING       2
    #define AMPLIFIER_GAIN_25_SETTING       3
    #define AMPLIFIER_GAIN_50_SETTING       4
    #define AMPLIFIER_GAIN_100_SETTING      5
    #define AMPLIFIER_GAIN_250_SETTING      6
    #define AMPLIFIER_GAIN_500_SETTING      7
******************************************************************************/
DataSet::AmpGain RemoteControl::InitAmpGain( int data_in ){

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

  Function: unsigned InitCaptureRate( int data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:
#define PICTURE_RATE_250MHZ             0
#define PICTURE_RATE_500MHZ             1
#define PICTURE_RATE_1000MHZ            2
#define PICTURE_RATE_2000MHZ            3
******************************************************************************/
DataSet::Rate RemoteControl::InitCaptureRate( int data_in ){

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

//******************************************************************************
//
//  Function: unsigned InitMaterialTravelDistance( int data_in_hi, int data_in_lo)
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
//  #define MAT_TRAVEL_DIST_MAX             600
//  #define MAT_TRAVEL_DIST_MIN             0.1
//******************************************************************************
unsigned RemoteControl::InitMaterialTravelDistance( int data_in_hi, int data_in_lo)
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
//  Function: unsigned InitMaterialTravelVelocity( int data_in_hi, int data_in_lo)
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

  Function: unsigned InitPropCalc( int data_in )

  Description:
  ============
  This routine initializes the material density

  data_in coded as follows:
#define CALC_METHOD_ARB_MU              2
#define CALC_METHOD_DERIVED_MU          1
#define CALC_METHOD_SIMPLE_E            0
******************************************************************************/
DataSet::EMethod RemoteControl::InitPropCalc(int data_in){

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

/******************************************************************************

  Function: unsigned InitPropDensity( int data_in_hi, int data_in_lo)

  Description:
  ============
  This routine initializes the material density

  data_in coded as follows:
    #define MAT_DENSITY_MAX                 500
    #define MAT_DENSITY_MIN                 50
******************************************************************************/
unsigned RemoteControl::InitPropDensity(int data_in_hi, int data_in_lo){

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

//******************************************************************************
//
//  Function: DataSet::Pulse InitPulse(int data_in)
//
//  Description:
//  ============
//  This routine initializes the pulse rate.  Mostly used to simplify the
//  InitializeRemoteControlForm Function
//
//  data_in coded as follows:
//  #define PULSES_PER_SEQ_1                1
//  #define PULSES_PER_SEQ_3                3
//  #define PULSES_PER_SEQ_10               10
//******************************************************************************
DataSet::Pulse RemoteControl::InitPulse(int data_in)
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

//******************************************************************************
//
//  Function: unsigned InitPulseCycleTime(int data_in)
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
//  #define CYCLE_TIME_MAX                  10
//  #define CYCLE_TIME_MIN                  0
//******************************************************************************
unsigned RemoteControl::InitPulseCycleTime(int data_in)
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

  Function: unsigned ToIntAmpGain( int data_in )
  Description:
  ============
  This routine initializes the amplifier gain

  data_in coded as follows:
    #define AMPLIFIER_GAIN_1_SETTING        0
    #define AMPLIFIER_GAIN_5_SETTING
    #define AMPLIFIER_GAIN_10_SETTING       2
    #define AMPLIFIER_GAIN_25_SETTING       3
    #define AMPLIFIER_GAIN_50_SETTING       4
    #define AMPLIFIER_GAIN_100_SETTING      5
    #define AMPLIFIER_GAIN_250_SETTING      6
    #define AMPLIFIER_GAIN_500_SETTING      7
******************************************************************************/
int RemoteControl::ToIntAmpGain( DataSet::AmpGain data_in ){

    int return_ampgain;

    switch(data_in){

    case DataSet::Gain_500:
        return_ampgain = AMPLIFIER_GAIN_500_SETTING;
        break;
    case DataSet::Gain_250:
        return_ampgain = AMPLIFIER_GAIN_250_SETTING;
        break;
    case DataSet::Gain_100:
        return_ampgain = AMPLIFIER_GAIN_100_SETTING;
        break;
    case DataSet::Gain_50:
        return_ampgain = AMPLIFIER_GAIN_50_SETTING;
        break;
    case DataSet::Gain_25:
        return_ampgain = AMPLIFIER_GAIN_25_SETTING;
        break;
    case DataSet::Gain_10:
        return_ampgain = AMPLIFIER_GAIN_10_SETTING;
        break;
    case DataSet::Gain_5:
        return_ampgain = AMPLIFIER_GAIN_5_SETTING;
        break;
    case DataSet::Gain_1:
        return_ampgain = AMPLIFIER_GAIN_1_SETTING;
        break;

    }
    return(return_ampgain);
}

/******************************************************************************

  Function: int ToIntCaptureRate( DataSet::Rate data_in )
  Description:
  ============
  This routine initializes the capture rate

  data_in coded as follows:
#define PICTURE_RATE_250MHZ             0
#define PICTURE_RATE_500MHZ             1
#define PICTURE_RATE_1000MHZ            2
#define PICTURE_RATE_2000MHZ            3
******************************************************************************/
int RemoteControl::ToIntCaptureRate( DataSet::Rate data_in ){

    int return_rate;

    switch( data_in ){

    case DataSet::RATE_250KHZ:
        return_rate = PICTURE_RATE_250MHZ;
        break;
    case DataSet::RATE_500KHZ:
        return_rate = PICTURE_RATE_500MHZ;
        break;
    case DataSet::RATE_1000KHZ:
        return_rate = PICTURE_RATE_1000MHZ;
        break;
    case DataSet::RATE_2000KHZ:
        return_rate = PICTURE_RATE_2000MHZ;
        break;
    }

    return( return_rate );
}

//******************************************************************************
//
//  Function: ToIntMaterialTravelDistance( unsigned data_in, int* out_ptr )
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
//  #define MAT_TRAVEL_DIST_MAX             600
//  #define MAT_TRAVEL_DIST_MIN             0.1
//******************************************************************************
void RemoteControl::ToIntMaterialTravelDistance( unsigned data_in, int* out_ptr )
{
/*    unsigned return_distance;

    return_distance = (((data_in_hi) & 0xFF00) >> 8) + ((data_in_lo) & 0x00FF);
    if (return_distance < MAT_TRAVEL_DIST_MIN)
    {
        return_distance = MAT_TRAVEL_DIST_MIN;
    }
    else if (return_distance > MAT_TRAVEL_DIST_MAX)
    {
        return_distance = MAT_TRAVEL_DIST_MAX;
    }

    return(return_distance);*/
}

//******************************************************************************
//
//  Function: unsigned ToIntMaterialTravelVelocity( unsigned data_in_hi, int* out_ptr )
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
//  #define MAT_TRAVEL_VEL_MAX              40000
//  #define MAT_TRAVEL_VEL_MIN              1000
//******************************************************************************
void RemoteControl::ToIntMaterialTravelVelocity( unsigned data_in_hi, int* out_ptr )
{
/*    unsigned return_velocity;

    return_velocity = (((data_in_hi) & 0xFF00) >> 8) + ((data_in_lo) & 0x00FF);
    if (return_velocity < MAT_TRAVEL_DIST_MIN)
    {
        return_velocity = MAT_TRAVEL_VEL_MIN;
    }
    else if (return_velocity > MAT_TRAVEL_VEL_MAX)
    {
        return_velocity = MAT_TRAVEL_VEL_MAX;
    }

    return(return_velocity);*/
}

/******************************************************************************

  Function: int ToIntPropCalc( DataSet::EMethod data_in )

  Description:
  ============
  This routine initializes the material density

  data_in coded as follows:
#define CALC_METHOD_ARB_MU              2
#define CALC_METHOD_DERIVED_MU          1
#define CALC_METHOD_SIMPLE_E            0
******************************************************************************/
int RemoteControl::ToIntPropCalc( DataSet::EMethod data_in ){

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

/******************************************************************************

  Function: void ToIntPropDensity(unsigned data_in, int* out_ptr)

  Description:
  ============
  This routine initializes the material density

  data_in coded as follows:
    #define MAT_DENSITY_MAX                 500
    #define MAT_DENSITY_MIN                 50
******************************************************************************/
void RemoteControl::ToIntPropDensity(unsigned data_in, int* out_ptr){
/*
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

    return( return_density );*/
}

//******************************************************************************
//
//  Function: int ToIntPulse( DataSet::Pulse data_in )
//
//  Description:
//  ============
//  This routine initializes the pulse rate.  Mostly used to simplify the
//  InitializeRemoteControlForm Function
//
//  data_in coded as follows:
//  #define PULSES_PER_SEQ_1                1
//  #define PULSES_PER_SEQ_3                3
//  #define PULSES_PER_SEQ_10               10
//******************************************************************************
int RemoteControl::ToIntPulse( DataSet::Pulse data_in )
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

//******************************************************************************
//
//  Function: int ToIntPulseCycleTime( unsigned data_in )
//
//  Description:
//  ============
//  This routine initializes the cycle time between pulse sequences
//
//  data_in coded as follows:
//  #define CYCLE_TIME_MAX                  10
//  #define CYCLE_TIME_MIN                  0
//******************************************************************************
 int RemoteControl::ToIntPulseCycleTime( unsigned data_in )
{
    int returntime;

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


