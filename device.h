#ifndef DEVICE_H
#define DEVICE_H

#include <QTextBrowser>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>

namespace Ui {
class Device;
}

class Device : public QMainWindow
{
    Q_OBJECT
    friend class TestDevice1;

public:
    explicit Device(QWidget *parent = nullptr);
    ~Device();

public slots:
    void updateBatteryLevel(double batteryLevel);
    //void updateNotEnoughPower();
    //void updateLowBattery();

private:
    Ui::Device *ui;
    QPushButton *buttonOn;
    /*
    QPushButton *buttonUp;
    QPushButton *buttonDown;
    QPushButton *buttonLeft;
    QPushButton *buttonRight;
    QPushButton *buttonSelect;
    QTextBrowser *menu;
    */
    QStringList *mainMenu;
    QStringList *programMenu;
    QStringList *settingsMenu;
    QStringList *languageMenu;
    QStringList *colourMenu;
    QStringList *frequencyMenu;
    QStringList *childrenMenu;
    QPalette listPalette;

    QTimer *timer;
    bool isPowerOn = false;
    bool isTreatmentStarted = false;
    bool isLowBattery = false;
    bool isApplied = false;
    bool isFreqAlt = false;
    int powerLevel;
    int powerLevelPrev;
    int frequency, frequency2;
    int freqLevelDflt;
    int ticksFreq1, ticksFreq2, ticksCountdownFreqAlt;
    int ticksSincePowerButton;
    int ticksSincePowerPressed;
    int tickNumMS;
    int ticksNumPerSec;
    QString electrodeAppliedMsg = "Electrode applied to the skin.";
    QString colourScheme = "Factory";

private slots:
    void backOption();
    void nextOption();
    void previousOption();
    void selectOption();
    void powerIncrease();
    void powerDecrease();
    void slotOnClicked(bool checked);
    void updateTick();
    void initShutdown();
    void shutdownDevice();
    void updateDisplayMsg(QString msg);
    void updateDisplayMenu0(QStringList *aStringListMenu);
    void updateDisplayMenu1(QStringList *aStringListMenu);
    void updateDisplayTreatment(QString strTreatment);
    void updateSensorInfo(bool isApplied);
    void setTreatmentAlternating(int freq1, int freq2, int secFreq1, int secFreq2);
    void setColourScheme(QString colourOption);
    //void goodbyeMsg();

signals:
    void sendTick(bool isPowerOn, bool isTreatmentStarted, int powerLevel, int frequency);
    void powerChange(bool isPowerOn);
    void treatmentChange(bool isTreatmentStarted);
    void signalDeviceApplied(bool isApplied);
};

#endif // DEVICE_H
