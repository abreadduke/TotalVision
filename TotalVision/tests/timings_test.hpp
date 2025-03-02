#pragma once
#include "../timings.hpp"
#include <thread>
#include <cassert>
#include <regex>
#include <filesystem>
#include "../commandhandler.hpp"

#define LOG(log_msg) std::cout << log_msg << std::endl
void TestTimings();