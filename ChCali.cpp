#include "ChCali.h"

ClassImp(TADCInfo);

double TADCInfo::GetPENum(int ch, double adc)
{
    return (adc - fPed[ch]) / fGain[ch];
}