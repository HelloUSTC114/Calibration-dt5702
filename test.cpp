#include "iostream"

#include "Multi_Gauss.h"
#include "FitSpectrum.h"

using namespace std;

int main()
{
    auto file = new TFile("Data/2-Ped.root");
    auto h = (TH1F*)file -> Get("h2;1");
    // FitSpectrum fit(h,5);
    // fit.Fit();
// 
    // TFitResult result(fit);
    // result.Print();
    // cout << result.GetGain() << endl;

    FitPedestal fit(h);
    fit.FitPed();

    auto file1 = new TFile("Test.root", "recreate");
    fit.Write(file1);


    // result.Write("test");
    file1 -> Close();
    file -> Close();
    // delete file;
}