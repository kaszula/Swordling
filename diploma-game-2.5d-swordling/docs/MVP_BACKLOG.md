# Backlog MVP - DiplomaGame

**Źródło wymagań:** `docs/GAME_MVP_REQUIREMENTS.md`

**Rola dokumentu:** lekki odpowiednik Jiry przechowujący kolejność sprintów, zadania, zależności, kryteria akceptacji i sposób weryfikacji.

**Aktualizacja:** po zakończeniu każdego zadania lub zmianie zakresu MVP.

## 1. Zasady używania backlogu

### Statusy

- `TODO` - zadanie gotowe do rozpoczęcia.
- `IN PROGRESS` - trwa implementacja; jednocześnie powinno być aktywne tylko jedno główne zadanie.
- `BLOCKED` - zadanie nie może być kontynuowane bez decyzji, danych lub ukończenia zależności.
- `DONE` - wszystkie kryteria akceptacji zostały spełnione i zweryfikowane.

### Priorytety

- `MUST` - wymagane do ukończenia MVP.
- `SHOULD` - realizowane po wszystkich zadaniach `MUST`, jeżeli nie zagrażają terminowi.
- `COULD` - dodatek możliwy wyłącznie po uzyskaniu stabilnego buildu MVP.

### Typy zadań

- `FEATURE` - nowa funkcja gry.
- `DB` - baza danych albo zapytania SQL.
- `UI` - interfejs użytkownika.
- `ASSET` - mapa, Blueprint lub asset Unreal Engine.
- `TEST` - weryfikacja, regresja albo build.
- `DOCS` - dokumentacja.
- `CHORE` - porządkowanie projektu bez nowej funkcji.

### Definition of Ready

Zadanie może przejść do `IN PROGRESS`, gdy:

- jego zależności mają status `DONE`,
- zakres i kryteria akceptacji są zrozumiałe,
- wiadomo, które pliki lub assety mogą się zmienić,
- dla zmian `.uasset` i `.umap` wiadomo, kto pracuje nad danym assetem,
- decyzje wymagające zgody właścicielki zostały zaakceptowane.

### Definition of Done

Zadanie może przejść do `DONE`, gdy:

- wszystkie kryteria akceptacji są spełnione,
- wykonano testy wymienione w zadaniu,
- kod kompiluje się, jeżeli zadanie zmienia C++,
- zmienione Blueprinty kompilują się bez błędów,
- Map Check nie zgłasza nowych błędów, jeżeli zadanie zmienia mapę,
- wypisano zmienione pliki, wyniki testów, ograniczenia i ryzyka,
- commit wykonano tylko wtedy, gdy właścicielka wydała osobne polecenie.

## 2. Mapa sprintów

| Sprint | Cel | Najważniejszy rezultat | Status |
|---|---|---|---|
| Sprint 0 | Stabilna baza projektu | Walka, mapa M01 i uporządkowane repozytorium | `DONE` |
| Sprint 1 | Postacie i stan pojedynczej próby | Podstawowe modele, animacje oraz lokalny system próby | `TODO` |
| Sprint 2 | Pełny przepływ poziomu | Start, obowiązkowa walka, brama, checkpointy i meta | `TODO` |
| Sprint 3 | Gra bez używania logów | Pseudonim, HUD, śmierć i lokalny ekran wyniku | `TODO` |
| Sprint 4 | Relacyjna baza SQLite | Schemat, migracje, widoki i zapytania demonstracyjne | `TODO` |
| Sprint 5 | SQLite działające w Unreal Engine | Subsystem bazy, migracje i bezpieczne operacje asynchroniczne | `TODO` |
| Sprint 6 | Zapis i ranking end-to-end | Zapis prób, rekord osobisty, pozycja i Top 10 | `TODO` |
| Sprint 7 | Finalna mapa i prezentacja gry | Środowisko, materiały oraz ukończalny poziom | `TODO` |
| Sprint 8 | Wydanie MVP | Testy regresji, build Windows i dokumentacja końcowa | `TODO` |

---

## Sprint 0 - stabilna baza projektu

**Cel sprintu:** zachować wyłącznie neutralny, własny rdzeń gry i przygotować bezpieczną bazę do dalszej implementacji.

**Kryterium zakończenia sprintu:** projekt posiada mapę startową M01, działający rdzeń walki oraz uporządkowaną dokumentację.

### MVP-001 - Uporządkowanie zakresu i dokumentacji

- **Status:** `DONE`
- **Typ:** `DOCS`
- **Priorytet:** `MUST`
- **Zależności:** brak

**Opis:** Ustalenie jednego źródła prawdy o grze i ograniczenie dokumentacji do plików potrzebnych podczas dalszej pracy.

**Kryteria akceptacji:**

- [x] `GAME_MVP_REQUIREMENTS.md` zawiera koncepcję, zakres i plan implementacji.
- [x] Duplikujące dokumenty specyfikacji i zakresu zostały usunięte.
- [x] Dzienniki sprintów pozostały niezmienionym zapisem historycznym.
- [x] Pozostałe dokumenty nie prowadzą do usuniętych plików.

**Weryfikacja:** kontrola odwołań w dokumentacji i `git diff --check`.

### MVP-002 - Domknięcie podstawowej walki

- **Status:** `DONE`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** brak

**Opis:** Przygotowanie kompletnej walki na placeholderach, niezależnej od finalnych modeli i animacji.

**Kryteria akceptacji:**

- [x] Gracz porusza się, skacze i wykonuje podstawowy atak.
- [x] Gracz oraz przeciwnik posiadają zdrowie i otrzymują obrażenia.
- [x] Ataki gracza i przeciwnika mają cooldown.
- [x] Przeciwnik wykrywa gracza, ściga go i atakuje w zasięgu.
- [x] Martwy gracz nie porusza się ani nie atakuje.
- [x] Śmierć gracza powoduje bezpieczny restart poziomu.
- [x] Martwy przeciwnik nie wykrywa i nie atakuje gracza.

**Weryfikacja:** kompilacja `DiplomaGameEditor`, kompilacja Blueprintów i test walki w Unreal Editorze.

### MVP-003 - Utworzenie neutralnej mapy integracyjnej

- **Status:** `DONE`
- **Typ:** `ASSET`
- **Priorytet:** `MUST`
- **Zależności:** `MVP-002`

