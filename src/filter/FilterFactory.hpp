#pragma once
#include <memory>
#include <string>

#include "filter/IFilter.hpp"

enum class FilterId {
    None = 0,
    Mirror,
    Negative,
    Pixel,
    PseudoColor,
    Cartoon,
    Edge,
    Glasses
};

std::unique_ptr<IFilter> makeFilter(FilterId id);
std::string filterName(FilterId id);
