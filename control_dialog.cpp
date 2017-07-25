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
    ui->comboBoxVoltage->addItem("Lo");
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
