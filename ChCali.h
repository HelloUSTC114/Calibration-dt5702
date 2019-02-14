#ifndef CHCALI_H
#define CHCALI_H

#include "iostream"
#include "TObject.h"

class TChannelInfo;
class TADCInfo;
class TOpticInfo;

class TADCInfo: public TObject
{
public:
    TADCInfo() = default;

    double GetPENum(int ch, double adc);

    double GetGain(int ch){return fGain[ch];}
    double GetPed(int ch){return fPed[ch];}

    bool SetGain(int channel, double gain){if(fGain[channel])return false;fGain[channel]=gain;return true;}
    bool SetPed(int channel, double ped){if(fPed[channel])return false;fPed[channel]=ped;return true;}
private:
    double fGain[32]{0};
    double fPed[32]{0};

    ClassDef(TADCInfo,1);
};
#endif