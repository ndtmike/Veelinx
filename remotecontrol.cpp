#include "remotecontrol.h"

RemoteControl::RemoteControl(QWidget *parent) : QWidget(parent)
{

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
  char    buffer[6] = {0x5A, 0x40, 0xFF, 0xFF, 0xA3};
  int     initData[1000] = {0};
  int     point;

  // Send initial settings message and retrieve received message
  initTime = TRUE;
  comBufferPtr = 0;
  comReceivedData = false;
  Com_OpenComPort();
  Com_MarkStartTime();
  SioPuts(comPort, buffer, 5);
  while (Com_CheckForRxTimeOut() == false || comReceivedData == true)
  {
    Com_CheckForRxData('Z', 5);
  }

  // Convert the received data into decimal values
  if ((comRxMsg[0] == 0x5A) && (comRxMsg[1] == 0x40))
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

  // Initialize the parameters using the received data
  if ((comRxMsg[0] == 0x5A) && (comRxMsg[1] == 0x40))
  {
    // Initialize the pulses per sequence
    if (initData[0] == PULSES_PER_SEQ_3)
    {
      RadioButtonPulses1->Checked = false;
      RadioButtonPulses3->Checked = true;
      RadioButtonPulses10->Checked = false;
      pulsesPerSequence = PULSES_PER_SEQ_3;
    }
    else if (initData[0] == PULSES_PER_SEQ_10)
    {
      RadioButtonPulses1->Checked = false;
      RadioButtonPulses3->Checked = false;
      RadioButtonPulses10->Checked = true;
      pulsesPerSequence = PULSES_PER_SEQ_10;
    }
    else
    {
      RadioButtonPulses1->Checked = true;
      RadioButtonPulses3->Checked = false;
      RadioButtonPulses10->Checked = false;
      pulsesPerSequence = PULSES_PER_SEQ_1;
    }

    // Initialize the pulse cycle time
    if ((initData[1] > CYCLE_TIME_MIN) && (initData[1] < CYCLE_TIME_MAX))
    {
      pulseCycleTime = initData[1];
    }
    else
    {
      pulseCycleTime = CYCLE_TIME_MIN;
    }
    EditCycleTime->Text = EditCycleTime->Text.sprintf("%i", pulseCycleTime);

    // Initialize the data save setting
    if (initData[2] == 1)
    {
      RadioButtonSaveOff->Checked = false;
      RadioButtonSaveOn->Checked = true;
    }
    else
    {
      RadioButtonSaveOff->Checked = true;
      RadioButtonSaveOn->Checked = false;
    }

    // Initialize the picture display setting
    if (initData[3] == 1)
    {
      RadioButtonDontShowPict->Checked = false;
      RadioButtonShowPict->Checked = true;
    }
    else
    {
      RadioButtonDontShowPict->Checked = true;
      RadioButtonShowPict->Checked = false;
    }

    // Initialize the measurement mode
    if (initData[4] == PULSE_CALC_VELOCITY)
    {
      RadioButtonDist->Checked = false;
      RadioButtonVel->Checked = true;
      pulseCalculation = PULSE_CALC_VELOCITY;
    }
    else
    {
      RadioButtonDist->Checked = true;
      RadioButtonVel->Checked = false;
      pulseCalculation = PULSE_CALC_DISTANCE;
    }

    // Initialize the material travel distance
    materialTravelDistance = (((initData[5]) & 0xFF00) >> 8) + ((initData[6]) & 0x00FF);
    if (materialTravelDistance < MAT_TRAVEL_DIST_MIN)
    {
      materialTravelDistance = MAT_TRAVEL_DIST_MIN;
    }
    else if (materialTravelDistance > MAT_TRAVEL_DIST_MAX)
    {
      materialTravelDistance = MAT_TRAVEL_DIST_MAX;
    }

    // Initialize the material travel velocity
    materialTravelVelocity = (((initData[7]) & 0xFF00) >> 8) + ((initData[8]) & 0x00FF);
    if (materialTravelVelocity < MAT_TRAVEL_VEL_MIN)
    {
      materialTravelVelocity = MAT_TRAVEL_VEL_MIN;
    }
    else if (materialTravelVelocity > MAT_TRAVEL_VEL_MAX)
    {
      materialTravelVelocity = MAT_TRAVEL_VEL_MAX;
    }

    // Update the material travel parameter
    if (pulseCalculation == PULSE_CALC_VELOCITY)
    {
      EditMatTravel->Text = EditMatTravel->Text.sprintf("%i", materialTravelVelocity);
    }
    else
    {
      EditMatTravel->Text = EditMatTravel->Text.sprintf("%5.1f", materialTravelDistance);
    }

    // Initialize the amplifier gain
    RadioButtonGain1->Checked = false;
    RadioButtonGain5->Checked = false;
    RadioButtonGain10->Checked = false;
    RadioButtonGain25->Checked = false;
    RadioButtonGain50->Checked = false;
    RadioButtonGain100->Checked = false;
    RadioButtonGain250->Checked = false;
    RadioButtonGain500->Checked = false;
    if (initData[10] == AMPLIFIER_GAIN_500_SETTING)
    {
      RadioButtonGain500->Checked = true;
    }
    else if (initData[10] == AMPLIFIER_GAIN_250_SETTING)
    {
      RadioButtonGain250->Checked = true;
    }
    else if (initData[10] == AMPLIFIER_GAIN_100_SETTING)
    {
      RadioButtonGain100->Checked = true;
    }
    else if (initData[10] == AMPLIFIER_GAIN_50_SETTING)
    {
      RadioButtonGain50->Checked = true;
    }
    else if (initData[10] == AMPLIFIER_GAIN_25_SETTING)
    {
      RadioButtonGain25->Checked = true;
    }
    else if (initData[10] == AMPLIFIER_GAIN_10_SETTING)
    {
      RadioButtonGain10->Checked = true;
    }
    else if (initData[10] == AMPLIFIER_GAIN_5_SETTING)
    {
      RadioButtonGain5->Checked = true;
    }
    else
    {
      RadioButtonGain1->Checked = true;
    }

    // Initialize the picture rate
    RadioButtonRate250->Checked = false;
    RadioButtonRate500->Checked = false;
    RadioButtonRate1000->Checked = false;
    RadioButtonRate2000->Checked = false;
    if (initData[11] == PICTURE_RATE_2000MHZ)
    {
      RadioButtonRate2000->Checked = true;
    }
    else if (initData[11] == PICTURE_RATE_1000MHZ)
    {
      RadioButtonRate1000->Checked = true;
    }
    else if (initData[11] == PICTURE_RATE_500MHZ)
    {
      RadioButtonRate500->Checked = true;
    }
    else
    {
      RadioButtonRate250->Checked = true;
    }

    // Initialize the pulser voltage
    if (initData[12] == 128)
    {
      RadioButtonHV->Checked = true;
      RadioButtonLV->Checked = false;
    }
    else
    {
      RadioButtonHV->Checked = false;
      RadioButtonLV->Checked = true;
    }

    // Initialize the wave type
    if (initData[13] == 1)
    {
      RadioButtonCompr->Checked = false;
      RadioButtonTransv->Checked = true;

      // Update the material travel radio buttons for transverse waves
      RadioButtonDist->Caption = RadioButtonDist->Caption.sprintf("S-Distance");
      RadioButtonVel->Caption = RadioButtonVel->Caption.sprintf("S-Velocity");
    }
    else
    {
      RadioButtonCompr->Checked = true;
      RadioButtonTransv->Checked = false;

      // Update the material travel radio buttons for compression waves
      RadioButtonDist->Caption = RadioButtonDist->Caption.sprintf("P-Distance");
      RadioButtonVel->Caption = RadioButtonVel->Caption.sprintf("P-Velocity");
    }

    // Initialize the material density
    materialDensity = (((initData[14]) & 0xFF00) >> 8) + ((initData[15]) & 0x00FF);
    if (materialDensity < MAT_DENSITY_MIN)
    {
      materialDensity = MAT_DENSITY_MIN;
    }
    else if (materialDensity > MAT_DENSITY_MAX)
    {
      materialDensity = MAT_DENSITY_MAX;
    }
    EditMatDensity->Text = EditMatDensity->Text.sprintf("%i", materialDensity);

    // Initialize the calculation method
    if (initData[16] == CALC_METHOD_ARB_MU)
    {
      // Update calculation method radio buttons
      RadioButtonSimpleE->Checked = false;
      RadioButtonDerivedMu->Checked = false;
      RadioButtonArbitraryMu->Checked = true;

      // Change wave type to compression
      RadioButtonCompr->Checked = true;
      RadioButtonTransv->Checked = false;
    }
    else if (initData[16] == CALC_METHOD_DERIVED_MU)
    {
      // Update calculation method radio buttons
      RadioButtonSimpleE->Checked = false;
      RadioButtonDerivedMu->Checked = true;
      RadioButtonArbitraryMu->Checked = false;
    }
    else
    {
      // Update calculation method radio buttons
      RadioButtonSimpleE->Checked = true;
      RadioButtonDerivedMu->Checked = false;
      RadioButtonArbitraryMu->Checked = false;

      // Change wave type to compression
      RadioButtonCompr->Checked = true;
      RadioButtonTransv->Checked = false;
    }

    // Initialize the review data set number
    reviewTestNum = (((initData[17]) & 0xFF00) >> 8) + ((initData[18]) & 0x00FF);
    if (reviewTestNum < REVIEW_TEST_NUM_MIN)
    {
      reviewTestNum = REVIEW_TEST_NUM_MIN;
    }
    else if (reviewTestNum > REVIEW_TEST_NUM_MAX)
    {
      reviewTestNum = REVIEW_TEST_NUM_MAX;
    }
    EditReviewTestNumber->Text = EditReviewTestNumber->Text.sprintf("%i", reviewTestNum);

    // Initialize the units
    if (initData[19] == UNITS_METRIC)
    {
      units = UNITS_METRIC;

      // Update the material density
      materialDensity *= MAT_DENSITY_MET_TO_IMP;
      EditMatDensity->Text = EditMatDensity->Text.sprintf("%i", materialDensity);

      // Update the material travel parameters
      materialTravelVelocity *= MAT_TRAVEL_VEL_MET_TO_IMP;
      materialTravelDistance *= MAT_TRAVEL_DIST_MET_TO_IMP;
      if (pulseCalculation == PULSE_CALC_VELOCITY)
      {
        EditMatTravel->Text = EditMatTravel->Text.sprintf("%i", materialTravelVelocity);
      }
      else
      {
        EditMatTravel->Text = EditMatTravel->Text.sprintf("%8.5f", materialTravelDistance);
      }
    }
    else
    {
      units = UNITS_IMPERIAL;
    }
  }
  else
  {
    // Initialize global variables
    materialDensity = MAT_DENSITY_MIN;
    materialTravelDistance = MAT_TRAVEL_DIST_MIN;
    materialTravelVelocity = MAT_TRAVEL_VEL_MIN;
    pulseCalculation = PULSE_CALC_DISTANCE;
    pulseCycleTime = CYCLE_TIME_MIN;
    pulsesPerSequence = PULSES_PER_SEQ_1;
    reviewTestNum = REVIEW_TEST_NUM_MIN;
    testExecNumOfTests = TEST_EXEC_NUM_MIN;
    units = UNITS_IMPERIAL;

    // Initialize text boxes
    EditCycleTime->Text = EditCycleTime->Text.sprintf("%i", pulseCycleTime);
    EditMatDensity->Text = EditMatDensity->Text.sprintf("%i", materialDensity);
    EditMatTravel->Text = EditMatTravel->Text.sprintf("%5.1f", materialTravelDistance);
    EditReviewTestNumber->Text = EditReviewTestNumber->Text.sprintf("%i", reviewTestNum);
    EditTestsToRun->Text = EditTestsToRun->Text.sprintf("%i", testExecNumOfTests);

    // Initialize radio buttons
    RadioButtonArbitraryMu->Checked = false;
    RadioButtonCompr->Checked = true;
    RadioButtonDerivedMu->Checked = false;
    RadioButtonDist->Checked = true;
    RadioButtonDontShowPict->Checked = true;
    RadioButtonGain1->Checked = true;
    RadioButtonGain5->Checked = false;
    RadioButtonGain10->Checked = false;
    RadioButtonGain25->Checked = false;
    RadioButtonGain50->Checked = false;
    RadioButtonGain100->Checked = false;
    RadioButtonGain250->Checked = false;
    RadioButtonGain500->Checked = false;
    RadioButtonHV->Checked = false;
    RadioButtonLV->Checked = true;
    RadioButtonPulses1->Checked = true;
    RadioButtonPulses3->Checked = false;
    RadioButtonPulses10->Checked = false;
    RadioButtonRate250->Checked = true;
    RadioButtonRate500->Checked = false;
    RadioButtonRate1000->Checked = false;
    RadioButtonRate2000->Checked = false;
    RadioButtonSaveOff->Checked = true;
    RadioButtonSaveOn->Checked = false;
    RadioButtonShowPict->Checked = false;
    RadioButtonSimpleE->Checked = true;
    RadioButtonTransv->Checked = false;
    RadioButtonVel->Checked = false;
  }
  Com_CloseComPort();
  initTime = FALSE;
}
