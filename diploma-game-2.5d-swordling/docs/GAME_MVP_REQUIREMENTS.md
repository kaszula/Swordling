# Plan implementacji samodzielnego MVP 2.5D

**Wersja:** 3.0

**Data:** 30 lipca 2026
**Architektura:** Unreal Engine 5 -> SQLiteCore -> lokalny plik `DiplomaGame.db`

## 1. Cel dokumentu

Ten plik jest jedynym bieżącym źródłem prawdy o koncepcji gry, zakresie MVP i kolejności realizacji projektu. Łączy wcześniejszą specyfikację gry, skrócony zakres MVP oraz szczegółowy plan implementacji. Opisuje bezpieczne przekształcenie istniejącego prototypu w małe, kompletne MVP gry 2.5D z pomiarem czasu, zapisem prób z Unreal Engine do lokalnej relacyjnej bazy SQLite, rankingiem i podstawowymi statystykami.

Operacyjna lista sprintów i zadań znajduje się w `docs/MVP_BACKLOG.md`.

Najważniejsza zasada: projekt ma być samodzielną pracą właścicielki repozytorium. Nie wolno wykorzystywać żadnych dawnych nazw, postaci, fabuły, świata, map, stylistyki, modeli, opisów ani pomysłów pochodzących z wcześniejszej współpracy. Można zachować wyłącznie neutralne systemy techniczne wykonane przez właścicielkę, po wcześniejszym zweryfikowaniu ich w repozytorium.

## 2. Docelowe MVP

Gra ma być krótkim poziomem typu time trial w perspektywie 2.5D.

Pętla rozgrywki:

1. Gracz podaje pseudonim, który jest przechowywany lokalnie na czas bieżącej sesji.
2. Rozpoczyna próbę.
3. Przekroczenie strefy startowej uruchamia licznik czasu.
4. Gracz porusza się, skacze i rozpoczyna krótką obowiązkową walkę.
5. Pokonanie przeciwnika otwiera przejście do pierwszego checkpointu. Dalszych przeciwników można pokonać lub ominąć.
6. Gracz aktywuje pozostałe checkpointy i dociera do mety.
7. Próba kończy się statusem `COMPLETED` albo `FAILED`.
8. Unreal Engine zapisuje podsumowanie i zdarzenia do lokalnego pliku SQLite w jednej transakcji.
9. Ekran końcowy pokazuje czas, podstawowe statystyki, rekord osobisty i pozycję w rankingu.

Zakres minimalny:

- jeden poziom,
- jedna postać gracza,
- jeden typ przeciwnika,
- ruch w lewo i prawo,
- skok,
- jeden podstawowy atak,
- zdrowie i obrażenia,
- śmierć oraz restart,
- wykrywanie, pościg i atak przeciwnika,
- cooldown ataków,
- licznik czasu,
- start, checkpointy i meta,
- HUD zdrowia i czasu,
- ekran wyniku i ekran niepowodzenia,
- bezpośrednie, parametryzowane zapisy i odczyty z lokalnego SQLite,
- ranking oraz statystyki gracza.

Poza MVP:

- boss,
- więcej niż jeden poziom,
- kilka klas przeciwników,
- combo, ekwipunek, rozwój postaci i zapis stanu świata,
- dialogi, fabuła, cutscenki,
- własne modelowanie, rigowanie i animowanie,
- rozbudowane mechaniki platformowe,
- system pancerza, stamina, wall jump i dynamiczny skok,
- sieciowa gra wieloosobowa,
- ASP.NET Core, Web API i komunikacja HTTP,
- PostgreSQL, SQL Server LocalDB oraz zewnętrzny serwer bazy,
- uwierzytelnianie użytkowników oraz ochrona rankingu przed modyfikacją lokalnych danych,
- trwała kolejka niewysłanych wyników działająca po ponownym uruchomieniu gry.

Priorytety:

