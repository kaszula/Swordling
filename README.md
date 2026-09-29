# Swordling

Swordling is a 2.5D action-platformer game developed in Unreal Engine 5 as part of a diploma project.

The project combines gameplay mechanics implemented in Unreal Engine with a backend and database used to store player results.

## Project overview

The player controls one of the available characters and explores a dark fantasy environment while fighting enemies, avoiding hazards and completing the level.

The main gameplay systems include:

- character movement and jumping,
- light attack,
- heavy attack,
- blocking with a shield,
- health system,
- stamina system,
- enemy AI,
- damage and death mechanics,
- HUD and gameplay interface,
- player statistics,
- scoreboard,
- communication with a backend service,
- storing game results in a database.

## Scoreboard

After completing the game, player statistics are stored in the database.

The stored data includes information such as:

- player name,
- selected hero,
- completion time,
- number of enemies defeated,
- damage taken.

The backend is responsible for processing the results and retrieving the best records displayed in the in-game scoreboard.

## Technologies

The project uses:

- Unreal Engine 5
- Blueprints / C++
- backend application
- SQL database
- REST API
- Git

## Project structure

The repository contains the source files required to open and develop the project.

Automatically generated Unreal Engine directories and temporary files are intentionally excluded from the repository.

## Running the project

1. Clone or download the repository.
2. Open the Unreal Engine project file.
3. Use the Unreal Engine version compatible with the project.
4. Generate project files if required.
5. Start the backend and database services if scoreboard functionality is required.
6. Open the project in Unreal Engine and run the game.

## External assets

The project may contain third-party assets, models, textures, sounds or music used according to their respective licences.

These resources were used as supporting assets, while the gameplay logic, game systems, integration and project implementation were created as part of the diploma project.

## Diploma project

This repository contains the final version of the project prepared for submission as part of a diploma project.

The repository was created to provide a clean version of the source code and project files required for evaluation.
