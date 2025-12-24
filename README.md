# Vision OOP - Vision par Ordinateur en C++ avec OpenCV

Un projet de vision par ordinateur structuré en orienté objet utilisant OpenCV pour le traitement d'images en temps réel.

## Table des Matières
- [Architecture](#architecture)
- [Prérequis](#prérequis)
- [Installation de MSYS2](#installation-de-msys2)
- [Récupération du projet](#récupération-du-projet)
- [Compilation](#compilation)
- [Exécution](#exécution)
- [Tests](#tests)
- [Dépannage](#dépannage)
- [Structure du Projet](#structure-du-projet)

## Architecture

Le projet suit une architecture orientée objet modulaire :

FrameSource → Pipeline (Filters) → OutputSink

- **FrameSource** : Acquisition d'images (webcam, fichier vidéo)
- **Pipeline** : Chaîne de traitement d'images (filtres, transformations)
- **OutputSink** : Sortie (affichage, enregistrement, streaming)

Le dépôt inclut également un exécutable **camera_test** pour tester la webcam avant d'utiliser le programme principal.

## Prérequis

- **Système d'exploitation** : Windows 10 / Windows 11
- **Matériel** : Webcam (intégrée ou USB)
- **Environnement** : MSYS2 installé
- **Terminal** : **MSYS2 UCRT64** (utilisez ce terminal spécifiquement)

⚠️ **Important** : Évitez WSL pour l'accès à la webcam. Le projet doit être exécuté en **Windows natif**.

## Installation de MSYS2

1. **Télécharger** MSYS2 depuis le site officiel (https://www.msys2.org/)
2. **Installer** MSYS2 (exemple : C:\msys64)
3. **Ouvrir** **MSYS2 UCRT64** depuis le menu Démarrer

Le terminal doit afficher :
UCRT64 /c/...

### Installation des dépendances

Dans **MSYS2 UCRT64** :

1. Mettre à jour le système :
pacman -Syu

Si MSYS2 demande de fermer le terminal, faites-le puis rouvrez **MSYS2 UCRT64**.

2. Installer les outils de compilation et dépendances :
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-opencv mingw-w64-ucrt-x86_64-qt6-base

**Pourquoi Qt6 ?**
OpenCV (HighGUI) utilise Qt pour afficher les fenêtres (cv::imshow). Sans Qt6, l'exécution peut échouer avec des DLL manquantes (ex: Qt6Core.dll).

## Récupération du projet

### Option A — Cloner depuis GitHub
cd /c/Users/Lenovo/Desktop
git clone https://github.com/<TON_USERNAME>/<TON_REPO>.git
cd <TON_REPO>

### Option B — Projet déjà présent sur le PC
cd "/c/Users/Lenovo/Desktop/Projet cpp"

Vérifiez que ces éléments existent :
- CMakeLists.txt
- Répertoire src/
- camera_test.cpp

## Compilation (Build)

Depuis la racine du projet dans **MSYS2 UCRT64** :

# Nettoyer les builds précédents (optionnel)
rm -rf build

# Configurer avec CMake
cmake -S . -B build -G "MinGW Makefiles"

# Compiler
cmake --build build

**Résultat attendu :**
Built target vision_oop
Built target camera_test

## Exécution (IMPORTANT)

**Note importante** : Ne pas lancer directement les .exe depuis MSYS2.

Les DLL OpenCV et Qt se trouvent dans :
C:\msys64\ucrt64\bin

Il faut donc lancer les programmes via CMD en ajoutant ce dossier au PATH.

## Tests

### Test webcam (camera_test)

À faire en premier pour vérifier que la webcam fonctionne.

Depuis **MSYS2 UCRT64** :
cmd.exe /c "set PATH=C:\msys64\ucrt64\bin;%PATH% && cd /d C:\Users\Lenovo\Desktop\Projet cpp\build && camera_test.exe & pause"

**Attendu :**
- Une fenêtre s'ouvre avec la vidéo en temps réel
- Appuyez sur ESC pour quitter
- pause garde la console ouverte après la fermeture

### Lancer le programme principal (vision_oop)

Depuis **MSYS2 UCRT64** :
cmd.exe /c "set PATH=C:\msys64\ucrt64\bin;%PATH% && cd /d C:\Users\Lenovo\Desktop\Projet cpp\build && vision_oop.exe & pause"

**Attendu :**
- Fenêtre "Vision OOP" s'ouvre
- Flux vidéo de la webcam en temps réel
- Appuyez sur ESC pour quitter

## Dépannage

### DLL Qt manquantes (ex: Qt6Core.dll introuvable)
- Vérifier l'installation de mingw-w64-ucrt-x86_64-qt6-base
- S'assurer d'utiliser la commande cmd.exe /c "set PATH=... && ..."
- Vérifier que C:\msys64\ucrt64\bin est bien dans le PATH

### Webcam noire / pas d'image
1. **Fermer** toutes les applications utilisant la webcam :
   - Teams / Discord / Zoom
   - Chrome / Firefox
   - OBS / autres logiciels de capture
2. **Vérifier** les permissions caméra dans Windows Paramètres
3. **Tester** plusieurs index caméra (0, 1, 2) dans le code
4. Redémarrer l'ordinateur si nécessaire

### Erreurs de compilation
- S'assurer d'utiliser MSYS2 UCRT64 et non un autre terminal MSYS2
- Vérifier que toutes les dépendances sont installées
- Nettoyer le répertoire build et recompiler

## Structure du Projet

Projet cpp/
├── CMakeLists.txt          # Configuration CMake
├── src/                    # Code source principal
│   ├── main.cpp
│   ├── FrameSource.cpp
│   ├── FrameSource.h
│   ├── FilterPipeline.cpp
│   ├── FilterPipeline.h
│   ├── OutputSink.cpp
│   └── OutputSink.h
├── camera_test.cpp         # Programme de test webcam
├── README.md               # Ce fichier
└── build/                  # Généré par CMake (à ignorer)

## Licence

Ce projet est sous licence MIT. Voir le fichier LICENSE pour plus de détails.

## Contact

Pour toute question ou problème, ouvrez une issue sur le dépôt GitHub.