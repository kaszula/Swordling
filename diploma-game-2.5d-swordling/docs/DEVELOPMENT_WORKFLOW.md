 Development Workflow

## Cel dokumentu

Dokument opisuje standardowy sposób pracy zespołu nad projektem **DiplomaGame**.

Jego celem jest:

- utrzymanie porządku w repozytorium,
- ograniczenie konfliktów w plikach Unreal Engine,
- ujednolicenie sposobu realizacji zadań,
- określenie roli używanych narzędzi,
- zapewnienie powiązania zmian z zadaniami w Jira.

Szczegółowe komendy Git znajdują się w pliku [`GIT_WORKFLOW.md`](GIT_WORKFLOW.md).

---

## Narzędzia

### Jira

Jira służy do:

- tworzenia i porządkowania backlogu,
- planowania sprintów,
- przypisywania zadań,
- śledzenia postępu prac,
- uzgadniania, kto edytuje dany plik lub element gry.

### Unreal Engine 5.7.4

Unreal Engine służy do:

- tworzenia i edytowania map,
- pracy z Blueprintami,
- przygotowywania materiałów i animacji,
- importowania assetów,
- konfiguracji projektu,
- uruchamiania i testowania gry.

### Visual Studio 2022

Visual Studio służy do:

- programowania w C++,
- przeglądania zmian w repozytorium,
- tworzenia branchy,
- wykonywania operacji Commit, Pull i Push,
- rozwiązywania konfliktów w plikach tekstowych.

### GitHub

GitHub służy do:

- przechowywania repozytorium,
- tworzenia Pull Requestów,
- przeglądu zmian,
- scalania branchy do `main`,
- przechowywania historii projektu.

---

## Standardowy proces realizacji zadania

### 1. Wybór zadania w Jira

Każda większa zmiana powinna mieć osobne zadanie w Jira.

Przykład:

```text
DIP-23 Implement player movement
```

Przed rozpoczęciem pracy należy:

- przypisać zadanie do siebie,
- sprawdzić jego opis i kryteria akceptacji,
- upewnić się, że druga osoba nie edytuje tych samych plików binarnych.

### 2. Aktualizacja brancha `main`

Przed utworzeniem brancha zadania należy przejść na `main` i pobrać najnowsze zmiany.

Nie należy rozpoczynać pracy na nieaktualnej wersji projektu.

### 3. Utworzenie brancha zadania

Każde zadanie realizujemy na osobnym branchu.

Nazwa brancha powinna zawierać:

- typ zmiany,
- numer zadania Jira,
- krótki opis.

Przykłady:

```text
feature/DIP-23-player-movement
bugfix/DIP-31-camera-collision
assets/DIP-42-import-environment-assets
docs/DIP-18-update-readme
chore/DIP-27-project-cleanup
```

### 4. Wprowadzenie zmian

Zmiany mogą być wykonywane w:

- Unreal Engine,
- Visual Studio,
- lub innym narzędziu wspierającym pracę.

Zakres zmian powinien odpowiadać jednemu zadaniu Jira. Nie należy łączyć w jednym branchu kilku niezależnych tematów.

### 5. Zapisanie projektu Unreal Engine

Przed sprawdzeniem zmian i wykonaniem commita należy w Unreal Engine wybrać:

```text
File → Save All
```

Git śledzi wyłącznie zapisane pliki.

### 6. Sprawdzenie zmienionych plików

Przed commitem należy przejrzeć wszystkie zmiany w oknie **Git Changes** w Visual Studio albo za pomocą komend Git.

Należy upewnić się, że commit nie zawiera:

- przypadkowo zmodyfikowanych plików,
- plików wygenerowanych lokalnie,
- zmian niezwiązanych z zadaniem,
- danych tymczasowych lub konfiguracyjnych właściwych tylko dla jednego komputera.

Nie należy bez sprawdzenia używać funkcji **Commit All** ani komendy `git add .`.

### 7. Wykonanie commita

Opis commita powinien zawierać numer zadania Jira oraz krótki opis zmiany.

Przykłady:

```text
DIP-23 Add basic player movement
DIP-31 Fix camera collision
DIP-18 Update development documentation
```

Dopuszczalne jest wykonanie kilku logicznych commitów w ramach jednego brancha.

### 8. Wysłanie brancha do GitHuba

Po wykonaniu commita należy wysłać branch do zdalnego repozytorium.

Nie wykonujemy Push bezpośrednio na `main`.

### 9. Utworzenie Pull Requesta

Pull Request tworzymy z brancha zadania do `main`.

Tytuł Pull Requesta powinien zawierać numer zadania Jira.

Przykład:

```text
DIP-23 Add basic player movement
```

