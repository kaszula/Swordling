# UNREAL PROJECT STANDARDS

## 1. Cel dokumentu

Dokument określa wspólne zasady organizacji projektu Unreal Engine 5: strukturę folderów, nazewnictwo assetów, Blueprintów, klas C++ i map. Ma ułatwiać wyszukiwanie plików, pracę zespołową i ograniczać konflikty.

## 2. Zasady ogólne

- Nazwy techniczne zapisujemy po angielsku.
- Nie używamy polskich znaków, spacji ani znaków specjalnych.
- Stosujemy `PascalCase`.
- Nazwa powinna jasno opisywać przeznaczenie elementu.
- Nie używamy nazw typu `NewBlueprint`, `Test2`, `FinalFinal`, `Object1`.
- Nie umieszczamy assetów bezpośrednio w głównym folderze `Content/`.
- Nie edytujemy równocześnie tych samych plików `.uasset` i `.umap`.
- Po przeniesieniu lub zmianie nazw assetów używamy `Fix Up Redirectors in Folder`.


## 3. Struktura repozytorium

```text
DiplomaGame/
├── Config/
├── Content/
├── docs/
│   ├── journal_ula/
│   ├── DEVELOPER_SETUP.md
│   ├── DEVELOPMENT_WORKFLOW.md
│   ├── GAME_MVP_REQUIREMENTS.md
│   ├── GIT_WORKFLOW.md
│   ├── MVP_BACKLOG.md
│   └── UNREAL_PROJECT_STANDARDS.md
├── Source/
├── DiplomaGame.uproject
└── README.md
```

Kod C++ znajduje się w `Source/`, a nie w folderze `Content/`.

## 4. Struktura folderu Content

```text
Content/
└── DiplomaGame/
    ├── Art/
    │	├── Characters/
    │	│	├── C00Common
    │	│	└── C01Player
    │	├── Environments/
    │	│	└── E00Common
    │	├── Props/
    │	│	└── P00Common
    │	└── VFX/
    │		└── V00Common
    │   ├── Animations/
    │   ├── Characters/
    │   ├── Environments/
    │   ├── Materials/
    │   ├── Meshes/
    │   ├── Textures/
    │   └── VFX/
    ├── Audio/
    │   ├── Music/
    │   ├── SFX/
    │   └── Voice/
    ├── Blueprints/
    │   ├── Characters/
    │   ├── Enemies/
    │   ├── Encounters/
    │   ├── Components/
    │   ├── Gameplay/
    │   ├── Interactions/
    │   └── Items/
    ├── Data/
    │   ├── DataAssets/
    │   ├── DataTables/
    │   ├── Enums/
    │   └── Structs/
    ├── Input/
    ├── Maps/
    │   ├── Development/
    │   ├── Tutorial/
    │   └── Main/
    ├── UI/
    │   ├── HUD/
    │   ├── Menus/
    │   └── Common/
    └── ThirdParty/
```

Assety zewnętrzne umieszczamy w `ThirdParty/<PackageName>/` i nie mieszamy ich z assetami tworzonymi przez zespół.


## 5. Struktura kodu C++

```text
Source/
└── DiplomaGame/
    ├── Characters/
    ├── Enemies/
    ├── RunSystem/
    ├── Components/
    ├── Gameplay/
    ├── Interaction/
    ├── SaveSystem/
    └── UI/
```

Nazwy plików odpowiadają nazwom klas bez prefiksu Unreal:

```text
PlayerCharacterBase.h
PlayerCharacterBase.cpp
HealthComponent.h
HealthComponent.cpp
SaveGameManager.h
SaveGameManager.cpp
```

Standardowe prefiksy klas:

| Prefiks | Zastosowanie | Przykład |
|---|---|---|
| `A` | Actor | `APlayerCharacterBase` |
| `U` | UObject lub komponent | `UHealthComponent` |
| `F` | struktura | `FPlayerSaveData` |
| `E` | enum | `EWeaponType` |
| `I` | interfejs | `IInteractable` |

Wspólne klasy bazowe mogą kończyć się słowem `Base`, np. `APlayerCharacterBase`, `AEnemyCharacterBase`.


## 6. Prefiksy assetów

### Blueprinty i wejście

