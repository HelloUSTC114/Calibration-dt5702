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

Multi_Gauss::Multi_Gauss(int n_peak)
{
    if(n_peak < 0)
    {
        std::cout << "Error! Wrong peak number." << std::endl;
        fNPeak = 0;
    }
    else
    {
        fNPeak = ((n_peak > Peak_Num_Max) ? Peak_Num_Max : n_peak);
    }

}

void Multi_Gauss::SetNPeak(int n_peak)
{
    if(n_peak < 0)
    {
        std::cout << "Error! Wrong peak number." << std::endl;
        return;
    }
    fNPeak = (n_peak > Peak_Num_Max) ? Peak_Num_Max : n_peak;
}

double Multi_Gauss::operator()(double *x, double *par)
{
    double y = 0;
    double Gauss_Par[Peak_Num_Max][3]{0};
    for(int i = 0; i < fNPeak; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            Gauss_Par[i][j] = par[i * 3 + j];
        }
        y += Gauss_Par[i][0] * TMath::Gaus(x[0], Gauss_Par[i][1], Gauss_Par[i][2]);
    }
    return y;
}

int Multi_Gauss::GetNPeak()
{
    return fNPeak;
}