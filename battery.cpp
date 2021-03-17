/* COMP 3004A Winter 2020
 * Team 9
 * Graham Airth, Hussien Dabarani, Rishi Parameswaran, Tom Roberts.
 *
 * A widget to show the battery level.
 * This is a separate widget, since the battery is pretty much a distinct object from the device,
 *   e.g., it can be taken out, replaced, etc.
 *
 */

#include "battery.h"
#include "ui_battery.h"

Battery::Battery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battery)
{
    ui->setupUi(this);

    setFixedSize(500,200);
    move(350,500);
    setWindowTitle("Battery");

    progressBar = new QProgressBar(this);
    progressBar->setRange(0,100);
    progressBar->setValue(int(batteryLevel));
    progressBar->setGeometry(100,50,300,100);

    pal = palette();

    // set green background
    pal.setColor(QPalette::Highlight, Qt::green);
    //progressBar->setAutoFillBackground(true);
    progressBar->setPalette(pal);
}

Battery::~Battery()
{
    delete ui;
    delete progressBar;
}

void Battery::updateTick(bool isPowerOn, bool isTreatmentStarted, int powerLevel)
{
    // Battery power should deplete as the device is being used.
    // Depletes faster when device powerLevel is higher during treatment.
    if (isPowerOn){
        batteryLevel -= 0.025 + 0.1*isTreatmentStarted*powerLevel/25.0;
        progressBar->setValue(int(batteryLevel));
        emit signalBatteryLevel(batteryLevel);
        if (int(batteryLevel)==19){
            pal.setColor(QPalette::Highlight, Qt::red);
            progressBar->setPalette(pal);
            //emit signalLowBattery();
        }
        /*
        else if (batteryLevel<3) {
            emit signalNotEnoughPower();
        } else if (int(batteryLevel)==10) {
            //emit signalLowBattery();  // Heck, signal a low battery, again.
        }
        */
    }
}

void Battery::setBatteryCharge(int value)
{
    // Lets the simulatorControl set the battery's charge,
    //   with it's slider.
    batteryLevel = value;
    progressBar->setValue(value);
    //emit signalBatteryLevel(batteryLevel);
    if (batteryLevel<=19){
        pal.setColor(QPalette::Highlight, Qt::red);
        progressBar->setPalette(pal);
    } else {
        pal.setColor(QPalette::Highlight, Qt::green);
        progressBar->setPalette(pal);
    }
}

/*
double Battery::getBatteryLevel()
{
    return batteryLevel;
}
*/
