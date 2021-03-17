/* COMP 3004A Winter 2020
 * Team 9
 * Tom Roberts, Graham Airth, Rishi Parameswaran, Hussien Dabarani
 *
 * The SimulatorControl is a class that manages certain top-level aspects of
 *   the simulator:
 *      -Applying the device to the skin
 *      -Shutting down the application (rather than just turning off the device)
 *      -Re-charging the battery
 *      -Keeping track of time on skin.
 *
 */

#include "simulatorcontrol.h"
#include "ui_simulatorcontrol.h"

SimulatorControl::SimulatorControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimulatorControl)
{
    ui->setupUi(this);

/*  // Putting things here didn't work, with the separate windows...!
 *
    MainWindow w;
    Battery aBattery; //(nullptr);      //&aDevice
    Waveform aWaveform; //(nullptr);    //&aDevice

    //aBattery.show();
    //aWaveform.show();
    //w.show();
    aBattery.setParent(this);
    aWaveform.setParent(this);
    w.setParent(this);

    QObject::connect(&w, SIGNAL(powerChange(bool)), this, SLOT (updateLCD(bool)));
    QObject::connect(&w, SIGNAL(powerChange(bool)), &aWaveform, SLOT (updateWave(bool)));
    //QObject::connect(&w, SIGNAL(treatmentChange(bool)), &aWaveform, SLOT (updateWaveform(bool)));
    QObject::connect(&w, SIGNAL(sendTick(bool, bool, int)), &aWaveform, SLOT (updateWave(bool, bool)));
    QObject::connect(&w, SIGNAL(sendTick(bool, bool, int)), &aBattery, SLOT (updateTick(bool, bool, int)));    //&Battery::
    QObject::connect(&w, SIGNAL(sendTick(bool, bool, int)), this, SLOT (updateLCD(bool, bool)));
    QObject::connect(this, SIGNAL(signalBatteryCharge(int)), &aBattery, SLOT (setBatteryCharge(int)));
    QObject::connect(this, SIGNAL(signalDeviceApplied(bool)), &aWaveform, SLOT (setDeviceApplied(bool)));
    QObject::connect(&aBattery, SIGNAL(signalNotEnoughPower()), &w, SLOT (updateNotEnoughPower()));
    QObject::connect(&aBattery, SIGNAL(signalLowBattery()), &w, SLOT (updateLowBattery()));
*/

    isApplied = false;
    setWindowTitle("Simulator Control");
    //setFixedSize(600,400);
    move(50,50);

    //buttonApply = new QPushButton("Apply", this);
    ui->buttonApply->setToolTip("Apply the device electrode to patient's skin");
    //ui->buttonApply->setGeometry(390,300,70,70);
    ui->buttonApply->setCheckable(true);
    ui->sliderBattery->setValue(100);

    //buttonClose = new QPushButton("Close", this);
    //ui->buttonClose->setGeometry(490,300,70,70);
    ui->buttonClose->setToolTip("Close simulator application.");

    QObject::connect(ui->buttonClose, SIGNAL (clicked(bool)), QApplication::instance(), SLOT (quit()));
    QObject::connect(ui->sliderBattery, SIGNAL (valueChanged(int)), this, SLOT (setBatteryCharge(int)));
    QObject::connect(ui->buttonApply, SIGNAL (clicked(bool)), this, SLOT (setDeviceApplied()));
}

SimulatorControl::~SimulatorControl()
{
    delete ui;
    //delete buttonApply;
    //delete buttonClose;
}

void SimulatorControl::setBatteryCharge(int value){
    // Charge the battery with the slider.
    emit signalBatteryCharge(value);
}

void SimulatorControl::setDeviceApplied(){
    // Signal that user has applied device/electrode to skin.
    if (!isApplied){
        isApplied = true;
    } else {
        isApplied = false;
    }
    emit signalDeviceApplied(isApplied);
}

void SimulatorControl::updateLCD(bool isPowerOn, bool isTreatmentStarted, int powerLevel, int frequency){
    // Update LCDs on SimulatorControl to show duration of treatment, and frequency.
    if (isPowerOn && isApplied && isTreatmentStarted){
        ui->durationLCD->display(ui->durationLCD->value() + 0.1);
        ui->durationLCD_2->display(ui->durationLCD_2->value() + 0.1);
    } else if (!isPowerOn){
        // Turned off, so reset session timer.
        ui->durationLCD->display(0);
        ui->durationLCD->display(0);
    }
    if (isPowerOn && isTreatmentStarted){
        ui->frequencyLCD->display(frequency);
    } else {
        ui->frequencyLCD->display(0);
    }
    if (isPowerOn){
        ui->powerLCD->display(powerLevel);
    } else {
        ui->powerLCD->display(0);
    }
}

void SimulatorControl::updateLCD(bool isPowerOn){
    // With a low-battery power-down,
    // the session timer ends up as 0.1??
    if (!isPowerOn){
        // Turned off, so reset session timer.
        ui->durationLCD->display(0);
        ui->frequencyLCD->display(0);
        ui->powerLCD->display(0);
        //ui->powerLCD->setEnabled(false);
    }
}

