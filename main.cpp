/* COMP 3004A Winter 2020
 * Team 9
 * Graham Airth, Hussien Dabarani, Rishi Parameswaran, Tom Roberts
 *
 * main() defines the top-level QApplication, contains the key simulator objects,
 *   and executes the app.
 * Defining the key objects here allows the application to have multiple windows,
 *   which arguably makes for a better user experience.
 *
 * It also defines the SIGNAL/SLOT connections between these objects.
 *
 */

#include <QApplication>
#include "simulatorcontrol.h"
#include "device.h"
#include "battery.h"
#include "electrode.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimulatorControl aSimControl(nullptr);    //= Window(nullptr,2);
    aSimControl.show();

    Device theDevice;
    Battery aBattery(nullptr);
    Electrode theElectrode(nullptr);

    aBattery.show();
    theElectrode.show();
    theDevice.show();


    QObject::connect(&theDevice, SIGNAL(powerChange(bool)), &aSimControl, SLOT (updateLCD(bool)));
    QObject::connect(&theDevice, SIGNAL(powerChange(bool)), &theElectrode, SLOT (updateElectrodeOutput(bool)));
    //QObject::connect(&theDevice, SIGNAL(treatmentChange(bool)), &theElectrode, SLOT (updateElectrodeInput(bool)));
    QObject::connect(&theDevice, SIGNAL(sendTick(bool, bool, int, int)), &theElectrode, SLOT (updateElectrodeOutput(bool, bool, int, int)));
    QObject::connect(&theDevice, SIGNAL(sendTick(bool, bool, int, int)), &aBattery, SLOT (updateTick(bool, bool, int)));    //&Battery::
    QObject::connect(&theDevice, SIGNAL(sendTick(bool, bool, int, int)), &aSimControl, SLOT (updateLCD(bool, bool, int, int)));
    QObject::connect(&theDevice, SIGNAL(signalDeviceApplied(bool)), &theElectrode, SLOT (setDeviceApplied(bool)));
    QObject::connect(&aSimControl, SIGNAL(signalDeviceApplied(bool)), &theDevice, SLOT (updateSensorInfo(bool)));
    QObject::connect(&aBattery, SIGNAL(signalBatteryLevel(double)), &theDevice, SLOT (updateBatteryLevel(double)));
    QObject::connect(&aSimControl, SIGNAL(signalBatteryCharge(int)), &aBattery, SLOT (setBatteryCharge(int)));

    //QObject::connect(&aBattery, SIGNAL(signalNotEnoughPower()), &theDevice, SLOT (updateNotEnoughPower()));
    //QObject::connect(&aBattery, SIGNAL(signalLowBattery()), &theDevice, SLOT (updateLowBattery()));


    return a.exec();
}
