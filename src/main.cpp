#include "app/VisionApp.hpp"
#include "source/CameraSource.hpp"
#include "pipeline/Pipeline.hpp"
#include "filter/DummyFilter.hpp"
#include "output/WindowSink.hpp"
#include <iostream>
#include "filter/EdgeFilter.hpp"
#include "filter/NegativeFilter.hpp"
#include "filter/PixelFilter.hpp"
#include "filter/PseudoColorFilter.hpp"
#include "filter/CartoonFilter.hpp"

int main() {
    std::cerr << "=== DEBUT ===" << std::endl;
    
    try {
        std::cerr << "Tentative ouverture camera..." << std::endl;
        auto source = std::make_unique<CameraSource>(0);  // Index 0 au lieu de 2
        std::cerr << "Camera ouverte!" << std::endl;

        std::cerr << "Creation fenetre..." << std::endl;
        auto sink = std::make_unique<WindowSink>("Vision OOP");
        std::cerr << "Fenetre creee!" << std::endl;

        std::cerr << "Configuration pipeline..." << std::endl;
        Pipeline pipeline;
        pipeline.addFilter(std::make_unique<DummyFilter>());
        std::cerr << "Pipeline pret!" << std::endl;
        pipeline.addFilter(std::make_unique<PseudoColorFilter>());        
        pipeline.addFilter(std::make_unique<NegativeFilter>());        
        
        std::cerr << "Demarrage application..." << std::endl;
        VisionApp app(std::move(source), std::move(pipeline), std::move(sink));
        
        std::cerr << "Lancement boucle..." << std::endl;
        app.run();
        
        std::cerr << "Fin normale" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ERREUR: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}