**Opis:** Utworzenie własnej mapy startowej służącej do integracji mechanik MVP.

**Kryteria akceptacji:**

- [x] `M01_IntegrationMap` jest mapą startową gry i edytora.
- [x] Mapa korzysta z `BP_DiplomaGameMode` i `BP_PlayerCharacter`.
- [x] Na mapie znajduje się przeciwnik i prosta geometria testowa.
- [x] Mapa ładuje się bez błędów Map Check.

**Weryfikacja:** odczyt konfiguracji projektu, załadowanie mapy i Map Check.

### MVP-004 - Usunięcie starej mapy developerskiej

- **Status:** `DONE`
- **Typ:** `CHORE`
- **Priorytet:** `MUST`
- **Zależności:** `MVP-003`

**Opis:** Usunięcie `M00_DevelopmentMap` oraz należących do niej External Actors i External Objects.

**Kryteria akceptacji:**

- [x] M00 nie istnieje w Asset Registry ani na dysku.
- [x] Nie pozostały External Actors ani External Objects mapy M00.
- [x] M01 nadal jest mapą startową i przechodzi Map Check.
- [x] Meshe z folderu `Development/Meshes` nie zostały usunięte.

**Weryfikacja:** Asset Registry, kontrola systemu plików i Map Check M01.

---

## Sprint 1 - podstawowe modele, animacje i system próby

**Cel sprintu:** wcześnie ustalić podstawową warstwę wizualną postaci oraz utworzyć lokalny, niezależny od bazy danych model pojedynczej próby.

**Kryterium zakończenia sprintu:** gracz i przeciwnik korzystają z wybranych modeli oraz podstawowych animacji, a próba może rozpocząć się, mierzyć czas, zbierać zdarzenia i zakończyć się lokalnym podsumowaniem `COMPLETED` albo `FAILED`.

### ART-101 - Wybór darmowych modeli i animacji postaci

- **Status:** `TODO`
- **Typ:** `ASSET`
- **Priorytet:** `MUST`
- **Zależności:** brak

**Opis:** Znalezienie małego, spójnego zestawu darmowych zasobów dla gracza i jednego typu przeciwnika, bez wybierania na tym etapie assetów finalnej mapy.

**Kryteria akceptacji:**

- [ ] Wybrano model gracza i jeden podstawowy model przeciwnika.
- [ ] Wybrane modele mają szkielety odpowiednie do animowania lub zgodne z planowanym sposobem retargetowania.
- [ ] Dostępne są animacje co najmniej dla: Idle, Run, Jump/Fall, Attack, Hit i Death albo wskazano bezpieczny sposób uzupełnienia braków.
- [ ] Dla każdego zasobu zapisano autora, bezpośrednie źródło, nazwę licencji i dozwolony sposób użycia w projekcie dyplomowym.
- [ ] Licencja pozwala na użycie, modyfikację i umieszczenie zasobu w buildzie gry.
- [ ] Zasoby nie wymagają importowania obcej logiki ruchu, walki ani dodatkowych pluginów.
- [ ] Przed pobraniem lub importem lista wybranych zasobów została zaakceptowana przez właścicielkę projektu.

**Weryfikacja:** przegląd stron źródłowych, warunków licencji, formatów plików, szkieletów i kompletności animacji.

### ART-102 - Import podstawowych modeli postaci

- **Status:** `TODO`
- **Typ:** `ASSET`
- **Priorytet:** `MUST`
- **Zależności:** `ART-101`

**Opis:** Import zaakceptowanych modeli gracza i przeciwnika oraz przygotowanie ich do podłączenia bez zmiany istniejącej logiki rozgrywki.

**Kryteria akceptacji:**

- [ ] Zaimportowano wyłącznie zaakceptowane modele, szkielety, materiały i tekstury potrzebne postaciom.
- [ ] Assety znajdują się w uporządkowanych folderach projektu i mają czytelne nazwy.
- [ ] Skala, orientacja oraz położenie modeli są poprawne względem kapsuł kolizji.
- [ ] Materiały i tekstury wyświetlają się poprawnie bez brakujących zależności.
- [ ] Nie zaimportowano przykładowych map, kontrolerów, GameMode'ów ani obcej logiki gameplayowej.
- [ ] Istniejące Blueprinty gracza i przeciwnika kompilują się bez błędów po przypisaniu modeli.
- [ ] Podstawowy ruch, kolizje i zadawanie obrażeń nadal działają.

**Weryfikacja:** przegląd zaimportowanych zależności, kompilacja Blueprintów oraz test ruchu, kolizji i walki na `M01_IntegrationMap`.

**Sugerowany commit:** `feat(art): import player and enemy models`

### ART-103 - Podłączenie podstawowych animacji

- **Status:** `TODO`
- **Typ:** `ASSET`
- **Priorytet:** `MUST`
- **Zależności:** `ART-102`

**Opis:** Podłączenie animacji postaci do istniejących mechanik gracza i przeciwnika z użyciem Animation Blueprintów i, jeśli będzie potrzebny, retargetowania.

**Kryteria akceptacji:**

- [ ] Gracz i przeciwnik przechodzą poprawnie między Idle i Run.
- [ ] Gracz posiada czytelne animacje Jump oraz Fall.
- [ ] Atak, otrzymanie trafienia i śmierć mają przypisane właściwe animacje.
- [ ] Kierunek modelu odpowiada kierunkowi ruchu i ataku w układzie 2.5D.
- [ ] Animacja ataku nie zmienia istniejących zasad obrażeń ani cooldownu.
- [ ] Kolizje i hitboxy pozostają niezależne od przypadkowej wielkości mesha.
- [ ] Animation Blueprinty i Blueprinty postaci kompilują się bez błędów.
- [ ] Dotychczasowe testy ruchu, walki, śmierci i restartu nadal przechodzą.

**Weryfikacja:** test wszystkich stanów animacji obu postaci oraz regresja ruchu, kolizji, walki, śmierci i restartu na mapie testowej.

**Sugerowany commit:** `feat(art): integrate character animations`

### RUN-101 - Struktury danych i status próby

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** `MVP-002`

**Opis:** Utworzenie neutralnych typów C++ reprezentujących stan, zdarzenie i podsumowanie próby.

**Zakres:**

- enum statusu: `NotStarted`, `Running`, `Completed`, `Failed`,
- `FRunEvent`,
- `FRunSummary`,
- identyfikator `client_run_id`,
- czasy i liczniki wymagane przez specyfikację.