- `MUST` - pełna lokalna pętla gry, obowiązkowa walka, timer, checkpointy, zapis prób, rekord osobisty, pozycja i Top 10,
- `SHOULD` - ponowienie nieudanego zapisu w bieżącej sesji i podstawowe statystyki gracza,
- `COULD` - dodatkowi opcjonalni przeciwnicy, rozbudowane statystyki w UI i dodatkowe efekty wizualne.

Elementy `SHOULD` i `COULD` nie mogą opóźnić działającego builda spełniającego wszystkie wymagania `MUST`.

## 3. Zasady bezpieczeństwa repozytorium

1. Przed pierwszą zmianą utwórz osobną gałąź, np. `solo-mvp-rework`.
2. Przed usunięciem pliku sprawdź jego referencje i upewnij się, że nie jest potrzebny w finalnym buildzie.
3. Nie wykonuj masowego wyszukiwania i zamiany w plikach binarnych Unreal Engine.
4. Nigdy nie edytuj bezpośrednio plików `.uasset` i `.umap` jako danych binarnych.
5. Zmiany wymagające Unreal Editora należy opisać jako dokładną instrukcję wykonania w edytorze; plików binarnych nie należy modyfikować poza edytorem.
6. Każdą większą zmianę przygotuj jako mały, logiczny zakres. Commit wykonuj wyłącznie po wyraźnym poleceniu właścicielki.
7. Nie zmieniaj jednocześnie kodu mechaniki, integracji bazy i assetów.
8. Nie zmieniaj nazwy pliku `.uproject`, modułu C++ ani głównych klas bez wcześniejszego raportu skutków i wyraźnej zgody właścicielki.
9. Nie dodawaj zewnętrznych bibliotek, pluginów ani dużych paczek bez uzasadnienia.
10. Nie zapisuj finalnej bazy użytkownika w `Content/`. Zapisywalny plik bazy ma znajdować się w `Saved/Database/`.
11. Włączenie wbudowanych pluginów `SQLiteCore` i `SQLiteSupport` oraz zmiana `.uproject` wymagają osobnego etapu i wcześniejszego opisu skutków.

## 4. Weryfikacja zmian

Przed rozpoczęciem każdego etapu należy krótko opisać jego cel, wskazać zmieniane pliki i potencjalne ryzyka. Zmiany nazw projektu, konfiguracji, migracji danych lub plików Unreal Editor wymagają wcześniejszego potwierdzenia zakresu.

Po zakończeniu etapu należy podać listę zmienionych plików, opisać wykonane zmiany, zapisać wynik kompilacji lub testów, wymienić wymagane kroki w Unreal Editor oraz wskazać znane ograniczenia.

Funkcji nie należy uznawać za działającą bez kompilacji i sprawdzenia w zakresie możliwym bez uruchamiania Unreal Editora.
## 5. Etap 0 - rozpoznanie projektu

Etap został wykonany przed rozpoczęciem zmian funkcjonalnych. Sprawdzono wersję Unreal Engine, strukturę modułów, kod C++, Blueprinty, mapy, konfigurację wejścia, istniejące mechaniki, zależności oraz możliwość ponownego wykorzystania neutralnych systemów technicznych.

Najważniejsze ustalenia zostały uwzględnione bezpośrednio w dalszych etapach tego dokumentu. Nie jest wymagany osobny raport audytowy.

### Kryterium ukończenia

Stan początkowy projektu został rozpoznany, a dalsze zadania opierają się na potwierdzonych elementach repozytorium.

## 6. Etap 1 - neutralizacja projektu

Ten etap rozpoczął się po rozpoznaniu stanu projektu.

### Zadania automatyczne

1. Zastąp widoczne stare teksty w kodzie, konfiguracji i dokumentacji neutralnymi nazwami technicznymi.
2. Usuń nieaktualne komentarze i opisy odnoszące się do poprzedniej koncepcji.
3. Dodaj do dokumentacji końcowej tabelę materiałów zewnętrznych zawierającą:
   - nazwa assetu,
   - autor lub wydawca,
   - źródło,
   - typ licencji,
   - data pobrania,
   - sposób użycia w projekcie.
