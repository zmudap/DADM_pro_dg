#include "ecg_baseline.h"

ECG_Baseline::ECG_Baseline(vector<float> values, int length)
{
    // konstruktor klasy ECG_Baseline
    // klasa ECG_Baseline przechowuje:
    // - vector float wartości sygnału wejściowego
    // - zmienną int określającą ilość próbek sygnału

    signalValues = values;
    signalLength = length;
}

vector<float> ECG_Baseline::setData(vector<float> amplitude)
{
    // ZMIENIONE CO DO POPRZEDNIEJ WERSJI: NIE PRZYJMUJE NAZWY PLIKU Z DANYMI

    // funkcja setData() przyjmuje:
    // - obiekt klasy ECG_Baseline, do którego próbki i ich ilość są zapisywane (?)

    // funkcja setData() zwraca:
    // - vector float wartości próbek amplitudy

    vector<float> dataIn;
    // ----------------------
    // wczytywanie danych ...
    // ----------------------
    return dataIn;
}

vector<float> ECG_Baseline::getResults(int choice, vector<float> amplitude)
{
    // ZMIENIONE CO DO POPRZEDNIEJ WERSJI: NIE PRZYJMUJE NAZWY PLIKU Z DANYMI

    // funkcja getResults() przyjmuje:
    // - zmienną int określającą wybór algorytmu usuwania zakłóceń linii bazowej, gdzie:
    //   1 - algorytm kroczącej średniej (Moving Average)
    //   2 - algorytm Savitzky-Golay + Moving Average
    //   3 - filtr Kalmana
    // - vector float wartości amplitudy, na którym wykonywane są operacje usuwania linii bazowej

    // funkcja getResults() zwraca:
    // - vector float wartości próbek amplitudy po usunięciu zakłóceń

    vector<float> dataOut;
    switch(choice)
    {
    case 1:
        dataOut = movingAverageAlgorithm(amplitude);
        break;
    case 2:
        dataOut = SavitzkyGolayAlgorithm(amplitude);
        dataOut = movingAverageAlgorithm(dataOut);
        break;
    case 3:
        dataOut = KalmanFilter(amplitude);
        dataOut = movingAverageAlgorithm(dataOut);
        break;
    default:
        cout << "Nie wybrano metody usuwania zakłóceń linii bazowej" << endl;
        break;
    }
    return dataOut;
}

vector<float> ECG_Baseline::movingAverageAlgorithm(vector<float> values)
{
    vector<float> signal = values;
    int L = signal.size();
    int N = 20;                             // długość okna
    int M = L + 2*N;                        // długość rozszerzonego sygnału
    int i, j, k;
    float my_signal;
    float temp_signal[M];                   // rozszerzony sygnał
    float moved_signal[L];                  // sygnał kroczącej średniej
    vector<float> result_signal;                 // sygnał po odjęciu kroczącej średniej
    for (i = 0; i < N; i++)
    {
        temp_signal[i] = signal[0];         // pierwsze N próbek sygnału rozszerzonego równe pierwszej próbce sygnału
    }
    for (i = N; i < M-N; i++)
    {
        temp_signal[i] = signal[i-N];       // środkowe L próbek sygnału rozszerzonego równe kolejnym próbkom sygnału
    }
    for (i = M-N-1; i < M; i++)
    {
        temp_signal[i] = signal[L-1];       // ostatnie N próbek sygnału rozszerzonego równe ostatniej próbce sygnału
    }
    for (k = 0; k < L; k++)
    {
        my_signal = 0;
        for (j = 0; j < 2*N+1; j++)
        {
            my_signal = my_signal + temp_signal[j+k];       // obliczanie kroczącej średniej (cz. I)
        }
        moved_signal[k] = my_signal;
    }
    for (i = 0; i < L; i++)
    {
        moved_signal[i] = moved_signal[i] / (2*N+1);        // obliczanie kroczącej średniej (cz. II)
        result_signal.push_back(signal[i] - moved_signal[i]);     // odejmowanie kroczącej średniej od sygnału
    }
    return result_signal;
}

vector<float> ECG_Baseline::SavitzkyGolayAlgorithm(vector<float> values)
{
    vector<float> signal = values;
    int N = signal.size();
    int M = 9;                                              // ilość współczynników filtra Savitzky-Golay
    int m = (M-1) / 2;                                      // numery współczynników
    int K = N + 2*m;                                        // długość rozszerzonego sygnału
    float b0[9] = {-0.0909, 0.0606, 0.1688, 0.2338, 0.2554, 0.2338, 0.1688, 0.0606, -0.0909};   // współczynniki
    float temp_signal[K];                                   // rozszerzony sygnał
    vector<float> result_signal;
    int i, j;
    float sum;
    for (i = 0; i < m; i++)
    {
        temp_signal[i] = signal[0];             // pierwsze m próbek rozszerzonego sygnału równe pierwszej próbce
    }
    for (i = m+1; i < K-m+1; i++)
    {
        temp_signal[i] = signal[i-m-1];         // środkowe N próbek rozszerzonego sygnału równe oryginalnemu sygnałowi
    }
    for (i = K-m; i < K; i++)
    {
        temp_signal[i] = signal[N-1];           // końcowe m próbek rozszerzonego sygnału równe ostatniej próbce
    }
    for (i = m; i < N-m+1; i++)
    {
        sum = 0;
        for (j = -m; j < m+1; j++)
        {
            sum = sum + b0[j+m] * temp_signal[i+j+m];       // obliczanie wartości ze wzoru Savitzky'ego-Golaya
        }
        result_signal.push_back(sum);
    }
    return result_signal;
}
vector<float> ECG_Baseline::KalmanFilter(vector<float> values)
{
    vector<float> U = values;
    vector<float> result;
    int i = U.size();
    int R = 40; // kowariancja szumu
    float H = 1;
    int Q = 10;
    int P = 0; // początkowa kowariancja błędu
    float K = 0;
    P = (1-K*H)*P+Q;
    K = (P*H)/(H*P*H + R);
    vector<float> Kalman;
    float U_hat[i];
    U_hat[0] = 0;
    for(int c = 0; c < i; c++)
    {
        U_hat[c+1] = U_hat[c] + K*(U[c]-H*U_hat[c]);
        if (c > 20)
        {
            Kalman.push_back(U_hat[c]);
        }
    }
    result = Kalman;
    return result;
}
