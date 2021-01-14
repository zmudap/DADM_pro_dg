#ifndef R_PEAKS_H
#define R_PEAKS_H

#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <complex>
#include <ctime>

using namespace std;

class R_peaks
{
public:
    vector<float> signalValues;

    R_peaks(vector<float> values);
    vector<int> getResults(int choice, R_peaks object);
    void setData(vector<float> values);

private:
    vector<float> myFilter(int typ_filtra, int Fs, int Fc, int M, int w);
    vector<float> conv(vector<float> const &f, vector<float> const &g);
    vector<float> diffFilter(const vector<float>& signal, float fs);
    vector<float> sqFilter(const vector<float> & signal);
    vector<int> PanTompkins(R_peaks object);
    float thr_noise_update(std::vector<float> const& v, int index);
    float thr_sig_update(std::vector<float> const& v, int index);
    complex<float> DFT(vector<float> input, int k, int size);
    complex<float> IDFT(vector<complex<double>> output, int n);
    vector<int> Hilbert(R_peaks object);
};

#endif
