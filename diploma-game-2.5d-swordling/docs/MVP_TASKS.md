# Prosta lista zadań MVP

Dokument opisuje najkrótszą drogę do wykonania gry zdefiniowanej w `docs/MVP_GAME_PLAN_FRESH.md`. Każdy ticket zawiera cel, wstępną instrukcję wykonania, uzasadnienie i oczekiwany rezultat. Zadania realizujemy po kolei, a rozbudowę odkładamy do czasu ukończenia działającego MVP.

## Etap 1 - ustalenie podstaw

### TASK-01 - Zatwierdzenie wyglądu i zasobów

- [ ] **Ticket ukończony**

**Cel:** Ustalić spójny i możliwy do szybkiego wykonania wygląd gry.

**Instrukcja wstępna:**

- [ ] Wybierz prosty model głównej postaci i jeden model bossa z biblioteki Fab dla Unreal Engine albo z innego legalnego źródła.
- [ ] Sprawdź, czy modele mają zgodne szkielety lub czy można je łatwo retargetować.
- [ ] Sprawdź dostępność animacji Idle, Run, Jump/Fall, Attack, Hit i Death.
- [ ] Znajdź jedną paczkę środowiska pasującą do modeli: podłoże, platformy, przeszkody, obiekty tła i materiały.
- [ ] Dla każdego wybranego zasobu zapisz nazwę, autora, link, licencję i planowany sposób użycia.
- [ ] Nie importuj zasobów do projektu przed zaakceptowaniem całego zestawu.

**Dlaczego:** Wczesny wybór spójnych zasobów ograniczy późniejsze poprawki skali, animacji, kolizji i wyglądu mapy.

**Oczekiwany rezultat:** Zatwierdzona lista modeli, animacji i assetów środowiska z potwierdzonymi licencjami.

### TASK-02 - Sprawdzenie obecnej wersji projektu

- [ ] **Ticket ukończony**

**Cel:** Ustalić, które istniejące mechaniki można bezpiecznie wykorzystać w nowym MVP.

**Instrukcja wstępna:**

- [ ] Uruchom aktualną mapę integracyjną w Unreal Editorze.
- [ ] Sprawdź osobno: ruch, skok, kamerę, obracanie postaci, atak, zdrowie, śmierć i restart.
- [ ] Sprawdź wykrywanie gracza, pościg, atak, obrażenia i śmierć obecnego przeciwnika.
- [ ] Zapisz wynik każdej próby jako `działa`, `wymaga poprawy` albo `nie istnieje`.
- [ ] Przygotuj listę tylko tych błędów, które blokują nowe MVP.

**Dlaczego:** Część mechanik już istnieje, dlatego należy je wykorzystać zamiast niepotrzebnie tworzyć ponownie.

**Oczekiwany rezultat:** Krótka checklista działających mechanik i niezbędnych poprawek.

## Etap 2 - postacie i podstawowa rozgrywka

### TASK-03 - Dodanie modeli i animacji

- [ ] **Ticket ukończony**

**Cel:** Zastąpić placeholdery zatwierdzonym modelem gracza i bossa.

**Instrukcja wstępna:**

- [ ] Utwórz osobne foldery dla gracza, bossa i współdzielonych animacji.
- [ ] Zaimportuj tylko zatwierdzone modele, szkielety, tekstury i materiały.
- [ ] Ustaw poprawną skalę, orientację i położenie modeli względem kapsuł kolizji.
- [ ] W razie potrzeby wykonaj retargetowanie animacji.
- [ ] Podłącz Idle, Run, Jump/Fall, Attack, Hit i Death w prostych Animation Blueprintach.
- [ ] Sprawdź wszystkie animacje na mapie testowej bez zmieniania zasad walki.

**Dlaczego:** Docelowe proporcje postaci są potrzebne przed ostatecznym ustawianiem przeszkód, zasięgów i kolizji.

**Oczekiwany rezultat:** Gracz i boss korzystają z właściwych modeli oraz czytelnych animacji.

### TASK-04 - Domknięcie ruchu i walki

