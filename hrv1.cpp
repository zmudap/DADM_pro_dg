
#include "hrv1.h"

hrv1::hrv1(vector<float> values)
{
    // konstruktor klasy: przyjmuje wektor typu int wartosci probek
    rValues = values;
}

void hrv1::setData()
{
    // ustawia wartości zmiennych klasy
    // this->rValues = prepareData(rValues);
   this->RR=calculateRR();
   this->RR_std=calculateRR_std();
   this->RMSSD=calculatermssd();
}

float hrv1::calculateRR()
{
    float temp=0;
    for(float i=0; i<rValues.size(); i++)
    {
        temp = temp +  rValues[i];
        }
    return temp/(rValues.size());
}


float hrv1::calculateRR_std()
{
    float temp =0;
    RR=calculateRR();
    for(float i=0; i<rValues.size(); i++)
    {
        temp = temp + pow( (rValues[i]-RR), 2);
        }
    return sqrt(temp/(rValues.size()));
}


float hrv1::calculatermssd()
{
    float temp=0;
    for(float i=0; i<rValues.size(); i++)
    {
      temp = temp+(rValues[i+1]-rValues[i])*(rValues[i+1]-rValues[i]);
    }
    return sqrt(temp/(rValues.size()));
}

float hrv1::calculateNN50()
{
   float temp=0;
   float threshold  = 50 ;
   for(float i=0; (i<rValues.size()-1); i++) {
        if (abs(rValues[i+1]-rValues[i]) > threshold)
            temp=temp+1;
        else
            temp=temp;
   }
    return temp;
}

// odsetek różnic pomiędzy interwałami RR, które przekraczają 50 ms [%]

float hrv1::calculatepNN50()
{
    NN50=calculateNN50();
    return ((NN50/(rValues.size()))*100);
}

//odchylenie standardowe ze wszystkich średnich interwałów RR w 5 minutowych segmentach czasu całego zapisu [ms]


float hrv1::calculateSDANN()
{
      int sampling = 500; //probkowanie
      int size_window = sampling*5*60; //okno o rozmiarze 5 minut
      int steps=floor((rValues.size()-1)/size_window);

      float SDANN[steps];

      for(int step=1;step<=steps;step++) {
            int start = (step-1) * size_window;
        int end = step * size_window;

        int start_index = 0;
        int end_index = 0;

	    for(int i=0; i<rValues.size(); i++)
	    {
		if(start<=rValues[i]) {
		start_index=i;
		break;
	    }
	    }

	    for(int i=0; i<rValues.size(); i++)
	    {
		if(end<=rValues[i]) {
		end_index=i;
		break;
	    }
	    }

	    float mean=0;

            for(float i=start_index; i<end_index+1; i++)
          {

           mean = mean +  rValues[i];

            }
        SDANN[step-1]=mean/size_window;
        }

    float mean1=0;
        for(int i=0; i<steps; i++)
          {
           mean1 = mean1 + SDANN[i];
              }

        mean1=mean1/steps;
    cout<<"SREDNIA:"<<mean1<<endl;
        float temp=0;
        for(int i=0; i<steps; i++)
    {
        temp = temp + pow( (SDANN[i]-mean1), 2);
        }
    return sqrt(temp/(steps));

}


float hrv1::calculateSDANN_index()
{
     int sampling = 5; //probkowanie
      int size_window = sampling*1; //okno o rozmiarze 5 minut
      int steps=floor((rValues.size()-1)/size_window);

      float SDANN[steps];
      float SDANN_index[steps];

      for(int step=1;step<=steps;step++) {
            int start = (step-1) * size_window;
        int end = step * size_window;

        int start_index = 0;
        int end_index = 0;

	    for(int i=0; i<rValues.size(); i++)
	    {
		if(start<=rValues[i]) {
		start_index=i;
		break;
	    }
	    }
	    for(int i=0; i<rValues.size(); i++)
	    {
		if(end<=rValues[i]) {
		end_index=i;
		break;
	    }
	    }

	    float mean=0;

            for(float i=start_index; i<end_index+1; i++)
          {

           mean = mean +  rValues[i];

            }

       SDANN[step-1]=mean/size_window;
           float temp=0;
           for(int i=start_index; i<end_index+1; i++)
       {
        temp = temp + pow( (rValues[i]-mean), 2);
           }

           SDANN_index[step-1]=sqrt(temp/size_window);
}

    float mean1=0;
        for(int i=0; i<steps; i++)
          {
           mean1 = mean1 + SDANN_index[i];
              }

        return mean1=mean1/steps;
}


// Analiza częstotliwościowa
void hrv1::calculateFFT(vector<float>* vec){
    std::vector<double> v(vec->begin(), vec->end());
    gsl_complex_packed_array data = v.data();
    size_t n = v.size();

    size_t stride = 1;

    gsl_fft_complex_radix2_forward (data, stride, n);
}

vector<float> hrv1::calculatePerio()
{

    // Filtracja z oknem barletta
    int M = 40;
    int Fc = 2*M/rValues.size();
    int Fs = 360;
    int N = 2*M+1;
    float fn = (float(Fc)/float(Fs))/2;
    int t[N];
    for (int i = -M; i <= M; i++)
    {
         t[i+M] = i;
    }

    float y[N];

    for (int i=0;i<N;i++)
        {
            y[i]=-sin(2*M_PI*fn*t[i])/(M_PI*t[i]);
        }
    y[M] = 1-2*fn;
    vector<float> result(y, y + N); //Periodogram
    return result;
}

float hrv1::calculateFreq_param()
{
    calculateFFT(&rValues);
    size_t n = rValues.size();

    int M = 40;
    int Fc = 2*M/rValues.size();
    int Fs = 360;
    int N = 2*M+1;
    float fn = (float(Fc)/float(Fs))/2;
    int t[N];

    float y[N];
    // Pętla licząca parametry
    for(int i = 1; i< n; i++) {
        y[i]=-sin(2*M_PI*fn*t[i])/(M_PI*t[i]);
        if(i<=0.003) {
            this->ULF += y[i];
        }
        else if(i>0.003 && i<=0.04) {
            this->VLF += y[i];
        }
        else if(i>0.04 && i<=0.15) {
            this->LF += y[i];
        }
        else if(i>0.15 && i<=0.4) {
            this->HF += y[i];
        }
    }
    this->TP = this->ULF + this->VLF + this->LF + this->HF;
    this->LFHF = this->LF / this->HF;
}


