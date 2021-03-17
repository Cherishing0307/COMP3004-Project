/* COMP 3004A Winter 2020
 * Team 9
 * Graham Airth, Hussien Dabarani, Rishi Parameswaran, Tom Roberts
 *
 * The Device displays the device as it would appear to the patient.
 * It includes the following:
 *      -Device UI:
 *          -Buttons
 *          -Display
 *      -Program/treatment selection
 *      -Timer for Qt event loop, which drives:
 *          -Battery depletion
 *          -Microcurrent waveform
 *      -Responses to battery level
 */

#include "device.h"
#include "ui_device.h"
#include "iostream"

Device::Device(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Device)
{
    ui->setupUi(this);

    setWindowTitle("Denas");
    setFixedSize(400,700);
    move(700,100);

    // Initialize certain state variables...
    powerLevel = 50; //25;
    powerLevelPrev = powerLevel;
    frequency = 60;
    freqLevelDflt = frequency;
    tickNumMS = 100;
    ticksSincePowerButton = 30;
    ticksNumPerSec = 1000/tickNumMS;

    buttonOn = new QPushButton("Turn On", this);
    buttonOn->setToolTip("Turn power on and off");   // Focus has to be on this window.
    buttonOn->setGeometry(160,600,80,80);
    buttonOn->setCheckable(true);
    buttonOn->setStyleSheet("color:darkred;");
    QObject::connect(buttonOn, SIGNAL (clicked(bool)), this, SLOT (slotOnClicked(bool)));

    // Pop-up messages, battery indicator, are initialized to be off.
    ui->labelScreenMsg->setVisible(false);
    ui->labelPower->setVisible(false);
    ui->labelPower->setText(QString("Power level: %1").arg(powerLevel));
    ui->batteryIndicator->setVisible(false);
    ui->batteryIndicator->setAlignment(Qt::AlignCenter);

    // Timer that runs event loop for all objects.
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTick()));

    // Define UI button functionality (apart from "ON").
    ui->listWidget->setCurrentRow(0);
    QObject::connect(ui->downButton, SIGNAL (clicked()), this, SLOT (nextOption()));
    QObject::connect(ui->upButton, SIGNAL (clicked()), this, SLOT (previousOption()));
    QObject::connect(ui->selectButton, SIGNAL (clicked()), this, SLOT (selectOption()));
    QObject::connect(ui->backButton, SIGNAL (clicked()), this, SLOT (backOption()));
    QObject::connect(ui->rightButton, SIGNAL (clicked()), this, SLOT (powerIncrease()));
    QObject::connect(ui->leftButton, SIGNAL (clicked()), this, SLOT (powerDecrease()));

    // Define menu and sub-menus.
    mainMenu = new QStringList;
    mainMenu->append("Programs");
    mainMenu->append("Frequency");
    mainMenu->append("Med");
    mainMenu->append("Screening");
    mainMenu->append("Children");
    mainMenu->append("Settings");
    ui->listWidget->setCurrentRow(0);

    frequencyMenu = new QStringList;
    //frequencyMenu->append("1.0-99Hz");
    frequencyMenu->append("10Hz");
    frequencyMenu->append("20Hz");
    frequencyMenu->append("60Hz");
    frequencyMenu->append("77Hz");
    frequencyMenu->append("125Hz");


    programMenu = new QStringList;
    programMenu->append("Allergy");
    programMenu->append("Pain");
    programMenu->append("Int.Pain");
    programMenu->append("Bloating");
    programMenu->append("Dystonia");
    programMenu->append("Gyn.Pain");
    //programMenu->append("Gynecology");
    programMenu->append("Hypotonia");
    programMenu->append("Head");
    programMenu->append("Throat");
    programMenu->append("Muscles");
    //programMenu->append("Diarrhea");
    //programMenu->append("ABD.Pain");
    programMenu->append("Constipation");
    programMenu->append("Trauma");


    settingsMenu = new QStringList;
    settingsMenu->append("Brightness");
    settingsMenu->append("Economy");
    settingsMenu->append("Recording");
    settingsMenu->append("Clock");
    settingsMenu->append("Alarm Clock");
    settingsMenu->append("Language");
    settingsMenu->append("Colour");
    settingsMenu->append("Volume");


    childrenMenu = new QStringList;
    childrenMenu->append("Up to 1 Year");
    childrenMenu->append("1-3 Years");
    childrenMenu->append("4-7 Years");
    childrenMenu->append("7-12 Years");

    languageMenu = new QStringList;
    languageMenu->append("English");
    languageMenu->append("French");
    languageMenu->append("German");
    languageMenu->append("Cantonese");

    colourMenu = new QStringList;
    colourMenu->append("Factory");
    colourMenu->append("Blue");
    colourMenu->append("Red");
    colourMenu->append("Green");
    colourMenu->append("Night");

    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->backButton->setEnabled(false);

    /*menu = new QTextBrowser;
    menu->setGeometry(160,600, 400,400);
    menu->setText("hello");*/


}

