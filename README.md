# CubeWars
A little game to test the network abilities of the Unreal Engine.

![CubeWars Screenshot](https://raw.githubusercontent.com/e0925357/CubeWars/master/Screenshots/ScreenshotAlpha0.0.3.png)

## Where can I download it?
If you want to download the latest release, go to the [release-page](https://github.com/e0925357/CubeWars/releases)!

## How to use
You can clone the repo and open the project with the Unreal Editor. If you want to start the project from your Visual Studio environment make sure you have the Unreal Engine installed and then right click the project file *CubeWars.uproject*. Look for the "Generate Visual Studio Project Files" option and click it. Now Unreal should create your *CubeWars.sln*.

## Documentation
Everything we come accross during development and that striks us as good-to-know, we write into this document: [Things To Know In Unreal 4](https://docs.google.com/document/d/1hdAtmGNz1iXU4afkwl60ZWZNHC873rm3fqy7Lf0NVBo/pub)

Our Trello board can be viewed here: [CubeWars Trello Board](https://trello.com/b/lxfkdOQE/cubewars)

## About the Game
This game can only be played with 2 players. Each player takes control over a cube that is able to slide to the right(Via [A] and [D]) and left, but cannot move forth nor back. A player may also rotate their cube using the mouse and fire with the left mouse button. The first player that reaches 0 health loses the match. To make things more interesting there are moveable barriers between the players to block their shots, but they can destroy them. Additionally power-ups are spawning in the middle of the battlefield. The first player that hits them gets a bonus for a limited amount of time.

### Power-ups
* Wreckingball
  * Player shoots a spiky ball
  * Slow, but powerful
  * Pierces through walls
* Shotgun
  * Burst-fires many little weak projectiles
  * Shoots them in a cone
* Needlegun
  * Player shoots weak needles in rapid succession
* Swift Cube
  * Player moves twice as fast
* Shield
  * Player gets a shield that will absorb a specific amount of damage
