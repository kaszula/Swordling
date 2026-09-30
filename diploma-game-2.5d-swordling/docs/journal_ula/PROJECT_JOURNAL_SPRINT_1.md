DZIENNIK PROJEKTU – DYPLOMÓWKA 2.5D – UNREAL - SPRINT 1
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Data: 13 lipca 2026

1. Zalogowałam się na swoje istniejące konto GitHub.

2. Utworzyłam nowe prywatne repozytorium:
diploma-game-2.5d

3. Podczas tworzenia repozytorium dodałam:
- README.md
- .gitignore dla Unreal Engine

4. Sklonowałam repozytorium na komputer do folderu:
D:\Projects\diploma-game-2.5d

5. Sprawdziłam poprawność połączenia lokalnego repozytorium z GitHubem.

6. Zweryfikowałam instalację:
- Git 2.51.2
- Git LFS 3.7.1

7. Skonfigurowałam Git LFS dla plików Unreal Engine:
- *.uasset
- *.umap

8. Utworzyłam plik:
.gitattributes

9. Wykonałam commit:
Configure Git LFS for Unreal Engine assets

10. Wysłałam zmiany do gałęzi main na GitHubie.

11. Sprawdziłam zawartość pliku .gitignore i potwierdziłam, że ignoruje on foldery oraz pliki generowane lokalnie przez Unreal Engine i Visual Studio, między innymi:
- Binaries
- Intermediate
- Saved
- DerivedDataCache
- .vs
- pliki .sln

12. Zaktualizowałam Epic Games Launcher i zalogowałam się na swoje konto.

13. Zainstalowałam Unreal Engine 5.7.4.

14. Zainstalowałam Visual Studio Community 2022 w wersji 17.14.35.

15. W Visual Studio 2022 zainstalowałam zestaw:
Projektowanie gier przy użyciu języka C++

16. Zainstalowałam wymagane komponenty do pracy z Unreal Engine, między innymi:
- MSVC v143
- Windows SDK
- Visual Studio Tools for Unreal Engine
- narzędzia debugowania Unreal Engine
- adapter testowy Unreal Engine
- narzędzia profilowania C++
- C++ AddressSanitizer
- narzędzia HLSL

17. Utworzyłam testowy projekt Unreal Engine:
- szablon: Third Person
- typ projektu: C++
- platforma: Desktop
- jakość: Maximum
- nazwa: DiplomaGameTest

18. Sprawdziłam poprawność działania środowiska:
- projekt został utworzony,
- kod został skompilowany,
- projekt uruchomił się w Unreal Engine,
- tryb Play działał,
- postać mogła się poruszać.

19. Ustawiłam Visual Studio 2022 jako edytor kodu źródłowego dla Unreal Engine.

20. Utworzyłam właściwy projekt:
- nazwa projektu: DiplomaGame
- szablon: Third Person
- typ projektu: C++
- platforma: Desktop
- jakość: Maximum

21. Sprawdziłam działanie właściwego projektu w trybie Play.

22. Skopiowałam do repozytorium właściwe elementy projektu:
- DiplomaGame.uproject
- Config
- Content
- Source

23. Nie dodałam do repozytorium folderów generowanych lokalnie:
- Binaries
- Intermediate
- Saved
- DerivedDataCache
- .vs

24. Dodałam pliki projektu do Git.

25. Wykonałam commit:
Add initial Unreal Engine project

26. Wysłałam projekt do repozytorium GitHub.

27. Git LFS poprawnie przesłał 740 plików Unreal Engine o łącznym rozmiarze około 141 MB.

28. Uruchomiłam projekt bezpośrednio z folderu repozytorium:
D:\Projects\diploma-game-2.5d\DiplomaGame.uproject

29. Unreal Engine przebudował lokalny moduł DiplomaGame, ponieważ folder Binaries nie jest przechowywany w repozytorium.

30. Sprawdziłam, że projekt z repozytorium poprawnie się uruchamia i działa w trybie Play.

31. Usunęłam zbędne kopie projektów

32. Zaprosiłam drugiego członka zespołu do prywatnego repozytorium GitHub.

33. Utworzyłam w repozytorium folder:
docs

