# Git Workflow

Dokument zawiera skróconą instrukcję pracy z Git w projekcie **DiplomaGame**.

Ogólne zasady realizacji zadań znajdują się w pliku [`DEVELOPMENT_WORKFLOW.md`](DEVELOPMENT_WORKFLOW.md).

Komendy można wykonywać w PowerShellu, terminalu programu Visual Studio lub innym kliencie Git. Te same operacje są również dostępne w interfejsie **Git Changes** w Visual Studio 2022.

---

## 1. Rozpoczęcie pracy

Przejdź na `main` i pobierz najnowsze zmiany:

```powershell
git switch main
git pull
```

Sprawdź, czy katalog roboczy jest czysty:

```powershell
git status
```

Nie rozpoczynaj nowego zadania, jeżeli w repozytorium znajdują się niezapisane lub niezatwierdzone zmiany z poprzedniej pracy.

---

## 2. Utworzenie brancha

Każde zadanie realizuj na osobnym branchu.

Format nazwy:

```text
typ/NumerJira-krotki-opis
```

Stosowane prefiksy:

- `feature/` – nowa funkcja,
- `bugfix/` – poprawka błędu,
- `assets/` – import lub modyfikacja assetów,
- `docs/` – dokumentacja,
- `chore/` – prace techniczne i porządkowe.

Przykład:

```powershell
git switch -c feature/DIP-23-player-movement
```

---

## 3. Sprawdzenie zmian

Po zakończeniu pracy zapisz projekt w Unreal Engine przez:

```text
File → Save All
```

Następnie sprawdź zmodyfikowane pliki:

```powershell
git status
git diff
```

`git diff` pokazuje głównie zmiany w plikach tekstowych. Dla plików `.uasset` i `.umap` Git pokaże, że plik został zmieniony, ale nie wyświetli czytelnej różnicy jego zawartości.

---

## 4. Dodanie plików do commita

Dodawaj świadomie tylko pliki należące do danego zadania.

Pojedynczy plik:

```powershell
git add ścieżka/do/pliku
```

Kilka wskazanych plików:

```powershell
git add ścieżka/do/pliku1 ścieżka/do/pliku2
```

Po dodaniu plików ponownie sprawdź status:

```powershell
git status
```

Nie używaj bez sprawdzenia:

```powershell
git add .
```

---

## 5. Wykonanie commita

Opis commita powinien zawierać numer zadania Jira.

```powershell
git commit -m "DIP-23 Add basic player movement"
```

Dobry commit:

- dotyczy jednego logicznego zakresu,
- nie zawiera przypadkowych plików,
- ma krótki i jednoznaczny opis.

---

## 6. Wysłanie brancha

Przy pierwszym Push nowego brancha:

```powershell
git push -u origin feature/DIP-23-player-movement
```

Przy kolejnych Pushach tego samego brancha wystarczy:

```powershell
git push
```

Nie wykonuj Push bezpośrednio na `main`.

---

## 7. Pull Request

Na GitHubie utwórz Pull Request:

```text
branch zadania → main
```

Tytuł powinien zawierać numer zadania Jira, na przykład:

```text
DIP-23 Add basic player movement
```

Przed scaleniem sprawdź:

- zmienione pliki,
- commity,
- konflikty,
- zgodność zakresu z zadaniem,
- działanie projektu.

Zmiany do `main` trafiają wyłącznie przez Pull Request.

---

## 8. Aktualizacja brancha przed scaleniem

Jeżeli `main` zmienił się podczas pracy, zaktualizuj swój branch:

```powershell
git switch main
git pull
git switch feature/DIP-23-player-movement
git merge main
```

Po rozwiązaniu ewentualnych konfliktów:

```powershell
git add ścieżka/do/rozwiązanego/pliku
git commit
git push
```

Nie próbuj automatycznie scalać konfliktujących wersji tego samego pliku `.uasset` lub `.umap`. W takim przypadku należy ustalić, która wersja pliku ma zostać zachowana, albo odtworzyć jedną ze zmian ręcznie w Unreal Engine.

---

## 9. Po scaleniu Pull Requesta

Pobierz aktualny `main`:

```powershell
git switch main
git pull
```

Usuń lokalny branch:

```powershell
git branch -d feature/DIP-23-player-movement
```

Jeżeli Git odmawia usunięcia brancha mimo jego prawidłowego scalenia, najpierw sprawdź historię i stan repozytorium. Opcji `-D` używaj wyłącznie świadomie.

Usuń informacje o nieistniejących już zdalnych branchach:

```powershell
git fetch --prune
```

---

## 10. Przydatne komendy

Sprawdzenie aktualnego brancha i zmian:

```powershell
git status
```

Lista lokalnych branchy:

```powershell
git branch
```

Lista lokalnych i zdalnych branchy:

```powershell
git branch -a
```

Ostatnie commity:

```powershell
git log --oneline --graph --decorate -10
```

Wycofanie pliku z obszaru staged bez usuwania zmian:

```powershell
git restore --staged ścieżka/do/pliku
```

