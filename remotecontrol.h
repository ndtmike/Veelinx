//******************************************************************************
//
//  Filename: RemoteControl.h
//
//  Copyright 2010-2017, NDT James Instruments Inc.  All rights reserved.
//
//  Description:
//  ============
//  This file contains all the #defines associated with the operation of the
//   Velocilinx's remote control panel.
//
//  Revision History:
//  =================
//  Date        Author         Description
//  ----------  -------------  -------------------------------------------------
//  12/22/2010  Charlie Green  Initial version
//  09/08/2011  Charlie Green  Changed name from V-Meter to Velocilinx
//  07/07/2017  Michael Hoag   Adapted fro QT and Changed Name to Veelinx
//
//******************************************************************************

#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QWidget>

#include "inst_data.h"

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
#define CYCLE_TIME_MIN                  0
#define MAT_DENSITY_MAX                 500
#define MAT_DENSITY_MAX_MET             8010
#define MAT_DENSITY_MET_TO_IMP          0.062428
#define MAT_DENSITY_MIN                 50
#define MAT_DENSITY_MIN_MET             801
#define MAT_TRAVEL_DIST_MAX             600
#define MAT_TRAVEL_DIST_MAX_MET         15.240000
#define MAT_TRAVEL_DIST_MET_TO_IMP      39.370079
#define MAT_TRAVEL_DIST_MIN             0.1
#define MAT_TRAVEL_DIST_MIN_MET         0.002540
#define MAT_TRAVEL_VEL_MAX              40000
#define MAT_TRAVEL_VEL_MAX_MET          12191
#define MAT_TRAVEL_VEL_MET_TO_IMP       3.280851
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


class RemoteControl : public QWidget
{
    Q_OBJECT
public:
    explicit RemoteControl(QWidget *parent = 0);
    void  InitializeRemoteControlForm(void);

signals:

public slots:


private:
/*    void   Com_CheckForDataSet(char firstChar, int timeout);
    void   Com_CheckForRxData(char firstChar, int timeout);
    bool   Com_CheckForRxTimeOut(void);
     void  Com_CloseComPort(void);
     void  Com_MarkStartTime(void);
     void  Com_OpenComPort(void);
     void  Com_SendMessage(char * txBuffer);
     void  SaveDataFile(char * vMeterData, char * fileName);
*/
    void GetData();
    DataSet::AmpGain InitAmpGain( int data_in );
    DataSet::Rate InitCaptureRate(int data_in);
    unsigned InitMaterialTravelDistance( int data_in_hi, int data_in_lo);
    unsigned InitMaterialTravelVelocity( int data_in_hi, int data_in_lo);
    DataSet::EMethod InitPropCalc( int data_in );
    unsigned InitPropDensity( int data_in_hi, int data_in_lo );
    DataSet::Pulse InitPulse(int data_in);
    unsigned InitPulseCycleTime( int data_in);
};

#endif // REMOTECONTROL_H
