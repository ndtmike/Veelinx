#include "remotecontrol.h"

RemoteControl::RemoteControl(QWidget *parent) : QWidget(parent)
{

}

//******************************************************************************
//
//  Function: TRemoteControlForm::GetData()
//
//  Description:
//  ============
//  Gets data from the V-Meter Unit
//
//******************************************************************************
void TRemoteControlForm::GetData(void){
     char    buffer[6] = {0x5A, 0x40, 0xFF, 0xFF, 0xA3};
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
void TRemoteControlForm::InitializeRemoteControlForm(void)
{
    int initData[1000] = {0};
    Dataset::Prop current_prop;

  // Initialize the parameters using the received data
  if ((comRxMsg[0] == 0x5A) && (comRxMsg[1] == 0x40))
  {
    current_prop.Pulse = InitPulse( initData[0]);
    current_prop.CycleTime = InitPulseCycleTime( initData[1]);
    current_prop.PropDataSave = initData[2] == 1 ? true : false;
    current_prop.PropPicSave = initData[3] == 1 ? true : false;
    current_prop.PropCalc = initData[4] == PULSE_CALC_VELOCITY ? DataSet::Vel : DataSet::Dist;
    current_prop.MaterialTravelDistance = InitMaterialTravelDistance(initData[5], initData[6]);
    current_prop.MaterialTravelVelocity = InitMaterialTravelVelocity(initData[7], initData[8]);
    current_prop.PropAmpGain = InitAmpGain( initData[10]);
    current_prop.PropCasptureRate = InitCaptureRate(initData[11]);
    current_prop.Voltage = initData[12] == 128 ? DataSet::Hi : DataSet::Lo;
    current_prop.Wave = initData[13] == 1 ? DataSet::SWave : DataSet::PWave;
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
      current_prop.Pulse = DataSet::PulsePerSeq_3;
      current_prop.CycleTime = CYCLE_TIME_MIN;
      current_prop.PropDataSave = false;
      current_prop.PropPicSave = false;
      current_prop.PropCalc = DataSet::Vel ;
      current_prop.MaterialTravelDistance = MAT_TRAVEL_DIST_MIN;
      current_prop.MaterialTravelVelocity = MAT_TRAVEL_VEL_MIN;
      current_prop.PropAmpGain = DataSet::Gain_25;
      current_prop.PropCasptureRate = DataSet::RATE_500KHZ;
      current_prop.Voltage = DataSet::Hi;
      current_prop.Wave = DataSet::PWave;
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
//******************************************************************************/
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
//******************************************************************************/
DataSet::Rate RemoteControl::InitCaptureRate( int data_in ){

    DataSet::Rate return_rate;

    if (initData[11] == PICTURE_RATE_2000MHZ)
    {
        return_rate = DataSet::RATE_2000KHZ;
    }
    else if (initData[11] == PICTURE_RATE_1000MHZ)
    {
        return_rate = DataSet::RATE_2000KHZ;
    }
    else if (initData[11] == PICTURE_RATE_500MHZ)
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
unsigned RemotControl::InitMaterialTravelDistance( int data_in_hi, int data_in_lo)
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
unsigned RemotControl::InitMaterialTravelVelocity( int data_in_hi, int data_in_lo)
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
//
//  Function: unsigned InitPropCalc( int data_in )
//
//  Description:
//  ============
//  This routine initializes the material density
//
//  data_in coded as follows:
#define CALC_METHOD_ARB_MU              2
#define CALC_METHOD_DERIVED_MU          1
#define CALC_METHOD_SIMPLE_E            0
//******************************************************************************/
unsigned RemoteControl::InitPropCalc(int data_in){

    DataSet::EMethod return_e;

    if (initData[16] == CALC_METHOD_ARB_MU)
    {
        current_prop.PropCalc = DataSet::ArbMu;
    }
    else if (initData[16] == CALC_METHOD_DERIVED_MU)
    {
        current_prop.PropCalc = DataSet::DerivedMu;
    }
    else
    {
        current_prop.PropCalc = DataSet::SimpleE;
    }

    return(return_e);
}

/******************************************************************************
//
//  Function: unsigned InitPropDensity( int data_in_hi, int data_in_lo)
//
//  Description:
//  ============
//  This routine initializes the material density
//
//  data_in coded as follows:
    #define MAT_DENSITY_MAX                 500
    #define MAT_DENSITY_MIN                 50
//******************************************************************************/
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
    DataSet::Pulse returnPulse;

    // Initialize the pulses per sequence
    if (data_in == PULSES_PER_SEQ_3)
    {
      current_prop.Pulse = DataSet::PulsePerSeq_3;
    }
    else if (initData[0] == PULSES_PER_SEQ_10)
    {
      current_prop.Prop == DataSet::PulsePerSeq_10;
    }
    else
    {
      current_prop.Prop == DataSet::PulsePerSeq_1;
    }

    return(returnPulse);
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
      returntime = initData[1];
    }
    else
    {
      returntime = CYCLE_TIME_MIN;
    }

    return( returntime );
}