| Typ | Prefiks | Przykład |
|---|---|---|
| Blueprint | `BP_` | `BP_PlayerCharacter` |
| Blueprint Component | `BPC_` | `BPC_HealthComponent` |
| Blueprint Interface | `BPI_` | `BPI_Interactable` |
| Widget Blueprint | `WBP_` | `WBP_MainMenu` |
| Animation Blueprint | `ABP_` | `ABP_Player` |
| Input Action | `IA_` | `IA_Jump` |
| Input Mapping Context | `IMC_` | `IMC_Player` |

### Grafika i animacja

| Typ | Prefiks | Przykład |
|---|---|---|
| Static Mesh | `SM_` | `SM_LevelGate` |
| Skeletal Mesh | `SK_` | `SK_Player` |
| Skeleton | `SKEL_` | `SKEL_Player` |
| Physics Asset | `PHYS_` | `PHYS_Player` |
| Animation Sequence | `A_` | `A_Player_Attack` |
| Animation Montage | `AM_` | `AM_Player_Attack` |
| Blend Space | `BS_` | `BS_Player_Locomotion` |

### Materiały i tekstury

| Typ | Prefiks | Przykład |
|---|---|---|
| Material | `M_` | `M_LevelSurface` |
| Material Instance | `MI_` | `MI_LevelSurface_Dark` |
| Material Function | `MF_` | `MF_Dissolve` |
| Texture | `T_` | `T_LevelSurface_BC` |

Sufiksy tekstur:

- `_BC` — Base Color,
- `_N` — Normal,
- `_R` — Roughness,
- `_M` — Metallic,
- `_AO` — Ambient Occlusion,
- `_E` — Emissive,
- `_ORM` — połączona mapa AO/Roughness/Metallic.


### Audio, VFX i dane

| Typ | Prefiks | Przykład |
|---|---|---|
| Sound Wave | `SW_` | `SW_AttackHit_01` |
| Sound Cue | `SC_` | `SC_AttackHit` |
| MetaSound Source | `MS_` | `MS_LevelAmbience` |
| Niagara System | `NS_` | `NS_AttackImpact` |
| Niagara Emitter | `NE_` | `NE_AttackSparks` |
| Data Asset | `DA_` | `DA_PlayerStats` |
| Data Table | `DT_` | `DT_EnemyStats` |
| Enum Blueprint | `E_` | `E_WeaponType` |
| Struct Blueprint | `ST_` | `ST_SaveData` |

### Mapy

| Typ | Prefiks | Przykład |
|---|---|---|
| Mapa produkcyjna | `L_` | `L_Tutorial` |
| Mapa testowa | `L_Test_` | `L_Test_Combat` |
| Mapa deweloperska | `L_Dev_` | `L_Dev_Movement` |


## 7. Nazewnictwo funkcji i zmiennych

Funkcje nazywamy jako czasownik i obiekt:

```text
ApplyDamage
StartSprint
EquipWeapon
SaveGame
UpdateHealthDisplay
```

Funkcje logiczne zaczynamy od `Is`, `Can`, `Has` lub `Should`:

```text
IsDead
CanJump
HasRequiredItem
ShouldBlockAttack
```

Zmienne:

```text
CurrentHealth
AttackCooldown
ActiveWeapon
TargetEnemy
```

Zmienne logiczne w C++ zaczynamy od `b`:

```cpp
bool bIsDead;
bool bCanParry;
bool bHasRequiredItem;
```

Tablice i kolekcje nazywamy w liczbie mnogiej, np. `Enemies`, `UnlockedAbilities`, `SaveSlots`.

Eventy i delegaty:

```text
OnHealthChanged
OnEnemyDefeated
OnRequiredEncounterStarted
OnCheckpointActivated
```


## 8. Komponenty, sockety i tagi

Komponenty nazywamy zgodnie z ich funkcją:

```text
Camera
CameraBoom
Capsule
CharacterMesh
HealthComponent
CombatComponent
InteractionComponent
WeaponMesh
ShieldMesh
```

Nie zostawiamy nazw typu `Scene1`, `StaticMeshComponent0`.

Sockety:

