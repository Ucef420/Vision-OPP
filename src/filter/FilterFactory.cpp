#include "FilterFactory.hpp"

#include "filter/BackgroundCompositeFilter.hpp"
#include "background/ParticlesBackground.hpp"
// #include "background/VideoLoopBackground.hpp"

#include "filter/MirrorFilter.hpp"
#include "filter/NegativeFilter.hpp"
#include "filter/PixelFilter.hpp"
#include "filter/PseudoColorFilter.hpp"
#include "filter/CartoonFilter.hpp"
#include "filter/EdgeFilter.hpp"
#include "filter/GlassesFilter.hpp"
#include "filter/PortraitBlurFilter.hpp"

#include <memory>

std::unique_ptr<IFilter> makeFilter(FilterId id) {

    static std::shared_ptr<IBackgroundProvider> bgParticles =
        std::make_shared<ParticlesBackground>(300);

    // plus tard tu peux remplacer par vidéo :
    // static std::shared_ptr<IBackgroundProvider> bgVideo =
    //     std::make_shared<VideoLoopBackground>("assets/bg_office.mp4");

    switch (id) {
        case FilterId::Mirror:       return std::make_unique<MirrorFilter>();
        case FilterId::Negative:     return std::make_unique<NegativeFilter>();
        case FilterId::Pixel:        return std::make_unique<PixelFilter>();
        case FilterId::PseudoColor:  return std::make_unique<PseudoColorFilter>();
        case FilterId::Cartoon:      return std::make_unique<CartoonFilter>();
        case FilterId::Edge:         return std::make_unique<EdgeFilter>();

        case FilterId::PortraitBlur:
            return std::make_unique<PortraitBlurFilter>(
                "assets/cascades/haarcascade_frontalface_default.xml"
            );

        case FilterId::Glasses:
            return std::make_unique<GlassesFilter>(
                "assets/cascades/haarcascade_frontalface_default.xml",
                "assets/cascades/haarcascade_eye.xml",
                "assets/glasses.png"
            );

        case FilterId::Background:
            // IMPORTANT: chemin relatif au dossier depuis lequel tu lances l’exe
            return std::make_unique<BackgroundCompositeFilter>(
                bgParticles,
                "../src/assets/models/modnet.onnx"
            );

        case FilterId::None:
        default:
            return nullptr;
    }
}

std::string filterName(FilterId id) {
    switch (id) {
        case FilterId::None:         return "None";
        case FilterId::Mirror:       return "Mirror";
        case FilterId::Negative:     return "Negative";
        case FilterId::Pixel:        return "Pixel";
        case FilterId::PseudoColor:  return "PseudoColor";
        case FilterId::Cartoon:      return "Cartoon";
        case FilterId::Edge:         return "Edge";
        case FilterId::PortraitBlur: return "PortraitBlur";
        case FilterId::Glasses:      return "Glasses";
        case FilterId::Background:   return "Background";
        default:                     return "Unknown";
    }
}