**Kryteria akceptacji:**

- [ ] Typy są dostępne w C++ i tam, gdzie potrzebne, również w Blueprintach.
- [ ] Czas jest przechowywany jako całkowita liczba milisekund.
- [ ] `FRunSummary` zawiera pseudonim, status, czas, checkpointy, pokonanych przeciwników, obrażenia i zdarzenia.
- [ ] `client_run_id` jest unikalnym UUID tworzonym dla każdej nowej próby.
- [ ] Zamknięte podsumowanie nie jest później modyfikowane przez aktywną rozgrywkę.

**Weryfikacja:** kompilacja oraz test utworzenia struktur i UUID.

**Sugerowany commit:** `feat: add run state data structures`

### RUN-102 - GameRunSubsystem i monotoniczny timer

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-101`

**Opis:** Utworzenie `UGameRunSubsystem` jako centralnego właściciela stanu próby.

**Kryteria akceptacji:**

- [ ] Subsystem rozpoczyna próbę tylko ze stanu `NotStarted`.
- [ ] Moment startu jest zapisywany raz.
- [ ] Aktualny czas wynika z zegara świata lub monotonicznego czasu silnika, a nie wyłącznie z sumowania `DeltaSeconds`.
- [ ] Ponowne wywołanie startu nie resetuje timera.
- [ ] Subsystem kończy próbę dokładnie raz jako `Completed` albo `Failed`.
- [ ] Restart poziomu pozwala utworzyć nową próbę z nowym UUID.
- [ ] Subsystem udostępnia dane potrzebne później przez HUD i bazę.

**Weryfikacja:** test startu, podwójnego startu, zakończenia i utworzenia kolejnej próby.

**Sugerowany commit:** `feat: add run subsystem and timer`

### RUN-103 - Rejestrowanie zdarzeń i statystyk

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-102`

**Opis:** Zbieranie w pamięci chronologicznej listy zdarzeń oraz liczników podczas aktywnej próby.

**Kryteria akceptacji:**

- [ ] Obsługiwane są co najmniej: `RUN_STARTED`, `MANDATORY_ENEMY_DEFEATED`, `CHECKPOINT_REACHED`, `ENEMY_DEFEATED`, `PLAYER_DAMAGED`, `PLAYER_DIED`, `RUN_COMPLETED`.
- [ ] Każde zdarzenie ma czas w milisekundach od początku próby.
- [ ] Zdarzenia mają rosnący `sequence_number`.
- [ ] Obrażenia zwiększają łączny licznik `damage_taken`.
- [ ] Śmierć przeciwnika zwiększa `enemies_defeated`.
- [ ] Zdarzenia poza aktywną próbą nie zanieczyszczają podsumowania.
- [ ] Na tym etapie żadne zdarzenie nie wykonuje zapytania SQL.

**Weryfikacja:** test kolejności, czasów zdarzeń i liczników na jednej pełnej próbie.

**Sugerowany commit:** `feat: collect run events and statistics`

### RUN-104 - Integracja śmierci gracza z próbą

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-102`, `RUN-103`

**Opis:** Połączenie istniejącej mechaniki śmierci i restartu z cyklem życia próby.

**Kryteria akceptacji:**

- [ ] Śmierć podczas aktywnej próby ustawia status `Failed`.
- [ ] Rejestrowane są `PLAYER_DIED` oraz kompletne podsumowanie porażki.
- [ ] Podsumowanie powstaje przed restartem mapy.
- [ ] Restart nie nadpisuje poprzedniego podsumowania.
- [ ] Nowa próba po restarcie otrzymuje nowy `client_run_id`.

**Weryfikacja:** śmierć, opóźnienie restartu, ponowne rozpoczęcie i porównanie UUID.

**Sugerowany commit:** `feat: finish failed runs on player death`

---

## Sprint 2 - przepływ poziomu

**Cel sprintu:** połączyć istniejącą walkę z pełną trasą od startu do mety.

**Kryterium zakończenia sprintu:** gracz może przejść poprawną próbę, a system odrzuca nieprawidłową kolejność bramy, checkpointów i mety.

### LVL-201 - Strefa startowa próby

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-102`

**Opis:** Utworzenie aktora `ARunStartTrigger` rozpoczynającego próbę po wejściu gracza.

**Kryteria akceptacji:**

- [ ] Trigger reaguje wyłącznie na gracza.
- [ ] Pierwsze poprawne wejście uruchamia próbę i timer.
- [ ] Kolejne wejścia nie resetują timera ani UUID.
- [ ] Trigger rejestruje `RUN_STARTED`.
- [ ] Trigger nie zawiera logiki bazy danych.

**Weryfikacja:** pierwsze wejście, ponowne wejście i wejście innym aktorem.

**Sugerowany commit:** `feat: add run start trigger`

### LVL-202 - Obowiązkowa walka i brama

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-103`, `LVL-201`

**Opis:** Utworzenie krótkiego obowiązkowego starcia, którego ukończenie otwiera dalszą drogę.

**Kryteria akceptacji:**

- [ ] Brama jest zamknięta przed pokonaniem wskazanego przeciwnika.
- [ ] Tylko śmierć obowiązkowego przeciwnika otwiera bramę.
- [ ] Śmierć opcjonalnego przeciwnika nie otwiera bramy.
- [ ] Brama otwiera się dokładnie raz.
- [ ] Rejestrowane jest `MANDATORY_ENEMY_DEFEATED`.
- [ ] Martwy przeciwnik nie generuje ponownie zdarzenia otwarcia.
- [ ] Rozwiązanie działa na placeholderach bez finalnych animacji.

**Weryfikacja:** próba przejścia przed walką, śmierć opcjonalnego wroga i wielokrotna obsługa zdarzenia śmierci.

**Sugerowany commit:** `feat: add mandatory encounter gate`

### LVL-203 - Checkpointy zaliczane jeden raz

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-103`, `LVL-202`

**Opis:** Utworzenie `ARunCheckpointTrigger` z jednoznacznym identyfikatorem i kolejnością checkpointów.

**Kryteria akceptacji:**

