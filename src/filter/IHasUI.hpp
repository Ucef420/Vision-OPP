#pragma once
#include <string>

class IHasUI {
public:
    virtual ~IHasUI() = default;

    // Crée les trackbars (une seule fois)
    virtual void setupUI(const std::string& windowName,
                         const std::string& prefix) = 0;

    // Lit les valeurs des trackbars -> variables internes du filtre
    virtual void syncFromUI() = 0;
};
