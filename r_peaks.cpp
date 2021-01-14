#include "r_peaks.h"

R_peaks::R_peaks(vector<float> values)
{
    // konstruktor klasy: przyjmuje wektor typu float wartosci probek
    signalValues = values;
}

void R_peaks::setData(vector<float> values)
{
    // ustawawia wartosci probek na te przyjmowane z wektora typu float
    this->signalValues = values;
}


vector<int> R_peaks::getResults(int choice, R_peaks object)
{
    // przyjmuje choice: wybor stosowanego algorytmu(1 dla Pan-Tompkins lub 2 dla H znajdowania pikow oraz object: objekt klasy R_peaks(czyli wektor float)
    // zawsze zwraca wektor typu int numerow probek w ktorych wykryto zalamek R
    /*  Przyklad uzycia w main.cpp:
     *
     *  std::vector<float>signal_201;   // sygnal wejsciowy
     *  R_peaks signal(signal_201);     // tworzenie obiektu R_peaks zainicjalizowanego sygnalem wejsciowym
     *  vector<int> resultValues = signal.getResults(1, signal);  //rezultat: wektor typu int z numerami probek zalamkow R wedlug algorytmu Pan-Tompkins
     *
     * */
    vector<int> result;
    switch(choice)
    {
    case 1:
        result = PanTompkins(object);
        break;
    case 2:
        result = Hilbert(object);
        break;
    default:
        cout << "1 - Pan-Tompkins" << endl << "2 - Hilbert" << endl;
        break;
    }
    return result;
}

vector<float> R_peaks::conv(vector<float> const &f, vector<float> const &g) { // convolution
  int const nf = f.size();
  int const ng = g.size();
  int const n  = nf + ng - 1;
  vector<float> out(n, float());
  for(auto i(0); i < n; ++i) {
    int const jmn = (i >= ng - 1)? i - (ng - 1) : 0;
    int const jmx = (i <  nf - 1)? i            : nf - 1;
    for(auto j(jmn); j <= jmx; ++j) {
      out[i] += (f[j] * g[i - j]);
    }
  }
  return out;
}

vector<float> R_peaks::myFilter(int typ_filtra, int Fs, int Fc, int M, int w) // filtracja
{
    int N = 2*M+1;
    float fn = (float(Fc)/float(Fs))/2;
    int t[N];
    for (int i = -M; i <= M; i++)
    {
         t[i+M] = i;
    }

    float y[N];
    switch (typ_filtra) {
        case 1:  //LP
        for (int i=0;i<N;i++)
        {
            y[i]=sin(2*M_PI*fn*t[i])/(M_PI*t[i]);
        }
        y[M] = 2*fn;
        break;
        case 2:  //HP
        for (int i=0;i<N;i++)
        {
            y[i]=-sin(2*M_PI*fn*t[i])/(M_PI*t[i]);
        }
        y[M] = 1-2*fn;
        break;
    }

    vector<float> result(y, y + N);
    switch (w) {
        case 1:
        break;
        case 2:
        break;
    }

    return result;
}


vector<float> R_peaks::diffFilter(const vector<float>& signal, float fs = 360.0) // rozniczkowanie
{
    float alpha = fs / 8.0;
    vector<float> result;
    auto max = max_element(signal.begin(),signal.end());
    for (int index = 2; index < signal.size() - 2; ++index)
    {
        float value = -signal[index - 2] - 2 * signal[index - 1] +
            2 * signal[index + 1] + signal[index + 2];
        value *= alpha;
        value = value/ *max;
        result.push_back(value);
    }
    float value;
    value = result.front();
    result.insert(result.begin(), 1, value);
    result.insert(result.begin(), 1, value);
    value = result.back();
    result.push_back(value);
    result.push_back(value);
    return result;
 }


vector<float> R_peaks::sqFilter(const vector<float> & signal) // potegowanie
{
    vector<float> result;
    for (int index = 0; index < signal.size(); ++index)
    {
        float value = signal[index] * signal[index];
        result.push_back(value);
    }
    return result;
}

float R_peaks::thr_sig_update(std::vector<float> const& v, int index) // aktualizacja progu adaptacyjnego glownego
{
    if (index > 360) {
        vector<float>::const_iterator first = v.begin() + index - 360;
        vector<float>::const_iterator last = v.begin() + index + 360;
        vector<float> indice(first, last);
        auto result = *max_element(indice.begin(), indice.end());
        result /= 2;

        return result;

    } else {
        vector<float>::const_iterator first = v.begin();
        vector<float>::const_iterator last = v.begin() + index + 360;
        vector<float> indice(first, last);
        auto result = *max_element(indice.begin(), indice.end());
        result /= 2;

        return result;
    }

}