4. Dodaj `Database/README.md` opisujący:
   - lokalizację pliku `Saved/Database/DiplomaGame.db`,
   - kolejność uruchamiania skryptów SQL,
   - sposób odtworzenia pustej bazy,
   - sposób wykonania zapytań demonstracyjnych.

### Zadania ręczne w Unreal Editor

Instrukcja wykonania w Unreal Editor powinna obejmować:

- zmiany nazw widocznych assetów i folderów,
- użycia `Fix Up Redirectors in Folder`,
- sprawdzenia referencji po zmianie nazw,
- utworzenia nowej, pustej mapy integracyjnej,
- usunięcia z finalnej mapy elementów poprzedniej koncepcji.

### Kryterium ukończenia

Projekt nie pokazuje dawnych nazw ani opisów, a finalna koncepcja jest neutralna i niezależna.

## 7. Etap 2 - domknięcie rdzenia walki

Najpierw zachowaj placeholdery i nie importuj docelowych modeli.

### Zadania

1. Zweryfikuj przepływ zadawania obrażeń graczowi.
2. Dokończ śmierć gracza i bezpieczny restart poziomu.
3. Dodaj cooldown ataku gracza i przeciwnika.
4. Zablokuj wielokrotne naliczanie obrażeń z jednego ataku, jeżeli obecna implementacja na to pozwala.
5. Upewnij się, że martwa postać nie może się poruszać ani atakować.
6. Upewnij się, że martwy przeciwnik nie wykrywa i nie atakuje gracza.
7. Dodaj podstawowe logi diagnostyczne z osobną kategorią logowania.
8. Wykonaj test integracyjny:
   - gracz trafia przeciwnika,
   - przeciwnik traci zdrowie,
   - przeciwnik umiera,
   - przeciwnik trafia gracza,
   - gracz traci zdrowie,
   - gracz umiera,
   - poziom może zostać zrestartowany,
   - cooldown uniemożliwia spamowanie atakiem.

### Kryterium ukończenia

Pełna walka działa na placeholderach i nie zależy od docelowych modeli ani animacji.

## 8. Etap 3 - system próby, czasu i statystyk

Preferowana architektura po stronie Unreal Engine:

- `UGameRunSubsystem` jako `UGameInstanceSubsystem` zarządzający stanem próby,
- `ARunStartTrigger` uruchamiający próbę,
- `ARunCheckpointTrigger` rejestrujący checkpoint,
- `ARunFinishTrigger` kończący próbę,
- `FRunEvent` jako `USTRUCT`,
- `FRunSummary` jako `USTRUCT`.

Nazwy można dostosować do istniejącej konwencji projektu, ale muszą pozostać neutralne.

### Stan próby

Zdefiniuj enum, np.:

- `NotStarted`,
- `Running`,
- `Completed`,
- `Failed`.

`NotStarted` i `Running` są stanami lokalnymi. Do bazy trafiają wyłącznie zamknięte próby `Completed` i `Failed`. Rejestrowanie prób porzuconych przez zamknięcie gry pozostaje poza MVP.

### Dane przechowywane podczas próby

- `client_run_id` - lokalny UUID używany również do idempotentnego zapisu w bazie,
- pseudonim gracza przechowywany lokalnie do momentu zapisu,
- czas rozpoczęcia,
- czas zakończenia,
- czas trwania w milisekundach,
- liczba osiągniętych checkpointów,
- liczba pokonanych przeciwników,
- łączne otrzymane obrażenia,
- lista zdarzeń,
- status zapisu danych do bazy.

### Wymagania implementacyjne

