# Red Faction Armageddon Improvement Patch
- Version: v0.xx Pre Pre Alpha 20-09-2022 Build 112
- Work-in-progress project that adds content, fixes bugs, and enhances many features. 

# Releases

[Download](https://www.youtube.com/watch?v=dQw4w9WgXcQ)

# Credits

[HeyItsDuke](https://github.com/HeyItsDuke) for Research, Testing, Quality Assurance, and LOTS of Help!

# Changelog
* All objects now cast shadows 
* All lights now cast shadows
* All ruin and infestation maps are unlocked immediately 
* All unlocks and cheats are now free to purchase (for dev/test purposes)
* All weapons unlocked by default in infestation (for dev/test purposes)
* Adds the missing mp_weapon_overrides.xtbl file so we can balance weapons separately for multiplayer and singleplayer (tweaked a few things to make it more like guerrilla in some ways after playing 10 rounds but it's wip)
* Adds new characters to multiplayer (not final, needs xdeltas for rosters due to a limit of how many show in menu)
* Adds a survival version of Dust to Dust
* Adds all the cheat weapons to Ruin mode
* Adds bloom that was disabled for some reason and added weather/lighting files for maps that were missing them
* Added every weapon including cheats and bonus weapons to infestation (they still need to be balanced)
* Buffed shotgun in multiplayer (needs details, not final)
* Buffed Assault rifle  (needs details, not final)
* Buffed enforcer  (needs details, not final)
* Buffed laser pistol  (needs details, not final)
* Buffed plasma thrower  (needs details, not final)
* Buffed rocket launcher  (needs details, not final)
* Compresses multiple files to reduce the memory pool load
* Disables startup videos
* Delayed Explosion Laser Handheld, Lava Barge Turret Handheld, Napalm Laser Handheld, Nano Grenade, Plasma Thrower and the Unicorn gun can now be used in Total Chaos Ruin mode in addition to Free Play
* Disables camera shake
* Disables radial blur
* Disables caps on various visual effects (console nerfs)
* Glass bullet decals now always appear and only one per bullet appears for realism
* Stops debris despawning instantly and adds collision that reacts to player movement
* Stops gibs despawning instantly
* Stops bullet mags despawning instantly
* Improves draw distance of lighting
* Improves general lighting quality
* Improved alien lighting
* Improves blood decals (needs details)
* Increases number of aliens that spawn per wave (not all waves done, up to wave 15 or so, only singleplayer waves changed for the most part so far, not final)
* Increases decal lifetimes 
* Increases draw distance of shadows
* Increases draw distance of blood, bullet hole and blast decals
* Increases draw distance of NPCs
* Increases draw distances of various misc objects and structures
* Increases FOV to 85
* Increases the speed of climbing ladders (needs details)
* Improves destruction by making it similar to RFG instead of everything being made of cardboard
* Improves connection in matchmaking
* NPCs now do the same amount of damage to each other as the player
* NPCs now do the same damage to each others vehicles as the player
* NPCs now take 30 minutes to remove attached projectiles 
* Player now does same damage to friendly NPCs as enemies
* Players now move the same speed when moving sideways in Infestation
* Reduces respawn time to 2 seconds from 5 in Infestation 
* Reduces respawn invulnerability to 2 seconds from 5 in Infestation
* Reduces revive invulnerability to 2 seconds from 6 in Infestation
* Removes the pre wave delay in Infestation
* Removes the post wave delay in Infestation
* Removed delay on repair grenade (for dev/test purposes)
* Removed cost of resetting player upgrades  (for dev/test purposes)
* Reduced lives in Infestation to 8 from 12 for one player, to 4 from 6 for two players, to 3 from 4 for 3 players and to 2 from 3 for 4 players
* Fixes formatting for all XTBL files so the game is easier to mod on top of the patch
* Fixes various bugs
* Various improvements to player movement and lots of other stuff in tweak_table.xtbl
* Various tweaks to AI behavior
* Aliens are more aggressive

# Installing

1. Install the files in optimized to both misc and table folders first
2. Install the files in XTBL to both of those folders next and override if asked
3. Install the interface files to interface and override; packVPP all of the folders
4. Use Camo's compression scripts to compress misc and table .vpp_pc files
5. Compressed files will be in the proper folder as an unnamed .vpp_pc file. Name file to proper name and install to RFA's build/pc/cache folder

NOTE: Interface doesn't like compression nor does it need it

# TODO
See [Here](https://github.com/CamoRF/Red-Faction-Armageddon-Improvement-Patch/blob/main/to_do_list.txt) 

# Development
[Placeholder]

# Known Bugs
[Placeholder]
