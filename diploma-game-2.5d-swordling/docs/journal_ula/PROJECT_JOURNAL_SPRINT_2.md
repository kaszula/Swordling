DZIENNIK PROJEKTU – DYPLOMÓWKA 2.5D – UNREAL - SPRINT 2
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Data: 20 lipca 2026

1. Utworzenie komponentu zdrowia i obsługi obrażeń – DIP-32

Rozpoczęłam drugi sprint od realizacji zadania DIP-32 – Utworzenie komponentu zdrowia i obsługi obrażeń.

Utworzyłam nową klasę C++ UHealthComponent, dziedziczącą po UActorComponent. Dzięki temu komponent może być dodawany do różnych obiektów, między innymi do postaci gracza i przeciwników.

W komponencie dodałam:
- MaxHealth – maksymalną liczbę punktów zdrowia,
- CurrentHealth – aktualną liczbę punktów zdrowia,
- inicjalizację aktualnego zdrowia na wartość maksymalnego zdrowia przy rozpoczęciu gry,
- metodę ApplyDamage(), która odejmuje obrażenia i zabezpiecza zdrowie przed spadkiem poniżej zera,
- metodę IsDead(), która sprawdza, czy aktualne zdrowie wynosi zero,
- możliwość korzystania z pól i metod komponentu z poziomu Blueprintów.

Dodałam HealthComponent do BP_PlayerCharacter i przeprowadziłam test działania. Przy rozpoczęciu gry postać otrzymywała 30 punktów obrażeń. Początkowe zdrowie wynosiło 100 punktów, a po zadaniu obrażeń na ekranie została wyświetlona wartość 70, co potwierdziło poprawne działanie komponentu.

Po zakończeniu testu usunęłam testową logikę z Event BeginPlay, skompilowałam i zapisałam projekt.

Zmiany zostały zapisane na branchu:
feature/DIP-32-health-component

Commit:
DIP-32 Add reusable health component

Pull Request:
DIP-32 Create reusable health component

2. Konfiguracja akcji wejścia dla podstawowego ataku – DIP-33

W ramach zadania DIP-33 przygotowałam obsługę wejścia dla podstawowego ataku postaci gracza z wykorzystaniem systemu Enhanced Input.

Utworzyłam nową akcję wejścia:
IA_Attack

Dla akcji ustawiłam typ wartości:
Digital (Bool)

Następnie dodałam IA_Attack do istniejącego kontekstu mapowania:
IMC_Player

Jako przycisk odpowiedzialny za podstawowy atak przypisałam:
Left Mouse Button

W klasie PlayerCharacterBase dodałam:
- pole AttackAction typu UInputAction,
- metodę Attack(),
- podpięcie AttackAction w SetupPlayerInputComponent(),
- obsługę zdarzenia ETriggerEvent::Started.

Po naciśnięciu lewego przycisku myszy wywoływana jest metoda Attack(). Na tym etapie metoda nie wykonuje jeszcze właściwego ataku, tylko zapisuje w Output Log komunikat:
Attack input received

Dzięki temu sprawdziłam, że cały łańcuch wejścia działa poprawnie:
Left Mouse Button → IA_Attack → IMC_Player → AttackAction → Attack()

Przeprowadziłam test w Unreal Engine. Po kliknięciu lewego przycisku myszy komunikat pojawiał się w Output Log. Sprawdziłam również, że istniejące sterowanie ruchem na A/D oraz skok na Spacji nadal działają poprawnie.

Zmiany zostały zapisane na branchu:
feature/DIP-33-attack-input

Commit:
DIP-33 Add basic attack input

Pull Request:
DIP-33 Configure basic attack input

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Data: 21 lipca 2026

3. W ramach zadania DIP-34 zaimplementowałam podstawowy atak wręcz postaci gracza, wykorzystujący przygotowaną wcześniej akcję wejścia IA_Attack.

