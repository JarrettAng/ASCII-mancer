# ASCII-mancer
Team: Casual Coding Party

Class: CSD1401f22-b

### Team Members:
Amadeus Jinhan Chia	  (amadeusjinhan.chia@digipen.edu)

Ang Jiawei Jarrett	  (a.jiaweijarrett@digipen.edu)

Justine Carlo Villa Ilao  (justine.c@digipen.edu)

Muhammad Farhan Bin Ahmad (b.muhammadfarhan@digipen.edu)

Tan Jun Rong		  (t.junrong@digipen.edu)

### Genre
Turn based, Survival.
### Gameplay Premise
Defend yourself from the horde of zombies using Tetris blocks to eliminate them.
### Play Area
A grid system like Plant vs Zombies.
### How to Play
Players place Tetris blocks on the grid to stop incoming zombies.
### Game Cycle
Players will have a hand of Tetris blocks to choose from to place on the grid stop the incoming zombie horde. Everytime a block is placed, the player's turn ends and the Zombie's turn starts. The zombies move left in the grid and more zombies (if any left in the wave) will spawn. 
### Wave system
If all zombies in the wave are spawned and only a few remain, the current wave ends and the next wave starts.
###Controls:
Mouse only

#### Damage Zombies / Build Walls
Left click from hand, drag onto board and release.

#### Rotate Pieces
Right click to the piece clockwise.

#### Debug / Playtest Mode
Shift+D to toggle mode

0 - Spawns wall

1 - Spawns normal zombie

2 - Spawns leaper zombie

3 - Spawns tank zombie

4 - Spawns breacher zombie

5 - Spawns grave zombie

O - Loses a heart

P - Gains a heart

W - Sets wave count to 30 (The last wave)

R - Clears the board

## Folders
This is how our folders are structured.
### ASCII-mancy
The project settings are updated to match the Visual Studio Setup.
### Assets
Where we store our assets like sounds and images, includes all default assets in the CProcessing sample.
### Extern
External resources, only CProcessing is allowed for now.
### Inc
Where header files are stored.
### Src
Where source (C) files are stored.
## Folders excluded by git-ignore
### bin
Where Debug/Release builds are stored.