34. Dodałam pliki dokumentacyjne:
- docs/PROJECT_JOURNAL.md
- docs/DEVELOPER_SETUP.md

35. Przygotowałam instrukcję instalacji środowiska, klonowania repozytorium i uruchomienia projektu dla drugiego członka zespołu.

Aktualny status:

Do repozytorium została dodana dokumentacja projektu oraz instrukcja dla drugiego członka zespołu.

Następne planowane działania:

- zsynchronizowanie lokalnego repozytorium po zmianach wykonanych na stronie GitHub,
- przygotowanie zasad pracy na branchach,
- utworzenie projektu Jira,
- przygotowanie wstępnego backlogu,
- sprawdzenie klonowania i uruchomienia projektu na komputerze drugiego członka zespołu.
-----------------------------------------------------------------------------------------------------------------------------------

Data: 14 lipca 2026

36. Drugi członek zespołu zainstalował wymagane środowisko i sklonował repozytorium.

37. Projekt został poprawnie przebudowany i uruchomiony na drugim komputerze.

38. Potwierdzono, że Unreal Engine współpracuje z Visual Studio 2022 na drugim stanowisku.

39. Drugi członek zespołu nie wprowadził jeszcze żadnych zmian do repozytorium.

40. Rozpoczęłam test przepływu pracy z wykorzystaniem osobnej gałęzi Git.

41. Utworzyłam testową gałąź:
docs/test-branch-workflow

42. Wykonałam zmianę w dokumentacji, utworzyłam commit i wysłałam gałąź do repozytorium GitHub.

43. Utworzyłam pierwszy Pull Request do gałęzi main.

44. Sprawdziłam zmienione pliki i poprawnie scaliłam Pull Request z gałęzią main.

45. Usunęłam niepotrzebną gałąź zdalną i lokalną oraz zsynchronizowałam lokalne repozytorium.

49. Utworzyłam gałąź:
docs/add-git-workflow

50. Utworzyłam instrukcję pracy zespołu z systemem Git:
docs/GIT_WORKFLOW.md

51. Rozpoczęłam test integracji Jiry z GitHubem dla zadania DIP-1.

52. Utworzyłam projekt Scrum w Jira o nazwie:
DiplomaGame

53. Ustawiłam klucz projektu Jira:
DIP

54. Dodałam drugiego członka zespołu do przestrzeni projektu w Jira.

55. Zainstalowałam oficjalną aplikację:
GitHub for Atlassian

56. Połączyłam konto GitHub z kontem Atlassian.

57. Ograniczyłam dostęp integracji tylko do repozytorium:
kaszula/diploma-game-2.5d

58. Sprawdziłam, że integracja GitHub z Jira działa w zakładce:
Programowanie

59. Utworzyłam w Jira zadanie:
DIP-1 – Test GitHub integration

60. Utworzyłam gałąź powiązaną z zadaniem Jira:
docs/DIP-1-test-github-integration

61. Wykonałam zmianę w dokumentacji i utworzyłam commit zawierający klucz zadania:
DIP-1 Test Jira and GitHub integration

62. Wysłałam gałąź do GitHuba i utworzyłam Pull Request z kluczem DIP-1 w tytule.

63. Sprawdziłam, że Jira automatycznie wykryła powiązany Pull Request i repozytorium GitHub.

64. Scaliłam Pull Request do gałęzi main.

65. Usunęłam niepotrzebną gałąź zdalną i lokalną oraz zsynchronizowałam lokalne repozytorium.

66. Oznaczyłam zadanie DIP-1 w Jira jako wykonane.

67. Utworzyłam pliki `DEVELOPMENT_WORKFLOW.md` oraz `GIT_WORKFLOW.md`.

68. W pliku `DEVELOPMENT_WORKFLOW.md` opisałam cały proces pracy zespołu z wykorzystaniem Jiry, Unreal Engine, Visual Studio i GitHuba.

69. W pliku `GIT_WORKFLOW.md` opisałam podstawowe komendy Git oraz sposób pracy na osobnych gałęziach.

70. Ujednoliciłam zasady nazewnictwa gałęzi i commitów z wykorzystaniem klucza zadania Jira.

