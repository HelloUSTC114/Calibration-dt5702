#ifndef FITSPECTRUM_H
#define FITSPECTRUM_H

#include "TSpectrum.h"
#include "TMath.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"

#include "Multi_Gauss.h"
#include "Read_Spectrum.cpp"
using namespace std;

class FitSpectrum;

class TFitResult: public TObject
{
public:
    TFitResult();
    TFitResult(FitSpectrum&);
    ~TFitResult();

    void Clear();

    bool Reset(FitSpectrum&);
    void Print();
    void Refresh(FitSpectrum&);
private:
    Int_t fPeakNum;   
    Double_t *fArrPeak;   //[fPeakNum]
    Double_t *fArrSig;    //[fPeakNum]
    Double_t *fArrDev;    //[fPeakNum-1]


    ClassDef(TFitResult,1);
};

class FitSpectrum
{
    friend class TFitResult;
public:
    FitSpectrum(){}
    FitSpectrum(TH1F *H, int PeakNum):h(H),Peak_Num(PeakNum){}
    ~FitSpectrum();

    void Reset(TH1F* h_reset, int PeakNum);
    void Reset();

    bool Fit();

    void Print();
    bool Write(TDirectory *);
    

private:
    int Peak_Num;
    TH1F *h = NULL;
    TSpectrum *sp = NULL;   // Need to be destructed, because it's created by this class
    TH1* h_back = NULL;
    TH1F* h_copy = NULL;

    TF1 *fit_fun_array[Peak_Num_Max]{0};
    TCanvas* c = NULL;

    bool fFitFlag = 0;

};

#endif