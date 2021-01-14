#include "visualisation.h"
#include "ui_visualisation.h"
#include "QDebug"
#include <vector>

Visualisation::Visualisation(QWidget *parent) :
    QCustomPlot(parent),
    ui(new Ui::Visualisation),
    mRubberBand(new QRubberBand(QRubberBand::Rectangle,this))
{
    ui->setupUi(this);
    this->setFixedSize(451,261);
    setInteractions(QCP::iRangeZoom | QCP::iRangeDrag | QCP::iSelectItems);
    axisRect()->setRangeDrag(Qt::Horizontal);

    //legend
    QFont legendFont = font();
    legend->setVisible(true);
    legend->setFont(legendFont);
    legend->setSelectedFont(legendFont);

}

void Visualisation::ecgInput(std::vector<float> inputdata)
{
    setWindowTitle("ECG input");
    addData(inputdata);

    xAxis->setTickLabelType(QCPAxis::ltDateTime);
    xAxis->setTickLabelRotation(90);
    xAxis->setTickLabelFont(QFont("Helvetica [Cronyx]", 8));
    xAxis->setDateTimeFormat("ms:zzz");
    xAxis->setAutoTickStep(false);
    xAxis->setTickStep(0.1);
    xAxis->setPadding(1);
    yAxis->setRange(-2,2);
}

void Visualisation::ecgBaseline(std::vector<float> ecgBaselineValues)
{
    setWindowTitle("ECG_Baseline");
    addData(ecgBaselineValues);

    xAxis->setTickLabelType(QCPAxis::ltDateTime);
    xAxis->setTickLabelRotation(90);
    xAxis->setTickLabelFont(QFont("Helvetica [Cronyx]", 8));
    xAxis->setDateTimeFormat("ms:zzz");
    xAxis->setAutoTickStep(false);
    xAxis->setTickStep(0.1);
    xAxis->setPadding(1);
    yAxis->setRange(-2,2);
}

void Visualisation::rrPeaks(std::vector<float> ecgBaselineValues, std::vector<int> RpeaksResults)
{
    Visualisation::ecgBaseline(ecgBaselineValues);
    setWindowTitle("R Peaks");
    addRR(ecgBaselineValues, RpeaksResults);
}

QPair<QVector<double>,QVector<double>> Visualisation::addPoint(std::vector<float> ecgBaselineValues, std::vector<int> pointVector)
{
    QPair<QVector<double>,QVector<double>> xy;
    QVector<double> xAxis;
    QVector<double> yAxis;
    int i = 0;

    while(pointVector[i]>firstSample && pointVector[i]<=lastSample)
    {
        xAxis.append((double(pointVector[i]))/FREQ);
        yAxis.append(ecgBaselineValues[pointVector[i]]);
        i++;
    }
    xy.first = xAxis;
    xy.second = yAxis;
    return xy;
}

void Visualisation::addData(std::vector<float> ecgBaselineValues)
{
    xData.clear();
    yData.clear();
    //graph()->clearData();
    this->addGraph();
    double xValue = firstSample;
    xAxis->setRange(xValue/FREQ,xValue/FREQ+3);
    while(xValue<lastSample)
    {
        xData.append(xValue/FREQ);
        yData.append(ecgBaselineValues[xValue]);
        xValue++;
    }
    this->graph()->setData(xData, yData);
    graph()->setName("ECG");
}

void Visualisation::addRR(std::vector<float> ecgBaselineValues, std::vector<int> RpeaksResults)
{
    QPair<QVector<double>,QVector<double>> rVector;
    rVector = addPoint(ecgBaselineValues, RpeaksResults);

    QCPGraph *graph1 = this->addGraph();
    //graph1->clearData();
    graph1->setData(rVector.first,rVector.second);
    graph1->setName("RR");
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    graph1->setLineStyle(QCPGraph::lsNone);
    graph1->setPen(QPen(Qt::red, 3));
}


void Visualisation::hrvDfa(std::vector<float> Flog10)

{
    QVector<double> x;
    for(int i = 4; i<65; i++)
    {
       x.push_back(log10(i));
    }

    std::vector<double> Flog10double(Flog10.begin(), Flog10.end());
    QVector<double> Flog10doubleQ = QVector<double>::fromStdVector(Flog10double);

    QCPGraph *hrvdfa = this->addGraph();
    //hrvdfa->clearData();
    xAxis->setScaleType(QCPAxis::stLogarithmic);
    yAxis->setScaleType(QCPAxis::stLogarithmic);
    xAxis->setRange(0.1,100);
    yAxis->setRange(1,15);
    xAxis->setSubTickCount(10);
    xAxis->grid()->setSubGridVisible(true);
    xAxis->setLabel("log10(n)");
    yAxis->setLabel("log10(Fn)");
    hrvdfa->setName("hrv_dfa");
    hrvdfa->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    hrvdfa->setLineStyle(QCPGraph::lsNone);
    hrvdfa->setPen(QPen(Qt::red, 3));

    hrvdfa->addData(x,Flog10doubleQ);
}