1. Nie zwiększaj czasu przez sumowanie `DeltaSeconds` jako jedynego źródła prawdy. Zapisz moment startu i wyliczaj czas na podstawie zegara świata lub monotonicznego czasu silnika.
2. Czas zapisuj w bazie jako liczby całkowite w milisekundach.
3. Zabezpiecz start przed wielokrotnym uruchomieniem.
4. Każdy checkpoint może zostać zaliczony tylko raz na próbę.
5. Meta nie kończy próby, jeżeli wymagane checkpointy nie zostały zaliczone.
6. Śmierć gracza kończy próbę statusem `Failed`.
7. Restart po śmierci tworzy nową próbę, a nie nadpisuje poprzednią.
8. Zdarzenia zbieraj lokalnie i zapisuj zbiorczo przy zakończeniu próby.
9. Pierwszy checkpoint staje się dostępny dopiero po pokonaniu obowiązkowego przeciwnika.
10. Zapis pseudonimu, zakończonej próby, zdarzeń i rekordu osobistego wykonuj w jednej transakcji SQLite.

### Minimalne typy zdarzeń

- `RUN_STARTED`,
- `MANDATORY_ENEMY_DEFEATED`,
- `CHECKPOINT_REACHED`,
- `ENEMY_DEFEATED`,
- `PLAYER_DAMAGED`,
- `PLAYER_DIED`,
- `RUN_COMPLETED`.

### Kryterium ukończenia

Próba może zostać rozpoczęta, mierzona, zakończona sukcesem lub porażką, a pełne podsumowanie jest dostępne lokalnie bez bazy danych.

## 9. Etap 4 - HUD i ekrany

### Minimalne widgety

- HUD z paskiem zdrowia, czasem i liczbą checkpointów,
- ekran startowy z pseudonimem,
- ekran ukończenia próby,
- ekran śmierci,
- ekran rankingu,
- prosty komunikat o błędzie otwarcia lub zapisu lokalnej bazy.

### Zasady

1. Logika gry nie może znajdować się wyłącznie w Blueprintach widgetów.
2. Widgety odczytują dane z komponentów, subsystemów lub kontrolera.
3. Brak możliwości otwarcia lub zapisania pliku SQLite nie może uniemożliwiać przejścia poziomu.
4. Jeżeli zapis się nie uda, ekran wyniku pokazuje status `wynik niezapisany` i umożliwia ponowną próbę zapisu.
5. Nie importuj gotowego systemu HUD, jeżeli prosty interfejs można wykonać w UMG.
6. W grze obowiązkowe są jedynie rekord osobisty, pozycja gracza i Top 10. Rozbudowane analizy mogą być prezentowane przez skrypty SQL.

### Kryterium ukończenia

Całą próbę można przejść od ekranu startowego do ekranu wyniku bez korzystania z logów deweloperskich.

## 10. Etap 5 - baza SQLite

Utwórz katalog `Database/` i przechowuj w nim wersjonowane skrypty SQL.

Preferowana kolejność plików:

- `001_create_tables.sql`,
- `002_create_constraints_indexes.sql`,
- `003_create_triggers.sql`,
- `004_create_views.sql`,
- `005_seed_data.sql`,
- `006_demo_queries.sql`.

SQLite nie obsługuje osobnych schematów ani procedur składowanych. Atomowa operacja kończąca próbę jest wykonywana przez C++ jako zestaw parametryzowanych poleceń w jednej transakcji.

### Inicjalizacja

1. Plik bazy użytkownika znajduje się w `Saved/Database/DiplomaGame.db`.
2. Katalog `Saved/Database/` jest tworzony automatycznie, jeżeli nie istnieje.
3. Po każdym otwarciu połączenia wykonaj `PRAGMA foreign_keys = ON`.
4. Wersję schematu przechowuj przez `PRAGMA user_version`.
5. Nową bazę utwórz przez wykonanie wersjonowanych skryptów w kolejności.
6. Skrypty wymagane w buildzie muszą zostać jawnie dołączone do pakowania jako pliki Non-UFS albo zastąpione kontrolowanym mechanizmem migracji w C++. Nie zakładaj, że katalog `Database/` zostanie spakowany automatycznie.

### Tabele

#### `players`