71. Dodałam zasady dotyczące Pull Requestów, synchronizacji repozytorium i usuwania niepotrzebnych gałęzi.

72. Opisałam sposób pracy z plikami binarnymi Unreal Engine, takimi jak `.uasset` i `.umap`.
  
73. Przeanalizowałam prezentację oraz założenia gry przekazane przez Tomasza.

74. Utworzyłam plik `GAME_SPECIFICATION.md` ze wstępną specyfikacją wersji demonstracyjnej gry.

75. Opisałam w nim główną pętlę rozgrywki, poziomy, ruch, walkę, magię, przeciwników, bossa, zapis, UI i wymagania techniczne.

76. Wskazałam elementy poza podstawowym zakresem dema oraz decyzje wymagające uzupełnienia przez Tomasza.

77. Utworzyłam plik `UNREAL_PROJECT_STANDARDS.md`.

78. Określiłam strukturę folderów projektu oraz zasady nazewnictwa assetów, Blueprintów, klas i plików C++ oraz map.

79. Dodałam standardy organizacji kodu, map testowych, assetów roboczych i listę kontrolną przed commitem.

-----------------------------------------------------------------------------------------------------------------------------------

Data: 15 lipca 2026

80. Sprawdziłam pracę Tomasza w GitHubie i Jirze. Tomasz utworzył branch `zadanie/DIP-2-reorganizacja-folderu-art`, wykonał Pull Request #5, zmergował zmiany do `main` oraz oznaczył zadanie `DIP-2` jako gotowe.

81. Otrzymałam od Tomasza potwierdzenie, że aktualna wersja dokumentu `GAME_SPECIFICATION.md` jest poprawna i może stanowić podstawę do przygotowania backlogu projektu.

82. Zdecydowałam, że ze względu na krótki termin realizacji projektu praca będzie organizowana w tygodniowych sprintach. Celem jest ukończenie projektu najpóźniej do końca sierpnia 2026.

83. Utworzyłam w Jirze siedem sprintów: `DIP Sprint 1`–`DIP Sprint 7`. Kolejne sprinty będą obejmować tygodniowe etapy pracy, a ostatni sprint zostanie przeznaczony głównie na testy, poprawki i przygotowanie końcowego buildu.

84. Ustaliłam, że pierwsza wersja gry będzie rozwijana jako MVP z wykorzystaniem placeholderów. Brak docelowego modelu Beatrix nie będzie blokował implementacji podstawowych mechanik ani przygotowania map testowych.

85. Utworzyłam w Jirze dziesięć epików grupujących główne obszary projektu:

* Konfiguracja projektu i fundament techniczny,
* Postać gracza i ruch,
* System walki i obrażeń,
* Magia i zasoby,
* Przeciwnicy i AI,
* Poziomy i eksploracja,
* Zapis, checkpointy i odradzanie,
* UI, menu i HUD,
* Boss i zakończenie dema,
* Testy, optymalizacja i build.

86. Określiłam zakres pierwszego sprintu. Celem Sprintu 1 jest przygotowanie podstawowego grywalnego fundamentu 2.5D, umożliwiającego uruchomienie mapy, sterowanie placeholderem postaci, poruszanie się, skakanie oraz korzystanie z kamery bocznej.

87. Utworzyłam w `DIP Sprint 1` następujące zadania:

* `DIP-19` – Przygotowanie mapy testowej 2.5D z placeholderami,
* `DIP-20` – Utworzenie bazowej klasy postaci gracza,
* `DIP-23` – Konfiguracja systemu sterowania Enhanced Input,
* `DIP-24` – Implementacja ruchu lewo–prawo i ograniczenia do płaszczyzny 2.5D,
* `DIP-25` – Implementacja podstawowego skoku postaci,
* `DIP-26` – Konfiguracja kamery bocznej dla rozgrywki 2.5D,
* `DIP-27` – Integracja postaci z mapą testową i test podstawowej rozgrywki.

88. Uzupełniłam zadania Sprintu 1 o opisy i kryteria akceptacji, określające oczekiwane rezultaty oraz sposób sprawdzenia poprawności implementacji.

89. Zaplanowałam równoległy podział pracy. Tomasz będzie mógł przygotowywać mapę testową i placeholdery środowiska, podczas gdy ja zajmę się bazową klasą postaci, sterowaniem, ruchem, skokiem i kamerą.

