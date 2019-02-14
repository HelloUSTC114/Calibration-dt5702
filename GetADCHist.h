#ifndef GETADCHISTO_H
#define GETADCHISTO_H

#include "TFile.h"
#include "TH1F.h"
#include "iostream"
#include "TObject.h"
#include "ChCali.h"
#include <string>
using namespace std;

class GetADCHist
{
public:
    virtual ~GetADCHist(){};
    virtual TH1F* GetSPEHist(int ch) = 0;
    virtual TH1F* GetPedHist(int ch) = 0;
private:
};

class GetOneByOne : public GetADCHist
{
public:
    virtual ~GetOneByOne() override;
    virtual TH1F* GetSPEHist(int ch) override;
    virtual TH1F* GetPedHist(int ch) override;

protected:
    virtual string GetFilePath();
    virtual string GetSPEFileName(int ch);
    virtual string GetPedFileName(int ch);

private:
    TFile *f[64]{NULL};     //First 32 are for SPE, last 32 are for Ped
};
#endif