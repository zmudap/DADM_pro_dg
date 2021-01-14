#ifndef  DFA_H
#define DFA_H

#include <vector>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_statistics_double.h>
#include <math.h>
class Dfa {

    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> integral;
    std::vector<double> F;
    std::vector<double> Flog10;
    double a1;
    double a2;

    void calculate_a1_a2();
    void calculateF();
    std::vector<double> leastSqr(std::vector<double> x, std::vector<double> y);
    void integrate();
    std::vector<double> subVector(std::vector<double> inData, int a, int b);
    double vectorSum(std::vector<double> vec);
public:
    void process();
    void setData(std::vector<float> tachogram);
    void getData(double& aa1, double& aa2, std::vector<float>& Flog10);
};

#endif // ! DFA_H
