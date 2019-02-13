#include "TSpectrum.h"
#include "TMath.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TH1I.h"

#include "Multi_Gauss.h"
#include "Read_Spectrum.cpp"

class FitSpectrum
{
public:
    FitSpectrum() = default;
    FitSpectrum(TH1I *H, int PeakNum):h(H),Peak_Num(PeakNum){}
    ~FitSpectrum();

    void Reset(TH1I* h_reset, int PeakNum){h=h_reset;Peak_Num=PeakNum;delete sp;sp=NULL;}

    bool Fit();

private:
    int Peak_Num;
    TH1I *h;
    TSpectrum *sp;   // Need to be destructed, because it's created by this class
}
