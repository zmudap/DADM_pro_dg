#ifndef VISUALISATION_H
#define VISUALISATION_H

#include <qcustomplot.h>
#include <ui_visualisation.h>


#define SAMPLES 10000
#define FREQ 360
/*
namespace Ui {
class Visualisation;
}
*/
class Visualisation : public QCustomPlot
{
    Q_OBJECT
public:

    explicit Visualisation(QWidget *parent = nullptr);
    ~Visualisation() override;

    void ecgBaseline(std::vector<float> ecgBaselineValues);
    void rrPeaks(std::vector<float> ecgBaselineValues, std::vector<int> RpeaksResults);

    void hrvDfa(std::vector<float> Flog10);
    void hrvII(std::pair <std::vector <float>, std::vector <float>> hist);
    void ecgInput(std::vector<float> inputdata);
    void hrvIIPoincare(std::pair <std::vector <float>, std::vector <float>> poincare);
    void hrvI(std::vector<float> hrv1_per);


    int firstSample = 0;
    int lastSample = 10000;

private slots:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;


private:

    Ui::Visualisation *ui;
    QRubberBand * mRubberBand;
    QPoint mOrigin;

    QVector<double> xData;
    QVector<double> yData;

    QPair<QVector<double>,QVector<double>> addPoint(std::vector<float> ecgBaselineValues, std::vector<int> pointVector);
    void addData(std::vector<float> ecgBaselineValues);
    void addRR(std::vector<float> ecgBaselineValues, std::vector<int> RpeaksResults);

};

#endif // VISUALISATION_H
