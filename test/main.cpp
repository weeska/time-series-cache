#include <iostream>
#include <chrono>
#include <random>

#include "../include/time-series-cache.h"

#define CATCH_CONFIG_MAIN
#include "../extern/catch-mini/catch.hpp"

using namespace std;

TEST_CASE("Should initially only accept sizes greater zero") {
    try {
        time_series_cache<float, int> cache(0);
        REQUIRE(false);
    } catch( std::invalid_argument &e) {
        REQUIRE(true);
    }
}

TEST_CASE("Should only accept sizes greater zero") {
    try {
        time_series_cache<float, int> cache(1);
        cache.set_max_size(0);
        REQUIRE(false);
    } catch( std::invalid_argument &e) {
        REQUIRE(true);
    }
}

TEST_CASE("Works with one sample") {
    time_series_cache<float, int> cache(5);

    cache.add({1, 1.0f});

    REQUIRE(cache.get(0) == 1.0f);
    REQUIRE(cache.get(1) == 1.0f);
    REQUIRE(cache.get(2) == 1.0f);
}

TEST_CASE("Should remove values if new size is to small") {
    time_series_cache<float, int> cache(5);

    cache.add({1, 1.0f});
    cache.add({2, 2.0f});
    cache.add({3, 3.0f});
    cache.add({4, 4.0f});
    cache.add({5, 5.0f});

    cache.set_max_size(4);

    REQUIRE(cache.get(2) == 2.0f);
}

TEST_CASE("Fetches the oldest sample if timestamp is too old") {
    time_series_cache<float, int> cache(100);

    cache.add({4, 4.0f});
    cache.add({5, 5.0f});
    cache.add({6, 6.0f});

    REQUIRE(cache.get(3) == 4.0f);
}

TEST_CASE("Prefer older samples if keys don't match exactly") {
    time_series_cache<float, int> cache(100);

    cache.add({3, 3.0f});
    cache.add({4, 4.0f});
    cache.add({6, 6.0f});
    cache.add({7, 7.0f});

    REQUIRE(cache.get(5) == 4.0f);
}

TEST_CASE("Fetches the matching sample") {
    time_series_cache<float, int> cache(100);

    cache.add({4, 4.0f});
    cache.add({5, 5.0f});
    cache.add({6, 6.0f});

    REQUIRE(cache.get(5) == 5.0f);
}

TEST_CASE("Fetches the latest sample if the timestamp is too new") {
    time_series_cache<float, int> cache(100);

    cache.add({3, 3.0f});
    cache.add({4, 4.0f});
    cache.add({5, 5.0f});
    cache.add({6, 6.0f});

    REQUIRE(cache.get(7) == 6.0f);
}

TEST_CASE("Does not crash on empty cache") {
    time_series_cache<float, int> cache(1);

    REQUIRE(cache.get(1) == float());
}

TEST_CASE("Caches a maximum number of elements") {
    time_series_cache<float, int> cache(5);

    cache.add({1, 1.0f});
    cache.add({2, 2.0f});
    cache.add({3, 3.0f});
    cache.add({4, 4.0f});
    cache.add({5, 5.0f});
    cache.add({6, 6.0f});

    REQUIRE(cache.get(1) == 2.0f);
}
