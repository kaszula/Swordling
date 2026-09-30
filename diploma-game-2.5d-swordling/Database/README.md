# Lokalna baza SQLite

Katalog `Database/` będzie zawierał wersjonowane skrypty SQL potrzebne do utworzenia, aktualizacji i demonstracji bazy projektu.

## Plik bazy użytkownika

Gra tworzy zapisywalny plik bazy pod ścieżką:

`Saved/Database/DiplomaGame.db`

Plik runtime nie należy do repozytorium i nie może znajdować się w `Content/`. Katalog `Saved/Database/` ma być tworzony automatycznie przez kod gry.

## Planowana kolejność skryptów

1. `001_create_tables.sql`
2. `002_create_constraints_indexes.sql`
3. `003_create_triggers.sql`
4. `004_create_views.sql`
5. `005_seed_data.sql`
6. `006_demo_queries.sql`

Skrypty nie istnieją jeszcze na etapie neutralizacji. Zostaną dodane podczas implementacji bazy.

## Odtworzenie pustej bazy

Docelowa procedura:

1. Zamknąć grę i Unreal Editor, aby zwolnić połączenie z plikiem.
2. Zachować kopię bazy, jeżeli jej dane są potrzebne.
3. Usunąć wyłącznie plik `Saved/Database/DiplomaGame.db`.
4. Uruchomić grę.
5. Subsystem bazy tworzy katalog, otwiera nowy plik i wykonuje migracje w kolejności wersji.
6. Zweryfikować `PRAGMA user_version` oraz obecność tabel i widoków.

Do czasu implementacji subsystemu powyższa procedura jest projektem zachowania, a nie działającą funkcją.

## Zapytania demonstracyjne

Po utworzeniu skryptu `006_demo_queries.sql` zapytania będzie można wykonać w narzędziu obsługującym SQLite, wskazując kopię pliku `DiplomaGame.db`.

Skrypt demonstracyjny ma pokazywać między innymi:

- relacje i klucze obce,
- ograniczenia oraz trigger,
- ranking oparty na `DENSE_RANK()`,
- statystyki z agregacjami i `GROUP BY`,
- CTE,
- funkcję okna `LAG()`,
- zachowanie transakcji i ponownego zapisu tego samego `client_run_id`.

Nie należy wykonywać demonstracyjnych operacji modyfikujących dane na jedynym egzemplarzu bazy zawierającym wyniki potrzebne do prezentacji.

## Pakowanie

Jeżeli gra będzie odczytywać skrypty SQL jako osobne pliki, muszą zostać jawnie dołączone do buildu jako pliki Non-UFS. Alternatywą jest kontrolowany system migracji przechowujący treść zapytań w kodzie C++.

Ostateczny sposób pakowania zostanie ustalony podczas integracji SQLite z Unreal Engine.
