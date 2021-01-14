#include <QApplication>
//#include <QCoreApplication>
#include <vector>
#include <fstream>
#include <iostream>

#include "ecg_baseline.h"
#include "r_peaks.h"
#include "mainwindow.h"
#include "Dfa.h"
#include "hrv1.h"
#include "hrv2.h"
#include "Waves.h"

using namespace std;
/*
vector <float> data_in(string filename)
{
  ifstream myFile;            //creates stream myFile
  myFile.open(filename, std::ifstream::in);  //opens .txt file

  if (!myFile.is_open())  // check file is open, quit if not
  {
      cerr << "failed to open file1\n";
      //return 1;
  }

  float number = 0 ;
  vector <float> data_out;
  while(myFile >> number){
      data_out.push_back(number);
    }

      return data_out;
}

vector <float> ECG(vector <float> &data_in, int par)
{
  //ecg_baseline
  ECG_Baseline ecg(data_in, data_in.size());
  return ecg.getResults(par, data_in);
}

vector <int> RPK(vector <float> &data_in, int par)
{
  //r_peaks
  R_peaks signal(data_in);
  return signal.getResults(par, signal);
}

vector <float> R2R(vector <float> &data_ecg, vector <int> &data_rpk)
{
  int s = data_rpk.size() -1;
  float time_tab[s];
  float peak_time[s];
  float sample_time = 360;
//  float between_samples_time = sample_time/data_ecg.size();
  float between_samples_time = 1/sample_time;
  for(int i=0; i<=s; i++) {
      peak_time[i] = between_samples_time*data_rpk[i];
  }

  for(int j=0;j<=s;j++) {
      time_tab[j] = peak_time[j+1] - peak_time[j];
      time_tab[j] = time_tab[j] * 1000;
  }
  // create an empty vector and push all elements
  std::vector<float> hrv2out;

  for (int i: time_tab) {
      hrv2out.push_back(i);
  }
  return hrv2out;
}

vector <float> H1(vector<float> &data_in1)
{
  hrv1 h1(data_in1);
  h1.setData();
  return h1.calculatePerio();
}

void H2(vector<float> &data_in, vector <float> &par, pair <vector <float>, vector <float>> &a, pair <vector <float>, vector <float>> &b, pair <vector <float>, vector <float>> &c)
{
  Hrv2 h2(data_in);
  h2.setData();
  par = h2.getResultsParams();
  a = h2.getResultsHistogram();
  b = h2.getResultsPoincare();
  c = h2.getResultsTriangle();
}

void DFA(vector <float> &data_in, double &a1, double &a2, vector <float> &aFlog10)
{
  Dfa dfa;
  dfa.setData(data_in);
  dfa.process();
  dfa.getData(a1, a2, aFlog10);
}

vector <vector <int>> WAV(vector <float> &data_ecg, vector <int> &data_rpk)
{
  //waves
  Waves waves;
  waves.setData(data_ecg, data_rpk);
  return waves.getResults();
}
*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/*
    vector<float>ECGSignal;  //vector to store the numerical values in
    vector<float> ecgBaselineValues; //vector to store ECG Baseline results
    vector<int> RpeaksResults; //vector to store RpeaksResults
    vector<float> RpekHrv; //vector to strore rpeaks to interval
    vector<float> hrv1_per; //wektor float do narysowania periodogramu
    vector <float> hrv2_par;
    pair <vector <float>, vector <float>> hrv2_Histogram;
    pair <vector <float>, vector <float>> hrv2_Poincare;
    pair <vector <float>, vector <float>> hrv2_Triangle;
    double dfa_1, dfa_2; //values of dfa parameters
    vector<float> dfa; //vector to store dfa results
    //vector <vector<int>> wav;//vector of vector to store waves

    ECGSignal = data_in("ECG_original_signal_10s.txt");
    ecgBaselineValues = ECG(ECGSignal, 1);
    RpeaksResults = RPK(ecgBaselineValues, 1);
    RpekHrv = R2R(ECGSignal, RpeaksResults);
   // hrv1_per = H1(RpekHrv);
    H2(RpekHrv, hrv2_par, hrv2_Histogram, hrv2_Poincare, hrv2_Triangle);

   // for (int i = 0; i < hrv2_par.size(); i++)
      cout<<dfa_1<<" "<<dfa_1<<endl;

    DFA(RpekHrv, dfa_1, dfa_2, dfa);

    //wav = WAV(ecgBaselineValues, RpeaksResults);
*/
    MainWindow ui;
    ui.show();

return a.exec();
}


