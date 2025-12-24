
# Vision OOP (C++ / OpenCV) — Guide complet (Windows + MSYS2 UCRT64)

Ce projet est un mini-framework orienté objet pour la vision par ordinateur en temps réel.

Architecture générale :

**FrameSource → Pipeline(Filters) → OutputSink**

Le dépôt contient aussi un exécutable **`camera_test`** permettant de vérifier que la webcam fonctionne correctement avant de lancer le programme principal.

---

## 0) Prérequis

- Windows 10 ou Windows 11
- Une webcam (intégrée ou USB)
- MSYS2 installé
- Terminal utilisé : **MSYS2 UCRT64** (important)

> ⚠️ Ne pas utiliser WSL pour la webcam.
> WSL ne donne pas toujours accès à la caméra (`/dev/video0`).
> Pour ce projet, on travaille **en Windows natif**.

---

## 1) Installer MSYS2

1. Télécharger MSYS2 depuis le site officiel
2. Installer MSYS2 (exemple : `C:\msys64`)
3. Ouvrir **“MSYS2 UCRT64”** depuis le menu Démarrer

Le terminal doit afficher quelque chose comme :

UCRT64 /c/...


---

## 2) Installer les packages nécessaires

Dans **MSYS2 UCRT64**, exécuter :

```bash
pacman -Syu

Si MSYS2 demande de fermer le terminal, fais‑le puis rouvre MSYS2 UCRT64, ensuite :

pacman -S --needed \
  mingw-w64-ucrt-x86_64-toolchain \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-opencv \
  mingw-w64-ucrt-x86_64-qt6-base

Pourquoi Qt6 ?

OpenCV (module HighGUI) utilise Qt pour afficher les fenêtres (cv::imshow).

Sans Qt6, le programme compile mais échoue à l’exécution avec des erreurs comme :

    Qt6Core.dll introuvable

    Qt6Gui.dll introuvable

3) Récupérer le projet
Option A — Cloner depuis GitHub

Dans MSYS2 UCRT64 :

cd /c/Users/Lenovo/Desktop
git clone https://github.com/<TON_USERNAME>/<TON_REPO>.git
cd "<TON_REPO>"

Option B — Projet déjà présent sur le PC

cd "/c/Users/Lenovo/Desktop/Projet cpp"

Vérifier que les fichiers suivants existent :

    CMakeLists.txt

    src/

    camera_test.cpp

ls

4) Compiler le projet (Build)

Depuis la racine du projet (là où se trouve CMakeLists.txt) :

rm -rf build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build

Si tout se passe bien, tu dois voir :

Built target vision_oop
Built target camera_test

5) Exécution — point important

Il ne faut PAS lancer directement ./vision_oop.exe ou ./camera_test.exe depuis MSYS2.

Pourquoi ?
Parce que Windows doit trouver les DLL OpenCV et Qt6, qui sont situées dans :

C:\msys64\ucrt64\bin

La solution la plus simple consiste à lancer les exécutables via CMD en ajoutant ce dossier au PATH.
6) Tester la webcam (camera_test) — À FAIRE EN PREMIER

Depuis MSYS2 UCRT64, exécuter :

cmd.exe /c "set PATH=C:\msys64\ucrt64\bin;%PATH% && cd /d \"C:\Users\Lenovo\Desktop\Projet cpp\build\" && camera_test.exe & pause"

Résultat attendu :

    Une fenêtre cam_test s’ouvre

    La vidéo est affichée en temps réel

    Appuyer sur ESC pour quitter

    La console reste ouverte (pause)

Si la commande ne fonctionne pas depuis MSYS2

Exécute exactement la même commande directement depuis CMD ou PowerShell.
7) Lancer le programme principal (vision_oop)

Depuis MSYS2 UCRT64 :

cmd.exe /c "set PATH=C:\msys64\ucrt64\bin;%PATH% && cd /d \"C:\Users\Lenovo\Desktop\Projet cpp\build\" && vision_oop.exe & pause"

Résultat attendu :

    Une fenêtre Vision OOP s’ouvre

    La webcam est affichée en temps réel

    Appuyer sur ESC pour quitter

8) Problèmes fréquents et solutions
A) DLL Qt manquantes

Erreur du type :

Qt6Core.dll introuvable

Solution :

    Vérifier que mingw-w64-ucrt-x86_64-qt6-base est installé

    Toujours lancer les exécutables avec :

    set PATH=C:\msys64\ucrt64\bin;%PATH%

B) Webcam noire ou aucune image

    Fermer les applications utilisant la caméra :
    Teams, Discord, Zoom, Chrome, OBS

    Essayer différents index caméra (0, 1, 2)

    Vérifier les permissions Windows :

        Settings → Privacy & security → Camera

        Camera access = ON

        Let desktop apps access your camera = ON

C) Le programme se ferme immédiatement

    Tester d’abord camera_test

    Si camera_test fonctionne, le problème vient du code du pipeline principal