90. Ustaliłam, że każda osoba będzie pracować na osobnym branchu odpowiadającym zadaniu w Jirze. Ze względu na binarny format plików Unreal Engine ustaliłam również, że jedna osoba naraz powinna edytować konkretny plik `.uasset` lub `.umap`, aby uniknąć konfliktów niemożliwych do automatycznego scalenia.

91. Nie uruchomiłam jeszcze Sprintu 1. Przed jego rozpoczęciem pozostaje ostateczne przypisanie właścicieli zadań, ewentualne dodanie Story Pointów, ustawienie dat oraz przekazanie Tomaszowi informacji o przygotowanym podziale pracy.

-----------------------------------------------------------------------------------------------------------------------------------
Data: 16 lipca 2026

92. Uruchomiłam i zweryfikowałam środowisko Unreal Engine 5.7 oraz Visual Studio 2022.

93. Sprawdziłam obsługę repozytorium Git bezpośrednio z Visual Studio.

94. Utworzyłam branch `feature/DIP-20-player-character-base`.

95. Przeanalizowałam istniejącą klasę `ADiplomaGameCharacter` pochodzącą z szablonu Unreal Engine.

96. Zdecydowałam o pozostawieniu klasy szablonowej bez zmian, ponieważ zawierała już logikę ruchu, kamery, skoku i Enhanced Input, należącą do kolejnych zadań.

97. Utworzyłam folder kodu `Source/DiplomaGame/Characters`.

98. Utworzyłam klasę C++ `APlayerCharacterBase`, dziedziczącą po `ACharacter`.

99. Ograniczyłam klasę do podstawowego szkieletu bez ruchu, inputu, kamery i własnej logiki skoku.

100. Wyłączyłam niepotrzebne wykonywanie funkcji `Tick`.

101. Utworzyłam folder assetów `Content/Characters/Player`.

102. Utworzyłam Blueprint `BP_PlayerCharacter`, dziedziczący po `APlayerCharacterBase`.

103. Zweryfikowałam odziedziczone komponenty postaci: `Capsule Component`, `Mesh` oraz `Character Movement`.

104. Sprawdziłam możliwość umieszczenia `BP_PlayerCharacter` na mapie oraz uruchomiłam Play In Editor bez błędów.

105. Wykonałam pełny build projektu `DiplomaGame`, zakończony sukcesem.

106. Ustaliłam, że błędy wyświetlane przez IntelliSense nie były błędami kompilacji, ponieważ projekt budował się poprawnie.

107. Podjęłam próbę konfiguracji pluginu `VisualStudioTools`, która spowodowała błąd modułu blokujący kompilację.

108. Cofnęłam konfigurację `VisualStudioTools`, usunęłam plugin z instalacji silnika i ponownie potwierdziłam poprawny build projektu.

109. Ustaliłam, że należy budować bezpośrednio projekt `DiplomaGame`, zamiast używać `Build Solution`, które próbowało budować także dodatkowe projekty silnika i testów.

110. Tymczasowo umieściłam postać na mapie, co utworzyło plik w `ExternalActors`; usunęłam ten plik przed commitem, aby nie modyfikować mapy szablonowej.

111. Wykonałam commit `DIP-20: add base player character class` i wypchnęłam branch do zdalnego repozytorium.

112. Utworzyłam Pull Request dla DIP-20, zweryfikowałam cztery zmienione pliki i zmergowałam zmiany do `main`.

113. Usunęłam branch `feature/DIP-20-player-character-base` lokalnie i zdalnie.

114. Zakończyłam zadanie DIP-20. Powstała minimalna baza postaci gracza przygotowana do dalszej implementacji Enhanced Input, ruchu, skoku i kamery bocznej.
-----------------------------------------------------------------------------------------------------------------------------------

Data: 17 lipca 2026

115. Utworzyłam folder `Content/DiplomaGame/Input` przeznaczony na assety związane ze sterowaniem gracza.

116. Utworzyłam asset `IA_Move` typu `Input Action`.

117. Ustawiłam dla `IA_Move` typ wartości `Axis2D (Vector2D)`.

