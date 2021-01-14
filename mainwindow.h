#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>
using namespace std;
#include "Visualisation/visualisation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   // Visualisation on_R_PEAK_graph_destroyed(Visualisation arg1);
    float Results();

public slots:
    int ChoseFile();
    void SignalVisualizeGraph();
    void ECGBaselineResults();
    void RPeaksResults();
    void DFAResults();
    void HRV1LoadResults();
    void HRV2LoadResults();
    int  PDFgenerate();
    void WavesLoadResults();
/*
    void on_EKG_BASELINE_graph_destroyed();

    void R_PEAK_zaladuj_button();

    void on_R_PEAK_graph_destroyed();
*/
private slots:
    void on_pushButton_3_glowna_clicked();
    void on_Zamknij_program_button_clicked();

  //  void on_R_PEAK_graph_destroyed(QObject *arg1);



private:
    Ui::MainWindow *ui;
    QScopedPointer<Visualisation> m_plot;

    std::vector<float> hrv2out;
    std::vector<float>ECGSignal;  //vector to store the numerical values in
    vector<float> ecgBaselineValues; //vector to store ECG Baseline results
    vector<int> RpeaksResults; //vector to store RpeaksResults
    vector <int> Hrv2Results;

    //hrv1
    float RR;
    float RR_std;
    float RMSSD;
    float NN50;
    float pNN50;
    float SDANN;
    float SDANN_index;
    float SDSD;
    //parametry do analizy częstotliwościowej
    float TP;
    float HF;
    float LF;
    float VLF;
    float ULF;
    float LFHF;
    vector <float> freqParams;

    //hrv2
    vector <float> par;
    pair <vector <float>, vector <float>> a;
    pair <vector <float>, vector <float>> b;
    pair <vector <float>, vector <float>> c;
    float tinn;
    float hrvi;
    float sd1;
    float sd2;
/*
    //ecg baseline
    std::vector<float> ecgBaselineValues; //vector to store ECG Baseline results
    std::vector<int> RpeaksResults; // vector to store R peaks results
    vector<vector<int>> WavesResults;
*/
    //dfa
    double a1;
    double a2;
    std::vector<float> Flog10;
};
#endif // MAINWINDOW_H