- `player_id` - `INTEGER PRIMARY KEY`,
- `nickname` - po przycięciu od 3 do 20 znaków, niepusty,
- `created_at` - czas UTC jako `INTEGER` w milisekundach Unix,
- `last_played_at` - czas UTC jako `INTEGER` w milisekundach Unix.

#### `levels`

- `level_id` - `INTEGER PRIMARY KEY`,
- `level_code`,
- `level_name`,
- `level_version`,
- `par_time_ms`,
- `is_active` - `INTEGER` z wartością `0` albo `1`.

#### `runs`

- `run_id` - `INTEGER PRIMARY KEY`,
- `client_run_id` - unikalny `TEXT` z UUID nadanym przez grę, zabezpieczający ponowienie zapisu,
- `player_id` - klucz obcy,
- `level_id` - klucz obcy,
- `started_at` - czas UTC jako `INTEGER` w milisekundach Unix,
- `finished_at` - czas UTC jako `INTEGER` w milisekundach Unix,
- `duration_ms` - nieujemny `INTEGER`,
- `status` - `TEXT` ograniczony do `COMPLETED` i `FAILED`,
- `enemies_defeated`,
- `damage_taken`,
- `checkpoints_reached`,
- `created_at`.

#### `run_events`

- `event_id` - `INTEGER PRIMARY KEY`,
- `run_id` - klucz obcy z usuwaniem zależnym,
- `event_type`,
- `event_time_ms` - nieujemny `INTEGER` liczony od rozpoczęcia próby,
- `event_value` - opcjonalny `INTEGER`,
- `sequence_number`,
- `created_at`.

#### `personal_bests`

- `player_id`,
- `level_id`,
- `best_run_id`,
- `best_time_ms`,
- `updated_at`,
- złożony klucz główny `(player_id, level_id)`.

### Wymagane elementy SQL

1. Klucze główne i obce oraz aktywne `PRAGMA foreign_keys = ON`.
2. Ograniczenia `CHECK` dla statusów, wartości nieujemnych i spójności dat.
3. Unikalność pseudonimu bez rozróżniania wielkich i małych liter, np. przez unikalny indeks na `LOWER(nickname)`.
4. Unikalność pary `(level_code, level_version)`; sam `level_code` może występować w wielu wersjach.
5. Indeksy pod ranking oraz historię prób gracza.
6. Transakcja kończąca próbę wykonywana przez warstwę C++.
7. `UPSERT` rekordu osobistego.
8. Trigger walidujący lub uzupełniający dane.
9. Widok rankingu z funkcją okna `DENSE_RANK()`.
10. Widok statystyk gracza z `COUNT`, `MIN`, `AVG`, `SUM`, `CASE` i `GROUP BY`.
11. Co najmniej jedno zapytanie z CTE.
12. Co najmniej jedno zapytanie wykorzystujące funkcję okna inną niż ranking, np. różnicę względem poprzedniej próby przy użyciu `LAG()`.
13. Skrypt demonstracyjny pokazujący działanie transakcji, widoków i walidacji.
14. Ranking obejmujący wyłącznie próby `COMPLETED`, najlepszy wynik każdego gracza oraz konkretną parę `level_code` i `level_version`.
15. Powtórny zapis tego samego `client_run_id` nie może utworzyć drugiej próby i powinien zwrócić dane wcześniej zapisanej próby.

### Transakcja kończąca próbę

Warstwa C++ przyjmuje pseudonim, kompletne podsumowanie zakończonej próby oraz listę zdarzeń. Następnie:

1. wykonuje `BEGIN IMMEDIATE`,
2. sprawdza `client_run_id`,
3. dla powtórzonego `client_run_id` odczytuje wcześniej zapisaną próbę, kończy transakcję bez zmian i zwraca poprzedni wynik,
4. normalizuje pseudonim i wykonuje `UPSERT` gracza,
5. tworzy zamkniętą próbę `COMPLETED` albo `FAILED`,
6. zapisuje statystyki końcowe i uporządkowane zdarzenia,
7. aktualizuje `last_played_at`,
8. wykonuje `UPSERT` rekordu osobistego wyłącznie dla `COMPLETED`,
9. wykonuje `COMMIT`,
10. przy dowolnym błędzie wykonuje `ROLLBACK`,
11. zwraca identyfikator próby, informację o rekordzie osobistym oraz status zapisu.