- [ ] Każdy checkpoint ma stabilny identyfikator.
- [ ] Ten sam checkpoint może zostać zaliczony tylko raz w jednej próbie.
- [ ] Pierwszy checkpoint jest niedostępny przed obowiązkową walką.
- [ ] Zaliczony checkpoint zwiększa licznik i rejestruje `CHECKPOINT_REACHED`.
- [ ] Powtórne wejście nie zmienia licznika ani listy zdarzeń.
- [ ] Restart nowej próby czyści lokalny stan zaliczonych checkpointów.

**Weryfikacja:** wejście przed walką, poprawna aktywacja, podwójna aktywacja i nowa próba.

**Sugerowany commit:** `feat: add idempotent run checkpoints`

### LVL-204 - Meta i ukończenie próby

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** `LVL-203`

**Opis:** Utworzenie `ARunFinishTrigger` kończącego wyłącznie prawidłową próbę.

**Kryteria akceptacji:**

- [ ] Meta reaguje wyłącznie podczas statusu `Running`.
- [ ] Brak wymaganych checkpointów nie kończy próby.
- [ ] Komplet checkpointów kończy próbę jako `Completed`.
- [ ] Rejestrowane jest `RUN_COMPLETED`.
- [ ] Czas końcowy zostaje zamrożony i nie rośnie po ukończeniu.
- [ ] Kolejne wejście do mety nie tworzy drugiego podsumowania.

**Weryfikacja:** wejście bez checkpointów, poprawna trasa i ponowne wejście po ukończeniu.

**Sugerowany commit:** `feat: add validated finish trigger`

### LVL-205 - Integracja pełnej pętli na M01

- **Status:** `TODO`
- **Typ:** `ASSET`
- **Priorytet:** `MUST`
- **Zależności:** `LVL-201`, `LVL-202`, `LVL-203`, `LVL-204`

**Opis:** Rozmieszczenie na `M01_IntegrationMap` kompletnej trasy testowej.

**Kryteria akceptacji:**

- [ ] Kolejność poziomu to: start, obowiązkowa walka, checkpoint 1, przeszkoda, checkpoint 2, checkpoint 3, meta.
- [ ] Gracz nie może ominąć obowiązkowej bramy.
- [ ] Trasa jest możliwa do ukończenia przy istniejącym ruchu i skoku.
- [ ] Opcjonalni przeciwnicy nie atakują wszyscy jednocześnie.
- [ ] Kamera nie pokazuje pustych lub nieprzeznaczonych obszarów mapy.
- [ ] Map Check ma zero błędów.

**Weryfikacja:** pełne przejście sukcesu, porażka i próby złamania kolejności.

**Sugerowany commit:** `feat: integrate complete run flow on M01`

---

## Sprint 3 - pseudonim, HUD i ekrany

**Cel sprintu:** umożliwić przejście pełnej lokalnej próby bez korzystania z Output Log.

**Kryterium zakończenia sprintu:** gracz podaje pseudonim, widzi stan próby i otrzymuje czytelny ekran sukcesu albo porażki.

### UI-301 - Ekran pseudonimu i stan sesji

- **Status:** `TODO`
- **Typ:** `UI`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-102`

**Opis:** Utworzenie prostego ekranu startowego przechowującego pseudonim w bieżącej sesji.

**Kryteria akceptacji:**

- [ ] Pseudonim jest przycinany z białych znaków.
- [ ] Akceptowane są wartości od 3 do 20 znaków.
- [ ] Obsługiwane są polskie znaki oraz apostrof.
- [ ] Niepoprawny pseudonim pokazuje czytelny komunikat.
- [ ] Poprawny pseudonim trafia do stanu sesji, a nie bezpośrednio do bazy.
- [ ] Brak bazy danych nie blokuje rozpoczęcia gry.

**Weryfikacja:** wartości puste, za krótkie, za długie, polskie znaki i apostrof.

**Sugerowany commit:** `feat(ui): add nickname entry screen`

### UI-302 - HUD zdrowia, czasu i checkpointów

- **Status:** `TODO`
- **Typ:** `UI`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-102`, `LVL-203`

**Opis:** Utworzenie HUD-u odczytującego dane z komponentu zdrowia i subsystemu próby.

**Kryteria akceptacji:**

- [ ] HUD pokazuje aktualne i maksymalne zdrowie.
- [ ] Czas jest aktualizowany podczas próby i zatrzymuje się po jej zakończeniu.
- [ ] HUD pokazuje liczbę zaliczonych oraz wymaganych checkpointów.
- [ ] Aktywacja checkpointu daje krótką informację zwrotną.
- [ ] Widget nie jest właścicielem logiki próby.
- [ ] HUD nie generuje błędów po śmierci, restarcie ani zmianie mapy.

**Weryfikacja:** obrażenia, upływ czasu, checkpoint, śmierć i restart.

**Sugerowany commit:** `feat(ui): add run HUD`

### UI-303 - Lokalne ekrany sukcesu i porażki

- **Status:** `TODO`
- **Typ:** `UI`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-104`, `LVL-204`

**Opis:** Wyświetlanie lokalnego podsumowania próby przed podłączeniem bazy danych.

**Kryteria akceptacji:**

- [ ] Sukces pokazuje czas, checkpointy, pokonanych przeciwników i obrażenia.
- [ ] Porażka pokazuje status `Failed` oraz dostępne statystyki.
- [ ] Po zakończeniu próby sterowanie rozgrywką jest zablokowane zgodnie z przyjętym przepływem.
- [ ] Gracz może rozpocząć nową próbę.
- [ ] UI działa bez SQLite.
- [ ] Dane na ekranie odpowiadają niezmiennemu `FRunSummary`.

**Weryfikacja:** pełna próba sukcesu, śmierć i ponowienie.

**Sugerowany commit:** `feat(ui): add local run result screens`

### UI-304 - Stan błędu zapisu i ponowienie

- **Status:** `TODO`
- **Typ:** `UI`
- **Priorytet:** `SHOULD`
- **Zależności:** `UI-303`, `DBI-505`

**Opis:** Przygotowanie komunikatu `wynik niezapisany` i przycisku ponowienia zapisu w bieżącej sesji.

**Kryteria akceptacji:**

- [ ] Błąd bazy nie usuwa lokalnego podsumowania.
- [ ] Ekran rozróżnia zapis udany, trwający i nieudany.
- [ ] Ponowienie używa tego samego `client_run_id`.
- [ ] Wielokrotne kliknięcie nie uruchamia równoległych zapisów.

**Weryfikacja:** wymuszony błąd pliku, ponowienie i podwójne kliknięcie.

**Sugerowany commit:** `feat(ui): add save status and retry`

---

## Sprint 4 - relacyjna baza SQLite

**Cel sprintu:** przygotować bazę odtwarzaną od zera oraz komplet elementów SQL wymaganych na zaliczenie.

**Kryterium zakończenia sprintu:** skrypty tworzą poprawną bazę i demonstracyjnie pokazują relacje, ograniczenia, transakcje, trigger, widoki, CTE oraz funkcje okna.

### DB-401 - Struktura katalogu Database i migracje

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-101`

