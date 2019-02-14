#include "MultiGauss.h"


// This is a class aiming at constructing a TF1 with multiple peaks.
class MultiGauss;


MultiGauss::MultiGauss(int n_peak)
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

void MultiGauss::SetNPeak(int n_peak)
{
    if(n_peak < 0)
    {
        std::cout << "Error! Wrong peak number." << std::endl;
        return;
    }
    fNPeak = (n_peak > Peak_Num_Max) ? Peak_Num_Max : n_peak;
}

double MultiGauss::operator()(double *x, double *par)
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

int MultiGauss::GetNPeak()
{
    return fNPeak;
}