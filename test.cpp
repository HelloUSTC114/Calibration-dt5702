#include "iostream"

#include "Multi_Gauss.h"
#include "FitSpectrum.h"

using namespace std;

int main()
{
    auto file = new TFile("Data/Ch4.root");
    auto h = (TH1F*)file -> Get("h4;1");

    FitSpectrum fit(h,5);
    fit.Fit();
    
    file -> Close();
    // delete file;
    cout << "test" << endl;
}