Rozszerzyłam klasę APlayerCharacterBase o parametry konfiguracyjne ataku:
- AttackRange – długość obszaru ataku,
- AttackHalfWidth – połowa szerokości obszaru,
- AttackHalfHeight – połowa wysokości obszaru,
- AttackDamage – liczba zadawanych obrażeń,
- bDrawAttackDebug – możliwość wyświetlania pomocniczego obszaru ataku.

W metodzie Attack() dodałam jednorazowe sprawdzenie kolizji za pomocą:
OverlapMultiByObjectType()

Obszar trafienia ma kształt prostopadłościanu i jest tworzony przed kapsułą postaci. Jego położenie uwzględnia promień kapsuły gracza, dzięki czemu obszar nie zaczyna się za postacią i nie obejmuje niepotrzebnie przestrzeni za jej plecami.

Kierunek ataku jest pobierany z aktualnej rotacji komponentu Mesh. Było to konieczne, ponieważ w projekcie podczas zmiany kierunku ruchu obracany jest wyłącznie model postaci, natomiast cały Actor i jego Capsule Component zachowują stałą rotację. Dzięki temu atak jest wykonywany zgodnie z kierunkiem, w którym zwrócony jest model.

Overlap wyszukuje obiekty korzystające z typów kolizji:
- Pawn,
- WorldDynamic.

Dodałam zabezpieczenie ignorujące postać gracza wykonującą atak.

Do przechowywania aktorów trafionych w ramach jednego ataku wykorzystałam:
TSet<AActor*> DamagedActors

Zapobiega to wielokrotnemu zadaniu obrażeń temu samemu aktorowi, jeżeli overlap wykryje kilka jego komponentów.

Jeżeli trafiony aktor posiada UHealthComponent, atak wywołuje bezpośrednio metodę:
HealthComponent->ApplyDamage(AttackDamage)

Dodałam również obsługę standardowego systemu obrażeń Unreal Engine za pomocą:
UGameplayStatics::ApplyDamage()

Pozwala to w przyszłości zadawać obrażenia aktorom korzystającym z metody TakeDamage(), nawet jeśli nie posiadają UHealthComponent.

W celu ułatwienia testowania dodałam wizualizację obszaru ataku za pomocą DrawDebugBox():
- zielony obszar oznacza brak wykrytego celu,
- czerwony obszar oznacza wykrycie aktora znajdującego się w zasięgu.

Podczas implementacji dodałam brakujący nagłówek:
Engine/OverlapResult.h

Jest on wymagany do korzystania z pełnej definicji struktury FOverlapResult.

W BP_PlayerCharacter skonfigurowałam wartości parametrów ataku i pozostawiłam włączone wyświetlanie debugowego obszaru trafienia.

Przeprowadziłam test w Unreal Engine. Potwierdziłam, że:
- atak uruchamia się po naciśnięciu lewego przycisku myszy,
- obszar trafienia pojawia się przed postacią,
- po zmianie kierunku ruchu obszar pojawia się po odpowiedniej stronie modelu,
- elementy mapy typu WorldStatic nie są traktowane jako cele ataku,
- ruch, skok i kamera nadal działają prawidłowo.

Test zadawania obrażeń właściwemu przeciwnikowi zostanie przeprowadzony po zaimplementowaniu przeciwników w kolejnym zadaniu.

Projekt został skompilowany w konfiguracji:
Development Editor | Win64

Kompilacja zakończyła się powodzeniem i nie wprowadziła nowych błędów.

Zmiany zostały zapisane na branchu:
feature/DIP-34-basic-melee-attack

Commit:
DIP-34 Add basic player melee attack

Pull Request: DIP-34 Implement basic player melee attack

4. Utworzenie bazowej klasy przeciwnika – DIP-35

W ramach zadania DIP-35 utworzyłam bazową klasę przeciwnika, która będzie stanowiła podstawę dla późniejszych typów wrogów.

Utworzyłam klasę C++:
AEnemyCharacterBase