**Opis:** Utworzenie wersjonowanej struktury skryptów SQL i instrukcji odtworzenia bazy.

**Kryteria akceptacji:**

- [ ] Istnieje katalog `Database/`.
- [ ] Skrypty mają kolejność `001`-`006` zgodną ze specyfikacją.
- [ ] `Database/README.md` opisuje kolejność, lokalizację pliku i odtworzenie bazy.
- [ ] Migracje korzystają z `PRAGMA user_version`.
- [ ] Nową bazę można utworzyć bez ręcznego poprawiania skryptów.

**Weryfikacja:** odtworzenie pustej bazy wyłącznie według README.

**Sugerowany commit:** `db: add migration structure and setup guide`

### DB-402 - Tabele, relacje i ograniczenia

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DB-401`

**Opis:** Utworzenie tabel `players`, `levels`, `runs`, `run_events` i `personal_bests`.

**Kryteria akceptacji:**

- [ ] Wszystkie tabele mają odpowiednie klucze główne.
- [ ] Relacje posiadają klucze obce, a usuwanie zdarzeń próby działa zgodnie z projektem.
- [ ] `PRAGMA foreign_keys = ON` jest wymagane i testowane.
- [ ] Status próby dopuszcza wyłącznie `COMPLETED` albo `FAILED`.
- [ ] Czasy i liczniki nie mogą być ujemne.
- [ ] Pseudonim jest unikalny bez rozróżniania wielkości liter.
- [ ] Para `(level_code, level_version)` jest unikalna.
- [ ] `client_run_id` jest unikalny.
- [ ] Istnieją indeksy pod ranking i historię gracza.

**Weryfikacja:** poprawne inserty oraz celowo błędne dane naruszające każde ograniczenie.

**Sugerowany commit:** `db: add relational schema and constraints`

### DB-403 - Trigger, widoki i zapytania analityczne

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DB-402`

**Opis:** Dodanie wymaganych elementów SQL używanych przez ranking i prezentację zaliczeniową.

**Kryteria akceptacji:**

- [ ] Istnieje co najmniej jeden użyteczny trigger.
- [ ] Widok rankingu używa `DENSE_RANK()`.
- [ ] Ranking uwzględnia tylko najlepszy wynik `COMPLETED` każdego gracza dla wybranej wersji poziomu.
- [ ] Widok statystyk wykorzystuje `COUNT`, `MIN`, `AVG`, `SUM`, `CASE` i `GROUP BY`.
- [ ] Co najmniej jedno zapytanie używa CTE.
- [ ] Co najmniej jedno zapytanie używa `LAG()`.
- [ ] Remisy otrzymują tę samą pozycję.
- [ ] Wyniki różnych wersji poziomu nie mieszają się.

**Weryfikacja:** zestaw danych testowych z remisami, porażkami i dwiema wersjami poziomu.

**Sugerowany commit:** `db: add leaderboard views and analytics`

### DB-404 - Transakcja, UPSERT i idempotencja

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DB-402`, `DB-403`

**Opis:** Przygotowanie oraz udokumentowanie atomowej sekwencji zapisu jednej zakończonej próby.

**Kryteria akceptacji:**

- [ ] Sekwencja obejmuje `BEGIN IMMEDIATE`, zapis gracza, próby, zdarzeń, rekordu i `COMMIT`.
- [ ] Dowolny błąd powoduje `ROLLBACK`.
- [ ] Profil gracza i rekord osobisty używają kontrolowanego `UPSERT`.
- [ ] Rekord osobisty aktualizuje się tylko dla lepszego wyniku `COMPLETED`.
- [ ] `FAILED` trafia do historii, ale nie do rankingu.
- [ ] Powtórny `client_run_id` nie tworzy drugiego rekordu.
- [ ] Powtórzenie zwraca dane wcześniej zapisanej próby.

**Weryfikacja:** udany zapis, wymuszony błąd w środku transakcji, gorszy czas i podwójny UUID.

**Sugerowany commit:** `db: add atomic run transaction`

### DB-405 - Seed i demonstracja SQL

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DB-403`, `DB-404`

**Opis:** Przygotowanie danych oraz `006_demo_queries.sql` do pokazania na zaliczeniu.

**Kryteria akceptacji:**

- [ ] Seed tworzy kilku graczy, sukcesy, porażki, remisy i różne wersje poziomu.
- [ ] Demo pokazuje relacje, ograniczenia, trigger, transakcję, UPSERT i rollback.
- [ ] Demo pokazuje ranking, statystyki, CTE, `DENSE_RANK()` i `LAG()`.
- [ ] Każdy wynik ma krótki opis oczekiwanego rezultatu.
- [ ] Skrypt można uruchomić wielokrotnie na świeżo odtworzonej bazie.

**Weryfikacja:** pełne wykonanie `001`-`006` na pustym pliku SQLite.

**Sugerowany commit:** `db: add seed data and demo queries`

---

## Sprint 5 - integracja SQLite z Unreal Engine

**Cel sprintu:** umożliwić grze bezpośrednią, bezpieczną komunikację z lokalnym SQLite.

**Kryterium zakończenia sprintu:** gra tworzy plik bazy, wykonuje migracje, obsługuje błąd bez crasha i wykonuje operacje poza głównym wątkiem.

