# Developer Setup – DiplomaGame

Instrukcja przygotowania środowiska, sklonowania repozytorium oraz uruchomienia projektu pracy dyplomowej 2.5D w Unreal Engine.

## 1. Wymagane konta

Przed rozpoczęciem pracy należy posiadać:

- konto GitHub,
- konto Epic Games,
- opcjonalnie konto Microsoft do Visual Studio.

Repozytorium projektu jest prywatne. Dostęp jest możliwy po zaakceptowaniu zaproszenia na GitHubie.

Repozytorium:
https://github.com/kaszula/diploma-game-2.5d

## 2. Wymagane oprogramowanie

### Git for Windows

Pobieranie:
https://git-scm.com/download/win

Po instalacji otwórz PowerShell i sprawdź:

git --version

### Git LFS

Pobieranie:
https://git-lfs.com/

Po instalacji wykonaj:

git lfs version
git lfs install

Polecenie git lfs install należy wykonać jednorazowo dla danego konta użytkownika na komputerze.

Git LFS jest używany do przechowywania dużych plików Unreal Engine, między innymi:

- *.uasset
- *.umap

### Epic Games Launcher i Unreal Engine

Pobieranie Epic Games Launcher:
https://www.unrealengine.com/download

Po instalacji:

1. Zaloguj się na konto Epic Games.
2. Otwórz zakładkę Unreal Engine.
3. Przejdź do Library.
4. Zainstaluj dokładnie:

Unreal Engine 5.7.4

Oboje członkowie zespołu muszą korzystać z tej samej wersji Unreal Engine.

### Visual Studio Community 2022

Pobieranie:
https://aka.ms/vs/17/release/vs_community.exe

W instalatorze zaznacz workload:

Game development with C++

W szczegółach instalacji powinny być zaznaczone co najmniej:

- MSVC v143 – VS 2022 C++ build tools,
- Windows 10 SDK lub Windows 11 SDK,
- Visual Studio Tools for Unreal Engine,
- Visual Studio debugger tools for Unreal Engine Blueprints,
- Unreal Engine Test Adapter.

Dodatkowo można zainstalować:

- C++ profiling tools,
- C++ AddressSanitizer,
- HLSL Tools.

Nie należy instalować Unreal Engine z poziomu Visual Studio Installer. Silnik instalujemy przez Epic Games Launcher.

## 3. Klonowanie repozytorium

Najpierw zaakceptuj zaproszenie do prywatnego repozytorium na GitHubie.

Utwórz folder na projekty, na przykład:

D:\Projects

Otwórz PowerShell i wykonaj:

cd D:\Projects
git clone https://github.com/kaszula/diploma-game-2.5d.git
cd diploma-game-2.5d
git lfs pull
git status

Jeżeli GitHub poprosi o uwierzytelnienie, zaloguj się na konto, które otrzymało dostęp do repozytorium.

Prawidłowy status powinien zawierać:

On branch main
Your branch is up to date with 'origin/main'.

nothing to commit, working tree clean

## 4. Uruchomienie projektu

Projekt znajduje się w pliku:

D:\Projects\diploma-game-2.5d\DiplomaGame.uproject

Uruchom go dwukrotnym kliknięciem.

Jeżeli Unreal Engine wyświetli komunikat:

Missing DiplomaGame Modules
Would you like to rebuild them now?

wybierz:

Yes

Foldery takie jak Binaries, Intermediate, Saved i DerivedDataCache nie są przechowywane w repozytorium. Unreal Engine wygeneruje je lokalnie.

Po otwarciu projektu:

1. Poczekaj na zakończenie kompilacji i przetwarzania shaderów.
2. Kliknij Play.
3. Sprawdź, czy postać się porusza.
4. Nie zapisuj przypadkowych zmian w mapach ani assetach.

## 5. Visual Studio

W Unreal Engine sprawdź:

Edit → Editor Preferences → Source Code

Jako Source Code Editor powinno być wybrane:

Visual Studio 2022

Visual Studio nie musi uruchamiać się automatycznie razem z Unreal Engine.

Można je otworzyć z poziomu Unreal Engine:

Tools → Open Visual Studio

## 6. Zasady pracy z Git

Nie pracujemy bezpośrednio na gałęzi main.

Przed rozpoczęciem zadania należy utworzyć osobną gałąź, na przykład:

git switch main
git pull
git switch -c feature/nazwa-zadania

Przed rozpoczęciem pracy zawsze wykonujemy:

git pull

Po zakończeniu zadania:

git status
git add .
git commit -m "Krótki opis wykonanej zmiany"
git push -u origin feature/nazwa-zadania

Następnie na GitHubie tworzymy Pull Request.

Nie edytujemy równocześnie tego samego pliku .uasset lub .umap, ponieważ są to pliki binarne, których Git nie potrafi standardowo połączyć.

## 7. Pliki, których nie dodajemy ręcznie

Nie dodajemy do repozytorium folderów:

- .vs
- Binaries
- DerivedDataCache
- Intermediate
- Saved

Są one generowane lokalnie i ignorowane przez .gitignore.

## 8. Rozwiązywanie podstawowych problemów

### Brak assetów po sklonowaniu

Wykonaj:

git lfs install
git lfs pull

### Projekt nie kompiluje się

1. Zamknij Unreal Engine.
2. Kliknij prawym przyciskiem DiplomaGame.uproject.
3. Wybierz Generate Visual Studio project files.
4. Otwórz projekt ponownie.
5. Przekaż komunikat błędu drugiemu członkowi zespołu.

### Git pokazuje nieoczekiwane zmiany

Nie wykonuj od razu:

git add .

Najpierw sprawdź:

git status

i przejrzyj listę zmienionych plików przed wykonaniem commita.
