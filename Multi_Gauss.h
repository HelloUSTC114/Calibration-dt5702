#include "TMath.h"
#include <iostream>


// This is a class aiming at constructing a TF1 with multiple peaks.
class Multi_Gauss;

const int Peak_Num_Max = 10;

class Multi_Gauss
{
public:
    Multi_Gauss(int n_peak);
    void SetNPeak(int n_peak);
    int GetNPeak();
    double operator()(double *x, double *par);
private:
    int fNPeak;
};