void Visualisation::hrvI(std::vector<float> hrv1_per)
{
    // frequency
    int M = 40;
    int N = 2*M+1;
    QVector<double> x;

    for (int i=0;i<N;i++)
    {
       x.push_back(i);
    }

    std::vector<double> power(hrv1_per.begin(), hrv1_per.end());
    QVector<double> powerQ = QVector<double>::fromStdVector(power);

    QCPGraph *hrvI = this->addGraph();
   // hrvI->clearData();
    hrvI->setName("HRVI");
    yAxis->setRange(-0.1,1.2);
    xAxis->setRange(0,50);
    xAxis->setLabel("Frequency");
    yAxis->setLabel("Power Spectral Density");
    hrvI->setData(x,powerQ);
}


void Visualisation::hrvII(std::pair <std::vector <float>, std::vector <float>> hist)
{
    setWindowTitle("HRV II histogram");
    std::vector <float> histX = hist.first;
    std::vector <float> histY = hist.second;
    std::vector<double> histDatadoublex(histX.begin(), histX.end());
    std::vector<double> histDatadoubley(histY.begin(), histY.end());
    QVector<double> histDatax = QVector<double>::fromStdVector(histDatadoublex);
    QVector<double> histDatay = QVector<double>::fromStdVector(histDatadoubley);

    QCPBars *hrvGraph = new QCPBars(xAxis,yAxis);
    //hrvGraph->clearData();
    hrvGraph->setAntialiased(false);
    hrvGraph->setAntialiasedFill(false);
    hrvGraph->setWidth(50);
    xAxis->setAutoTickCount(false);
    xAxis->setAutoSubTicks(false);
    xAxis->setAutoTicks(false);
    xAxis->setSubTickCount(0);
    xAxis->setTickLength(0);
    xAxis->grid()->setVisible(false);
    xAxis->setAutoTickLabels(true);
    xAxis->setTickVector(histDatax);
    xAxis->setRange(0,500);
    yAxis->setRange(0,10);
    hrvGraph->setData(histDatax,histDatay);
}

void Visualisation::hrvIIPoincare(std::pair <std::vector <float>, std::vector <float>> poincare)
{
    setWindowTitle("HRV II Poincare");
    std::vector <float> poincareX = poincare.first;
    std::vector <float> poincareY = poincare.second;
    std::vector<double> poincareDatadoublex(poincareX.begin(), poincareX.end());
    std::vector<double> poincareDatadoubley(poincareY.begin(), poincareY.end());
    QVector<double> poincareDatax = QVector<double>::fromStdVector(poincareDatadoublex);
    QVector<double> poincareDatay = QVector<double>::fromStdVector(poincareDatadoubley);

    QCPGraph *hrvpoincare = this->addGraph();
   // hrvpoincare->clearData();
    xAxis->grid()->setSubGridVisible(true);
    xAxis->setLabel("RRn [ms]");
    yAxis->setLabel("RRn+1 [ms]");
    xAxis->setRange(0,2000);
    yAxis->setRange(0,2000);
    hrvpoincare->setName("hrvII Poincare");
    hrvpoincare->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    hrvpoincare->setLineStyle(QCPGraph::lsNone);
    hrvpoincare->setPen(QPen(Qt::blue, 3));
    hrvpoincare->addData(poincareDatax,poincareDatay);
}

void Visualisation::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::RightButton)
    {
        mOrigin = event->pos();
        mRubberBand->setGeometry(QRect(mOrigin, QSize()));
        mRubberBand->show();
    }

    QCustomPlot::mousePressEvent(event);
}

void Visualisation::mouseMoveEvent(QMouseEvent * event)
{
    if(mRubberBand->isVisible())
    {
        mRubberBand->setGeometry(QRect(mOrigin, event->pos()).normalized());
    }
    QCustomPlot::mouseMoveEvent(event);
}

void Visualisation::mouseReleaseEvent(QMouseEvent * event)
{
    if (mRubberBand->isVisible())
    {
        const QRect & zoomRect = mRubberBand->geometry();
        int xp1, yp1, xp2, yp2;
        zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
        auto x1 = xAxis->pixelToCoord(xp1);
        auto x2 = xAxis->pixelToCoord(xp2);
        auto y1 = yAxis->pixelToCoord(yp1);
        auto y2 = yAxis->pixelToCoord(yp2);

        xAxis->setRange(x1, x2);
        yAxis->setRange(y1, y2);

        mRubberBand->hide();
        replot();
    }
    QCustomPlot::mouseReleaseEvent(event);
}


Visualisation::~Visualisation()
{
    delete ui;
    delete mRubberBand;
}


