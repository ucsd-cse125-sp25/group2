#pragma once
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class TimeDebugger {
public:
  // Used to time a function via RAII
  class ScopedTimer {
  public:
    ScopedTimer(const std::string &label)
        : label(label), start(std::chrono::high_resolution_clock::now()) {}
    ~ScopedTimer() {
      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration<double, std::milli>(end - start).count();
      TimeDebugger::addTiming(label, duration);
    }

  private:
    std::string label;
    std::chrono::high_resolution_clock::time_point start;
  };

  static void addTiming(const std::string &label, double milliseconds) {
    std::lock_guard<std::mutex> lock(mutex_);
    timings_[label] = milliseconds;
    counts_[label]++;
  }

  static void printSummary() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::pair<std::string, double>> sortedTimings;
    for (const auto &[label, total] : timings_) {
      sortedTimings.emplace_back(label, total);
    }

    std::sort(sortedTimings.begin(), sortedTimings.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; });

    std::cout << "\n=== Time Debugger Summary ===\n";
    for (const auto &[label, total] : sortedTimings) {
      int count = counts_[label];
      std::cout << std::setw(30) << std::left << label << std::setw(10) << count
                << " calls | " << std::fixed << std::setprecision(3) << total
                << " ms total | ";
    }
    std::cout << "=============================\n";
  }

private:
  static inline std::unordered_map<std::string, double> timings_;
  static inline std::unordered_map<std::string, int> counts_;
  static inline std::mutex mutex_;
};