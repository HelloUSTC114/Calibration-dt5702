#include "FitSpectrum.h"

ClassImp(TFitResult);

double First_Peak_Start_Point = 50;
double First_Peak_Mean_Start_Limit = 180;
double First_Peak_Mean_End_Limit = 250;
double Gain_Guess = 50;

FitSpectrum::~FitSpectrum()
{
    if(sp)
    {
        delete sp;
        sp = NULL;
    }
    for(int i = 0; i < Peak_Num_Max; i++)
    {
        if(fit_fun_array[i])
        {
            delete fit_fun_array[i];
            fit_fun_array[i] = NULL;
        }
    }
    // if(h_copy){
    //     delete h_copy;
    //     h_copy = NULL;
    // }
    if(c){
        delete c;
        c = NULL;
    }

    fFitFlag = 0;

}

bool FitSpectrum::Fit()
{
    if(!h) return false;
    if(fFitFlag)    return true;
    Reset();

    sp = new TSpectrum();
    
    h_back = sp->Background(h);
    h_copy = new TH1F(*h);

    // Subtract background
    h->Add(h_back, -1);

    if(!c)
    {
        c = new TCanvas("c1", "c1", 1);
    }

    // Define class MultiGauss
    MultiGauss MultiGauss(0);

    // Start to fit
    for (int peak_index = 0; peak_index < Peak_Num; peak_index++)
    {
        // First thing to do is to establish a function with i peaks
        int peak_num_temp = peak_index + 1;
        MultiGauss.SetNPeak(peak_num_temp);

        fit_fun_array[peak_index] = new TF1(Form("FitFun_%d", peak_index), MultiGauss, 0, 4096, 3 * peak_num_temp);
        for (int i = 0; i < 3 * peak_num_temp; i++)
        {
            // Set Limits of fit function that all parameters should be larger than 0;
            fit_fun_array[peak_index]->SetParLimits(i, 0, 20000);
        }

        // Judge if this is the first time to fit. If is, than initial condition should be given.
        if (peak_index == 0)
        {
            // Set first Peak limits
            fit_fun_array[peak_index]->SetParLimits(1, First_Peak_Mean_Start_Limit, First_Peak_Mean_End_Limit);
            fit_fun_array[peak_index]->SetParameter(0, 60);
            fit_fun_array[peak_index]->SetParameter(1, 300);
            fit_fun_array[peak_index]->SetParameter(2, 20);

            // Start Position should be given and fixed in further fit.
            h->Fit(fit_fun_array[peak_index], "NQ", "", First_Peak_Start_Point, First_Peak_Start_Point + 160);
        }
        else
        {
            // First, get the mean position of last peak
            double last_peak_mean = fit_fun_array[peak_index - 1]->GetParameter(3 * (peak_index - 1) + 1);

            // Define start position, in which case, I set it at first peak minus 5 sigma.
            double start = fit_fun_array[0]->GetParameter(1) - 5 * fit_fun_array[0]->GetParameter(2);
            // Define End Poistion, I set it at the last fit peak plus 160, which is enough for our fit.
            double end = last_peak_mean + 160;

            // Pass all parameter of the last fit function to the new function just established.
            for (int par_index = 0; par_index < 3 * peak_index; par_index++)
            {
                fit_fun_array[peak_index]->SetParameter(par_index, fit_fun_array[peak_index - 1]->GetParameter(par_index));
                fit_fun_array[peak_index]->SetParLimits(par_index, fit_fun_array[peak_index - 1]->GetParameter(par_index)*0.9, fit_fun_array[peak_index - 1]->GetParameter(par_index) * 1.1);
            }

            // Set Initial value of parameters.
            fit_fun_array[peak_index]->SetParameter(3 * peak_index + 1, last_peak_mean + Gain_Guess);
            fit_fun_array[peak_index]->SetParLimits(3 * peak_index + 1, last_peak_mean, last_peak_mean + 40 + Gain_Guess);
            fit_fun_array[peak_index]->SetParameter(3 * peak_index + 2, 10);
            fit_fun_array[peak_index]->SetParLimits(3 * peak_index + 2, 5, 40);

            h->Fit(fit_fun_array[peak_index], "NQ", "", start, end);
        }
    }

    // Add fit function to background to get the final fit result.
    c -> cd();
    h->SetName("Sub_Back");
    h->SetTitle("Substrct background");
    h->Draw();

    h_back -> Add(fit_fun_array[Peak_Num - 1]);
    h_back -> SetName("Fit_Histo");
    h_back -> SetTitle("Fit Histo of Spectrum");
    h_back -> Draw();

    h_copy -> SetName("Original");
    h_copy -> SetTitle("Original Histo");
    h_copy -> Draw("same");

    cout << "Mean" << "\t" << "Sigma" << "\t" << "Variation" << endl;
    c -> BuildLegend();
    c -> SaveAs("test.pdf");

    fFitFlag = true;
    return true;
}

void FitSpectrum::Reset(TH1F* h_reset, int PeakNum)
{
    h=h_reset;
    Peak_Num=PeakNum;
    Reset();
}