Device::~Device()
{
    delete ui;
/*  THESE AREN'T DELETED, BECAUSE THEY'RE COMPONENTS OF THE UI.
 *  The UI destructor handles them.
 *
    delete buttonUp;
    delete buttonDown;
    delete buttonLeft;
    delete buttonRight;
    delete buttonSelect;
    delete menu;
*/
    delete buttonOn;
    delete mainMenu;
    delete programMenu;
    delete settingsMenu;
    delete languageMenu;
    delete colourMenu;
    delete frequencyMenu;
    delete childrenMenu;
    delete timer;
}
void Device::backOption(){
    // Back-button. Depends on current place in menu.
    ui->labelScreenMsg->setVisible(false);
    isFreqAlt = false;

    if(settingsMenu->contains(ui->listWidget->currentItem()->text())){
        updateDisplayMenu0(mainMenu);

        // Reset scheme -- settings were ghosted.
        setColourScheme(colourScheme);
        /*
        QFont f = ui->listWidget->font();
        f.setItalic(false);
        QPalette p = ui->listWidget->palette();
        if (colourScheme=="Factory"){
            p.setColor(QPalette::Text, Qt::black);
        }
        ui->listWidget->setFont(f);
        ui->listWidget->setPalette(p);
        */
    }
    else if(programMenu->contains(ui->listWidget->currentItem()->text())){
        updateDisplayMenu0(mainMenu);
    }
    else if(frequencyMenu->contains(ui->listWidget->currentItem()->text())){
        updateDisplayMenu0(mainMenu);
    }
    else if(childrenMenu->contains(ui->listWidget->currentItem()->text())){
        updateDisplayMenu0(mainMenu);
    }
    else if(languageMenu->contains(ui->listWidget->currentItem()->text())){
        updateDisplayMenu0(settingsMenu);
    }
    else if(colourMenu->contains(ui->listWidget->currentItem()->text())){
        updateDisplayMenu0(settingsMenu);
        setColourScheme("Ghosted");
    }
    else if(ui->listWidget->currentItem()->text() == "Applying Program Treatment"){
        frequency = freqLevelDflt;
        isTreatmentStarted = false;
        updateDisplayMenu1(programMenu);
    }
    else if(ui->listWidget->currentItem()->text() == "Applying Frequency Treatment"){
        frequency = freqLevelDflt;
        isTreatmentStarted = false;
        updateDisplayMenu1(frequencyMenu);
    }
    else if(ui->listWidget->currentItem()->text() == "Applying MED Treatment"){
        powerLevel = powerLevelPrev;
        frequency = freqLevelDflt;
        isTreatmentStarted = false;
        updateDisplayMenu1(mainMenu);
    }
    else if(ui->listWidget->currentItem()->text() == "Applying Screening Treatment"){
        powerLevel = powerLevelPrev;
        frequency = freqLevelDflt;
        isTreatmentStarted = false;
        updateDisplayMenu1(mainMenu);
    }
    else if(ui->listWidget->currentItem()->text() == "Applying Child Treatment"){
        powerLevel = powerLevelPrev;
        frequency = freqLevelDflt;
        isTreatmentStarted = false;
        updateDisplayMenu1(childrenMenu);
    }
}