float R_peaks::thr_noise_update(std::vector<float> const& v, int index) // aktualizacja adaptacyjnego progu szumu
{
    if (index > 360) {
        vector<float>::const_iterator first = v.begin() + index - 360;
        vector<float>::const_iterator last = v.begin() + index + 360;
        vector<float> indice(first, last);
        auto n = indice.size();
        float mean = 0.0f;
        if (n != 0) {
            mean = accumulate(indice.begin(), indice.end(), 0.0) / n;
        }

        auto result = 0.5f * mean;

        return result;
    } else {
        vector<float>::const_iterator first = v.begin();
        vector<float>::const_iterator last = v.begin() + index + 360;
        vector<float> indice(first, last);
        auto n = indice.size();
        float mean = 0.0f;
        if (n != 0) {
            mean = accumulate(indice.begin(), indice.end(), 0.0) / n;
        }

        auto result = 0.5f * mean;

        return result;
    }
}

complex<float> R_peaks::DFT(vector<float> input, int k, int size)  // dyskretna transformata fouriera
{
    float a = 0;
    float b = 0;
    int N = size;
    for(int n = 0; n < N; n++)
    {
        a+= cos((2 * M_PI * k * n) / N) * input[n];
        b+= -sin((2 * M_PI * k * n) / N) * input[n];
    }
    complex<float> temp(a, b);
    return temp;
}

complex<float> R_peaks::IDFT(vector<complex<double>> output, int n) // odwrotna dyskretna transformata fouriera
{
    const auto ci = complex<double>(0, 1);
    complex<float> result;
    int N = output.size();
    for (int k = 0; k < N; k++)
        result += exp((1. / N) * 2 * M_PI * k * n * ci) * output[k];
    result /= N;
    return result;
}


/*
 *  Pan-Tompkins wedlug algorytmu:
 * 1. Filtracja pasmowo przepustowa
 * 2. Rozniczkowanie sygnalu
 * 3. Potegowanie sygnalu
 * 4. Progowanie adaptacyjne i wyznaczanie zalamkow R
*/

vector<int> R_peaks::PanTompkins(R_peaks object) //
{
    vector<float> signal = object.signalValues;
    vector<float> oursignal = signal;
    int fs = 360;
    int fcL = 14;
    int fcH = 3;
    int M = 5;
    int N = 2*M + 1;

    vector<float> hwL = myFilter(1,fs,fcL,M,1); // LP

    M = 10;
    vector<float> hwH = myFilter(2,fs,fcH,M,1); // HP

    auto Y1 = conv(hwL, oursignal);  // splot z LP
    auto Y = conv(hwH, Y1); // splot z HP

    vector<float> signal_diff = diffFilter(Y,fs); // rozniczkowanie
    vector<float> signal_sq = sqFilter(signal_diff); // potegowanie

    signal_sq.erase(signal_sq.begin(), signal_sq.begin() + 9); // kompensacja przesuniecia wywolanego filtrami


    // progowanie adaptacyjne
    auto max = *max_element(signal_sq.begin(),signal_sq.begin()+fs*2);
    float THR_SIG = max;

    auto n = signal_sq.size();
    float THR_NOISE = 0.0f;
    if ( n != 0) {
         THR_NOISE = accumulate(signal_sq.begin(), signal_sq.end()+fs*2, 0.0) / n;
    }

    auto firstmax = max_element(signal_sq.begin(),signal_sq.begin()+300);
    auto firstloc = distance(signal_sq.begin(), firstmax);

    vector<float> peaks;
    vector<int> indexes;
    vector<float> noisepeaks;
    vector<int> noiseindexes;
    unsigned int index = 0;
    float thr_sum = THR_SIG;
    float threshold = 0.25*THR_SIG;
    unsigned int loc = 0;
    int i = 0;

    while ((loc + index + 200) < signal_sq.size())
    {
        THR_SIG = thr_sig_update(signal_sq, index);
        THR_NOISE = thr_noise_update(signal_sq, index);
        auto peak = max_element(signal_sq.begin() + index, signal_sq.begin() + index + 200);
        auto loc = distance(signal_sq.begin(), peak);
        if (*peak > threshold) {
            peaks.push_back(*peak);
            indexes.push_back(loc);
            threshold = 0.25 * (0.875 * THR_SIG + 0.125 * *peak);
        }
        else if (*peak < THR_NOISE) {
            noisepeaks.push_back(*peak);
            noiseindexes.push_back(loc);
            THR_NOISE = 0.25 * (0.875 * THR_SIG + 0.125 * *peak);
        }
        index = loc + 200;
        i++;
    }


    return indexes;
}

