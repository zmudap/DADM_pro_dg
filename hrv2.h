#ifndef HRV2_H
#define HRV2_H

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <utility> //pair
#include <iterator>
#include <algorithm>
#include <numeric>
// #include <gsl/gsl_statistics_double.h>

using namespace std;

class Hrv2
{
public:
    vector<float> rValues; //input data

    Hrv2(vector<float> values);
    ~Hrv2();
    void setData(); //dane potrzebne do obliczeń
    //map<char, float> getResultsParams();
    vector <float> getResultsParams();  // 0 - tinn, 1 - hrvi, 2 - sd1, 3 - sd2
    pair <vector <float>, vector <float>> getResultsHistogram();
    pair <vector <float>, vector <float>> getResultsPoincare();
    pair <vector <float>, vector <float>> getResultsTriangle();
private:
    //vector<float> RRvector;
    float hrvi;
    float sd1;
    float sd2;
    float tinn;
    vector<float> poincareX;
    vector<float> poincareY;
    vector<float> histogramX;
    vector<float> histogramY;
    pair <vector <float>, vector <float>> trianglePofloats;

    vector<float> calculateHistY();
    vector<float> calculateHistX();
    vector<float> calculatePoincareX();
    vector<float> calculatePoincareY();
    float calculateTinn();
    float calculateHRVInd();
    float calculateSD1();
    float calculateSD2();
    // bool checkCondition(float);
    // vector<float> prepareData(vector<float>);
};

#endif