void Device::updateDisplayMenu0(QStringList *aStringListMenu){
    // Helper function to refresh display to show given menu.
    ui->listWidget->clear();
    ui->listWidget->addItems(*aStringListMenu);
    ui->listWidget->setCurrentRow(0);
}

void Device::updateDisplayMenu1(QStringList *aStringListMenu){
    // Helper function to refresh display to show given menu.
    updateDisplayMenu0(aStringListMenu);
    //emit treatmentChange(isTreatmentStarted);
    ui->upButton->setDisabled(false);
    ui->downButton->setDisabled(false);
}

void Device::updateDisplayTreatment(QString strTreatment){
    ui->listWidget->clear();
    ui->listWidget->addItem(strTreatment);
    ui->listWidget->setCurrentRow(0);
    ui->upButton->setDisabled(true);
    ui->downButton->setDisabled(true);
}

void Device::nextOption(){
    if(ui->listWidget->currentRow()<ui->listWidget->count()-1){
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow()+1);
    } else {
        ui->listWidget->setCurrentRow(0);
    }
}

void Device::previousOption(){
    if(ui->listWidget->currentRow()>0){
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow()-1);
    } else {
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    }
}
void Device::selectOption(){
    if(ui->listWidget->currentItem()->text() == ("Programs")){
        updateDisplayMenu0(programMenu);
        /*
        ui->listWidget->clear();
        ui->listWidget->addItems(*programMenu);
        ui->listWidget->setCurrentRow(0);
        */
    }
    else if(ui->listWidget->currentItem()->text() == ("Settings")){
        updateDisplayMenu0(settingsMenu);
        /*
        ui->listWidget->clear();
        ui->listWidget->addItems(*settingsMenu);
        ui->listWidget->setCurrentRow(0);
        */
        // Ghost-out the Settings options, because most are not implemented.
        if (colourScheme=="Factory"){
            setColourScheme("Ghosted");
        }
    }
    else if(ui->listWidget->currentItem()->text() == ("Frequency")){
        updateDisplayMenu0(frequencyMenu);
    }
    else if(ui->listWidget->currentItem()->text() == ("Language")){
        updateDisplayMenu0(languageMenu);
    }
    else if(ui->listWidget->currentItem()->text() == ("Colour")){
        updateDisplayMenu0(colourMenu);
        setColourScheme(colourScheme);
    }
    else if(ui->listWidget->currentItem()->text() == ("Children")){
        updateDisplayMenu0(childrenMenu);
    }
    else if(programMenu->contains(ui->listWidget->currentItem()->text())){
        QString progOption = ui->listWidget->currentItem()->text();

        // Define some program-mode treatments.
        // Ideally, each treatment could be its own class/subclass,
        //   to encapsulate all the details and just swap it out.
        // i.e. something like a State design pattern.
        if (progOption=="Allergy"){
            frequency = 77;
        } else if (progOption=="Pain"){
            setTreatmentAlternating(30, 85, 5, 2);
        } else if (progOption=="Int.Pain"){
            setTreatmentAlternating(30, 125, 5, 2);
        } else if (progOption=="Bloating" || progOption=="Dystonia"){
            setTreatmentAlternating(77, 10, 4, 4);
        } else if (progOption=="Throat" || progOption=="Diarrhea" || progOption=="Constipation"){
            frequency = 77;
        } else if (progOption=="Head"){
            frequency = 60;
        } else if (progOption=="Muscles"){
            setTreatmentAlternating(20, 40, 5, 3);
        } else if (progOption=="Trauma"){
            setTreatmentAlternating(35, 200, 5, 5);
        }

        updateDisplayTreatment("Applying Program Treatment");
        isTreatmentStarted=true;
        //emit treatmentChange(isTreatmentStarted);
        updateSensorInfo(isApplied);
    }
    else if(frequencyMenu->contains(ui->listWidget->currentItem()->text())){
        QString freqOption = ui->listWidget->currentItem()->text();
        if (freqOption=="1.0-99Hz"){
            frequency = 20;
            frequency2 = 90;
            isFreqAlt = true;
            ticksFreq1 = 2*ticksNumPerSec;
            ticksFreq2 = 2*ticksNumPerSec;
            ticksCountdownFreqAlt = ticksFreq1;
        } else if (freqOption=="10Hz"){
            frequency = 10;
        } else if (freqOption=="20Hz"){
            frequency = 20;
        } else if (freqOption=="60Hz"){
            frequency = 60;
        } else if (freqOption=="77Hz"){
            frequency = 77;
        } else if (freqOption=="125Hz"){
            frequency = 125;
        }
        updateDisplayTreatment("Applying Frequency Treatment");

        isTreatmentStarted=true;
        //emit treatmentChange(isTreatmentStarted);
        updateSensorInfo(isApplied);
    }
    else if(ui->listWidget->currentItem()->text()=="Med"){
        frequency = 10;
        powerLevelPrev = powerLevel;
        powerLevel = 5;
        updateDisplayTreatment("Applying MED Treatment");

        isTreatmentStarted=true;
        //emit treatmentChange(isTreatmentStarted);
        updateSensorInfo(isApplied);
    }
    else if(ui->listWidget->currentItem()->text()=="Screening"){
        frequency = 35;
        powerLevelPrev = powerLevel;
        powerLevel = 35;
        updateDisplayTreatment("Applying Screening Treatment");
        isTreatmentStarted=true;
        //emit treatmentChange(isTreatmentStarted);
        updateSensorInfo(isApplied);
    }
    else if(childrenMenu->contains(ui->listWidget->currentItem()->text())){
        QString chldrnOption = ui->listWidget->currentItem()->text();
        powerLevelPrev = powerLevel;
        if (chldrnOption=="Up to 1 Year"){
            powerLevel = 10;
            frequency = 7;
        } else if (chldrnOption=="1-3 Years"){
            powerLevel = 20;
            frequency = 20;
        } else if (chldrnOption=="4-7 Years"){
            powerLevel = 30;
            frequency = 35;
        } else if (chldrnOption=="7-12 Years"){
            powerLevel = 40;
            frequency = 50;
        }
        updateDisplayTreatment("Applying Child Treatment");
        isTreatmentStarted=true;
        //emit treatmentChange(isTreatmentStarted);
        updateSensorInfo(isApplied);
    }
    else if(colourMenu->contains(ui->listWidget->currentItem()->text())){
        // Change the device colour scheme!
        QString colourOption = ui->listWidget->currentItem()->text();
        setColourScheme(colourOption);
    }
}

