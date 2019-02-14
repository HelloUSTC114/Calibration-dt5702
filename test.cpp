#include "iostream"
#include "TFile.h"

#include "MultiGauss.h"
#include "FitSpectrum.h"
#include "ChCali.h"
#include "GetADCHist.h"


using namespace std;

int main()
{
    // auto file = new TFile("/home/john/Documents/Calibration-dt5702/Data/Calibration-SPE/Ch2.root");
    // auto h = (TH1F*)file -> Get("h2;1");

    GetOneByOne HistManager;
    auto h = HistManager.GetSPEHist(4);
    cout << h << endl;
    FitSpectrum fit(h,5);
    fit.Fit();
    
    TFitResult result(fit);
    result.Print();
    cout << result.GetGain() << endl;

    // FitPedestal fit(h);
    // fit.FitPed();

    auto file1 = new TFile("Test.root", "recreate");
    fit.Write(file1);
    result.Write();
    file1 -> Close();

}