Klasa dziedziczy po ACharacter. Dzięki temu posiada gotowe elementy potrzebne do dalszego rozwoju przeciwników:
- Capsule Component,
- Skeletal Mesh Component,
- Character Movement Component,
- możliwość późniejszego dodania AI, animacji oraz logiki ataku.

W klasie AEnemyCharacterBase dodałam UHealthComponent za pomocą CreateDefaultSubobject(). Komponent zdrowia jest tworzony automatycznie dla każdego przeciwnika dziedziczącego po tej klasie.

Dodałam również metodę:
GetHealthComponent()

Metoda została oznaczona jako BlueprintPure, dzięki czemu komponent zdrowia może być pobierany z poziomu Blueprintów bez wykonywania dodatkowej logiki.

Skonfigurowałam kapsułę przeciwnika:
- promień kapsuły: 42,
- połowa wysokości kapsuły: 96,
- profil kolizji: Pawn,
- Collision Enabled: Query and Physics,
- Generate Overlap Events: włączone.

Wyłączyłam kolizję komponentu Mesh. Za fizyczną kolizję i wykrywanie przeciwnika odpowiada wyłącznie Capsule Component. Zapobiega to wykrywaniu kilku komponentów tego samego przeciwnika podczas ataku.

Wyłączyłam wykonywanie funkcji Tick, ponieważ bazowy przeciwnik na tym etapie nie wykonuje logiki aktualizowanej co klatkę.

Wyłączyłam automatyczne sterowanie przeciwnikiem przez AI:
- Auto Possess AI ustawione na Disabled,
- AI Controller Class ustawione na nullptr.

Klasa nie zawiera jeszcze logiki AI, poruszania się ani ataku. Elementy te zostaną dodane w kolejnych zadaniach.

Na podstawie klasy AEnemyCharacterBase utworzyłam Blueprint:
BP_EnemyBase

Jako tymczasowy wygląd przeciwnika ustawiłam model:
SKM_Manny_Simple

Skonfigurowałam położenie modelu wewnątrz kapsuły:
- Location Z = -90,
- Rotation Z = -90.

Nie przypisywałam docelowego Animation Blueprintu. Przeciwnik pozostaje nieruchomy w podstawowej pozie, co jest wystarczające na tym etapie projektu.

Potwierdziłam, że BP_EnemyBase zawiera odziedziczone komponenty:
- Capsule Component,
- Mesh,
- Character Movement,
- Health Component.

Ustawiłam maksymalne zdrowie przeciwnika na 100 punktów.

Umieściłam instancję BP_EnemyBase na mapie testowej M00_DevelopmentMap. Ze względu na korzystanie przez mapę z systemu World Partition przeciwnik został zapisany jako osobny plik w folderze __ExternalActors__.

Przeprowadziłam test w Unreal Engine. Potwierdziłam, że:
- przeciwnik jest widoczny na mapie,
- przeciwnik pozostaje nieruchomy,
- przeciwnik nie posiada jeszcze AI ani logiki ataku,
- gracz nie może przejść przez kapsułę przeciwnika,
- kapsuła poprawnie blokuje postać gracza,
- przeciwnik jest wykrywany przez obszar podstawowego ataku gracza,
- debugowy obszar ataku zmienia kolor na czerwony po wykryciu przeciwnika,
- przeciwnik otrzymuje obrażenia przez HealthComponent.

Test potwierdził również poprawne działanie systemu obrażeń przygotowanego w poprzednim zadaniu dotyczącym podstawowego ataku wręcz.

Projekt został skompilowany w konfiguracji:
Development Editor | Win64

Kompilacja zakończyła się powodzeniem i nie wprowadziła nowych błędów.

Podczas pracy dostosowałam również boczną kamerę postaci. Zmieniłam jej tryb projekcji na Orthographic i ustawiłam Ortho Width na 750. Dzięki temu kamera pokazuje rozgrywkę całkowicie z boku, bez efektu perspektywy. Dostosowałam również położenie PlayerStart na mapie testowej.