void Device::setColourScheme(QString colourOption){
    // Helper function to set the colour scheme according to
    //   the supplied argument.
    listPalette = ui->listWidget->palette();
    QFont f = ui->listWidget->font();
    if (colourOption=="Ghosted"){
        f.setItalic(true);
        ui->listWidget->setFont(f);
        listPalette.setColor(QPalette::Text, Qt::lightGray);
        ui->listWidget->setPalette(listPalette);
        return;
    }
    colourScheme = colourOption;
    f.setItalic(false);
    ui->listWidget->setFont(f);
    if (colourOption=="Night" || colourOption=="Black"){
        listPalette.setColor(QPalette::Text, Qt::white);
        listPalette.setColor(QPalette::Base, Qt::black);
    } else if (colourOption=="Factory"){
        listPalette.setColor(QPalette::Text, Qt::black);
        listPalette.setColor(QPalette::Base, Qt::white);
    } else if (colourOption=="Blue"){
        listPalette.setColor(QPalette::Text, Qt::darkGreen);
        listPalette.setColor(QPalette::Base, Qt::cyan);
    } else if (colourOption=="Red"){
        listPalette.setColor(QPalette::Text, Qt::darkRed);
        listPalette.setColor(QPalette::Base, Qt::gray);
    } else if (colourOption=="Green"){
        listPalette.setColor(QPalette::Text, Qt::darkBlue);
        listPalette.setColor(QPalette::Base, Qt::green);
    }
    ui->listWidget->setPalette(listPalette);
}


