#ifndef WAVES
#define WAVES

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

class Waves
{
public:
    Waves() = default;
    ~Waves() = default;

    void setData(const std::vector<float> ecg, const std::vector<int>& r_peaks);
    std::vector<std::vector<int> > getResults() const;

private:
    /* METHODS */

    std::vector<int> detectQ(const std::vector<float>& array, const std::vector<int>& r_peaks) const;
    std::vector<int> detectS(const std::vector<float>& array, const std::vector<int>& r_peaks) const;
    std::vector<int> detectT(const std::vector<float>& array, const std::vector<int>& r_peaks, const size_t margin) const;
    std::vector<int> detectP(const std::vector<float>& array, const std::vector<int>& r_peaks, const size_t margin) const;

    enum class Range { ONSET, OFFSET };
    std::vector<int> detectRange(const std::vector<int>& vector,
        const std::vector<float>& array,
        const float limiter,
        const float precision,
        const Range onOff) const;
    std::vector<int> detectOnset(const std::vector<int>& vector,
        const std::vector<float>& array,
        const float limiter,
        const float precision) const;
    std::vector<int> detectOffset(const std::vector<int>& vector,
        const std::vector<float>& array,
        const float limiter,
        const float precision) const;

    std::vector<float> sliceArray(std::vector<float>& array, const size_t left, const size_t rigth) const;

    void fillZero(std::vector<float>& array, const size_t from, const size_t to) const;

    /* MEMBERS */
    std::vector<int> Q;
    std::vector<int> S;
    std::vector<int> T;
    std::vector<int> T_onset;
    std::vector<int> T_offset;
    std::vector<int> P;
    std::vector<int> P_onset;
    std::vector<int> P_offset;
};

#endif // WAVES