Ujednoliciłam zasady formatu plików tekstowych w projekcie:
- UTF-8 bez BOM,
- zakończenia linii CRLF zgodne z .gitattributes,
- brak zmiany kodowania istniejących plików bez wyraźnej potrzeby lub zgody.

Usunęłam również znacznik BOM z pliku PlayerCharacterBase.cpp, zachowując kodowanie UTF-8 oraz zakończenia linii CRLF.

Zmiany zostały zapisane na branchu:
feature/DIP-35-enemy-base-class

Commity:
DIP-35 Add base enemy character
Adjust orthographic side camera and player start

Pull Request: DIP-35 Create base enemy character

5. Dodanie zdrowia, obrażeń i śmierci przeciwnika – DIP-36

W ramach zadania DIP-36 rozszerzyłam bazową klasę przeciwnika o pełną obsługę zdrowia, otrzymywania obrażeń oraz śmierci.

Przeciwnik posiadał już UHealthComponent utworzony w poprzednim zadaniu. W ramach DIP-36 rozszerzyłam działanie komponentu o możliwość informowania właściciela o osiągnięciu zerowego poziomu zdrowia.

W klasie UHealthComponent dodałam zdarzenie:
OnDeath

Zdarzenie wykorzystuje FSimpleMulticastDelegate i jest wywoływane, gdy aktualne zdrowie przeciwnika spadnie do zera.

Dodałam również metody:
- GetCurrentHealth() – zwraca aktualną wartość zdrowia,
- GetMaxHealth() – zwraca maksymalną wartość zdrowia.

Aktualne zdrowie jest inicjalizowane w BeginPlay() wartością MaxHealth. Dla bazowego przeciwnika maksymalne zdrowie wynosi 100 punktów.

Metoda ApplyDamage() została rozszerzona o:
- odrzucanie obrażeń o wartości mniejszej lub równej zero,
- zabezpieczenie przed zadawaniem kolejnych obrażeń martwemu obiektowi,
- odejmowanie obrażeń od aktualnego zdrowia,
- ograniczenie zdrowia do zakresu od 0 do MaxHealth za pomocą FMath::Clamp(),
- wyświetlanie informacji o otrzymanych obrażeniach w Output Log,
- uruchamianie zdarzenia OnDeath po osiągnięciu zerowego zdrowia.

Po każdym trafieniu w Output Log wyświetlany jest komunikat zawierający:
- nazwę trafionego aktora,
- wartość otrzymanych obrażeń,
- aktualną wartość zdrowia,
- maksymalną wartość zdrowia.

Przykładowy komunikat:
BP_EnemyBase received 25.0 damage. Health: 75.0 / 100.0

W klasie AEnemyCharacterBase nadpisałam metodę BeginPlay(). Przeciwnik podłącza w niej metodę HandleDeath() do zdarzenia OnDeath komponentu zdrowia.

Dodałam pole:
bIsDead

Pole przechowuje informację, czy przeciwnik został już uznany za martwego, i zabezpiecza przed wielokrotnym wykonaniem logiki śmierci.

W metodzie HandleDeath() dodałam:
- ustawienie bIsDead na true,
- wyświetlenie komunikatu o śmierci w Output Log,
- wyłączenie kolizji całego aktora,
- wyłączenie kolizji Capsule Component,
- wyłączenie Character Movement,
- usunięcie przeciwnika z rozgrywki za pomocą Destroy().

Dzięki wyłączeniu kolizji i usunięciu aktora martwy przeciwnik nie blokuje dalszego poruszania się gracza.

Na tym etapie nie dodawałam:
- animacji otrzymania obrażeń,
- animacji śmierci,
- ragdolla,
- odrzucania przeciwnika,
- efektów wizualnych,
- dźwięków,
- systemu lootu.

