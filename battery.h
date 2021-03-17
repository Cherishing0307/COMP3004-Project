#ifndef BATTERY_H
#define BATTERY_H

#include <QWidget>
#include <QProgressBar>

namespace Ui {
class Battery;
}

class Battery : public QWidget
{
    Q_OBJECT

public:
    explicit Battery(QWidget *parent = nullptr);
    //double getBatteryLevel();
    ~Battery();

private:
    Ui::Battery *ui;
    QProgressBar *progressBar;
    QPalette pal;
    double batteryLevel = 100;

signals:
    void signalBatteryLevel(double batteryLevel);
    //void signalLowBattery();
    //void signalNotEnoughPower();

public slots:
    void updateTick(bool isPowerOn, bool isTreatmentStarted, int powerLevel);
    void setBatteryCharge(int value);
};

#endif // BATTERY_H