### Kryterium ukończenia

Baza może zostać utworzona od zera przez uruchomienie skryptów w kolejności, a `006_demo_queries.sql` prezentuje wymagane bardziej złożone operacje.

## 11. Etap 6 - integracja SQLite w Unreal Engine

Projekt nie zawiera ASP.NET Core, Web API ani komunikacji HTTP. Gra korzysta z lokalnego pliku SQLite przez wbudowane moduły Unreal Engine.

### Moduły i pluginy

Przed zmianą `.uproject` i `Build.cs` przedstaw plan oraz poproś o akceptację. Docelowo:

- włącz wbudowane pluginy `SQLiteCore` i `SQLiteSupport`,
- dodaj wymagane moduły do `DiplomaGame.Build.cs`,
- nie dodawaj zewnętrznej biblioteki, pluginu ani serwera bazy.

### Preferowana architektura

- `UGameDatabaseSubsystem` jako `UGameInstanceSubsystem`,
- jedna klasa odpowiedzialna za otwarcie pliku bazy i wykonywanie zapytań,
- osobne struktury C++ dla podsumowania próby, zdarzenia, wpisu rankingu i statystyk,
- ścieżka bazy budowana przez `FPaths::ProjectSavedDir()` i kończąca się `Database/DiplomaGame.db`,
- dedykowana kategoria logowania bez wypisywania pełnych zapytań z danymi użytkownika,
- delegaty lub callbacki informujące UI o sukcesie i błędzie,
- jedna serializowana kolejka operacji obsługiwana przez dedykowany wątek roboczy,
- jedno połączenie nie może być używane równocześnie przez kilka zadań,
- kontrolowane ponowne otwarcie pliku po błędzie,
- bezpieczne zatrzymanie kolejki i zamknięcie połączenia przy wyłączaniu gry,
- zapytania SQL znajdujące się wyłącznie w warstwie bazy, a nie w klasach rozgrywki.

### Minimalne operacje

- utworzenie lub otwarcie pliku `DiplomaGame.db`,
- odczyt i aktualizacja `PRAGMA user_version`,
- wykonanie `PRAGMA foreign_keys = ON`,
- atomowe utworzenie albo pobranie gracza oraz zapis zakończonej próby i jej zdarzeń,
- pobranie rekordu osobistego, pozycji gracza i Top 10,
- pobranie podstawowych statystyk gracza.

### Bezpieczeństwo i ograniczenia

1. Wszystkie wartości pochodzące z gry przekazuj jako parametry; nie sklejaj zapytań z pseudonimem ani innym tekstem.
2. Nie zapisuj pliku bazy w `Content/`, ponieważ finalny build może nie mieć prawa zapisu w tym miejscu.
3. Błąd otwarcia, migracji lub zapisu pliku nie może powodować crasha ani blokować ukończenia poziomu.
4. Nie przechowuj obiektów połączenia w Blueprintach.
5. Nie wykonuj zapytań synchronicznie na głównym wątku, ponieważ mogą zatrzymać obraz.
6. Jest to architektura przeznaczona do lokalnej prezentacji projektu. Uwierzytelnianie, synchronizacja między komputerami i ochrona przed ręczną edycją pliku pozostają poza MVP.

### Kryterium ukończenia

Gra potrafi utworzyć bazę w `Saved/Database/`, wykonać migracje, obsłużyć błąd pliku bez crasha i pobrać wynik prostego zapytania.

## 12. Etap 7 - zapis prób i ranking w grze

### Przepływ

