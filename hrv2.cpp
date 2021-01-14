#include "hrv2.h"

using namespace std;


Hrv2::Hrv2(vector<float> values)
{
    // konstruktor klasy: przyjmuje wektor typu float wartosci probek
    rValues = values;
}

void Hrv2::setData()
{
    // ustawia wartości zmiennych klasy
    // this->rValues = prepareData(rValues);
    this->histogramX = calculateHistX();
    this->histogramY = calculateHistY();
    this->poincareX = calculatePoincareX();
    this->poincareY = calculatePoincareY();
    this->tinn = calculateTinn();
    this->hrvi = calculateHRVInd();
    this->sd1 = calculateSD1();
    this->sd2 = calculateSD2();
}

// vector <float> Hrv2::prepareData(vector<float> rrData)
// {
// //   vector <float> prepData = rrData;
// // //   float* pbegin = rrData;                          // ^
// // //   float* pend = rrData+sizeof(rrData)/sizeof(float); // ^
// //   prepData = remove_if(rrData.begin(), rrData.end(), checkCondition);
// //   return prepData;
// }

// bool Hrv2::checkCondition(float sample)
// {
//     return sample>3000;
// }

vector <float> Hrv2::getResultsParams()
{
    vector <float> result;
    result.push_back(tinn);
    result.push_back(hrvi);
    result.push_back(sd1);
    result.push_back(sd2);
    return result;
}
// map<char, float> Hrv2::getResultsParams()
// {
//     map<char, float> result;
//     result.insert(pair<char, float>('tinn', tinn));
//     result.insert(pair<char, float>('hrvi', hrvi));
//     result.insert(pair<char, float>('sd1', sd1));
//     result.insert(pair<char, float>('sd2', sd2));
//     return result;
// }

pair <vector <float>, vector <float>> Hrv2::getResultsHistogram()
{
    pair <vector <float>, vector <float>> result;

    result.first = histogramX;
    result.second = histogramY;

    return result;
}

pair <vector <float>, vector <float>> Hrv2::getResultsPoincare()
{
    pair <vector <float>, vector <float>> result;

    result.first = poincareX;
    result.second = poincareY;

    return result;
}

pair <vector <float>, vector <float>> Hrv2::getResultsTriangle()
{
    return trianglePofloats;
}

vector<float> Hrv2::calculateHistY()
{
    float floatervalNumber = 40; //number of columns in histogram [ms]
    float chosenRange = 50; //range
    vector<float>data; //values
    data.assign(floatervalNumber,0); //counting from 0

    for(float i = 0; i<rValues.size(); i++){
        float a = 0;
        for(float j=0;j<floatervalNumber; j++){
            if(rValues[i]>=a && rValues[i] <a+chosenRange){
              data[j] = data[j]+1;
            }
            a +=chosenRange;
        }
    }
    return data;
}

vector<float> Hrv2::calculateHistX(){
    float floatervalNumber = 40; //number of columns in histogram [ms]
    float chosenRange = 50; //range
    vector<float> data(floatervalNumber);

    float a=chosenRange/2; //center of column
    for(float i=0; i<floatervalNumber; i++){
        data[i]=a;
        a +=chosenRange;
    }

    return data;
}


float Hrv2::calculateTinn()
{
    float indexMax = distance(histogramY.begin(), max_element(histogramY.begin(), histogramY.end()));
    float indexN = indexMax;
    float indexM = indexMax;

    float valueMax = histogramY[indexMax];
    float threshold = round(valueMax*0.05); // 5% maksimum

    bool searchN = true;
    bool searchM = true;

    while(searchN || searchM){
        if(searchN){indexN = indexN-1;}
        if(searchM){indexM = indexM+1;}

        if(histogramY[indexN]<=threshold){
            searchN = false;
        }

        if(histogramY[indexM]<=threshold){
            searchM = false;
        }
    }

    float TINN = histogramX[indexM]-histogramX[indexN];

    vector <float> xTriangle;
    xTriangle.push_back(histogramX[indexN]);
    xTriangle.push_back(histogramX[indexMax]);
    xTriangle.push_back(histogramX[indexM]);
    vector <float> yTriangle;
    yTriangle.push_back(0);
    yTriangle.push_back(valueMax);
    yTriangle.push_back(0);
    pair <vector <float>, vector <float>> triangle;
    triangle.first = xTriangle;
    triangle.second = yTriangle;
    this->trianglePofloats = triangle;

    return TINN;
}

float Hrv2::calculateHRVInd()
{
    float HRVi = tinn/2;
    return HRVi;
}

vector<float> Hrv2::calculatePoincareX()
{
    vector<float> pX = rValues;
    pX.pop_back();
    return pX;
}

vector<float> Hrv2::calculatePoincareY()
{
    vector<float> pY = rValues;
    reverse(pY.begin(), pY.end());
    pY.pop_back();
    reverse(pY.begin(), pY.end());
    return pY;
}

float Hrv2::calculateSD1(){
    float sd1 = 0;
    vector <float> data;
    for(float i = 0; i < poincareX.size(); i++){
        data.push_back(poincareX[i]-poincareY[i]);
    }

    float length = data.size();
    float mean = accumulate( data.begin(), data.end(), 0.0) / length;
    float variance = 0;
    for(float n = 0; n < length; n++ ) //standard deviation
        {
            variance += (data[n] - mean) * (data[n] - mean);
        }
    variance /= length;
    // double variance = gsl_stats_variance(data, 1, length);
    sd1 = sqrt(2*variance)/2;
    return sd1;
}

float Hrv2::calculateSD2(){
    float sd2 = 0;

    float length = poincareX.size();
    float mean = accumulate( poincareX.begin(), poincareX.end(), 0.0) / length;
    float variance = 0;
    for(float n = 0; n < length; n++ ) //standard deviation
        {
            variance += (poincareX[n] - mean) * (poincareX[n] - mean);
        }
    variance /= length;
    // double variance = gsl_stats_variance(poincareX, 1, length);
    sd2 = sqrt(2*variance-(sd1*sd1));

    return sd2;
}

Hrv2::~Hrv2()
{
// dekonstruktor
}
