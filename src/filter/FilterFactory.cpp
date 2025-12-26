#include "FilterFactory.hpp"

#include "filter/MirrorFilter.hpp"
#include "filter/NegativeFilter.hpp"
#include "filter/PixelFilter.hpp"
#include "filter/PseudoColorFilter.hpp"
#include "filter/CartoonFilter.hpp"
#include "filter/EdgeFilter.hpp"
#include "filter/GlassesFilter.hpp"

std::unique_ptr<IFilter> makeFilter(FilterId id) {
    switch (id) {
        case FilterId::Mirror:      return std::make_unique<MirrorFilter>();
        case FilterId::Negative:    return std::make_unique<NegativeFilter>();
        case FilterId::Pixel:       return std::make_unique<PixelFilter>();
        case FilterId::PseudoColor: return std::make_unique<PseudoColorFilter>();
        case FilterId::Cartoon:     return std::make_unique<CartoonFilter>();
        case FilterId::Edge:        return std::make_unique<EdgeFilter>();
        case FilterId::Glasses:
            return std::make_unique<GlassesFilter>(
                "assets/cascades/haarcascade_frontalface_default.xml",
                "assets/cascades/haarcascade_eye.xml",
                "assets/glasses.png"
            );
        case FilterId::None:
        default:
            return nullptr;
    }
}

std::string filterName(FilterId id) {
    switch (id) {
        case FilterId::None:        return "None";
        case FilterId::Mirror:      return "Mirror";
        case FilterId::Negative:    return "Negative";
        case FilterId::Pixel:       return "Pixel";
        case FilterId::PseudoColor: return "PseudoColor";
        case FilterId::Cartoon:     return "Cartoon";
        case FilterId::Edge:        return "Edge";
        case FilterId::Glasses:     return "Glasses";
        default:                    return "Unknown";
    }
}