1. Przed próbą gra zapisuje pseudonim wyłącznie w stanie bieżącej sesji. Brak możliwości otwarcia pliku bazy nie blokuje rozpoczęcia gry.
2. Przekroczenie strefy startowej tworzy lokalny `client_run_id` UUID i uruchamia timer.
3. Zdarzenia są zbierane w pamięci bez wykonywania zapytania przy każdym zdarzeniu.
4. Po śmierci albo ukończeniu powstaje niezmienne podsumowanie próby.
5. Subsystem bazy zapisuje pseudonim, podsumowanie i zdarzenia przez jedną transakcję SQLite. Transakcja tworzy albo pobiera gracza i zapisuje całą próbę.
6. Po udanym zapisie pobierane są rekord osobisty, pozycja i Top 10 dla aktualnej wersji poziomu.
7. Przy błędzie podsumowanie pozostaje w pamięci do ponowienia w bieżącej sesji.

### Reguły danych

- pseudonim jest przycinany, musi mieć od 3 do 20 znaków i może zawierać polskie znaki,
- unikalność pseudonimu nie rozróżnia wielkich i małych liter,
- do rankingu trafiają wyłącznie próby `COMPLETED`,
- ranking porównuje wyniki tej samej wartości `level_code` i `level_version`,
- dla każdego gracza liczy się jego najlepszy czas,
- remisy otrzymują tę samą pozycję przez `DENSE_RANK()`,
- próby `FAILED` pozostają w historii i statystykach, ale nie trafiają do rankingu,
- ponowienie zapisu z tym samym `client_run_id` nie tworzy duplikatu i zwraca wynik pierwszego zapisu,
- trwałe przechowywanie kolejki po zamknięciu gry pozostaje poza MVP.

### Kryterium ukończenia

Ukończenie lub porażka tworzy rzeczywisty rekord w lokalnej bazie, a ekran wyniku może pokazać status zapisu, rekord osobisty, pozycję i Top 10.

## 13. Etap 8 - assety i finalna mapa

Przed użyciem assetu należy potwierdzić jego legalne źródło i licencję.

### Zasady wyboru

- jedna spójna stylistycznie paczka środowiska,
- jedna humanoidalna lub robotyczna postać gracza,
- jeden humanoidalny lub robotyczny przeciwnik,
- najlepiej wspólny albo zgodny szkielet,
- minimalny zestaw animacji: Idle, Run, Jump/Fall, Attack, Hit, Death,
- brak własnego modelowania i rigowania,
- unikanie paczek zawierających rozbudowaną logikę gameplayową.

### Zadania integracyjne

1. Przygotuj checklistę importu i integracji dla Unreal Editora.
2. Wskaż, które klasy i Blueprinty powinny otrzymać nowe meshe i Animation Blueprinty.
3. Przygotuj neutralne nazwy folderów, np. `Content/Characters/Player`, `Content/Characters/Enemy`, `Content/Environment`, `Content/UI`.
4. Nie kopiuj kodu z paczki assetów do głównej logiki gry bez przeglądu.
5. Uzupełnij informacje o źródłach i licencjach na podstawie danych dostarczonych przez właścicielkę.

### Układ finalnego poziomu

- krótka bezpieczna strefa startowa,
- strefa uruchamiająca timer,
- zamknięta, krótka sekcja walki z jednym przeciwnikiem,
- przejście otwierane po pokonaniu tego przeciwnika i checkpoint 1 za przejściem,
- prosta przeszkoda wymagająca skoku,
- dwa kolejne checkpointy,
- opcjonalne instancje tego samego przeciwnika rozmieszczone tak, aby nie atakowały wszystkie naraz,
- meta,
- brak ruchomych platform i skomplikowanej nawigacji AI.

### Kryterium ukończenia

Poziom da się ukończyć od startu do mety, a docelowe assety nie psują kolizji, walki ani kamery.

## 14. Etap 9 - testy, build i dokumentacja

### Testy obowiązkowe