118. Utworzyłam asset `IA_Jump` typu `Input Action`.

119. Pozostawiłam dla `IA_Jump` typ wartości `Digital (Bool)`.

120. Utworzyłam asset `IMC_Player` typu `Input Mapping Context`.

121. W `IMC_Player` przypisałam akcję `IA_Jump` do klawisza `Space Bar`.

122. W `IMC_Player` przypisałam akcję `IA_Move` do klawisza `D`.

123. Dodałam drugie mapowanie `IA_Move` dla klawisza `A`.

124. Dla mapowania klawisza `A` dodałam modyfikator `Negate`, aby zwracał ujemną wartość osi ruchu.

125. W modyfikatorze `Negate` pozostawiłam zaznaczoną wyłącznie oś `X`.

126. Sprawdziłam plik `DiplomaGame.Build.cs` i potwierdziłam, że moduł `EnhancedInput` znajduje się już na liście `PublicDependencyModuleNames`.

127. Rozszerzyłam klasę `APlayerCharacterBase` o obsługę Enhanced Input.

128. Dodałam do `PlayerCharacterBase.h` deklaracje typów `UInputAction`, `UInputMappingContext` oraz `FInputActionValue`.

129. Nadpisałam metodę `BeginPlay()` w klasie `APlayerCharacterBase`.

130. Nadpisałam metodę `SetupPlayerInputComponent()` odpowiedzialną za podłączanie akcji sterowania.

131. Dodałam metodę `Move(const FInputActionValue& Value)` obsługującą ruch gracza.

132. Dodałam do klasy pola `DefaultMappingContext`, `MoveAction` i `JumpAction`, dostępne do konfiguracji w Blueprintach.

133. W `BeginPlay()` dodałam aktywowanie domyślnego `Input Mapping Context` w `UEnhancedInputLocalPlayerSubsystem`.

134. W `SetupPlayerInputComponent()` podłączyłam `MoveAction` do metody `Move()`.

135. Podłączyłam `JumpAction` do wbudowanych metod `Jump()` oraz `StopJumping()` klasy `ACharacter`.

136. Zaimplementowałam ruch gracza za pomocą `AddMovementInput()`.

137. Skompilowałam projekt w konfiguracji `Development Editor | Win64`. Kompilacja zakończyła się powodzeniem.

138. W `BP_PlayerCharacter` przypisałam:
    - `Default Mapping Context` → `IMC_Player`,
    - `Move Action` → `IA_Move`,
    - `Jump Action` → `IA_Jump`.

139. Przeniosłam `BP_PlayerCharacter` z folderu `Content/Characters/Player` do folderu `Content/DiplomaGame/Characters`.

140. Po przeniesieniu assetu wykonałam `Update Redirector References` i usunęłam pusty folder `Content/Characters/Player`.

141. Ustaliłam zasadę organizacji projektu: wszystkie assety tworzone na potrzeby gry będą umieszczane w `Content/DiplomaGame`, natomiast foldery template’u Unreal pozostaną tymczasowo na głównym poziomie `Content`.

142. Ustawiłam mapę testową zespołu jako `Editor Startup Map` oraz `Game Default Map`.

143. Utworzyłam folder `Content/DiplomaGame/GameModes`.

144. Utworzyłam Blueprint `BP_DiplomaGameMode` dziedziczący po `GameModeBase`.

145. W `BP_DiplomaGameMode` ustawiłam `Default Pawn Class` na `BP_PlayerCharacter`.

146. Ustawiłam `BP_DiplomaGameMode` jako domyślny GameMode projektu.

147. Sprawdziłam ustawienia mapy testowej i wykryłam, że posiadała ona własny `GameMode Override` wskazujący na `BP_ThirdPersonGameMode`.

148. Zmieniłam `GameMode Override` mapy testowej na `BP_DiplomaGameMode`.

149. Pozostawiłam istniejący obiekt `PlayerStart`, ponieważ odpowiada on jedynie za miejsce tworzenia postaci.

150. Otworzyłam pełny edytor `BP_PlayerCharacter` i dodałam komponent `Spring Arm` nazwany `CameraBoom`.

