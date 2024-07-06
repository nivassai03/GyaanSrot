#include <iostream>
#include <string_view>
#include <string>
#include <chrono>

std::string titleToSlug(const std::string &title, const char del = '-', const int len = 50);
std::string generateImageDir(const std::string &category, const std::string &source);
std::string generateImageDir(std::string_view category, std::string_view source);
std::chrono::system_clock::time_point fromEpochSeconds(int64_t epochtime);
int64_t fromTimePoint(const std::chrono::system_clock::time_point &tp);
