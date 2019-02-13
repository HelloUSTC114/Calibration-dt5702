#include "iostream"

#include "Multi_Gauss.h"
#include "FitSpectrum.h"

using namespace std;

int main()
{
    auto file = new TFile("Data/Ch9.root");
    auto h = (TH1F*)file -> Get("h9;1");
    FitSpectrum fit(h,5);
    fit.Fit();

    TFitResult result(fit);
    result.Print();
    cout << result.GetGain() << endl;


    auto file1 = new TFile("Test.root", "recreate");
    fit.Write(file1);

    // result.Write("test");
    file1 -> Close();
    file -> Close();
    // delete file;
}