- [ ] **Ticket ukończony**

**Cel:** Przygotować stabilny rdzeń sterowania i pojedynku z bossem.

**Instrukcja wstępna:**

- [ ] Napraw problemy z ruchem, skokiem i kierunkiem postaci wykryte w TASK-02.
- [ ] Sprawdź atak gracza, zasięg trafienia, cooldown i jednokrotne naliczanie obrażeń.
- [ ] Skonfiguruj istniejącego przeciwnika jako jedynego prostego bossa MVP.
- [ ] Sprawdź wykrywanie, pościg, atak, zdrowie, otrzymywanie obrażeń i śmierć bossa.
- [ ] Sprawdź śmierć gracza, blokadę sterowania i bezpieczny restart próby.
- [ ] Nie dodawaj nowych rodzajów ataków ani dodatkowych przeciwników.

**Dlaczego:** Stabilny rdzeń rozgrywki jest podstawą wszystkich dalszych testów poziomu.

**Oczekiwany rezultat:** Gracz może poruszać się i walczyć, a jeden boss może rozpocząć, prowadzić i zakończyć pojedynek.

## Etap 3 - próba i grywalna mapa

### TASK-05 - Dodanie stanu próby i timera

- [ ] **Ticket ukończony**

**Cel:** Utworzyć jedno źródło prawdy o aktualnej próbie gracza.

**Instrukcja wstępna:**

- [ ] Zdefiniuj stany próby: nierozpoczęta, aktywna, ukończona i nieudana.
- [ ] Przy rozpoczęciu utwórz unikalny identyfikator próby i zapisz moment startu.
- [ ] Mierz czas na podstawie zegara silnika i przechowuj wynik w milisekundach.
- [ ] Rejestruj rozpoczęcie próby, checkpointy, otrzymane obrażenia, pokonanie bossa, śmierć gracza i dotarcie do mety.
- [ ] Kończ próbę tylko raz: sukcesem na mecie albo porażką po śmierci.
- [ ] Przy restarcie utwórz nową próbę zamiast nadpisywać poprzednią.

**Dlaczego:** Timer i wynik są głównym celem rozgrywki oraz źródłem danych zapisywanych później w bazie.

**Oczekiwany rezultat:** System tworzy kompletne i niezmienne podsumowanie każdej zakończonej próby.

### TASK-06 - Zbudowanie mapy z prostych brył

- [ ] **Ticket ukończony**

**Cel:** Zbudować grywalny układ całego poziomu przed dodaniem finalnych assetów.

**Instrukcja wstępna:**

- [ ] Zbuduj mapę wyłącznie z prostych brył i podstawowych materiałów testowych.
- [ ] Ustaw kolejno: bezpieczny start, uruchomienie timera, checkpoint 1, pierwszą sekcję skoków, checkpoint 2, drugą sekcję parkour i checkpoint 3.
- [ ] Za checkpointem 3 umieść zamkniętą arenę bossa, wyjście otwierane po walce i metę.
- [ ] Dopasuj szerokość platform, wysokość skoków i odległości do aktualnych możliwości gracza.
- [ ] Przejdź mapę kilka razy i dostosuj ją tak, aby pierwsze poprawne przejście trwało około 3–5 minut.
- [ ] Sprawdź, czy boczna kamera nie pokazuje pustych ani nieprzeznaczonych obszarów.

**Dlaczego:** Najpierw trzeba potwierdzić, że poziom jest grywalny i ma odpowiednią długość, zanim poświęcimy czas na jego wygląd.

**Oczekiwany rezultat:** Kompletna szara wersja trasy `start → parkour → boss → meta`.

### TASK-07 - Dodanie checkpointów, bossa i mety

- [ ] **Ticket ukończony**

**Cel:** Wymusić prawidłową kolejność przejścia całego poziomu.

**Instrukcja wstępna:**