151. Dodałam komponent `Camera` nazwany `FollowCamera` jako dziecko `CameraBoom`.

152. Ustawiłam tymczasową długość ramienia kamery `Target Arm Length` na `500`.

153. Przypisałam do komponentu `Mesh` tymczasowy model `SKM_Quinn_Simple`.

154. Ustawiłam pozycję mesha na `Location Z = -90`.

155. Ustawiłam obrót mesha na `Rotation Z = -90°`.

156. Skompilowałam i zapisałam `BP_PlayerCharacter`.

157. Uruchomiłam mapę testową i potwierdziłam, że gra tworzy nowy `BP_PlayerCharacter` w miejscu `PlayerStart`.

158. Potwierdziłam działanie sterowania:
    - klawisz `D` uruchamia ruch w jednym kierunku,
    - klawisz `A` uruchamia ruch w przeciwnym kierunku,
    - klawisz `Space Bar` uruchamia skok.

159. Potwierdziłam, że system Enhanced Input działa prawidłowo od strony technicznej.

160. Kierunek ruchu względem docelowego widoku 2.5D oraz blokowanie i właściwe ustawienie kamery pozostawiłam do realizacji w osobnych zadaniach.

161. Ustaliłam, że przed każdą edycją współdzielonych plików binarnych Unreal, szczególnie `.umap` i `.uasset`, należy poinformować drugą osobę w zespole o rozpoczęciu pracy nad plikiem, a po zakończeniu przekazać informację, że plik jest ponownie dostępny.

-----------------------------------------------------------------------------------------------------------------------------------

Data: 20 lipca 2026

162. Pobrałam aktualną wersję brancha main i utworzyłam branch:
feature/DIP-24-movement-plane

163. Rozpoczęłam realizację zadania DIP-24 dotyczącego ruchu postaci lewo–prawo oraz ograniczenia jej do płaszczyzny 2.5D.

164. W klasie APlayerCharacterBase skonfigurowałam CharacterMovementComponent:
- włączyłam bConstrainToPlane,
- jako blokowaną oś ustawiłam Y,
- włączyłam bSnapToPlaneAtStart.

165. Podczas pierwszego testu stwierdziłam konflikt pomiędzy bOrientRotationToMovement = true a ruchem wykorzystującym GetActorForwardVector(). Po obróceniu całego aktora zmieniał się również kierunek ruchu, a kamera obracała się razem z postacią.

166. Analiza projektu wykazała, że:
- mapa testowa jest ułożona wzdłuż światowej osi X,
- oś Y powinna pozostać zablokowana,
- PlayerStart posiada obrót Yaw = 180°,
- ruch nie powinien zależeć od aktualnej rotacji aktora.

167. Zmieniłam ruch postaci na ruch po stałej światowej osi X:
AddMovementInput(FVector::ForwardVector, MovementValue.X);

168. Wyłączyłam automatyczne obracanie całego aktora:
MovementComponent->bOrientRotationToMovement = false;
bUseControllerRotationYaw = false;

169. Dodałam metodę UpdateMeshFacing(), która obraca wyłącznie komponent Mesh, bez obracania CapsuleComponent, aktora i komponentów kamery.

170. Zapisałam początkowe przesunięcie rotacji modelu w BeginPlay(), dzięki czemu zachowałam rotację ustawioną wcześniej w BP_PlayerCharacter.

171. Przetestowałam ruch w obu kierunkach. Potwierdziłam:
- poprawne poruszanie się po osi X,
- brak możliwości zejścia z płaszczyzny XZ,
- poprawne obracanie modelu,
- stabilne działanie kamery,
- dalsze działanie skoku.

172. Naprawiłam brak dyrektywy:
#pragma once
w pliku PlayerCharacterBase.h, który powodował błąd Unreal Header Tool dotyczący wielokrotnego dołączenia PlayerCharacterBase.generated.h.

173. Poprawnie skompilowałam projekt po zamknięciu Unreal Editora, który wcześniej blokował plik DLL podczas linkowania.

174. Zakończyłam zadanie DIP-24, wykonałam commit, push, pull request i merge do brancha main.

175. Utworzyłam branch dla zadania DIP-25:
feature/DIP-25-basic-jump

