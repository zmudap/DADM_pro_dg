#ifndef HRV1
#define HRV1

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <math.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_complex.h>


using namespace std;

using namespace std;


class hrv1
{
public:
    vector<float> rPeaks;

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

    void setData();
    vector<float> rValues; //input data
    hrv1(vector<float> values);


    float calculateNN50();
    float calculatepNN50();
    float calculateRR();
    float calculateRR_std();
    float calculatermssd();
    float calculateSDANN();
    float calculateSDANN_index();
    void calculateFFT(vector<float>*);
    vector<float> calculatePerio();
    float calculateFreq_param();
};

#endif