- [ ] Dodaj strefę startową uruchamiającą timer tylko przy pierwszym wejściu gracza.
- [ ] Nadaj trzem checkpointom stałe numery i zaliczaj każdy z nich tylko raz.
- [ ] Pozwól wejść do areny bossa dopiero po zaliczeniu trzech checkpointów.
- [ ] Zamknij wyjście z areny na czas walki.
- [ ] Po śmierci bossa zarejestruj jedno zdarzenie `BOSS_DEFEATED` i otwórz wyjście dokładnie raz.
- [ ] Pozwól mecie zakończyć próbę tylko po zaliczeniu checkpointów i pokonaniu bossa.

**Dlaczego:** Te elementy tworzą pełny i możliwy do jednoznacznego sprawdzenia przebieg poziomu.

**Oczekiwany rezultat:** Nie można ominąć parkouru, checkpointów ani walki z bossem przed dotarciem do mety.

## Etap 4 - interfejs

### TASK-08 - Dodanie pseudonimu i HUD-u

- [ ] **Ticket ukończony**

**Cel:** Pokazać graczowi informacje potrzebne do rozpoczęcia i ukończenia próby.

**Instrukcja wstępna:**

- [ ] Utwórz prosty ekran startowy z polem pseudonimu i przyciskiem rozpoczęcia.
- [ ] Odrzucaj pusty, zbyt krótki lub zbyt długi pseudonim i pokaż czytelny komunikat błędu.
- [ ] Dodaj HUD pokazujący zdrowie gracza, aktualny czas i liczbę zaliczonych checkpointów z trzech.
- [ ] Po wejściu na arenę pokaż prosty pasek zdrowia bossa.
- [ ] Po zakończeniu próby zatrzymaj aktualizowanie czasu i ukryj elementy niepotrzebne na ekranie wyniku.

**Dlaczego:** Gracz musi rozumieć stan rozgrywki, a pseudonim jest potrzebny do zapisu i rankingu.

**Oczekiwany rezultat:** Gracz może podać pseudonim i przez całą próbę widzi jej najważniejszy stan.

### TASK-09 - Dodanie ekranów zakończenia

- [ ] **Ticket ukończony**

**Cel:** Zapewnić czytelne zakończenie udanej i nieudanej próby bez zależności od SQLite.

**Instrukcja wstępna:**

- [ ] Po dotarciu do mety pokaż ekran sukcesu z pseudonimem, czasem, checkpointami i informacją o pokonaniu bossa.
- [ ] Po śmierci pokaż ekran porażki z czasem osiągniętym przed śmiercią, checkpointami i etapem próby.
- [ ] Zablokuj sterowanie postacią po wyświetleniu podsumowania.
- [ ] Dodaj możliwość rozpoczęcia nowej próby.
- [ ] Pobieraj wszystkie dane z podsumowania próby, a nie bezpośrednio z widgetów.

**Dlaczego:** Każda próba musi mieć wyraźne zakończenie i informację zwrotną jeszcze przed integracją bazy.

**Oczekiwany rezultat:** Sukces i porażka mają osobne, działające ekrany podsumowania.

## Etap 5 - baza danych

### TASK-10 - Zaprojektowanie bazy SQLite

- [ ] **Ticket ukończony**

**Cel:** Przygotować mały relacyjny model danych wystarczający do zapisu prób i rankingu Top 5.

**Instrukcja wstępna:**

- [ ] Utwórz tabelę graczy z unikalnym pseudonimem.
- [ ] Utwórz tabelę prób powiązaną z graczem, zawierającą identyfikator klienta, status, czas, checkpointy i otrzymane obrażenia.
- [ ] Nie dodawaj licznika pokonanych przeciwników, ponieważ w MVP występuje tylko jeden boss.
- [ ] Utwórz tabelę zdarzeń próby, w której można zapisać między innymi `BOSS_DEFEATED`.
- [ ] Dodaj klucze obce, ograniczenia statusu, unikalność próby i potrzebne indeksy.
- [ ] Przygotuj zapytania zwracające rekord osobisty oraz pięć najlepszych ukończonych wyników.
- [ ] Sprawdź schemat na małym zestawie danych testowych, w tym na remisie i nieudanej próbie.

**Dlaczego:** Poprawny schemat jest najważniejszym elementem części bazodanowej projektu podyplomowego.

