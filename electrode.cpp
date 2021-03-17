/* COMP 3004A Winter 2020
 * Team 9
 * Tom Roberts, Graham Airth, Hussien Dabarani, Rishi Parameswaran
 *
 * A widget to show the Electrode output.
 * Although not required, this gives a visualization of how the device state,
 *   and treatment, are changing based on user inputs/actions.
 *
 *  (It seemed like a neat thing to have)
 *
 * E.g., when you press "Apply Electrode" on the Simulator Control, the waveform changes to reflect
 *   that the device is being applied to the patient's skin.
 * Changes gradually as power level and frequency change, also.
 *
 */

#include "electrode.h"
#include "ui_electrode.h"

Electrode::Electrode(QChartView *parent) :
    QChartView(parent), //QWidget(parent),
    ui(new Ui::Electrode)
{
    ui->setupUi(this);

    setWindowTitle("Electrode Output");
    setFixedSize(500,400);
    move(1100,50);

    isDeviceApplied = false;
    maxX = 1000;
    x_offset = 0;

    // A bunch of QChart components require set-up.
    series = new QtCharts::QLineSeries();
    seriesZ = new QtCharts::QLineSeries();

    axisY1 = new QValueAxis;
    axisY2 = new QValueAxis;

    axisY1->setRange(-.7, .7);
    axisY2->setRange(-.7, .7);
    axisY1->hide();
    axisY2->hide();
    //axisX->setTickCount(10);
    //axisX->setLabelFormat("%.2f");

    // Populate the waveform series to show electrode output.
    for (int i=0; i<20; i++){
        seriesZ->append(i,0);
    }
    for (int i=0; i<maxX; i++){
        series->append(i*0.05,0); //append(i*0.1,qSin(i*0.1)*qSqrt(1/(i*0.1+1.0)));   //*(1/(i+1))   ((maxY-i)/(maxY+i/10.0))
    }

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    //chart->createDefaultAxes();
    chart->setTitle("Electrode Output: Microcurrent Waveform");
    chart->addAxis(axisY1, Qt::AlignLeft);
    series->attachAxis(axisY1);

    chartZ = new QChart();
    chartZ->legend()->hide();
    chartZ->addSeries(seriesZ);
    //chartZ->createDefaultAxes();
    chartZ->setTitle("Electrode Output: Microcurrent Waveform");
    chartZ->addAxis(axisY2, Qt::AlignLeft);
    seriesZ->attachAxis(axisY2);

    setChart(chartZ);

    //QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    //chartView->setRenderHint(QPainter::Antialiasing);
    //chartView->setGeometry(50,50,400,300);
    //chartView->show();

}

Electrode::~Electrode()
{
    delete series;
    delete seriesZ;
    delete chart;
    delete chartZ;
    delete axisY1;
    delete axisY2;
    delete ui;
}

void Electrode::updateElectrodeOutput(bool /* unused */, bool isTreatmentStarted, int powerLevel, int frequency){
    // Rebuild the waveform according to current device settings.
    //int x = 0;
    //double x_input;
    double y_input;
    double y_scale;
    int maxFreq = 125;
    int numNewPts = 20; //15 + int((25.0*frequency)/maxFreq); //50;
    double y_decay = 0.99 - 0.02*frequency/maxFreq; //0.97;
    if (isTreatmentStarted && isDeviceApplied){
        //x = 1;  // Adjust waveform when device is applied.
        setWindowTitle("Treatment selected and device applied.");
    }
    if (isTreatmentStarted){
        // This keeps incrementing with each tick.
        // Was using this when the entire chart was being replaced.
        x_offset = x_offset + 2.0*(frequency/(1.0*maxFreq))/(1.0+isDeviceApplied);
        chart->removeSeries(series);

        // Try to only partially modify wave.~~~~~~~~****~~~~~~~~~****_______****~~~~~~~
        /* Take out the oldest points, replace them with new ones. */
        /* The new values have declining X-values (wave is moving left to right); so countdown from zero. */
        QPointF point1stOld = series->at(0);
        series->removePoints(maxX-numNewPts,numNewPts);
        for (int i= -1; i>= -numNewPts; i--){
        //for (int i=numNewPts; i>=0; i--){
            //x_input = i + point1stOld.rx();
            y_input = i*(frequency/(6.0*maxFreq))/(1.0+2*isDeviceApplied) + point1stOld.rx();
            y_scale = (qSqrt(powerLevel*1.0)/10.0);
            QPointF tmpPoint(y_input,qSin(y_input*1.0)*y_scale); //*qSqrt(1/(qMax(i,1)/25.0+1.0)));   //-x_offset
            series->insert(0, tmpPoint);   // (1/(i+1))   ((maxY-i)/(maxY+i/10.0))
        }
        //QPointF point1stNew = series->at(numNewPts-1);
        for (int i=numNewPts; i<maxX; i++){  //numNewPts
            /* Add decay to older data points */
            //y_scale = (qSqrt(powerLevel*1.0)/10.0);
            QPointF tmpPoint = series->at(i);
            //x_input = tmpPoint.rx() + ;
            //y_decay = 0.97; //qSqrt(1/(i/25.0+1.0));
            series->replace(i, tmpPoint.rx(), tmpPoint.ry()*y_decay);   // (1/(i+1))  +point1stNew.rx()  ((maxY-i)/(maxY+i/10.0))
        }
        /*
         // Rebuild the entire waveform
        series->removePoints(0,maxX);

        for (int i=0; i<maxX; i++){
            x_input = i*(frequency/(2.0*maxFreq))/(1.0+2*isDeviceApplied);
            y_scale = (qSqrt(powerLevel*1.0)/10.0);
            series->append(x_input,qSin(x_input-x_offset)*y_scale*qSqrt(1/(i/25.0+1.0)));   // (1/(i+1))   ((maxY-i)/(maxY+i/10.0))
        }
        */
        chart->addSeries(series);
        series->attachAxis(axisY1);
        setChart(chart);
    } else {
        //setChart(chartZ);
        resetSeries();
    }
}

void Electrode::resetSeries(){
    chart->removeSeries(series);
    series->removePoints(0,maxX);
    for (int i=0; i<maxX; i++){
        series->append(i*0.05,0); //append(i*0.1,qSin(i*0.1)*qSqrt(1/(i*0.1+1.0)));   //*(1/(i+1))   ((maxY-i)/(maxY+i/10.0))
    }
    chart->addSeries(series);
    series->attachAxis(axisY1);
    setChart(chart);
}

void Electrode::updateElectrodeOutput(bool isPowerOn){
    if (!isPowerOn){
        //setChart(chartZ);
        resetSeries();
    }
}

void Electrode::setDeviceApplied(bool isApplied){
    isDeviceApplied = isApplied;
    /*
    if (isDeviceApplied){
        setWindowTitle("Patient has applied the device.");
    } else {
        setWindowTitle("");
    }
    */
}
