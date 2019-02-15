#include "iostream"
#include "TFile.h"

#include "MultiGauss.h"
#include "FitSpectrum.h"
#include "ChCali.h"
#include "GetADCHist.h"


using namespace std;

int main(int argc, char **argv)
{
    FitSpectrum fit_spe;
    TFitresult result;

    auto file = new TFile(argv[1]);

    auto h = (TH1F*)file -> Get(argv[2]);
    if(h)
    {
        fit_spe.Reset(h, 4);
        fit_spe.Fit();
        result.Reset(fit_spe);
        result.Print();

        auto file1 = new TFile("Test.root", "recreate");
        file1->cd();
        fit_spe.Write(file1);
        file1->Close();
        delete file1;
        file1=NULL;
    }
    file -> Close();
    delete file;
}