### DBI-501 - Techniczny test SQLiteCore i SQLiteSupport

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DB-401`

**Opis:** Minimalny test wbudowanych modułów SQLite w aktualnej wersji Unreal Engine przed budową pełnej warstwy danych.

**Kryteria akceptacji:**

- [ ] Przed zmianą `.uproject` i `Build.cs` przedstawiono plan oraz uzyskano zgodę.
- [ ] Włączone są wyłącznie wbudowane moduły `SQLiteCore` i `SQLiteSupport`.
- [ ] Nie dodano zewnętrznej biblioteki ani serwera.
- [ ] Projekt kompiluje się w konfiguracji Editor.
- [ ] Minimalny test otwiera lokalny plik i wykonuje `SELECT 1`.
- [ ] Sprawdzono możliwość użycia modułów w buildzie Windows.

**Weryfikacja:** kompilacja Editor, test zapytania i mały build techniczny.

**Sugerowany commit:** `feat(db): enable Unreal SQLite modules`

### DBI-502 - GameDatabaseSubsystem i ścieżka pliku

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DBI-501`

**Opis:** Utworzenie centralnej warstwy dostępu do lokalnego pliku SQLite.

**Kryteria akceptacji:**

- [ ] `UGameDatabaseSubsystem` jest `UGameInstanceSubsystem`.
- [ ] Ścieżka kończy się `Saved/Database/DiplomaGame.db`.
- [ ] Katalog jest tworzony automatycznie.
- [ ] Połączenie włącza `PRAGMA foreign_keys = ON`.
- [ ] Zapytania SQL nie znajdują się w klasach rozgrywki ani widgetach.
- [ ] Logi nie wypisują pełnych danych użytkownika.
- [ ] Zamknięcie gry bezpiecznie zamyka połączenie.

**Weryfikacja:** pierwsze uruchomienie, ponowne uruchomienie i zamknięcie gry.

**Sugerowany commit:** `feat(db): add game database subsystem`

### DBI-503 - Migracje i pakowanie skryptów

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DB-405`, `DBI-502`

**Opis:** Automatyczne tworzenie i aktualizowanie bazy według `PRAGMA user_version`.

**Kryteria akceptacji:**

- [ ] Nowa baza wykonuje migracje w prawidłowej kolejności.
- [ ] Zastosowana migracja nie wykonuje się drugi raz.
- [ ] Błąd migracji nie zostawia częściowo zmienionego schematu.
- [ ] Skrypty wymagane przez grę są dostępne w buildzie jako kontrolowane dane Non-UFS albo przez uzgodniony mechanizm C++.
- [ ] Brak lub uszkodzenie skryptu zwraca kontrolowany błąd.

**Weryfikacja:** nowa baza, baza aktualna, przerwana migracja i uruchomienie poza edytorem.

**Sugerowany commit:** `feat(db): add versioned SQLite migrations`

### DBI-504 - Serializowana kolejka operacji

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DBI-502`

**Opis:** Przeniesienie operacji bazy poza główny wątek bez równoczesnego używania jednego połączenia.

**Kryteria akceptacji:**

- [ ] Operacje są wykonywane przez jedną serializowaną kolejkę.
- [ ] Jedno połączenie nie jest używane jednocześnie przez kilka zadań.
- [ ] Wyniki wracają do UI na głównym wątku przez delegaty lub callbacki.
- [ ] Zamknięcie gry zatrzymuje kolejkę bez użycia zwolnionych obiektów.
- [ ] Błąd operacji nie powoduje crasha.
- [ ] Dłuższe zapytanie nie zatrzymuje obrazu.

**Weryfikacja:** kilka kolejnych operacji, próba równoległa, zamknięcie podczas pracy i kontrolowany błąd.

**Sugerowany commit:** `feat(db): serialize database operations`