Przywrócenie pliku do ostatniej zatwierdzonej wersji:

```powershell
git restore ścieżka/do/pliku
```

Uwaga: ostatnia komenda usuwa lokalne, niezatwierdzone zmiany w podanym pliku.

---

## Najważniejsze zasady

- Każde zadanie ma osobny branch.
- Nazwa brancha i commit zawierają numer zadania Jira.
- Przed rozpoczęciem pracy aktualizujemy `main`.
- Nie wykonujemy Push bezpośrednio na `main`.
- Nie używamy bez sprawdzenia `git add .` ani **Commit All**.
- Nie edytujemy jednocześnie tych samych plików `.uasset` i `.umap`.
- Zmiany trafiają do `main` przez Pull Request.
- Po scaleniu usuwamy niepotrzebne branche.

---

## Współpraca przy plikach Unreal Engine

Pliki `.uasset` i `.umap` są plikami binarnymi. Git LFS umożliwia ich przechowywanie w repozytorium, ale Git nie potrafi automatycznie połączyć dwóch wersji tego samego pliku.

Pliki `.uasset` obejmują między innymi:

- Blueprinty,
- Input Actions i Input Mapping Contexts,
- materiały,
- modele i animacje,
- Widget Blueprinty,
- Behavior Trees i Blackboards,
- tekstury i dźwięki.

Pliki `.umap` zawierają mapy i levele, w tym rozmieszczenie aktorów, platform, świateł, kamer, punktów startowych i pozostałych elementów sceny.

### Zasady pracy z plikami binarnymi

Dwie osoby nie powinny równocześnie edytować tego samego konkretnego pliku `.uasset` lub `.umap` na osobnych branchach.

Można pracować równocześnie nad różnymi plikami. Przykładowo jedna osoba może edytować mapę `L_Test.umap`, a druga Blueprint postaci `BP_Player.uasset` lub pliki kodu C++.

Przed rozpoczęciem edycji współdzielonego pliku należy dodać komentarz do odpowiedniego zadania w Jirze:

```text
Aktualnie edytuję: Content/Maps/L_Test.umap
```

Druga osoba nie powinna edytować wskazanego pliku do czasu zakończenia pracy i zmergowania zmian do `main`.

Po zakończeniu pracy należy dodać komentarz:

```text
Zmiany zostały zmergowane do main. Plik Content/Maps/L_Test.umap można ponownie edytować.
```

Jeżeli druga osoba chce później edytować ten sam plik, musi rozpocząć pracę od aktualnego `main`.

Przed pobieraniem zmian i przełączaniem brancha należy zapisać pracę oraz zamknąć Unreal Editor.

```powershell
git switch main
git pull origin main
git switch -c feature/DIP-XX-short-name
```

Dopiero po pobraniu najnowszych zmian można ponownie otworzyć projekt i rozpocząć edycję pliku.

### Konflikty plików binarnych

Jeżeli wystąpi konflikt w pliku `.uasset` lub `.umap`, nie należy przypadkowo wybierać wersji `ours` albo `theirs`.

Należy:

1. ustalić, która kompletna wersja pliku zostanie zachowana,
2. zachować wersję jednej osoby,
3. otworzyć projekt w Unreal Editorze,
4. ręcznie odtworzyć brakujące zmiany z drugiej wersji,
5. ponownie uruchomić i przetestować projekt.

### Nowe assety

Nowe assety można tworzyć równolegle, jeżeli mają różne nazwy i ścieżki.

Przed utworzeniem ważnego lub współdzielonego assetu należy wpisać w zadaniu w Jirze jego planowaną nazwę i ścieżkę:

```text
Planowany asset: Content/Input/IA_Move.uasset
```

### Pliki tekstowe

Pliki tekstowe, takie jak `.cpp`, `.h`, `.ini`, `.md`, `.json`, `.uproject` i `.uplugin`, mogą być edytowane równolegle.

Git zwykle potrafi automatycznie połączyć zmiany wykonane w różnych częściach pliku. Jeżeli obie osoby zmienią te same linie, konflikt należy rozwiązać ręcznie i ponownie przetestować projekt.

## Standard nazewnictwa i języka

W projekcie obowiązują następujące zasady:

- zadania, opisy i komentarze w Jirze są prowadzone po polsku,
- nazwy branchy są zapisywane po angielsku,
- komunikaty commitów i tytuły Pull Requestów są zapisywane po angielsku,
- kod, klasy, funkcje, zmienne i komentarze techniczne są zapisywane po angielsku,
- nazwy folderów, plików, map, Blueprintów i pozostałych assetów są zapisywane po angielsku.

Schemat nazwy brancha:

```text
feature/DIP-XX-short-name
```

Przykłady:

```text
feature/DIP-19-test-map
feature/DIP-20-player-character
feature/DIP-23-enhanced-input
```

Schemat komunikatu commita:

```text
DIP-XX: Short description
```

Przykład:

```text
DIP-20: Add base player character
```

Istniejących branchy nie trzeba zmieniać. Nowy standard obowiązuje dla kolejnych zadań.
