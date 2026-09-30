# Świeży plan MVP gry

## 1. Cel projektu

Stworzyć małą, kompletną grę platformowo-zręcznościową 2.5D, którą można szybko ukończyć i zaprezentować jako projekt na studia podyplomowe. Gra ma być prosta w produkcji, ale powinna pokazywać połączenie mechanik Unreal Engine, interfejsu, sztucznej inteligencji i relacyjnej bazy SQLite.

## 2. Główne założenie gry

Gracz przechodzi jeden krótki poziom na czas. Najpierw pokonuje prostą trasę parkour z trzema checkpointami, a następnie walczy z jednym bossem, którego pokonanie otwiera drogę do mety. Śmierć kończy próbę porażką, a ukończenie poziomu zapisuje wynik i pozwala porównać go z rekordem osobistym oraz rankingiem Top 5.

## 3. Przebieg pojedynczej próby

1. Gracz wpisuje pseudonim i rozpoczyna grę.
2. Wejście do strefy startowej uruchamia timer.
3. Gracz pokonuje prostą sekcję parkour i zalicza po drodze trzy checkpointy.
4. Po zaliczeniu checkpointów gracz dociera do krótkiej areny z jednym bossem.
5. Pokonanie bossa otwiera wyjście z areny i drogę do mety.
6. Dotarcie do mety zatrzymuje timer i kończy próbę.
7. Ekran wyniku pokazuje czas, podstawowe statystyki, rekord osobisty i Top 5.

## 4. Mechaniki potrzebne w MVP

- ruch w lewo i prawo w układzie 2.5D,
- skok,
- jeden podstawowy atak,
- zdrowie, otrzymywanie obrażeń i śmierć,
- jeden prosty boss z wykrywaniem gracza, pościgiem i atakiem,
- timer próby,
- obowiązkowa walka z bossem po sekcji parkour,
- trzy checkpointy,
- meta i zakończenie próby,
- pseudonim gracza,
- HUD ze zdrowiem, czasem i checkpointami,
- ekran sukcesu i porażki,
- lokalny zapis wyników w SQLite,
- rekord osobisty i ranking Top 5.

## 5. Mapa

MVP zawiera jedną liniową mapę możliwą do ukończenia w około 3–5 minut przy pierwszym poprawnym przejściu. Jej kolejność to: bezpieczny start, uruchomienie timera, checkpoint 1, prosta sekcja skoków, checkpoint 2, krótka dalsza trasa parkour, checkpoint 3, arena bossa, otwierane wyjście i meta.

Mapa nie będzie zawierać skomplikowanych zagadek, ruchomych platform, rozbudowanej nawigacji, wielu rozgałęzień ani dodatkowych przeciwników. Jedynym przeciwnikiem w MVP jest boss znajdujący się na końcu trasy parkour.

## 6. Wygląd gry

Gra powinna mieć prosty, spójny styl stylizowany lub low-poly. Kamera pozostaje boczna, ważne elementy trasy muszą być łatwe do zauważenia, a gracz, boss, przeszkody, checkpointy i meta powinny wyraźnie odróżniać się od tła.

Do środowiska wybieramy jedną małą, spójną paczkę assetów. Modele, animacje, materiały i tekstury mogą pochodzić z darmowych lub posiadanych legalnie źródeł, ale dla każdego zasobu zapisujemy autora, link, licencję i sposób wykorzystania. Nie tworzymy własnych modeli ani rozbudowanych efektów, jeżeli gotowe zasoby wystarczą do czytelnego przedstawienia gry.

## 7. Dane zapisywane w SQLite

- pseudonim gracza,
- wynik próby: ukończona albo nieudana,
- czas przejścia,
- liczba zaliczonych checkpointów,
- zdarzenie pokonania bossa,
- suma otrzymanych obrażeń,
- podstawowe zdarzenia z przebiegu próby,
- rekord osobisty,
- dane potrzebne do rankingu Top 5.

## 8. Co pokazuje wartość projektu

- połączenie C++ i Blueprintów w Unreal Engine,
- kompletny przepływ od rozpoczęcia próby do wyniku,
- proste AI bossa,
- komunikację mechanik z interfejsem,
- projekt relacyjnej bazy danych,
- integrację Unreal Engine z SQLite,
- zapis prób, statystyki i ranking,
- testy, dokumentację oraz działający build Windows.

## 9. Elementy poza MVP

- wiele poziomów,
- fabuła, dialogi i cutscenki,
- ekwipunek, przedmioty i crafting,
- rozwój postaci i drzewko umiejętności,
- dodatkowi przeciwnicy i kolejni bossowie,
- tryb wieloosobowy,
- konta internetowe i ranking online,
- zaawansowane efekty, proceduralne poziomy i rozbudowane menu ustawień.

## 10. Warunek ukończenia MVP

MVP jest gotowe, gdy gracz może uruchomić build Windows, wpisać pseudonim, przejść sekcję parkour, pokonać bossa i dotrzeć do mety albo zginąć, zobaczyć podsumowanie, zapisać próbę do lokalnej bazy i odczytać rekord osobisty oraz Top 5. Wszystkie użyte zewnętrzne materiały mają udokumentowane źródła i licencje.