```text
Socket_Weapon_R
Socket_Weapon_L
Socket_Weapon_Back
Socket_Shield_L
Socket_VFX_Hand_R
```

Przykładowe profile kolizji:

```text
Player
Enemy
PlayerWeapon
EnemyWeapon
Interactable
Projectile
Climbable
```

Jeżeli użyjemy Gameplay Tags, zapisujemy je hierarchicznie:

```text
Player.State.Dead
Player.State.Stunned
Player.Action.Attacking
Enemy.State.Alerted
Damage.Type.Physical
Damage.Type.Special
Weapon.Type.Melee
Weapon.Type.Ranged
```


## 9. Standardy Blueprintów

- Blueprint powinien mieć jedną jasno określoną odpowiedzialność.
- Powtarzalną logikę przenosimy do komponentów, funkcji, interfejsów lub klas bazowych.
- Unikamy rozbudowanej logiki w `Event Tick`, jeżeli można użyć zdarzenia lub timera.
- Zmienne publiczne grupujemy w kategorie, np. `Combat`, `Movement`, `Audio`, `Debug`.
- Zmienne edytowalne powinny mieć czytelny tooltip.
- Usuwamy nieużywane node'y i odłączone fragmenty grafu.
- Większe fragmenty grafu opisujemy komentarzami.
- Przed commitem wykonujemy `Compile` oraz `Save`.

## 10. Standardy C++

- Kod powinien być zgodny ze stylem Unreal Engine.
- Używamy `UPROPERTY()` i `UFUNCTION()` tam, gdzie wymaga tego integracja z silnikiem.
- Nie umieszczamy rozbudowanej logiki w konstruktorze aktora.
- Większe klasy dzielimy na komponenty, jeżeli mają kilka niezależnych odpowiedzialności.
- Unikamy powielania tej samej logiki w C++ i Blueprintach.
- Kod musi się kompilować przed wykonaniem commita.


## 11. Mapy testowe i assety robocze

Mapy testowe:

```text
L_Test_Movement
L_Test_Combat
L_Test_EnemyAI
L_Test_SaveSystem
L_Test_UI
```

Umieszczamy je w:

```text
Content/DiplomaGame/Maps/Development/
```

Tymczasowe eksperymenty umieszczamy w:

```text
Content/Developers/Urszula/
Content/Developers/Tomasz/
```

Assety produkcyjne nie mogą zależeć od plików pozostających w folderze `Developers`.

## 12. Branche i commity

Branche:

```text
feature/DIP-10-player-movement
bugfix/DIP-11-fix-camera
docs/DIP-12-update-specification
chore/DIP-13-project-cleanup
```

Commity:

```text
DIP-10 Implement basic player movement
DIP-11 Fix camera collision
DIP-12 Update game specification
```

Szczegółowy proces opisuje `GIT_WORKFLOW.md`.


## 13. Lista kontrolna przed commitem

Sprawdź:

- czy nazwy są po angielsku,
- czy użyto właściwych prefiksów,
- czy pliki znajdują się we właściwych folderach,
- czy nie powstały przypadkowe duplikaty,
- czy Blueprinty zostały skompilowane,
- czy wszystkie zmiany zostały zapisane,
- czy nie dodano niepotrzebnych assetów testowych,
- czy druga osoba nie edytuje tego samego pliku binarnego,
- czy po przeniesieniu assetów naprawiono redirectory.

## 14. Przykładowe nazwy

```text
BP_PlayerCharacter
ABP_Player
SK_Player
PHYS_Player
AM_Player_Attack
IA_Attack
IA_Jump
IMC_Player
BP_EnemyBase
BP_EnemyBase
BPC_HealthComponent
BPI_Interactable
DA_PlayerStats
DT_EnemyStats
ST_SaveData
WBP_PlayerHUD
WBP_MainMenu
L_Tutorial
L_MainLevel
L_Test_Combat
M_LevelSurface
MI_LevelSurface_Dark
T_LevelSurface_BC
NS_AttackImpact
SC_AttackHit
```

## 15. Aktualizowanie standardów

Standardy mogą być rozwijane wraz z projektem. Zmiany powinny wynikać z rzeczywistej potrzeby, być uzgodnione przez zespół i trafiać do repozytorium przez branch oraz Pull Request.
