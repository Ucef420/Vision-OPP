VISION OOP (C++ / OpenCV)
GUIDE COMPLET — WINDOWS + MSYS2 UCRT64
===================================

Ce projet est un mini-framework orienté objet pour la vision par ordinateur en temps réel.

ARCHITECTURE
------------
FrameSource -> Pipeline (Filters) -> OutputSink

Le dépôt contient aussi un exécutable "camera_test" permettant de vérifier que
la webcam fonctionne correctement avant de lancer le programme principal.


0) PREREQUIS
------------

- Windows 10 ou Windows 11
- Une webcam (intégrée ou USB)
- MSYS2 installé
- Terminal utilisé : MSYS2 UCRT64

ATTENTION :
Ne pas utiliser WSL pour la webcam.
WSL ne donne pas toujours accès à la caméra.
Pour ce projet, on travaille en Windows natif.


1) INSTALLER MSYS2
------------------

1. Télécharger MSYS2 depuis le site officiel
2. Installer MSYS2 (exemple : C:\msys64)
3. Ouvrir "MSYS2 UCRT64" depuis le menu Démarrer

Le terminal doit afficher quelque chose comme :
UCRT64 /c/...


2) INSTALLER LES PACKAGES NECESSAIRES
------------------------------------

Dans MSYS2 UCRT64, exécuter :

pacman -Syu

Si MSYS2 demande de fermer le terminal, le fermer puis rouvrir MSYS2 UCRT64,
ensuite exécuter :

pacman -S --needed ^
  mingw-w64-ucrt-x86_64-toolchain ^
  mingw-w64-ucrt-x86_64-cmake ^
  mingw-w64-ucrt-x86_64-opencv ^
  mingw-w64-ucrt-x86_64-qt6-base

Pourquoi Qt6 ?
OpenCV (module HighGUI) utilise Qt pour afficher les fenêtres (imshow).
Sans Qt6, le programme compile mais échoue à l’exécution avec des erreurs du type :
- Qt6Core.dll introuvable
- Qt6Gui.dll introuvable


3) RECUPERER LE PROJET
---------------------

OPTION A — Cloner depuis GitHub

Dans MSYS2 UCRT64 :

cd /c/Users/Lenovo/Desktop
git clone https://github.com/<TON_USERNAME>/<TON_REPO>.git
cd <TON_REPO>

OPTION B — Projet déjà présent sur le PC

cd "C:/Users/Lenovo/Desktop/Projet cpp"

Vérifier que les fichiers suivants existent :
- CMakeLists.txt
- src/
- camera_test.cpp

Commande :
ls


4) COMPILER LE PROJET (BUILD)
----------------------------

Depuis la racine du projet (là où se trouve CMakeLists.txt) :

rm -rf build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build

Résultat attendu :
- Built target vision_oop
- Built target camera_test


5) EXECUTION — POINT IMPORTANT
------------------------------

NE PAS lancer directement :
- ./vision_oop.exe
- ./camera_test.exe

depuis MSYS2.

Pourquoi ?
Windows doit trouver les DLL OpenCV et Qt6 qui se trouvent dans :
C:\msys64\ucrt64\bin

Il faut donc lancer les exécutables via CMD en ajoutant ce dossier au PATH.


6) TESTER LA WEBCAM (camera_test)
--------------------------------
A FAIRE EN PREMIER

Depuis MSYS2 UCRT64 :

cmd.exe /c "set PATH=C:\msys64\ucrt64\bin;%PATH% && cd /d C:\Users\Lenovo\Desktop\Projet cpp\build && camera_test.exe & pause"

Résultat attendu :
- Une fenêtre cam_test s’ouvre
- Vidéo affichée en temps réel
- Appuyer sur ESC pour quitter
- La console reste ouverte (pause)

Si la commande ne fonctionne pas depuis MSYS2,
exécuter exactement la même commande depuis CMD ou PowerShell.


7) LANCER LE PROGRAMME PRINCIPAL (vision_oop)
---------------------------------------------

Depuis MSYS2 UCRT64 :

cmd.exe /c "set PATH=C:\msys64\ucrt64\bin;%PATH% && cd /d C:\Users\Lenovo\Desktop\Projet cpp\build && vision_oop.exe & pause"

Résultat attendu :
- Une fenêtre Vision OOP s’ouvre
- Webcam affichée en temps réel
- Appuyer sur ESC pour quitter


8) DEPANNAGE
------------

A) DLL Qt manquantes (Qt6Core.dll introuvable)
- Vérifier que mingw-w64-ucrt-x86_64-qt6-base est installé
- Toujours lancer avec :
  set PATH=C:\msys64\ucrt64\bin;%PATH%

B) Webcam noire ou aucune image
- Fermer Teams, Discord, Zoom, Chrome, OBS
- Essayer plusieurs index caméra (0, 1, 2)
- Vérifier permissions Windows :
  Settings -> Privacy & security -> Camera
  Camera access = ON
  Let desktop apps access your camera = ON

C) Le programme se ferme immédiatement
- Tester d’abord camera_test
- Si camera_test fonctionne, le problème vient du programme principal


9) WORKFLOW RECOMMANDE (TRAVAIL A DEUX)
--------------------------------------

1. Ouvrir MSYS2 UCRT64
2. Compiler :
   cmake -S . -B build -G "MinGW Makefiles"
   cmake --build build
3. Tester camera_test
4. Lancer vision_oop
5. Modifier le code -> rebuild -> retester


10) GIT — COMMIT ET PUSH
-----------------------

git status
git add .
git commit -m "Update README + build and run instructions"
git push

Si un rebase est en cours :
- Résoudre le conflit
- Puis exécuter :

git add README.md
git rebase --continue
git push origin main
