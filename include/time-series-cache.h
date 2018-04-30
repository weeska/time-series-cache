#pragma once

#include <algorithm>
#include <chrono>
#include <deque>
#include <stdexcept>

template <typename Sample, typename Key>
class time_series_cache {
public:
    struct Entry {
        Key key;
        Sample sample;
    };

    time_series_cache(size_t max_size)
        : max_size(max_size)
    {
        if(max_size == 0) {
            throw std::invalid_argument("Maximum Size must be greater than zero!");
        }
    }


    void set_max_size(size_t max_size) {
        if(max_size == 0) {
            throw std::invalid_argument("Maximum Size must be greater than zero!");
        }

        this->max_size = max_size;
        while(series.size() > max_size) {
            series.pop_front();
        }

    }

    void add(Entry entry) {
        if(series.size() == max_size) {
            series.pop_front();
        }

        series.push_back(entry);
    }

    Sample get(const Key &key) {

        if(series.empty()) {
            return Sample();
        }

        const auto less = [](const Entry &a, const Entry &b) {
            return a.key < b.key;
        };

        Entry s{key, Sample()};
        const auto lower = std::lower_bound(std::begin(series), std::end(series), s, less);

        const bool atEnd = lower == std::end(series);
        if(atEnd) {
            return (lower-1)->sample;
        }

        const bool exactMatch = lower->key == key;
        const bool atStart = lower == std::begin(series);

        //prefer sample that is the last before the specified key
        //also, if there is no match, return the last
        const bool isLateSample = (!exactMatch && !atStart) || atEnd;

        if(isLateSample) {
            return (lower-1)->sample;
        }

        return lower->sample;
    }

private:
    size_t max_size;
    std::deque<Entry> series;
};