/* Hilbert wg. algorytmu:
 * 1. Filtracja pasmowo przepustowa
 * 2. Rozniczkowanie sygnalu
 * 3. Sygnal analityczny wedlug algorytmu funkcji hilbert() zastosowanego w matlabie:
 *  3.1. Wyliczenie DFT sygnalu wejsciowego
 *  3.2. Stworzenie wektora H:
 *      1 dla i = 1, (n/2)+1
 *      2 dla i = 2, 3, ... , (n/2)
 *      0 dla i = (n/2)+2, ... , n
 *  3.3. Mnozenie DFT i wektora H element po elemencie
 *  3.4. Wyliczenie iDFT z powstalego w poprzednim punkcie wektora
 * 4. Progowanie adaptacyjne i wyznaczanie zalamkow R
*/

vector<int> R_peaks::Hilbert(R_peaks object) //
{
    vector<float> signal = object.signalValues;
    vector<float> oursignal = signal;
    int fs = 360;
    int fcL = 14;
    int fcH = 3;
    int M = 5;
    int N = 2*M + 1;

    vector<float> hwL = myFilter(1,fs,fcL,M,1); // LP

    M = 10;
    vector<float> hwH = myFilter(2,fs,fcH,M,1); // HP

    auto Y1 = conv(hwL, oursignal);  // splot z LP
    auto Y = conv(hwH, Y1); // splot z HP

    vector<float> signal_diff = diffFilter(Y,fs); // rozniczkowanie

    signal_diff.erase(signal_diff.begin(), signal_diff.begin() + 12); // kompensacja przesuniecia wywolanego filtrami

    //obciecie ostatniej probki jesli dlugosc sygnalu nieparzysta
    if(signal_diff.size() % 2) {
        signal_diff.pop_back();
    }

    // calculate DFT
    vector<complex<float>> output;
    for(int k = 0; k < signal_diff.size(); k++)
        {
            output.push_back(DFT(signal_diff, k,signal_diff.size()));
        }

    // create vector h
    vector<int> h;
    h.push_back(1);

    for(int i = 1; i <= output.size()/2; i++) {
        h.push_back(2);
    }

    h.push_back(1);

    for(int i = output.size()/2+2; i < output.size(); i++) {
        h.push_back(0);
    }

    // element-wise product of fft and h
    vector<complex<double>> product;
    for(int i =0; i < output.size(); i++) {
       double tempreal = h[i]*output.at(i).real();
       double tempimag = h[i]*output.at(i).imag();
       complex<double> temp(tempreal, tempimag);
       product.push_back(temp);
    }

    // iDFT
    vector<complex<float>> result;
    for(int n = 0; n < product.size(); n++)
        {
            result.push_back(IDFT(product, n));
        }

    //final hilbert result
    vector<float> hilbertresult;
    for(int i = 0; i < result.size(); i++)
        {
            hilbertresult.push_back(result.at(i).imag());
        }


    // progowanie adaptacyjne
    auto max = *max_element(hilbertresult.begin(),hilbertresult.begin()+fs*2);
    float THR_SIG = max;

    auto n = hilbertresult.size();
    float THR_NOISE = 0.0f;
    if ( n != 0) {
         THR_NOISE = accumulate(hilbertresult.begin(), hilbertresult.end()+fs*2, 0.0) / n;
    }

    auto firstmax = max_element(hilbertresult.begin(),hilbertresult.begin()+300);
    auto firstloc = distance(hilbertresult.begin(), firstmax);

    vector<float> peaks;
    vector<int> indexes;
    vector<float> noisepeaks;
    vector<int> noiseindexes;
    unsigned int index = 0;
    float thr_sum = THR_SIG;
    float threshold = 0.25*THR_SIG;
    unsigned int loc = 0;
    int i = 0;

    while ((loc + index + 200) < hilbertresult.size())
    {
        THR_SIG = thr_sig_update(hilbertresult, index);
        THR_NOISE = thr_noise_update(hilbertresult, index);
        auto peak = max_element(hilbertresult.begin() + index, hilbertresult.begin() + index + 200);
        auto loc = distance(hilbertresult.begin(), peak);
        if (*peak > threshold) {
            peaks.push_back(*peak);
            indexes.push_back(loc);
            threshold = 0.25 * (0.875 * THR_SIG + 0.125 * *peak);
        }
        else if (*peak < THR_NOISE) {
            noisepeaks.push_back(*peak);
            noiseindexes.push_back(loc);
            THR_NOISE = 0.25 * (0.875 * THR_SIG + 0.125 * *peak);
        }
        index = loc + 200;
        i++;
    }

    return indexes;
}