### DBI-505 - Atomowy zapis zakończonej próby

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DB-404`, `DBI-503`, `DBI-504`

**Opis:** Implementacja parametryzowanej transakcji zapisującej pseudonim, próbę, zdarzenia i rekord osobisty.

**Kryteria akceptacji:**

- [ ] Wszystkie wartości gry są przekazywane jako parametry.
- [ ] Transakcja zapisuje komplet danych albo nic.
- [ ] `client_run_id` zapewnia idempotencję.
- [ ] Zdarzenia zachowują kolejność.
- [ ] Rekord osobisty zmienia się tylko po lepszym `COMPLETED`.
- [ ] Zwracany wynik zawiera `run_id`, status zapisu i informację o rekordzie.
- [ ] Błąd pozostawia lokalne podsumowanie do ponowienia.

**Weryfikacja:** sukces, porażka, apostrof w pseudonimie, rollback i podwójny UUID.

**Sugerowany commit:** `feat(db): save runs in one transaction`

### DBI-506 - Odczyt rankingu i statystyk

- **Status:** `TODO`
- **Typ:** `DB`
- **Priorytet:** `MUST`
- **Zależności:** `DBI-504`, `DB-403`

**Opis:** Udostępnienie operacji odczytujących rekord osobisty, pozycję, Top 10 i podstawowe statystyki.

**Kryteria akceptacji:**

- [ ] Zapytanie przyjmuje `level_code` i `level_version`.
- [ ] Zwracany jest rekord osobisty gracza.
- [ ] Zwracana jest pozycja z poprawną obsługą remisów.
- [ ] Zwracane jest maksymalnie 10 najlepszych graczy.
- [ ] Próby `FAILED` nie wpływają na ranking.
- [ ] Statystyki obejmują liczbę prób, ukończenia, najlepszy i średni czas oraz sumy rozgrywki.
- [ ] Brak wyników zwraca pusty, poprawny rezultat, a nie błąd.

**Weryfikacja:** pusta baza, remisy, porażki, dwie wersje poziomu i ponad 10 graczy.

**Sugerowany commit:** `feat(db): query leaderboard and player statistics`

---

## Sprint 6 - zapis i ranking end-to-end

**Cel sprintu:** połączyć lokalną próbę, bazę i UI w jeden odporny przepływ.

**Kryterium zakończenia sprintu:** sukces lub porażka zapisuje się w bazie, a gracz widzi status zapisu, rekord osobisty, pozycję i Top 10.

### INT-601 - Przekazanie podsumowania próby do bazy

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `MUST`
- **Zależności:** `RUN-104`, `LVL-204`, `DBI-505`

**Opis:** Automatyczne rozpoczęcie zapisu po utworzeniu zamkniętego `FRunSummary`.

**Kryteria akceptacji:**

- [ ] Zapis uruchamia się po `Completed` i `Failed`.
- [ ] Subsystem próby nie zawiera bezpośrednich zapytań SQL.
- [ ] Ta sama próba nie uruchamia dwóch niezależnych zapisów.
- [ ] Status zapisu jest dostępny dla UI.
- [ ] Brak bazy nie blokuje zakończenia poziomu.
- [ ] Po udanym zapisie lokalny wynik zna identyfikator rekordu.

**Weryfikacja:** sukces, porażka, brak pliku, podwójne zdarzenie końca.

**Sugerowany commit:** `feat: persist completed and failed runs`

### INT-602 - Ranking na ekranie wyniku

- **Status:** `TODO`
- **Typ:** `UI`
- **Priorytet:** `MUST`
- **Zależności:** `UI-303`, `DBI-506`, `INT-601`

**Opis:** Rozszerzenie ekranu wyniku o dane odczytane po zapisie.

**Kryteria akceptacji:**

- [ ] Ekran pokazuje rekord osobisty.
- [ ] Ekran pokazuje pozycję bieżącego gracza.
- [ ] Ekran pokazuje Top 10 aktualnej wersji poziomu.
- [ ] Remisy są prezentowane ze wspólną pozycją.
- [ ] Stan ładowania nie blokuje głównego wątku.
- [ ] Pusty ranking i błąd odczytu mają czytelne stany UI.

**Weryfikacja:** pierwsza próba, nowy rekord, gorszy wynik, remis i błąd odczytu.

**Sugerowany commit:** `feat(ui): show leaderboard on result screen`

### INT-603 - Ponowienie nieudanego zapisu w sesji

- **Status:** `TODO`
- **Typ:** `FEATURE`
- **Priorytet:** `SHOULD`
- **Zależności:** `UI-304`, `INT-601`

**Opis:** Zachowanie podsumowania w pamięci i bezpieczne ponowienie tej samej transakcji.

**Kryteria akceptacji:**

- [ ] Ponowienie używa tego samego `client_run_id`.
- [ ] Udane ponowienie nie tworzy duplikatu.
- [ ] Po sukcesie status UI aktualizuje się jeden raz.
- [ ] Trwała kolejka po zamknięciu gry nie jest implementowana.

**Weryfikacja:** chwilowa blokada pliku, odblokowanie i wielokrotne ponowienie.

**Sugerowany commit:** `feat: retry failed run saves in session`

### INT-604 - Test integralności przepływu danych

- **Status:** `TODO`
- **Typ:** `TEST`
- **Priorytet:** `MUST`
- **Zależności:** `INT-601`, `INT-602`

**Opis:** Porównanie danych z rzeczywistej rozgrywki z rekordami zapisanymi w SQLite.

**Kryteria akceptacji:**

- [ ] Czas w UI i bazie jest zgodny.
- [ ] Liczniki checkpointów, wrogów i obrażeń są zgodne.
- [ ] Kolejność i czasy zdarzeń są poprawne.
- [ ] Sukces wpływa na ranking, a porażka nie.
- [ ] Idempotencja działa również przez kod gry.
- [ ] Pseudonimy z apostrofem i polskimi znakami zapisują się poprawnie.

**Weryfikacja:** kontrolowana próba testowa i ręczne zapytania porównawcze.

---

## Sprint 7 - finalne środowisko i mapa

**Cel sprintu:** zastąpić placeholdery mapy spójnym środowiskiem bez naruszania gotowych mechanik ani wcześniej podłączonych postaci.

**Kryterium zakończenia sprintu:** poziom jest czytelny, ukończalny i korzysta wyłącznie z materiałów o znanym pochodzeniu.

### ART-701 - Wybór assetów środowiska i potwierdzenie licencji

- **Status:** `TODO`
- **Typ:** `ASSET`
- **Priorytet:** `MUST`
- **Zależności:** `LVL-205`

**Opis:** Wybór małego, spójnego zestawu środowiska, materiałów i tekstur dla finalnego wyglądu poziomu.

**Kryteria akceptacji:**

- [ ] Wybrano jedną spójną paczkę środowiska.
- [ ] Wybrano potrzebne materiały i tekstury dla podłoża, przeszkód oraz tła.
- [ ] Znane są autor, źródło, licencja i sposób użycia każdego materiału.
- [ ] Dowody licencji są zachowane poza repozytorium, jeżeli zawierają dane osobowe.
- [ ] Assety nie wprowadzają obcej logiki gameplayowej.

**Weryfikacja:** checklista źródeł i przegląd zawartości paczek przed importem.

### ART-702 - Finalny wygląd M01

- **Status:** `TODO`
- **Typ:** `ASSET`
- **Priorytet:** `MUST`
- **Zależności:** `ART-701`, `LVL-205`

**Opis:** Zastąpienie brył testowych czytelnym środowiskiem bez zmiany logiki trasy.

**Kryteria akceptacji:**

- [ ] Start, obowiązkowa walka, checkpointy i meta są wizualnie czytelne.
- [ ] Gracz rozpoznaje zamkniętą i otwartą bramę.
- [ ] Przeszkody są możliwe do pokonania przy obecnych parametrach ruchu.
- [ ] Kamera nie przenika przez geometrię i nie pokazuje nieprzeznaczonych obszarów.
- [ ] Opcjonalni przeciwnicy nie blokują trasy ani nie aktywują się jednocześnie.
- [ ] Map Check ma zero błędów.
- [ ] Pełna próba sukcesu i porażki nadal działa.

**Weryfikacja:** pełne przejście, Map Check i test regresji walki.

**Sugerowany commit:** `feat(art): finalize M01 environment`

### ART-703 - Podstawowy polish UI i informacji zwrotnej

- **Status:** `TODO`
- **Typ:** `UI`
- **Priorytet:** `SHOULD`
- **Zależności:** `UI-302`, `UI-303`, `ART-701`

**Opis:** Ujednolicenie wizualne HUD-u, ekranów i podstawowych efektów trafienia.

**Kryteria akceptacji:**

- [ ] Tekst i kolory są czytelne w docelowej rozdzielczości.
- [ ] Trafienie, śmierć, checkpoint i otwarcie bramy mają prostą informację zwrotną.
- [ ] UI nie zasłania kluczowego obszaru gry.
- [ ] Efekty nie utrudniają pomiaru czasu ani sterowania.
- [ ] Polish nie wprowadza nowych mechanik poza MVP.

**Weryfikacja:** pełna próba w docelowej rozdzielczości.

---

## Sprint 8 - testy, build i dokumentacja końcowa

**Cel sprintu:** przygotować stabilny, samodzielny build Windows oraz materiały do prezentacji projektu i bazy.

**Kryterium zakończenia sprintu:** build uruchamia się poza edytorem, pełna pętla działa, baza powstaje lokalnie, a wymagania SQL można zademonstrować.

### QA-801 - Pełna macierz testów MVP

- **Status:** `TODO`
- **Typ:** `TEST`
- **Priorytet:** `MUST`
- **Zależności:** `INT-604`, `ART-103`, `ART-702`

**Opis:** Wykonanie pełnej listy przypadków ze specyfikacji.

**Kryteria akceptacji:**

- [ ] Sprawdzono podwójny start timera.
- [ ] Sprawdzono bramę przed i po obowiązkowej walce.
- [ ] Sprawdzono podwójną aktywację checkpointu.
- [ ] Sprawdzono metę bez wymaganych checkpointów.
- [ ] Sprawdzono sukces, śmierć, restart i nowy UUID.
- [ ] Sprawdzono brak dostępu do bazy i ponowienie zapisu.
- [ ] Sprawdzono pseudonimy, rekord osobisty, gorszy wynik, remis i dwie wersje poziomu.
- [ ] Sprawdzono podwójny `client_run_id`.
- [ ] Wszystkie znalezione błędy mają osobne zadania lub zostały naprawione i zweryfikowane.

**Weryfikacja:** wypełniona macierz przypadków z wynikiem `PASS/FAIL`.

### QA-802 - Packaging danych SQLite

- **Status:** `TODO`
- **Typ:** `TEST`
- **Priorytet:** `MUST`
- **Zależności:** `DBI-503`

**Opis:** Potwierdzenie, że finalny build zawiera wszystko potrzebne do utworzenia bazy.

**Kryteria akceptacji:**

- [ ] Moduły `SQLiteCore` i `SQLiteSupport` są obecne w buildzie.
- [ ] Migracje są dołączone uzgodnioną metodą.
- [ ] Build tworzy bazę w swoim `Saved/Database/`.
- [ ] Build nie próbuje zapisywać bazy w `Content/`.
- [ ] Pierwsze i kolejne uruchomienie wykonują właściwe migracje.
- [ ] Brak pliku bazy nie wymaga ręcznej instalacji serwera.

**Weryfikacja:** czysty build uruchomiony w nowym katalogu bez pliku bazy.

### QA-803 - Finalny build Windows

- **Status:** `TODO`
- **Typ:** `TEST`
- **Priorytet:** `MUST`
- **Zależności:** `QA-801`, `QA-802`

**Opis:** Przygotowanie samodzielnej wersji demonstracyjnej uruchamianej poza Unreal Editorem.

**Kryteria akceptacji:**

- [ ] Packaging kończy się bez błędu.
- [ ] Build uruchamia się na Windows bez Unreal Editora.
- [ ] Można podać pseudonim, rozpocząć, przegrać i ukończyć próbę.
- [ ] Zapis sukcesu i porażki działa.
- [ ] Ranking pokazuje rekord, pozycję i Top 10.
- [ ] Ponowne uruchomienie zachowuje lokalne dane.
- [ ] Logi nie zawierają krytycznych błędów ani danych użytkownika w zapytaniach SQL.

**Weryfikacja:** test czystego buildu od startu do wyniku i po ponownym uruchomieniu.

### DOC-804 - Dokumentacja uruchomienia i prezentacji

- **Status:** `TODO`
- **Typ:** `DOCS`
- **Priorytet:** `MUST`
- **Zależności:** `QA-803`, `DB-405`

**Opis:** Uzupełnienie istniejącej dokumentacji bez tworzenia zbędnych, powtarzających się plików.

**Kryteria akceptacji:**

- [ ] `README.md` opisuje uruchomienie projektu, buildu i lokalnej bazy.
- [ ] `GAME_MVP_REQUIREMENTS.md` odpowiada finalnemu zakresowi.
- [ ] Architektura Unreal -> SQLite oraz model danych są przedstawione w jednym uzgodnionym miejscu.
- [ ] Udokumentowano źródła i licencje użytych materiałów.
- [ ] `Database/README.md` opisuje migracje i odtworzenie bazy.
- [ ] `006_demo_queries.sql` zawiera komentarze potrzebne podczas prezentacji.
- [ ] Udokumentowano wykonane testy i znane ograniczenia.
- [ ] Dokumentacja nie zawiera martwych odwołań ani sprzecznych wymagań.

**Weryfikacja:** przejście instrukcji na czystym środowisku oraz kontrola linków i poleceń.

**Sugerowany commit:** `docs: finalize project and database guide`

### REL-805 - Zamknięcie zakresu MVP

- **Status:** `TODO`
- **Typ:** `CHORE`
- **Priorytet:** `MUST`
- **Zależności:** `QA-803`, `DOC-804`

**Opis:** Ostateczne potwierdzenie, że wymagania `MUST` zostały zakończone, a dodatki nie blokują wydania.

**Kryteria akceptacji:**

- [ ] Wszystkie zadania `MUST` mają status `DONE`.
- [ ] Zadania `SHOULD` i `COULD` nie blokują buildu.
- [ ] Repozytorium nie zawiera plików tymczasowych ani przypadkowych zmian.
- [ ] Finalna mapa, konfiguracja i baza odpowiadają dokumentacji.
- [ ] Znane ograniczenia są zapisane i zaakceptowane.
- [ ] Wersja demonstracyjna została zachowana w ustalonym miejscu.

**Weryfikacja:** końcowy przegląd backlogu, repozytorium, buildu i dokumentacji.

## 3. Zadania poza MVP

Poniższe pomysły nie są planowane przed zakończeniem `REL-805`:

- boss,
- drugi poziom,
- wiele klas przeciwników,
- combo, wiele broni i ekwipunek,
- stamina, pancerz i wall jump,
- ruchome platformy,
- fabuła, dialogi i cutscenki,
- multiplayer,
- logowanie użytkowników,
- serwer aplikacyjny albo zewnętrzna baza,
- trwała kolejka nieudanych zapisów po zamknięciu gry,
- ochrona lokalnego rankingu przed ręczną edycją pliku.

## 4. Następne zadanie

Pierwszym niezakończonym zadaniem jest:

`RUN-101 - Struktury danych i status próby`

Przed rozpoczęciem należy sprawdzić stan `main`, utworzyć osobny branch i opisać planowane pliki oraz ryzyka.
