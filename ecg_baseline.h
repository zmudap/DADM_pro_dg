#ifndef ECG_BASELINE_H
#define ECG_BASELINE_H

#include <iostream>
#include <vector>
using namespace std;

class ECG_Baseline
{
public:
    vector<float> signalValues;
    int signalLength;

    ECG_Baseline(vector<float> values, int length);
    vector<float> getResults(int choice, vector<float> amplitude);
    vector<float> setData(vector<float> amplitude);

private:
    vector<float> movingAverageAlgorithm(vector<float> values);
    vector<float> SavitzkyGolayAlgorithm(vector<float> values);
    vector<float> KalmanFilter(vector<float> values);
};


#endif // ECG_BASELINE_H