W opisie Pull Requesta warto podać:

- zakres wykonanych zmian,
- sposób przetestowania,
- znane ograniczenia,
- listę szczególnie ważnych plików binarnych.

### 10. Sprawdzenie i scalenie zmian

Przed scaleniem należy sprawdzić:

- listę zmienionych plików,
- zakres commitów,
- zgodność zmian z zadaniem Jira,
- brak konfliktów,
- działanie projektu po pobraniu brancha.

Przy większych zmianach zalecany jest przegląd drugiej osoby. Przy małych i bezpiecznych poprawkach autor może scalić Pull Request samodzielnie.

Po sprawdzeniu wykonujemy Merge do `main` i usuwamy zdalny branch.

### 11. Synchronizacja lokalnego repozytorium

Po scaleniu należy:

- przejść lokalnie na `main`,
- wykonać Pull,
- usunąć lokalny branch zadania,
- sprawdzić, czy projekt nadal się uruchamia.

### 12. Zamknięcie zadania w Jira

Po scaleniu i sprawdzeniu zmian zadanie można oznaczyć jako **Gotowe**.

---

## Pliki śledzone w projekcie Unreal Engine

Git śledzi zapisane pliki znajdujące się w repozytorium, między innymi:

```text
*.cpp
*.h
*.uasset
*.umap
Config/*.ini
```

Zmiany mogą obejmować między innymi:

- kod C++,
- Blueprinty,
- mapy,
- materiały,
- animacje,
- położenie obiektów,
- ustawienia projektu,
- konfigurację sterowania.

---

## Ustawienia projektu

Wspólne ustawienia projektu są najczęściej przechowywane w folderze:

```text
Config/
```

Przykładowe pliki:

```text
DefaultEngine.ini
DefaultGame.ini
DefaultInput.ini
```

Zmiany w tych plikach są częścią repozytorium i po wykonaniu Pull trafiają do pozostałych członków zespołu.

Przed commitem należy sprawdzić, czy zmiana ustawień była zamierzona.

---

## Foldery ignorowane przez Git

Do repozytorium nie dodajemy folderów generowanych lokalnie:

```text
Binaries/
DerivedDataCache/
Intermediate/
Saved/
.vs/
```

Pełna lista znajduje się w pliku `.gitignore`.

---

## Pliki binarne Unreal Engine

Pliki:

```text
*.uasset
*.umap
```

są plikami binarnymi. Git nie potrafi bezpiecznie scalać dwóch różnych zmian wykonanych w tym samym pliku.

Dlatego dwie osoby nie powinny jednocześnie edytować:

- tej samej mapy,
- tego samego Blueprintu,
- tego samego materiału,
- tej samej animacji,
- tego samego assetu.

Podział pracy należy ustalić wcześniej w Jira lub bezpośrednio między członkami zespołu.

W przypadku większych map warto dzielić zawartość na osobne poziomy, sublevele, komponenty lub assety, aby ograniczyć liczbę konfliktów.

---

## Zasady obowiązkowe

### Zawsze

- pracuj na osobnym branchu,
- aktualizuj `main` przed rozpoczęciem zadania,
- powiąż zmianę z zadaniem Jira,
- sprawdzaj zmienione pliki przed commitem,
- zapisuj projekt przez **Save All**,
- przesyłaj zmiany przez Pull Request,
- usuwaj niepotrzebne branche po scaleniu,
- uzgadniaj edycję plików binarnych.

### Nigdy

- nie pracuj bezpośrednio na `main`,
- nie wysyłaj przypadkowych plików,
- nie commituj folderów generowanych lokalnie,
- nie używaj bez sprawdzenia **Commit All** ani `git add .`,
- nie edytuj równocześnie z drugą osobą tych samych plików `.uasset` lub `.umap`,
- nie łącz kilku niezależnych zadań w jednym branchu.

---

## Skrócona lista kontrolna

1. Wybierz i przypisz zadanie w Jira.
2. Sprawdź, czy nikt nie edytuje tych samych assetów.
3. Przejdź na `main` i wykonaj Pull.
4. Utwórz branch zawierający numer zadania Jira.
5. Wprowadź zmiany.
6. W Unreal Engine wybierz **File → Save All**.
7. Przejrzyj zmienione pliki.
8. Dodaj do commita wyłącznie potrzebne pliki.
9. Wykonaj commit z numerem zadania Jira.
10. Wykonaj Push brancha.
11. Utwórz Pull Request do `main`.
12. Sprawdź i przetestuj zmiany.
13. Wykonaj Merge.
14. Usuń zdalny branch.
15. Pobierz aktualny `main`.
16. Usuń lokalny branch.
17. Oznacz zadanie w Jira jako **Gotowe**.
