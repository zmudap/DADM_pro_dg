#include "Dfa.h"

void Dfa::calculate_a1_a2() {

    double buffer[(65 - 4) * 2];
    int i = 4;
    while(i < 65) {
        buffer[i - 4] = log10(i);
        buffer[i - 4 + 61] = log10(F[i - 4]);
        i++;
    }

    std::vector<double> blockSizes(buffer, buffer + i - 4);
    std::vector<double> Flog10(buffer + 61, buffer + i - 4 + 61);

    this->Flog10 = Flog10;

    double a1c0, a1c1, a1cov00, a1cov01, a1cov11, a1sumsq;
    gsl_fit_linear(subVector(blockSizes, 0, 13).data(), 1, subVector(Flog10, 0, 13).data(), 1, subVector(Flog10, 0, 13).size(), &a1c0, &a1c1, &a1cov00, &a1cov01, &a1cov11, &a1sumsq);
    this->a1 = a1c1;
    double a2c0, a2c1, a2cov00, a2cov01, a2cov11, a2sumsq;
    gsl_fit_linear(subVector(blockSizes, 13, 61).data(), 1, subVector(Flog10, 13, 61).data(), 1, subVector(Flog10, 13, 61).size(), &a2c0, &a2c1, &a2cov00, &a2cov01, &a2cov11, &a2sumsq);
    this->a2 = a2c1;
}



void Dfa::calculateF() {
    std::vector<double> F;
    double workingbuffer[100000];
    double workingbufferF[61];
    for (int blockSize = 4; blockSize < 65; blockSize++) {
        int iterations = integral.size() / blockSize;

        int ymlen = 0;
        int iteration = 0;
        for (iteration; iteration < iterations; iteration++) {
            std::vector<double> regLin = leastSqr(subVector(x, iteration * blockSize, (iteration + 1) * blockSize), subVector(integral, iteration * blockSize, (iteration + 1) * blockSize));
            std::copy(regLin.begin(), regLin.end(), workingbuffer+ymlen);
            ymlen += regLin.size();
        }
        std::vector<double> ym(workingbuffer, workingbuffer + ymlen);

        for (int i = 0; i < ym.size(); i++) {
            workingbuffer[i] = (integral[i] - ym[i]) * (integral[i] - ym[i]);
        }
        std::vector<double> squareErrors(workingbuffer, workingbuffer + ym.size());
        workingbufferF[blockSize - 4] = sqrt(vectorSum(squareErrors) / ym.size());
    }

    this->F = std::vector<double> (workingbufferF, workingbufferF + 61);
}


std::vector<double> Dfa::leastSqr(std::vector<double> x, std::vector<double> y) {

    double c0, c1, cov00, cov01, cov11, sumsq;
    double *regLinebuffer = new double[x.size()];

    gsl_fit_linear(x.data(), 1, y.data(), 1, y.size(), &c0, &c1, &cov00, &cov01, &cov11, &sumsq);

    for (int i = 0; i < x.size(); i++) {
        regLinebuffer[i] = x[i] * c1 + c0;
    }
    std::vector<double> regLine(regLinebuffer, regLinebuffer + x.size());
    delete[] regLinebuffer;
    return regLine;
}

void Dfa::integrate() {

    double mean = gsl_stats_mean(y.data(), 1, y.size());



    double* integralbuff = new double[y.size()];
    double* yminusmeanbuff = new double[y.size()];

    for (int i = 0; i < y.size(); i++) {
        yminusmeanbuff[i] = y[i] - mean;
    }
    std::vector<double> yminusmean(yminusmeanbuff, yminusmeanbuff+y.size());
    for (int i = 1; i <= y.size(); i++) {
        std::vector<double> sub = subVector(yminusmean, 0, i);
        integralbuff[i - 1] = vectorSum(sub);
    }


    this->integral = std::vector<double> (integralbuff, integralbuff + y.size());
    delete[] integralbuff;
    delete[] yminusmeanbuff;
}


std::vector<double> Dfa::subVector(std::vector<double> inData, int a, int b) {

    std::vector<double>::const_iterator first = inData.begin() + a;
    std::vector<double>::const_iterator last = inData.begin() + b;
    std::vector<double> newVec(first, last);
    return newVec;
}

double Dfa::vectorSum(std::vector<double> vec) {
    double sum_of_elems = 0;
    for (std::vector<double>::iterator it = vec.begin(); it != vec.end(); ++it)
        sum_of_elems += *it;
    return sum_of_elems;
}

void Dfa::process() {
    integrate();
    calculateF();
    calculate_a1_a2();
}
void Dfa::setData(std::vector<float> tachogram) {
   y = std::vector<double>(tachogram.begin(),tachogram.end());
   double* xbuffer = new double[tachogram.size()];
   for (int i = 1; i <= y.size(); i++) {
       std::vector<double> sub = subVector(y, 0, i);
      // x.push_back(vectorSum(sub));
       xbuffer[i - 1] = vectorSum(sub);
   }
   x = std::vector<double>(xbuffer, xbuffer + y.size());
}
void Dfa::getData(double& aa1, double& aa2, std::vector<float>& Flog10) {
    a1 = this->a1;
    a2 = this->a2;
    Flog10 = std::vector<float>(this->Flog10.begin(), this->Flog10.end());
}
