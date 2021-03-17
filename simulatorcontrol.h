#ifndef SIMULATORCONTROL_H
#define SIMULATORCONTROL_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
/*
#include "mainwindow.h"
#include "battery.h"
#include "waveform.h"
*/

namespace Ui {
class SimulatorControl;
}

class SimulatorControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimulatorControl(QWidget *parent = nullptr);
    ~SimulatorControl();

public slots:
    void updateLCD(bool isPowerOn, bool isTreatmentStarted, int powerLevel, int frequency);
    void updateLCD(bool isPowerOn);

private:
    Ui::SimulatorControl *ui;
    //QPushButton *buttonApply;
    //QPushButton *buttonClose;
    bool isApplied;

private slots:
    void setBatteryCharge(int value);
    void setDeviceApplied();

signals:
    void signalBatteryCharge(int value);
    void signalDeviceApplied(bool isApplied);
};

#endif // SIMULATORCONTROL_H
