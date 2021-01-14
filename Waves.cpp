#include "Waves.h"

//std::vector<std::vector<int>> Waves::setData(const std::vector<float>& signal, const std::vector<int>& r_peaks)
void Waves::setData( std::vector<float> ecg, const std::vector<int>& r_peaks)
{
    // detekcja Q
    Q = detectQ(ecg, r_peaks);
    // detekcja S

    S = detectS(ecg, r_peaks);
    // Wycięcie zespołu qrs
    for (size_t i{ 0 }; i < r_peaks.size(); ++i)
    {
        // wypełnienie zerami znalezione piki( sygnal, od, do )
        fillZero(ecg, Q[i], S[i] - Q[i]);
    }
    // Określenie części roboczej sygnału w celu usunięcia
    // wartości marginP i marginT domyślnie z pdf
    constexpr auto marginP = 30;
    constexpr auto marginT = 33;
    const auto sliceBegin = r_peaks[0] - marginP;
    const auto sliceEnd = r_peaks[r_peaks.size() - 1] + marginT;
    // stworzenie nowego array'a na podstawie wycinka ecg[poczatek, długosc wycinka, krok]
    std::vector<float> ecg1 = sliceArray(ecg, sliceBegin, sliceEnd);

    // Potęgowanie sygnałów
    std::vector<float> ecg2(ecg1.size());
    std::transform(ecg1.begin(), ecg1.end(), ecg2.begin(), [](const float& el) {return el * el; });
    ecg1.resize(0);// nie potrzeba juz wiecej tego wektora wiec kasujemy mu pamiec
    // Detekcja piku T
    T = detectT(ecg2, r_peaks, marginP);
    ecg2.resize(0);// nie potrzeba juz wiecej tego wektora wiec kasujemy mu pamiec
    // T_offset & T_onset
    // wartości distT_offset i distT_onset domyślnie z pdf
    constexpr auto distT_offset = 50;
    constexpr auto distT_onset = 50;
    constexpr auto Ttr = 0.001;
    T_onset = detectOnset(T, ecg, distT_onset, Ttr);
    T_offset = detectOffset(T, ecg, distT_offset, Ttr);

    //wycięcie T z sygnału
    for (size_t i{ 0 }; i < r_peaks.size(); ++i)
    {
        fillZero(ecg, T_onset[i], T_offset[i] - T_onset[i]);
    }
    // Detekcja piku P
    // stworzenie nowego array'a na podstawie wycinka ecg[poczatek, długosc wycinka, krok]
    P = detectP(sliceArray(ecg, sliceBegin, sliceEnd), r_peaks, marginP);
    // wartość Ptr domyślnie z pdf
    constexpr auto Ptr = 0.001;
    // P_offset & P_onset
    P_onset = detectOnset(P, ecg, distT_onset, Ptr);
    P_offset = detectOffset(P, ecg, distT_offset, Ptr);
}

std::vector<std::vector<int>> Waves::getResults() const
{
    return { Q, S, T, T_onset, T_offset, P, P_onset, P_offset };
}


std::vector<int> Waves::detectQ(const std::vector<float>& array, const std::vector<int>& r_peaks) const
{
    // vector o rozmiarze takim jak r_peaks
    std::vector<int> Q(r_peaks.size());
    // domyslne wartości z pdf
    constexpr auto distQ = 50;
    constexpr auto qtr = 0.01;
    for (size_t j{ 0 }; j < r_peaks.size(); ++j)
    {
        for (size_t i{ 0 }; i < distQ; ++i)
        {
            try {
                // to może wyrzucić wyjątek, jeśli r_peak < default distQ
                if (array[r_peaks[j] - i] < qtr)
                {
                    Q[j] = static_cast<int>(r_peaks[j] - i);
                    break;
                }
            }
            catch (...) {
                std::cerr << "ERROR invalid signal or r_peaks size" << std::endl;
                return {}; // błędne dane zwróci pusty wektor
            }
        }
    }
    return Q;
}