void Device::setTreatmentAlternating(int freq1, int freq2, int secFreq1, int secFreq2)
{
    // Method to set parameters for a treatment with two alternating frequencies.
    frequency = freq1;
    frequency2 = freq2;
    isFreqAlt = true;
    ticksFreq1 = secFreq1*ticksNumPerSec;
    ticksFreq2 = secFreq2*ticksNumPerSec;
    ticksCountdownFreqAlt = ticksFreq1;
}

void Device::slotOnClicked(bool checked)
{
    // Fire-up the device, if checked is "true".
    // Initiate shutdown, if "false".
    if (checked) {
        isPowerOn = true;
        isLowBattery = false;
        isFreqAlt = false;
        frequency = 60;
        buttonOn->setStyleSheet("color:darkgreen;");
        buttonOn->setText("Turn Off");
        ui->labelScreenMsg->setText(electrodeAppliedMsg);
        ui->batteryIndicator->setVisible(true);
        setColourScheme(colourScheme);

        timer->start(tickNumMS);
        //emit powerChange(isPowerOn);
        ui->upButton->setEnabled(true);
        ui->downButton->setEnabled(true);
        ui->leftButton->setEnabled(true);
        ui->rightButton->setEnabled(true);
        ui->selectButton->setEnabled(true);
        ui->backButton->setEnabled(true);
        ui->listWidget->addItems(*mainMenu);
        ui->listWidget->setCurrentRow(0);

        setWindowTitle("Denas");
    } else {
        updateDisplayMsg("Good Health, COMP3004!");

        // Prevent weird issues by disabling on/off and back button,
        //  prior to shutting down.
        buttonOn->setEnabled(false);
        ui->backButton->setEnabled(false);
        isTreatmentStarted = false;
        //QTimer::singleShot(2000, this, SLOT(initShutdown()));
        QTimer::singleShot(1700, this, SLOT(shutdownDevice()));
    }
}

void Device::updateTick()
{
    // Events that run on the Qt Event Loop.
    // Main thing is to update tick for other classes.
    // But also handle right/left buttons to change power level.
    if (isPowerOn){
        int tmpFreq = frequency;
        // Allow for basic alternating frequencies.
        // Zero is the threshold between frequency 1 & 2.
        // ----Might be better to let electrode handle this?----
        if (isFreqAlt){
            if (ticksCountdownFreqAlt<=0 && ticksCountdownFreqAlt> -ticksFreq2){
                tmpFreq = frequency2;
            } else if (ticksCountdownFreqAlt<= -ticksFreq2) {
                ticksCountdownFreqAlt = ticksFreq1;
            }
            ticksCountdownFreqAlt -= 1;
        }
        emit sendTick(isPowerOn, isTreatmentStarted, powerLevel, tmpFreq);

        // Label showing power level has to be made non-visible.
        if (ticksSincePowerButton<2*ticksNumPerSec){
            ticksSincePowerButton += 1;
        } else if (ticksSincePowerButton==2*ticksNumPerSec){
            ticksSincePowerButton += 1;
            ui->labelPower->setVisible(false);
        }

        // Allow holding down a right/left button to change the power.
        // Must hold down for more than about half a second.
        if (ui->rightButton->isDown()){
            ticksSincePowerButton = 0;
            ticksSincePowerPressed += 1;
            if (ticksSincePowerPressed>0.4*ticksNumPerSec){
                ui->labelPower->setVisible(true);
                powerLevel = qMin(powerLevel+1,100);
                ui->labelPower->setText(QString("Power level: %1").arg(powerLevel));
            }
        } else if (ui->leftButton->isDown()){
            ticksSincePowerButton = 0;
            ticksSincePowerPressed += 1;
            if (ticksSincePowerPressed>0.4*ticksNumPerSec){
                ui->labelPower->setVisible(true);
                powerLevel = qMax(powerLevel-1,1);
                ui->labelPower->setText(QString("Power level: %1").arg(powerLevel));
            }
        } else {
            ticksSincePowerPressed = 0;
        }
    }
}


