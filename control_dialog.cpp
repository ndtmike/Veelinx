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
    Set_comboBoxEMethod();
    Set_comboBoxPulseRate();
    Set_comboBoxWaveType();
    Set_comboBoxUnits();

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

Control_Dialog::~Control_Dialog()
{
    delete ui;
}
void Control_Dialog::Set_comboBoxAmpGain(){

    ui->comboBoxAmpGain->addItem("1x Gain", DataSet::Gain_1);
    ui->comboBoxAmpGain->addItem("5x Gain", DataSet::Gain_5);
    ui->comboBoxAmpGain->addItem("10x Gain", DataSet::Gain_10);
    ui->comboBoxAmpGain->addItem("25x Gain", DataSet::Gain_25);
    ui->comboBoxAmpGain->addItem("50x Gain", DataSet::Gain_50);
    ui->comboBoxAmpGain->addItem("100x Gain", DataSet::Gain_100);
    ui->comboBoxAmpGain->addItem("250x Gain", DataSet::Gain_250);
    ui->comboBoxAmpGain->addItem("500x Gain", DataSet::Gain_500);

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

void Control_Dialog::Set_comboBoxEMethod()
{
    ui->comboBoxEMethod->addItem("Arb. Mu");
    ui->comboBoxEMethod->addItem("Derived Mu");
    ui->comboBoxEMethod->addItem("Simple E");
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

void Control_Dialog::Set_comboBoxUnits()
{
    ui->comboBoxUnits->addItem("Standard");
    ui->comboBoxUnits->addItem("Metric");
}