176. Zweryfikowałam istniejącą implementację skoku opartą na Enhanced Input:
- IA_Jump jest przypisane do klawisza Space,
- zdarzenie Started wywołuje ACharacter::Jump,
- zdarzenie Completed wywołuje ACharacter::StopJumping.

177. Przetestowałam skok na mapie testowej. Potwierdziłam:
- pojedynczy skok po naciśnięciu Space,
- brak możliwości wykonania kolejnego skoku w powietrzu,
- poprawne działanie grawitacji,
- poprawne lądowanie na podłożu,
- poprawne lądowanie na platformach posiadających kolizje.

178. Sprawdziłam możliwość konfiguracji wysokości skoku za pomocą ustawienia:
Character Movement → Jumping / Falling → Jump Z Velocity
w BP_PlayerCharacter.

179. Potwierdziłam, że zmiana Jump Z Velocity wpływa na wysokość skoku. Nie dodawałam jawnego JumpMaxCount = 1, ponieważ jest to domyślna wartość klasy ACharacter.

180. Zakończyłam weryfikację zadania DIP-25.

181. Pobrałam aktualny main i utworzyłam branch:
feature/DIP-26-side-camera

182. W BP_PlayerCharacter skonfigurowałam istniejący komponent CameraBoom jako boczną kamerę do rozgrywki 2.5D.

183. Ustawiłam boczną rotację CameraBoom, stałą długość ramienia oraz podniesienie kadru za pomocą Socket Offset.

184. Wyłączyłam:
- Use Pawn Control Rotation,
- dziedziczenie Pitch, Yaw i Roll,
- Enable Camera Lag,
- Enable Camera Rotation Lag,
- Do Collision Test.

Dzięki temu kamera zachowuje stałą odległość i nie przybliża się przy przeszkodach.

185. Potwierdziłam, że kamera:
- pokazuje postać z boku,
- śledzi postać podczas ruchu i skoku,
- nie obraca się podczas zmiany kierunku,
- nie może być swobodnie obracana przez gracza,
- poprawnie działa na mapie testowej.

186. Stwierdziłam, że z wybranej strony kamery klawisze A i D były wizualnie odwrócone względem kierunków ekranu.

187. W IMC_Player przeniosłam modifier Negate:
- usunęłam Negate z klawisza A,
- dodałam Negate do klawisza D,
- pozostawiłam negację wyłącznie osi X.

188. Usunęłam pusty element modifiera ustawiony jako None, który powodował błąd walidacji assetu IMC_Player.

189. Ponownie przetestowałam sterowanie. Potwierdziłam:
- D przesuwa postać w prawo na ekranie,
- A przesuwa postać w lewo na ekranie,
- model obraca się zgodnie z kierunkiem ruchu.

190. Zwiększyłam wartość Jump Z Velocity, aby wysokość skoku była odpowiednia dla przeszkód znajdujących się na mapie testowej.

191. Zapisałam zmiany w:
BP_PlayerCharacter.uasset
IMC_Player.uasset

192. Wykonałam commit opisujący konfigurację kamery bocznej, ekranowe kierunki ruchu oraz zmianę wysokości skoku.

193. Zakończyłam zadanie DIP-26 i zmergowałam zmiany do main.

194. Przeprowadziłam zadanie integracyjne DIP-27 na aktualnej wersji brancha main.

195. Zweryfikowałam wspólne działanie wszystkich elementów przygotowanych w Sprincie 1:
- uruchomienie właściwej postaci przy PlayerStart,
- działanie Enhanced Input,
- ruch lewo–prawo,
- obracanie modelu,
- ograniczenie postaci do płaszczyzny 2.5D,
- pojedynczy skok,
- opadanie i lądowanie,
- kolizje z podłożem i platformami,
- działanie kamery bocznej,
- poprawne uruchamianie mapy testowej.

196. Wykonałam pełną kompilację projektu bez nowych błędów.

197. Potwierdziłam spełnienie wszystkich kryteriów akceptacji DIP-27. Zamknęłam zadanie bez dodatkowych zmian w kodzie.

198. Funkcjonalnie zakończyłam Sprint 1. Wszystkie zadania Sprintu 1 zostały wykonane i przetestowane.
