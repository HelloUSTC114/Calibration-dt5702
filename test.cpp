#include "iostream"
#include "TFile.h"

#include "MultiGauss.h"
#include "FitSpectrum.h"
#include "ChCali.h"
#include "GetADCHist.h"


using namespace std;

int main()
{
    GetOneByOne HistManager;
    FitSpectrum fit_spe;
    FitPedestal fit_ped;
    TFitresult result;

    TADCInfo ADCInfo;


    for(int i = 0; i < 32; i++)
    {
        auto h = HistManager.GetSPEHist(i);
        fit_spe.Reset(h, 5);
        fit_spe.Fit();
        result.Reset(fit_spe);
        double gain = result.GetGain();

        auto h_ped = HistManager.GetPedHist(i);
        fit_ped.Reset(h_ped);
        double ped = fit_ped.FitPed();

        ADCInfo.SetGain(i,gain);
        ADCInfo.SetPed(i,ped);
    }
    auto file = new TFile("ADCInfo.root", "recreate");
    ADCInfo.Write();
    file -> Close();
    delete file;
}