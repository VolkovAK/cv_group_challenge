#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <random>
#include <chrono>

typedef std::vector<int> Series;

std::vector<Series> tests{
    {5, 15, 14, 13, 12, 16, 10},
    {5},
    {},
    {1,2,3},
    {3,2,1},
    {3,2,1,2,3},
    {1,2,1},
    {1,2,2,2,1},
    {5, 5, 5, 5, 5, 16, 10, 5, 5, 5, 5},
    {5, 5, 5, 5, 5, 16, 10, 5, 5, 5, 5, 5},
    {5, 5, 5, 5, 5, 16, 10, 5, 5, 5, 5, 5, 5},
    {5, 5, 5, 5, 5, 16, 10},
    {5, -15, -14, -13, -12, 16, 10},
    {5, -12, -13, -14, -15, 16, 10},
    {5, 15, 14, 13, 12, 16, 10, 15, 14, 13, 12, 11, 10, 15, 14, 13, 12, 11, 10, 9, -10, -20},
    {5, 15, 14, 13, 12, 16, 10, 15, 14, 13, 12, 11, 10, 15, 14, 13, 12, 11, 10, 9, -10, -20, 2, 3},
    {5, 15, 14, 13, 12, 16, 10, 15, 14, 13, 12, 13, 12, 11, 10, 9, -10, -20, 11, 10, 15, 14}
};

struct Result{
    size_t start = 0;
    size_t stop = 0;
    size_t len = 0; 
};


Result find_longest_series_volkov(const Series& series){
    std::array<size_t, 2> edges_current{0, 0};
    std::array<size_t, 2> edges_biggest{0, 0};
    auto current_direction = 0;
    auto prev_direction = 0;
    
    for (size_t i = 0; i < series.size(); i++) {
        if (edges_current[1] - edges_current[0] > edges_biggest[1] - edges_biggest[0]) {
            edges_biggest[0] = edges_current[0];
            edges_biggest[1] = edges_current[1];
        }
        if (i == series.size() - 1) {
            break;
        }
        if (series[i+1] - series[i] > 0) {
            current_direction = 1;
        }
        else if (series[i+1] - series[i] < 0) {
            current_direction = -1;
        }
        // no diff, still monotonic
        edges_current[1] = i+1;
        if (prev_direction != current_direction && prev_direction != 0) {
            edges_current[0] = i;
        }
        prev_direction = current_direction;
    }
    auto len = series.size() == 0 ? 0 : edges_biggest[1] - edges_biggest[0] + 1;
    auto res = Result { edges_biggest[0], edges_biggest[1], len };

    return res;
}


// @fedruches comparator
auto first_comp = [](auto x, auto y) {
  if (x.second == y.second) {
    return std::min(x, y, [](auto x, auto y) { return x.first < y.first; });
  } else
    return std::max(x, y,
                    [](auto x, auto y) { return x.second < y.second; });
};
Result find_longest_series_fedruches(const Series& input) {
    // @fedruches
    std::pair<int, int> asc = {0, 0};
    std::pair<int, int> dsc = {0, 0};
    std::pair<int, int> max_pos = asc;
    std::pair<int, int> max_neg = dsc;
    bool is_pos = false;
    bool is_neg = false;

  if (input.size() < 2) {
      return Result { 0, 0, 0 };
  }

  for (int i = 0; i < input.size() - 1; ++i) {
    auto diff = input[i] - input[i + 1];

    if (diff < 0) {
      ++asc.second;

      if (!is_pos) {
        asc.first = i;
      }
      is_pos = true;
      is_neg = false;

      if (dsc.second > max_neg.second) {
        max_neg = dsc;
        dsc.second = 0;
      }
    } else if (diff > 0) {
      ++dsc.second;

      if (is_pos) {
        dsc.first = i;
      }
      is_pos = false;
      is_neg = true;

      if (asc.second > max_pos.second) {
        max_pos = asc;
        asc.second = 0;
      }
    }
    else {
      ++dsc.second;
      ++asc.second;
      is_pos = true;
      is_neg = true;
    }
  }


  auto pos = first_comp(max_pos, asc);
  auto neg = first_comp(max_neg, dsc);
  auto res = first_comp(pos, neg);

  return Result { 
        static_cast<size_t>(res.first), 
        static_cast<size_t>(res.first + res.second), 
        static_cast<size_t>(res.second + 1) 
    };
}



Result find_longest_series(const Series& series){
    // @iskinmike https://github.com/iskinmike/cv_group_challenge/tree/main
    Result res;

    size_t start = 0;
    size_t stop = 0;
    size_t len = 0;
    bool trend_up = false;
    bool trend_down = false;

    for (auto curr = 1; curr < series.size(); ++curr) {
        auto prev = curr-1;
        auto diff = series[curr] - series[prev];
        bool new_trend_up = true;
        bool new_trend_down = true;
        if (diff > 0) {
            new_trend_down = false; // decide that trend is not down
        } else if (diff < 0) {
            new_trend_up = false; // decide that trend is not up
        }
        
        if ((new_trend_up && trend_up) || 
            (new_trend_down && trend_down) || 
            (new_trend_down == new_trend_up)) 
        {
            len += 1;
            stop = curr;
        } else {
            if (len > res.len) {
                res.len = len;
                res.start = start;
                res.stop = stop;
            }
            start = prev;
            stop = curr;
            len = 1;
            trend_up = new_trend_up;
            trend_down = new_trend_down;
        }
    }

    if (len > res.len) {
        res.len = len;
        res.start = start;
        res.stop = stop;
    }

    return res;
}


void benchmark()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(-100, 100);
    auto gen = [&dist, &rng](){ return dist(rng); };

    std::vector<int> test(1000000);
    std::generate(test.begin(), test.end(), gen);

    uint64_t total_elapsed = 0;
    uint64_t total_generated = 0;
    for (size_t i = 0; i < 100; i++) {
        auto start = std::chrono::steady_clock::now();
        std::generate(test.begin(), test.end(), gen);
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        total_generated += elapsed;

        start = std::chrono::steady_clock::now();
        //
        // find_longest_series_fedruches(test);
        find_longest_series_volkov(test);
        // find_longest_series(test);

        end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        total_elapsed += elapsed;
    }
    std::cout << "Elapsed time in nanoseconds: gen " << total_generated << ", calc "<<  total_elapsed << std::endl;
}

int main(int argc, char const *argv[])
{
    std::cout << "Challenge accepted!" << std::endl;
    // for (auto& test : tests) {
    //     for (auto val : test){
    //         std::cout << val << ", ";
    //     }
    //     std::cout << std::endl;
    //
    //     auto res = find_longest_series_volkov(test);
    //     std::cout << "res [" << res.start << ", " << res.stop << "]  len:" << res.len << std::endl;
    // }

    for (size_t i = 0; i < 10; i++) {
        benchmark();
    }
    return 0;
}