void FitSpectrum::Reset(){

    if(sp){
        delete sp;
        sp=NULL;
    }
    if(h_back){
        delete h_back;
        h_back = NULL;
    }
    if(h_copy){
        delete h_copy;
        h_copy = NULL;
    }
    for(int i = 0; i < Peak_Num_Max; i++){
        if(fit_fun_array[i]){
            delete fit_fun_array[i];
            fit_fun_array[i] = NULL;
        }
    }
    if(c)
        c -> Clear();
    fFitFlag = 0;
}

void FitSpectrum::Print()
{
    if(!fFitFlag)
        return;

    for(int i = 0; i < Peak_Num; i++){
        auto fun_temp = fit_fun_array[Peak_Num - 1];
        cout << fun_temp -> GetParameter(3 * i + 1) << "\t" << fun_temp -> GetParameter(3 * i + 2);
        if(i == 0){
            cout << endl;
        }
        else{
            cout << "\t" << fun_temp -> GetParameter(3 * i + 1) - fun_temp -> GetParameter(3 * i - 2) << endl;
        }

    }
}

bool FitSpectrum::Write(TDirectory * dir)
{
    if(!dir)    return false;
    if(!dir -> IsWritable())    return false;
    dir -> cd();

    fit_fun_array[Peak_Num-1] -> Write();
    h -> Write();
    h_back -> Write();
    h_copy -> Write();

    // TFitResult temp(*this);
    return true;
}

TFitResult::TFitResult()
{
    fPeakNum = 0;
    fArrPeak = 0;
    fArrSig = 0;
    fArrDev = 0;
}

TFitResult::TFitResult(FitSpectrum& Fit)
{
    if(!Fit.fFitFlag)
    {
        fPeakNum = 0;
        fArrPeak = 0;
        fArrSig = 0;
        fArrDev = 0;
        return;
    }
    fPeakNum = Fit.Peak_Num;
    fArrPeak = new double[fPeakNum];
    fArrSig = new double[fPeakNum];
    fArrDev = new double[fPeakNum - 1];
    Refresh(Fit);
}

void TFitResult::Clear()
{
    if(fArrPeak)
        delete[] fArrPeak;
    if(fArrDev)
        delete[] fArrDev;
    if(fArrSig)
        delete[] fArrSig;
    fArrPeak = 0;
    fArrSig = 0;
    fArrDev = 0;
    fPeakNum = 0;
}
bool TFitResult::Reset(FitSpectrum& Fit)
{
    if(!Fit.fFitFlag)
    {
        return false;
    }
    Clear();
    fPeakNum = Fit.Peak_Num;
    fArrPeak = new double[fPeakNum];
    fArrSig = new double[fPeakNum];
    fArrDev = new double[fPeakNum - 1];

    Refresh(Fit);
    return true;
}

void TFitResult::Refresh(FitSpectrum& Fit)
{
    auto fun_temp = Fit.fit_fun_array[Fit.Peak_Num - 1];

    for(int i = 0; i < fPeakNum; i++){
        fArrPeak[i] = fun_temp -> GetParameter(3 * i + 1);
        fArrSig[i] = fun_temp -> GetParameter(3 * i + 2);
        if(i){
            fArrDev[i - 1] = fArrPeak[i] - fArrPeak[i-1];
        }
    }

}

TFitResult::~TFitResult()
{
    Clear();
}

void TFitResult::Print()
{
    cout << "NPeak:\t" << fPeakNum << endl;
    cout << "Peak:\t" << "Pos\t" << "Dev\t" << endl;
    for(int i = 0; i < fPeakNum; i++)
    {
        cout << i << '\t' << fArrPeak[i] << '\t';
        if(i)
            cout << fArrDev[i-1];
        cout << endl; 
    }
}

double TFitResult::GetGain()
{
    double average = 0;
    for(int i = 0; i < fPeakNum-1; i++)
    {
        average += fArrDev[i];
    }
    average = average / (double)(fPeakNum-1);

    double gain = 0;
    int counter = 0;
    for(int i = 0; i < fPeakNum-1; i++)
    {
        if(i == 0)   continue;
        if(TMath::Abs(fArrDev[i]-average)/average > 0.3)
            continue;
        gain += fArrDev[i];
        counter++;
    }
    gain = gain / (double)counter;
    return gain;
}

double FitPedestal::FitPed()
{
    if(!h)
        return -1;
    if(fFitFlag)
        return fPed;
    if(f_temp)  delete f_temp;

    f_temp = new TF1("gaus", "gaus", 0, 4096);
    h -> Fit(f_temp, "NQ", "", 0, 4096);
    fPed = f_temp -> GetParameter(1);

    cout << fPed << endl;
    fFitFlag = true;
    return fPed;
}

bool FitPedestal::Write(TDirectory *dir)
{
    if(!dir -> IsWritable())
        return false;
    h -> Write();
    f_temp -> Write();
    return true;
}

double FitPedestal::GetPed()
{
    return FitPed();
}