**Oczekiwany rezultat:** Gotowe skrypty SQLite tworzące bazę oraz zwracające rekord osobisty i Top 5.

### TASK-11 - Połączenie Unreal Engine z SQLite

- [ ] **Ticket ukończony**

**Cel:** Udostępnić mechanikom gry bezpieczny dostęp do lokalnej bazy.

**Instrukcja wstępna:**

- [ ] Potwierdź dostępność wbudowanych modułów SQLite w używanej wersji Unreal Engine.
- [ ] Włącz tylko wymagane moduły i dodaj je do konfiguracji kompilacji projektu.
- [ ] Utwórz osobny subsystem lub serwis C++ odpowiedzialny za bazę.
- [ ] Przy pierwszym uruchomieniu utwórz bazę w `Saved/Database/` i wykonaj migracje.
- [ ] Udostępnij proste operacje: zapis próby, pobranie rekordu osobistego i pobranie Top 5.
- [ ] Obsłuż brak pliku, błąd otwarcia i błąd zapisu bez crasha i bez blokowania ukończenia gry.
- [ ] Nie umieszczaj zapytań SQL w widgetach ani klasach postaci.

**Dlaczego:** Logika bazy powinna być oddzielona od mechanik i interfejsu, aby projekt był czytelny i łatwy do przetestowania.

**Oczekiwany rezultat:** Gra może utworzyć i otworzyć lokalną bazę oraz bezpiecznie wykonać podstawowe operacje.

### TASK-12 - Zapis próby i wyświetlenie rankingu

- [ ] **Ticket ukończony**

**Cel:** Zamknąć pełny przepływ danych od rozgrywki do ekranu wyniku.

**Instrukcja wstępna:**

- [ ] Po sukcesie albo porażce przekaż niezmienne podsumowanie próby do subsystemu bazy.
- [ ] W jednej transakcji utwórz lub pobierz gracza, zapisz próbę i wszystkie jej zdarzenia.
- [ ] Zabezpiecz zapis przed utworzeniem duplikatu tej samej próby.
- [ ] Po udanym zapisie pobierz rekord osobisty i Top 5 ukończonych wyników.
- [ ] Pokaż te dane na ekranie sukcesu; przy porażce pokaż status zapisu bez dodawania wyniku do rankingu.
- [ ] Przy błędzie pozostaw podsumowanie w pamięci i umożliw ponowienie zapisu w bieżącej sesji.

**Dlaczego:** To zamyka pełny przepływ danych od wydarzeń w grze do relacyjnej bazy i ponownie do interfejsu.

**Oczekiwany rezultat:** Każda próba zapisuje się dokładnie raz, a ukończona próba może zaktualizować rekord osobisty i Top 5.

## Etap 6 - finalny wygląd

### TASK-13 - Zastąpienie brył docelowym środowiskiem

- [ ] **Ticket ukończony**

**Cel:** Nadać sprawdzonej trasie spójny finalny wygląd bez przebudowy mechanik.

**Instrukcja wstępna:**

- [ ] Zaimportuj tylko potrzebne elementy zatwierdzonej paczki środowiska.
- [ ] Zachowaj położenie i rozmiary testowych platform, dopóki ich zamienniki nie przejdą testu kolizji.
- [ ] Zastąp bryły kolejno w sekcji startowej, parkourze, arenie bossa i przy mecie.
- [ ] Dodaj proste tło, materiały i oświetlenie pasujące do wybranego stylu.
- [ ] Oznacz checkpointy, arenę bossa, zamknięte wyjście i metę wyraźnymi kolorami lub obiektami.
- [ ] Po każdej sekcji wykonaj krótki test ruchu, skoku, walki i kamery.

**Dlaczego:** Oddzielenie wyglądu od projektowania poziomu zmniejsza ryzyko zepsucia mechanik i pozwala szybko osiągnąć spójny efekt.

**Oczekiwany rezultat:** Cała mapa ma spójne środowisko, a jej mechanika i czas przejścia pozostają niezmienione.