Przeprowadziłam test w Unreal Engine. Gracz zadawał przeciwnikowi po 25 punktów obrażeń za pomocą podstawowego ataku.

W Output Log pojawiały się kolejno wartości:
- 75 / 100 punktów zdrowia,
- 50 / 100 punktów zdrowia,
- 25 / 100 punktów zdrowia,
- 0 / 100 punktów zdrowia.

Po czwartym trafieniu:
- zostało uruchomione zdarzenie OnDeath,
- w logu pojawił się komunikat o śmierci,
- kolizja przeciwnika została wyłączona,
- przeciwnik zniknął z rozgrywki,
- przeciwnik przestał blokować gracza,
- nie było możliwe zadawanie mu kolejnych obrażeń.

Test potwierdził, że zdrowie przeciwnika nie spada poniżej zera.

Projekt został skompilowany w konfiguracji:
Development Editor | Win64

Kompilacja zakończyła się powodzeniem. Projekt uruchamia się bez nowych błędów.

Podczas pracy sprawdziłam również format zmodyfikowanych plików tekstowych. Pliki zostały zapisane jako:
- UTF-8 bez BOM,
- CRLF.

Doprecyzowałam kontrolę, aby zmodyfikowane pliki tekstowe korzystały z jednego spójnego formatu zakończeń linii i nie zawierały jednocześnie LF oraz CRLF.

Zmiany zostały zapisane na branchu:
feature/DIP-36-enemy-health-death

Commity:
DIP-36 Add enemy health damage and death handling
chore: require consistent line endings in modified files

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Data: 22 lipca 2026

6. Organizacja pracy i rozpoczęcie Sprintu 3

Sprawdziłam zmiany przygotowane przez drugiego członka zespołu na branchu DIP47-Base_camera. Wskazałam możliwe potencjalne błedy.
Ze względu na ich zakres oraz brak potrzeby wykorzystania ich w najbliższych zadaniach ustaliliśmy, że branch nie będzie na razie 
scalany z main i wrócimy do niego w późniejszym etapie projektu.

Przygotowałam i rozpoczęłam Sprint 3. Utworzyłam oraz opisałam zadania w Jira, wskazałam ich zależności, kolejność realizacji i podział odpowiedzialności w zespole.

Sprint 3 został oficjalnie rozpoczęty, a zadania są gotowe do realizacji.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Data: 23 lipca 2026

7. Implementacja podstawowej logiki przeciwnika

Zrealizowałam trzy zadania dotyczące podstawowej logiki przeciwnika:

- DIP-39 – wykrywanie gracza w konfigurowalnym zasięgu oraz rozpoznawanie wejścia i wyjścia gracza z zasięgu,
- DIP-40 – poruszanie przeciwnika w kierunku wykrytego gracza, obracanie modelu, zachowanie płaszczyzny 2.5D oraz zatrzymywanie w zasięgu ataku,
- DIP-41 – podstawowy atak przeciwnika z konfigurowalnymi obrażeniami, zasięgiem i czasem odnowienia.

Ze względu na brak gotowych animacji atak został tymczasowo przedstawiony za pomocą sfery debugowej. Obrażenia są przekazywane do komponentu zdrowia gracza. Martwy przeciwnik nie może wykonywać ataku.

Mechanizmy zostały sprawdzone przez kompilację oraz testy w Unreal Editor. Dodatkowo wykonano test uruchomieniowy potwierdzający wykrywanie gracza, zadawanie obrażeń i blokadę ataku po śmierci przeciwnika.

Zmiany zostały przygotowane na branchach:

feature/DIP-39-player-detection
feature/DIP-40-enemy-chase
feature/DIP-41-enemy-basic-attack

Commity:

implement enemy player detection
implement enemy chase movement
implement basic enemy attack

Zmodyfikowane pliki tekstowe zostały zapisane jako UTF-8 bez BOM z zakończeniami linii CRLF.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
