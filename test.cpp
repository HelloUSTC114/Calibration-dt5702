#include "iostream"

#include "Multi_Gauss.h"
#include "FitSpectrum.h"
#include "ChCali.h"
#include "TFile.h"

using namespace std;

int main()
{
    auto file = new TFile("/home/john/Documents/Calibration-dt5702/Data/Calibration-SPE/Ch2.root");
    auto h = (TH1F*)file -> Get("h2;1");
    cout << h << endl;
    FitSpectrum fit(h,5);
    fit.Fit();
    
    TFitResult result(fit);
    result.Print();
    cout << result.GetGain() << endl;

    // FitPedestal fit(h);
    // fit.FitPed();

    cout << "Test" << endl;
    auto file1 = new TFile("Test.root", "recreate");
    fit.Write(file1);
    result.Write();

    file1 -> Close();
    file -> Close();
    // delete file;
}