#include "app/VisionApp.hpp"
#include "source/CameraSource.hpp"
#include "output/WindowSink.hpp"
#include <iostream>

int main() {
    std::cerr << "=== DEBUT PROGRAMME ===" << std::endl;

    try {
        // 1) Création de la source vidéo (webcam)
        std::cerr << "[MAIN] Tentative ouverture camera..." << std::endl;
        auto source = std::make_unique<CameraSource>(0);
        std::cerr << "[MAIN] Camera ouverte avec succes." << std::endl;

        // 2) Création de la fenêtre de sortie
        std::cerr << "[MAIN] Creation de la fenetre..." << std::endl;
        auto sink = std::make_unique<WindowSink>("Vision OOP");
        std::cerr << "[MAIN] Fenetre creee." << std::endl;

        // 3) Création de l'application principale
        // VisionApp gère maintenant :
        // - les filtres gauche / droite
        // - le clavier
        // - les trackbars (UI)
        // - le split-screen
        std::cerr << "[MAIN] Creation de VisionApp..." << std::endl;
        VisionApp app(
            std::move(source),
            std::move(sink)
        );
        std::cerr << "[MAIN] VisionApp creee." << std::endl;

        // 4) Lancement de la boucle principale
        std::cerr << "[MAIN] Lancement de la boucle principale..." << std::endl;
        app.run();

        std::cerr << "[MAIN] Fin normale du programme." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "[MAIN] ERREUR FATALE: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
