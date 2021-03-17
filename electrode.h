#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QWidget>
//#include <QtCharts>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCore/qmath.h>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Electrode;
}

class Electrode : public QChartView //QWidget
{
    Q_OBJECT

public:
    explicit Electrode(QChartView *parent = nullptr);
    ~Electrode();

public slots:
    void updateElectrodeOutput(bool isPowerOn, bool isTreatmentStarted, int powerLevel, int frequency);
    void updateElectrodeOutput(bool isPowerOn);
    void setDeviceApplied(bool isApplied);

private:
    Ui::Electrode *ui;
    QLineSeries *series;
    QLineSeries *seriesZ;
    QChart *chart;
    QChart *chartZ;
    QValueAxis *axisY1;
    QValueAxis *axisY2;
    bool isDeviceApplied;
    int maxX;
    double x_offset;
    void resetSeries();
};

#endif // ELECTRODE_H
