#pragma once
#include <sstream>

template<typename T>
constexpr auto SSTR(T  x) { return (std::ostringstream() << std::dec << x).str(); }
