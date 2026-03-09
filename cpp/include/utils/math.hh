#pragma once

#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <variant>
#include <functional>
#include <array>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>

namespace cfd {

template<typename T>
using Box = std::unique_ptr<T>;

template<typename T>
using Rc = std::shared_ptr<T>;

template<typename T>
using WeakRc = std::weak_ptr<T>;

template<typename... Ts>
using Variant = std::variant<Ts...>;

template<typename T>
constexpr auto size_of = sizeof(T);

constexpr float PI = 3.14159265358979323846f;
constexpr float PI_2 = PI / 2.0f;
constexpr float PI_4 = PI / 4.0f;
constexpr float DEG_TO_RAD = PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI;

constexpr float GRAVITY = 9.80665f;
constexpr float AIR_GAS_CONSTANT = 287.05287f;
constexpr float GAMMA_AIR = 1.4f;
constexpr float SEA_LEVEL_PRESSURE = 101325.0f;
constexpr float SEA_LEVEL_TEMPERATURE = 288.15f;
constexpr float SEA_LEVEL_DENSITY = 1.225f;

constexpr float MPH_TO_MS = 0.44704f;
constexpr float MS_TO_MPH = 2.23694f;
constexpr float FT_TO_M = 0.3048f;
constexpr float M_TO_FT = 3.28084f;
constexpr float KNOT_TO_MS = 0.514444f;
constexpr float MI_TO_M = 1609.34f;
constexpr float KM_TO_M = 1000.0f;
constexpr float LBF_TO_N = 4.44822f;
constexpr float PSI_TO_PA = 6894.76f;

}
