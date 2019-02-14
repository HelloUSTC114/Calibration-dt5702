#include "GetADCHist.h"

GetOneByOne::~GetOneByOne()
{
    for(int i = 0; i < 64; i++){
        if(f[i]){
            f[i] -> Close();
            delete f[i];
            f[i] = NULL;
        }
    }
}

string GetOneByOne::GetSPEFileName(int ch)
{
    char c[32];
    string temp = "Ch";
    temp += to_string(ch) + ".root";
    return temp;
}

string GetOneByOne::GetPedFileName(int ch)
{
    char c[32];
    string temp = to_string(ch);
    temp +=  "-ped.root";
    return temp;
}

string GetOneByOne::GetFilePath()
{
    return "/home/john/Documents/Calibration-dt5702/Data/Calibration-SPE/";
}

TH1F *GetOneByOne::GetSPEHist(int ch)
{
    string FullFileName = GetFilePath() + GetSPEFileName(ch);
    int index = ch;

    if(!f[index])
    {
        auto file = new TFile(FullFileName.c_str());
        f[index] = file;
    }
    auto h = (TH1F*)f[ch] -> Get(Form("h%d", ch));
    return h;
}

TH1F *GetOneByOne::GetPedHist(int ch)
{
    string FullFileName = GetFilePath() + GetPedFileName(ch);
    int index = ch+32;

    if (!f[index])
    {
        auto file = new TFile(FullFileName.c_str());
        f[index] = file;
    }
    auto h = (TH1F *)f[ch]->Get(Form("h%d", ch));
    return h;
}
