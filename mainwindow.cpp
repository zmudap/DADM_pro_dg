#include <fstream>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ecg_baseline.h"
#include "hrv1.h"
#include "hrv2.h"
#include "qcustomplot.h"
#include "r_peaks.h"
#include "Waves.h"
#include "Dfa.h"


#include <vector>
#include <QPdfWriter>
#include <QMessageBox>
#include <QPrinter>
#include <QUrl>
#include <QPainter>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>

#include <QtCharts>

using namespace QtCharts;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Results();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_glowna_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_Zamknij_program_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

float MainWindow::Results()
{
}

int MainWindow::ChoseFile()
{
QString filePath = QFileDialog::getOpenFileName(this, "Open ecg file", QDir::homePath(), "txt Files(*.txt)");
ifstream myFile;            //creates stream myFile
myFile.open(filePath.toStdString().c_str(), std::ifstream::in);
if (!myFile.is_open())  // check file is open, quit if not
{
    cerr << "failed to open file1\n";
    return 1;
}

float number = 0 ;
while(myFile >> number){
    ECGSignal.push_back(number);
}

}

void MainWindow::SignalVisualizeGraph()
{
  Visualisation obj;
ui->Signal_graph->ecgInput(ECGSignal);
}

void MainWindow::ECGBaselineResults()

{
 Visualisation obj;
 ECG_Baseline ecg(ECGSignal, ECGSignal.size());


 //sprawdzic czy to pojdzie jak bedzie wykres
  // item1 - moving average
  //item2 - savitzky
  //item3 kalman

    ui->EKG_BASELINE_listWidget->currentItem();
    if (ui->EKG_BASELINE_listWidget->item(1)) {
        ecgBaselineValues = ecg.getResults(1, ECGSignal); //dane wyjsciowe
        ui->EKG_BASELINE_graph->ecgBaseline(ecgBaselineValues);
    }
    if (ui->EKG_BASELINE_listWidget->item(2)) {
        ecgBaselineValues = ecg.getResults(2, ECGSignal);
        ui->EKG_BASELINE_graph->ecgBaseline(ecgBaselineValues);
    }
    if (ui->EKG_BASELINE_listWidget->item(3)) {
        ecgBaselineValues = ecg.getResults(3, ECGSignal);
        ui->EKG_BASELINE_graph->ecgBaseline(ecgBaselineValues);
    }


}

void MainWindow::RPeaksResults()
{
    R_peaks signal(ecgBaselineValues); //parametry trzeba ogarnac


    Visualisation obj; //obj.rrPeaks(ecgBaselineValues, RpeaksResults);
    if (ui->R_PEAK_checkBoxA->isChecked()) {
        RpeaksResults = signal.getResults(1, signal);
        ui->R_PEAK_graph->rrPeaks(ecgBaselineValues, RpeaksResults);
    }

    if (ui->R_peak_checkBoxB->isChecked()) {
          RpeaksResults = signal.getResults(2, signal);
          ui->R_PEAK_graph->rrPeaks(ecgBaselineValues, RpeaksResults);

    }




}
void MainWindow::DFAResults()
{
Dfa dfa;
dfa.setData(hrv2out);
dfa.process();
dfa.getData(a1, a2, Flog10);
QString s = QString::number(a1);
ui->HRV_DFA_textBrowser->setText(s);
QString s2 = QString::number(a2);
ui->HRV_DFA_textBrowser_2->setText(s2);


Visualisation obj;
ui->HRV_DFA_graph->hrvDfa(Flog10);

}

