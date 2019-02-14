#ifndef MULTIGAUSS_H
#define MULTIGAUSS_H
#include "TMath.h"
#include <iostream>


// This is a class aiming at constructing a TF1 with multiple peaks.
const int Peak_Num_Max = 10;

class MultiGauss
{
public:
    MultiGauss(int n_peak);
    void SetNPeak(int n_peak);
    int GetNPeak();
    double operator()(double *x, double *par);
private:
    int fNPeak;
};

#endif