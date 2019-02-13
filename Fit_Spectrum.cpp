
#include "FitSpectrum.h"

double First_Peak_Start_Point = 350;
double First_Peak_Mean_Start_Limit = 350;
double First_Peak_Mean_End_Limit = 500;

FitSpectrum::~FitSpectrum()
{
    if(sp)
    {
        delete sp;
        sp = NULL;
    }
}

bool FitSpectrum::Fit()
{
    if(!h) return false;
    if(sp)
    {
        delete sp;
        sp = NULL;
    }

    sp = new TSpectrum();
    
    auto h_back = sp->Background(h);
    auto h_copy = new TH1I(*h);

    // Subtract background
    h->Add(h_back, -1);
    h->Draw();

    // Define class Multi_Gauss
    Multi_Gauss multi_gauss(0);
    TF1 *fit_fun_array[Peak_Num_Max];

    // Start to fit
    for (int peak_index = 0; peak_index < Peak_Num; peak_index++)
    {
        // First thing to do is to establish a function with i peaks
        int peak_num_temp = peak_index + 1;
        multi_gauss.SetNPeak(peak_num_temp);

        fit_fun_array[peak_index] = new TF1(Form("FitFun_%d", peak_index), multi_gauss, 0, 4096, 3 * peak_num_temp);
        for (int i = 0; i < 3 * peak_num_temp; i++)
        {
            // Set Limits of fit function that all parameters should be larger than 0;
            fit_fun_array[peak_index]->SetParLimits(i, 0, 20000);
        }

        // Judge if this time is the first time to fit. If is, than initial condition should be given.
        if (peak_index == 0)
        {
            // Set first Peak limits
            fit_fun_array[peak_index]->SetParLimits(1, First_Peak_Mean_Start_Limit, First_Peak_Mean_End_Limit);
            fit_fun_array[peak_index]->SetParameter(0, 60);
            fit_fun_array[peak_index]->SetParameter(1, 300);
            fit_fun_array[peak_index]->SetParameter(2, 20);

            // Start Position should be given and fixed in further fit.
            h->Fit(fit_fun_array[peak_index], "", "", First_Peak_Start_Point, First_Peak_Start_Point + 160);
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
            }

            // Set Initial value of parameters.
            fit_fun_array[peak_index]->SetParameter(3 * peak_index + 1, last_peak_mean + 120);
            fit_fun_array[peak_index]->SetParLimits(3 * peak_index + 1, last_peak_mean, last_peak_mean + 160);
            fit_fun_array[peak_index]->SetParameter(3 * peak_index + 2, 10);
            fit_fun_array[peak_index]->SetParLimits(3 * peak_index + 2, 5, 40);

            h->Fit(fit_fun_array[peak_index], "", "", start, end);
        }
    }
}

void Fit_Spectrum(string File_Name, int Channel, int Peak_Num)
{
    // use function Read_Spectrum to read a histogram from file.
    auto h = Read_Spectrum(File_Name, Channel);
    auto h_copy = new TH1I(*h);

    // use TSpectrum to calculate background
    auto sp = new TSpectrum();
    auto h_back = sp -> Background(h);

    auto c = new TCanvas("c1", "c1", 1);
    c -> cd();

    // Subtract background
    h -> Add(h_back, -1);
    h -> Draw();

    // Define class Multi_Gauss
    Multi_Gauss multi_gauss(0);
    TF1 *fit_fun_array[Peak_Num_Max];
    
    // Start to fit
    for(int peak_index = 0; peak_index < Peak_Num; peak_index++)
    {
        // First thing to do is to establish a function with i peaks
        int peak_num_temp = peak_index + 1;
        multi_gauss.SetNPeak(peak_num_temp);

        fit_fun_array[peak_index] = new TF1(Form("FitFun_%d", peak_index), multi_gauss, 0, 4096, 3 * peak_num_temp);
        for(int i = 0; i < 3 * peak_num_temp; i++)
        {
            // Set Limits of fit function that all parameters should be larger than 0;
            fit_fun_array[peak_index] -> SetParLimits(i, 0, 20000);
        }

        // Judge if this time is the first time to fit. If is, than initial condition should be given.
        if(peak_index == 0)
        {
            // Set first Peak limits
            fit_fun_array[peak_index] -> SetParLimits(1, First_Peak_Mean_Start_Limit, First_Peak_Mean_End_Limit);
            fit_fun_array[peak_index] -> SetParameter(0, 60);
            fit_fun_array[peak_index] -> SetParameter(1, 300);
            fit_fun_array[peak_index] -> SetParameter(2, 20);

            // Start Position should be given and fixed in further fit.
            h -> Fit(fit_fun_array[peak_index], "", "", First_Peak_Start_Point, First_Peak_Start_Point + 160);
        }
        else
        {
            // First, get the mean position of last peak
            double last_peak_mean = fit_fun_array[peak_index - 1] -> GetParameter(3 * (peak_index - 1) + 1);

            // Define start position, in which case, I set it at first peak minus 5 sigma.
            double start = fit_fun_array[0] -> GetParameter(1) - 5 * fit_fun_array[0] -> GetParameter(2);
            // Define End Poistion, I set it at the last fit peak plus 160, which is enough for our fit.
            double end = last_peak_mean + 160;

            // Pass all parameter of the last fit function to the new function just established.
            for(int par_index = 0; par_index < 3 * peak_index; par_index++)
            {
                fit_fun_array[peak_index] -> SetParameter(par_index, fit_fun_array[peak_index - 1] -> GetParameter(par_index));
            }

            // Set Initial value of parameters.
            fit_fun_array[peak_index] -> SetParameter(3 * peak_index + 1, last_peak_mean + 120);
            fit_fun_array[peak_index] -> SetParLimits(3 * peak_index + 1, last_peak_mean, last_peak_mean + 160);
            fit_fun_array[peak_index] -> SetParameter(3 * peak_index + 2, 10);
            fit_fun_array[peak_index] -> SetParLimits(3 * peak_index + 2, 5, 40);

            h -> Fit(fit_fun_array[peak_index], "", "", start, end);
        }
        

    }

    // Add fit function to background to get the final fit result.

    h_back -> Add(fit_fun_array[Peak_Num - 1]);
    h_back -> SetName("Fit_Histo");
    h_back -> SetTitle("Fit Histo of Spectrum");
    h_back -> Draw();

    h_copy -> SetName("Original");
    h_copy -> SetTitle("Original Histo");
    h_copy -> Draw("same");

    cout << "Mean" << "\t" << "Sigma" << "\t" << "Variation" << endl;
    for(int i = 0; i < Peak_Num; i++)
    {
        auto fun_temp = fit_fun_array[Peak_Num - 1];
        cout << fun_temp -> GetParameter(3 * i + 1) << "\t" << fun_temp -> GetParameter(3 * i + 2);
        if(i == 0)
        {
            cout << endl;
        }
        else
        {
            cout << "\t" << fun_temp -> GetParameter(3 * i + 1) - fun_temp -> GetParameter(3 * i - 2) << endl;
        }

    }

    c -> BuildLegend();

}






void Fit_Spectrum_test(string File_Name, int Channel)
{
    auto h = Read_Spectrum(File_Name, Channel);
    h -> Draw();

    auto sp = new TSpectrum();
    auto h_back = sp -> Background(h);
    h_back -> Draw("same");
}