void MainWindow::HRV1LoadResults()
{
    hrv1 h1(ECGSignal);
    h1.setData();
     //float
    RR = h1.calculateRR();
    RR_std = h1.calculateRR_std();
    RMSSD = h1.calculatermssd();
    NN50 = h1.calculateNN50();
    pNN50 = h1.calculatepNN50();
    SDANN = h1.calculateSDANN();
    SDANN_index = h1.calculateSDANN_index();
     //parametry do analizy częstotliwościowej
    //h1.calculateFFT(&ECGSignal);
    //h1.calculatePerio();
    //h1.calculateFreq_param();

    QString s = QString::number(RR);
    ui->textBrowser->setText(s);
    QString s2 = QString::number(RR_std);
    ui->textBrowser_2->setText(s2);
    QString s3 = QString::number(RMSSD);
    ui->textBrowser_3->setText(s3);
    QString s4 = QString::number(NN50);
    ui->textBrowser_4->setText(s4);
    QString s5 = QString::number(pNN50);
    ui->textBrowser_5->setText(s5);
    QString s6 = QString::number(SDANN);
    ui->textBrowser_6->setText(s6);
    QString s7 = QString::number(SDANN_index);
    ui->textBrowser_7->setText(s7);
    QString s8 = QString::number(TP);
    ui->textBrowser_8->setText(s8);
    QString s9 = QString::number(HF);
    ui->textBrowser_9->setText(s9);
    QString s10 = QString::number(LF);
    ui->textBrowser_10->setText(s10);
    QString s11 = QString::number(VLF);
    ui->textBrowser_11->setText(s11);
    QString s12 = QString::number(ULF);
    ui->textBrowser_12->setText(s12);
    QString s13 = QString::number(LFHF);
    ui->textBrowser_13->setText(s13);

    Visualisation obj;
    ui->HRV1_graph->hrvI(h1.calculatePerio());
}
void MainWindow::HRV2LoadResults()
{
    int s = RpeaksResults.size() -1;
    float time_tab[s];
    float peak_time[s];
    float sample_time = 360;
    float between_samples_time = 1/sample_time;
    for(int i=0; i<=s; i++) {
        peak_time[i] = between_samples_time*RpeaksResults[i];
    }
    for(int j=0;j<=s;j++) {
        time_tab[j] = peak_time[j+1] - peak_time[j];
        time_tab[j] = time_tab[j] * 1000;
    }
    // create an empty vector and push all elements
    //std::vector<float> hrv2out;
    for (int i: time_tab) {
        hrv2out.push_back(i);
    }
    Hrv2 h2(hrv2out);
    h2.setData();
    par = h2.getResultsParams();
    tinn = par[0];
    hrvi = par[1];
    sd1 = par[2];
    sd2 = par[3];
    //tinn = (double)tinn;
    QString s8 = QString::number(tinn);
    ui->HRV2_textBrowser_1->setText(s8);
    QString s2 = QString::number(hrvi);
    ui->HRV2_textBrowser_2->setText(s2);
    QString s3 = QString::number(sd1);
    ui->HRV2_textBrowser_3->setText(s3);
    QString s4 = QString::number(sd2);
    ui->HRV2_textBrowser_4->setText(s4);


    Visualisation obj;
    ui->HRV2_graph1->hrvII(h2.getResultsHistogram());
    ui->HRV2_graph2->hrvIIPoincare(h2.getResultsPoincare());

}

void MainWindow::WavesLoadResults()
{
   /*
    if (ui->Waves_checkBox_P->isChecked()) {

    }

    if (ui->Waves_checkBox_QRS->isChecked()) {

    }


    if (ui->Waves_checkBox_T->isChecked()) {

    }
*/
}

int MainWindow::PDFgenerate()
{
    QPixmap pixmap;
    pixmap=QPixmap::grabWindow(ui->centralwidget->winId());

    QPrinter printer;
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFormat(QPrinter::PdfFormat);

    printer.setOutputFileName("D:/ECG_results.pdf");

    QPainter painter(&printer);
    painter.drawText(10, 10, "ECG Project - PDF Generator");

    QRect rect=painter.viewport();
    QSize size=pixmap.size();

    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(pixmap.rect());
    //painter.drawPixmap(pixmap.rect().center(), pixmap);

    painter.drawPixmap(0,0,850,600,pixmap);



    if (! painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return 1;
    }


    if (! printer.newPage()) {
        qWarning("failed in flushing page to disk, disk full?");
        return 1;
    }
    painter.drawText(10, 10, "Second Page");
    painter.end();
}