- start nowej próby,
- próba ponownego uruchomienia timera,
- próba wejścia do checkpointu 1 przed pokonaniem obowiązkowego przeciwnika,
- otwarcie przejścia dokładnie jeden raz po pokonaniu obowiązkowego przeciwnika,
- checkpoint aktywowany drugi raz,
- dotarcie do mety bez checkpointów,
- ukończenie prawidłowej próby,
- śmierć gracza,
- restart i utworzenie nowej próby,
- brak możliwości utworzenia lub otwarcia pliku SQLite,
- utworzenie nowej bazy w `Saved/Database/` przy pierwszym uruchomieniu,
- ponowne uruchomienie bez powtórnego wykonywania zastosowanych migracji,
- aktywne klucze obce po `PRAGMA foreign_keys = ON`,
- ponowny zapis po chwilowym błędzie,
- pseudonim zawierający apostrof i polskie znaki,
- pseudonimy różniące się wyłącznie wielkością liter,
- pobicie rekordu osobistego,
- gorszy wynik bez nadpisania rekordu,
- dwukrotny zapis tego samego `client_run_id`,
- ranking przy remisach,
- ranking oddzielający dwie wersje tego samego `level_code`,
- obecność modułów `SQLiteCore` i `SQLiteSupport` w buildzie Windows,
- poprawne dołączenie skryptów migracyjnych wymaganych w buildzie,
- build Windows uruchomiony poza edytorem.

### Dokumentacja końcowa

- `README.md` - opis całego projektu i uruchomienia,
- `docs/GAME_MVP_REQUIREMENTS.md` - koncepcja gry, zakres MVP i plan realizacji,
- `docs/ARCHITECTURE.md`,
- `docs/ERD.md` lub obraz ERD,
- sekcja źródeł i licencji wykorzystanych materiałów,
- `Database/006_demo_queries.sql`,
- `Database/README.md` z opisem pliku bazy, migracji i odtworzenia schematu,
- krótki opis testów i znanych ograniczeń.

### Kryterium ukończenia projektu

1. Gra uruchamia się jako build Windows.
2. Gracz może rozpocząć i ukończyć poziom 2.5D.
3. Działa ruch, skok, walka, zdrowie, śmierć i AI.
4. Pokonanie obowiązkowego przeciwnika otwiera przejście do checkpointu 1 dokładnie jeden raz.
5. Timer, checkpointy i meta działają poprawnie.
6. Próby sukcesu i porażki zapisują się z gry do `Saved/Database/DiplomaGame.db`.
7. Ponowienie zapisu z tym samym `client_run_id` nie tworzy duplikatu.
8. Ranking i podstawowe statystyki można odczytać w grze.
9. SQL prezentuje relacje, ograniczenia, transakcję, trigger, UPSERT, widoki, agregacje, CTE i funkcje okna.
10. Projekt nie zawiera dawnych nazw ani pomysłów z wcześniejszej współpracy.
11. Użyte assety mają udokumentowane źródła i licencje, a projekt korzysta wyłącznie z wbudowanych modułów SQLite Unreal Engine.
12. Repozytorium zawiera instrukcję uruchomienia gry i lokalnej bazy.

## 15. Zalecana kolejność commitów

Przykładowo:

1. `chore: remove obsolete textual references`
2. `fix: complete player damage and death flow`
3. `feat: add attack cooldowns`
4. `feat: add run state and timer subsystem`
5. `feat: add checkpoints and finish trigger`
6. `feat: add HUD and result screens`
7. `db: add relational schema and constraints`
8. `db: add run transaction and leaderboard views`
9. `feat(db): enable Unreal SQLite modules`
10. `feat(ue): add game database subsystem`
11. `feat: save completed runs directly to database`
12. `feat: add leaderboard and player statistics UI`
13. `docs: add asset licenses and setup guide`
14. `fix: stabilize packaged build`

## 16. Jak korzystać z planu

Przed rozpoczęciem kolejnego zadania należy sprawdzić aktualny stan projektu i wybrać pierwszy niezakończony etap. Każdy etap powinien być realizowany oraz weryfikowany osobno.