void Device::updateBatteryLevel(double batteryLevel){
    // If battery is too low, turn screen yellow.
    // If really low, shutdown.
    ui->batteryIndicator->setValue(int(batteryLevel));
    if (batteryLevel<20){
        if (!isLowBattery){
            isLowBattery = true;
            setWindowTitle("Low battery power!");
            listPalette = ui->listWidget->palette();
            listPalette.setColor(QPalette::Base, Qt::yellow);
            ui->listWidget->setPalette(listPalette);
        }
        if (batteryLevel<4){
            // There is not enough power.
            // Shutdown.
            buttonOn->setChecked(false);

            updateDisplayMsg("Not enough battery power... shutting down :(");
            // Prevent weird issues by disabling on/off and back button.
            buttonOn->setEnabled(false);
            ui->backButton->setEnabled(false);
            isTreatmentStarted = false;
            QTimer::singleShot(2000, this, SLOT(shutdownDevice()));
        }
    } else {
        if (isLowBattery){
            isLowBattery = false;
            setWindowTitle("Denas");
            setColourScheme(colourScheme);
        }
    }
}

void Device::initShutdown(){
    // Sending a goodbye message this way isn't working...
    updateDisplayMsg("Good Health, COMP3004!");
    QTimer::singleShot(1700, this, SLOT(shutdownDevice()));
}

void Device::shutdownDevice(){
    // Function to disable buttons, clear display,
    //   and put device in an off-state.
    timer->stop();
    buttonOn->setStyleSheet("color:darkred;");
    buttonOn->setText("Turn On");
    buttonOn->setEnabled(true);
    isPowerOn = false;
    isTreatmentStarted = false;
    isFreqAlt = false;
    emit treatmentChange(isTreatmentStarted);
    emit powerChange(isPowerOn);
    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->backButton->setEnabled(false);

    // Make sure display is clear.
    ui->listWidget->clear();
    setWindowTitle("Denas");
    listPalette = ui->listWidget->palette();
    listPalette.setColor(QPalette::Base, Qt::white);
    ui->listWidget->setPalette(listPalette);

    ui->labelScreenMsg->setVisible(false);
    ui->labelPower->setVisible(false);
    ui->batteryIndicator->setVisible(false);
}


void Device::updateDisplayMsg(QString msg){
    ui->listWidget->clear();
    ui->labelPower->setVisible(false);
    ui->labelScreenMsg->setVisible(true);
    ui->labelScreenMsg->setText(msg);

    /*
    for (int i=0; i<5; i++){
        ui->listWidget->addItem("");  // Add some space
    }
    QListWidgetItem *itemMsg = new QListWidgetItem;
    itemMsg->setText(msg);
    itemMsg->setTextAlignment(Qt::AlignHCenter);
    ui->listWidget->addItem(itemMsg);
    ui->listWidget->setCurrentRow(0);
    */
}

void Device::updateSensorInfo(bool isApplied){
    // Show a message that the electrode has been applied.
    this->isApplied = isApplied;
    if (isPowerOn && isTreatmentStarted && isApplied){
        ui->labelScreenMsg->setVisible(true);
    } else {
        ui->labelScreenMsg->setVisible(false);
    }
    emit signalDeviceApplied(isApplied);
}

void Device::powerIncrease(){
    powerLevel = qMin(powerLevel+1,100);
    ui->labelPower->setText(QString("Power level: %1").arg(powerLevel));
    ui->labelPower->setVisible(true);
    ticksSincePowerButton = 0;
}

void Device::powerDecrease(){
    powerLevel = qMax(powerLevel-1,1);
    ui->labelPower->setText(QString("Power level: %1").arg(powerLevel));
    ui->labelPower->setVisible(true);
    ticksSincePowerButton = 0;
}