### TASK-14 - Podstawowy polish

- [ ] **Ticket ukończony**

**Cel:** Sprawić, aby gracz zawsze rozumiał, co wydarzyło się w grze.

**Instrukcja wstępna:**

- [ ] Dodaj krótką informację wizualną przy aktywacji każdego checkpointu.
- [ ] Pokaż zmianę stanu wyjścia z areny po pokonaniu bossa.
- [ ] Dodaj prostą reakcję na trafienie gracza i bossa oraz czytelne zakończenie ich śmierci.
- [ ] Ujednolić kolory, fonty i odstępy na HUD-zie oraz ekranach wyników.
- [ ] Sprawdź, czy interfejs nie zasłania parkouru ani walki z bossem.
- [ ] Nie dodawaj efektów ani ekranów, które nie pomagają zrozumieć rozgrywki.

**Dlaczego:** Kilka prostych efektów wystarczy, aby gra wyglądała na ukończoną bez niekontrolowanego zwiększania zakresu.

**Oczekiwany rezultat:** Gra jest czytelna i sprawia wrażenie ukończonej bez rozbudowanego polishu.

## Etap 7 - zakończenie projektu

### TASK-15 - Pełne testy gry i bazy

- [ ] **Ticket ukończony**

**Cel:** Potwierdzić działanie całego MVP w poprawnych i błędnych scenariuszach.

**Instrukcja wstępna:**

- [ ] Przejdź pełną trasę w kolejności `start → parkour → trzy checkpointy → boss → meta`.
- [ ] Spróbuj ominąć checkpoint, wejść wcześniej na arenę oraz dotrzeć do mety bez pokonania bossa.
- [ ] Sprawdź śmierć i restart podczas parkouru oraz podczas walki.
- [ ] Sprawdź ponowne wejście w checkpoint i ponowne zdarzenie śmierci bossa.
- [ ] Zweryfikuj zapis sukcesu i porażki bez licznika pokonanych przeciwników.
- [ ] Sprawdź nowy rekord, gorszy wynik, remis oraz poprawne ograniczenie rankingu do Top 5.
- [ ] Zasymuluj błąd otwarcia i zapisu bazy oraz sprawdź ponowienie zapisu.
- [ ] Zapisz wyniki przypadków jako `PASS` albo `FAIL` i napraw błędy blokujące MVP.

**Dlaczego:** Projekt musi działać w typowych i błędnych scenariuszach, a zapis danych nie może psuć rozgrywki.

**Oczekiwany rezultat:** Wszystkie obowiązkowe przypadki przechodzą, a znane ograniczenia są zapisane.

### TASK-16 - Build Windows i dokumentacja

- [ ] **Ticket ukończony**

**Cel:** Przygotować projekt do oddania i prezentacji bez używania Unreal Editora.

**Instrukcja wstępna:**

- [ ] Przygotuj konfigurację projektu i bazę startową do poprawnego działania w buildzie Windows.
- [ ] Zbuduj wersję Windows i uruchom ją na czystym katalogu danych użytkownika.
- [ ] Przejdź pełną próbę i sprawdź utworzenie bazy, zapis wyniku oraz Top 5.
- [ ] Napisz krótką instrukcję uruchomienia gry i lokalnej bazy.
- [ ] Opisz najważniejsze klasy, przepływ próby, schemat SQLite i sposób tworzenia rankingu.
- [ ] Dołącz wyniki testów, znane ograniczenia oraz tabelę wszystkich assetów z ich źródłami i licencjami.

**Dlaczego:** Działający build i czytelna dokumentacja są potrzebne do oddania i obrony projektu.

**Oczekiwany rezultat:** Samodzielny, przetestowany build Windows oraz komplet krótkiej dokumentacji projektowej.

## Zasada pilnowania zakresu

Nową funkcję dodajemy tylko wtedy, gdy wszystkie powyższe zadania są ukończone i przetestowane. Jeżeli pomysł nie jest potrzebny do przejścia poziomu, zapisu wyniku albo prezentacji wymagań studiów, odkładamy go poza MVP.