std::vector<int> Waves::detectS(const std::vector<float>& array, const std::vector<int>& r_peaks) const
{
    // vector o rozmiarze takim jak r_peaks
    std::vector<int> S(r_peaks.size());
    // wartość distS z pdf
    constexpr auto distS = 20;
    for (size_t i{ 0 }; i < r_peaks.size(); ++i)
    {
        try {
            // sprawdz minimum w zakresie, zwróci iterator do najmniejszego elementu
            const auto pos = std::min_element(std::begin(array) + r_peaks[i], std::begin(array) + r_peaks[i] + distS);
            // przypisanie na którym indeksie w array'u jest najmniejszy element. Stąd ustalenie odległości minimum od poczatku array'a
            S[i] = static_cast<int>(std::distance(std::begin(array), pos));
        }
        catch (...) {
            std::cerr << "ERROR! Invalid r_peak value. Value of r_peak is over ecg size!" << std::endl;
            return {}; // błędne dane zwróci pusty wektor
        }
    }
    return S;
}

std::vector<int> Waves::detectT(const std::vector<float>& array, const std::vector<int>& r_peaks, const size_t margin) const
{
    std::vector<int> T(r_peaks.size());
    // T_thr wartość domyslna z pdf
    constexpr auto T_thr = 0.1;
    std::vector<float> D(array.size());
    // różnica między kolejnymi elementami arraya. (1szy nie uzywany, w matabie pomijany jest)
    std::adjacent_difference(std::begin(array), std::end(array), std::begin(D));
    for (size_t i{ 1 }, j{ 0 }; i < array.size(); ++i)
    {
        if (array[i] > T_thr && D[i - 1] > 0.0001 && D[i] < 0.0001)
        {
            try {
                T[j++] = static_cast<int>(i + r_peaks[0] - margin);
            }
            catch (...) {
                std::cerr << "ERROR! invalid index of array!" << std::endl;
                return {}; // błędne dane zwróci pusty wektor
            }
        }
    }
    return T;
}

std::vector<int> Waves::detectP(const std::vector<float>& array, const std::vector<int>& r_peaks, const size_t margin) const
{
    return detectT(array, r_peaks, margin);
}

std::vector<int> Waves::detectRange(const std::vector<int>& vector,
    const std::vector<float>& array,
    const float limiter,
    const float precision,
    const Range onOff) const
{
    std::vector<int> range(vector.size());
    const auto sign = onOff == Range::OFFSET ? 1 : -1;
    for (size_t j{ 0 }; j < vector.size(); ++j)
    {
        for (size_t i{ 0 }; i < limiter; ++i)
        {
            if (array[vector[j] + (i * sign)] < precision)
            {
                range[j] = static_cast<int>(vector[j] + (i * sign));
                break;
            }
        }
    }
    return range;
}

std::vector<int> Waves::detectOnset(const std::vector<int>& vector,
    const std::vector<float>& array,
    const float limiter,
    const float precision) const
{
    return detectRange(vector, array, limiter, precision, Range::ONSET);
}

std::vector<int> Waves::detectOffset(const std::vector<int>& vector,
    const std::vector<float>& array,
    const float limiter,
    const float precision) const
{
    return detectRange(vector, array, limiter, precision, Range::OFFSET);
}

std::vector<float> Waves::sliceArray(std::vector<float>& array, const size_t left, const size_t right) const
{
    if (right > array.size() || (right - left) > array.size())
    {
        std::cerr << "ERROR! Invalid margins!" << std::endl;
        return {}; // błędne dane zwróci pusty wektor
    }
    // stworzenie nowego array'a na podstawie wycinka
    return std::vector<float>(array.begin() + left, array.begin() + right);
}

void Waves::fillZero(std::vector<float>& array, const size_t from, const size_t to) const
{
    std::fill_n(array.begin() + from, to, 0.);
}
