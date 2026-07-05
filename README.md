# OpenOJP

OpenOJP is a mod for Star Wars Jedi Knight: Jedi Academy based on the OpenJK engine, porting the full feature set of the **Open Jedi Project (OJP) Enhanced** into a modern, maintained codebase.

## Direction

OpenOJP is focused on **bug fixing, minor tweaks, and quality-of-life improvements** to the OJP Enhanced experience. This is a community-maintained project that aims to keep OJP's unique gameplay alive and stable.

This is **not** a replacement for MovieBattles 2 or any other large-scale modification. OpenOJP preserves the core Jedi Academy gameplay with OJP's enhancements — improved saber combat, dodge mechanics, bot AI, and game mode additions — without redefining the game into something entirely new.

Contributions are welcome. If you find a bug or have a small improvement, feel free to open an issue or pull request.

## Features

### Saber Combat System
- Directional blocking and parrying (quadrant-based block detection)
- Saber block cost system per style (fast/medium/strong/dual/staff/desann/tavion)
- Attack parry, quick parry, and attack fake mechanics
- Mishap/balance system with slow bounces and heavy slow bounces
- Saber lock input system with directional advances
- Saber knockdown and stun animations
- Back-blocking with force defense level 3
- Roll balance and mishap rolling
- Dodge kick countering

### Dodge & Movement System
- Full body dodge system with directional dodging (hop forward/back/left/right)
- Partial dodge damage scaling
- Dodge roll safety checks
- Ledge grab and wall run system with shimmy and pull-up
- Force fall braking (slow fall with the force)
- Knockdown getup rolls (forward/back/left/right)
- Force getup animations
- Crouch getup from knockdown
- Air kicking animations

### Dynamic Music System
- DMS (Dynamic Music Set) transitions between explore, action, and boss music
- Music length data and exit point synchronization
- Per-map music file loading

### Cinematic Camera System
- Move, pan, zoom, and fade camera effects
- Camera shake with intensity and duration
- Camera smoothing
- ROFF (Relative Offset File) camera animation
- Letterbox bar fading
- Camera following and tracking

### ROFF Animation System
- Relative Offset File playback for scripted entity animation
- ROFF caching and management
- Per-entity ROFF playback with frame timing

### Waypoint Navigator / Pathfinding
- Waypoint graph navigation with neighbor linking
- Route finding with distance-based path selection
- Closest waypoint queries

### Domination Game Mode
- Flag-based territory control
- Capture and hold scoring system
- Domination NPC support
- Dynamic spawn system based on controlled territory

### Force Holocron System (AOTC)
- Holocron item placement and pickup
- Force power granting from holocrons

### AI Behavior Types
- **AOTC AI** — Attack-aware combat with saber hit reactions
- **Hybrid AI** — Mix of ranged and melee combat tactics
- **TAB AI** — Full tactical behavior system with objectives, orders, scanning, strafing, and saber duel challenging

### New NPC Types
- **Assassin Droid** — Strafe-shooting blaster combat
- **Civilian** — Flee-based survival behavior
- **Rocket Trooper** — Rocket launcher combat with range management
- **Saber Droid** — Melee saber combat
- **Sand Creature** — Burrowing ambush combat
- **Vehicle NPC** — Vehicle-based combat with ranged attacks

### Game Systems
- **Arena Tournament** — Post-game scoreboard with accuracy tracking
- **Autosave** — Periodic automatic save creation
- **Breakable Objects** — Destructible environment pieces with radius damage
- **Crash Physics** — High-speed collision damage
- **ICARUS Stubs** — Script system compatibility layer
- **Extended Saber Colors** — White, black, RGB, pimp, and scripted saber colors
- **Skills System** — Experience-based skill progression (weapon skills, saber styles, equipment)

### CGame Additions
- Camera system with cinematic effects
- Weapon holster system
- Dynamic lights with lifetime management
- Holocron icon rendering

### Admin System
- **`centersay`** — Server console command to send center-printed messages to all players
- **IP Ban Management** — `addip`, `removeip`, `listip` commands with persistent bans via `banip.txt`
- **Auto-Kick/Auto-Ban** — Automatic enforcement for team-kill and kill spamming with configurable thresholds (`g_autoKickTKSpammers`, `g_autoBanTKSpammers`, `g_autoKickKillSpammers`, `g_autoBanKillSpammers`)
- **Chat Spam Protection** — Configurable debounce timer (`ojp_chatProtectTime`)
- **All admin features accessible via RCON** — No additional admin plugin required

### Ghoul2 & Utility
- G2 API wrapper layer for bolt matrix, bone angles, and ragdoll
- C++ vector math library (CVec3)
- Real trace system for accurate saber/ghoul2 collision detection
- Saber ballistics (thrown saber physics with stick/bounce)

## Building

OpenOJP uses the CMake build system from OpenJK. See `OPENJK.md` for build instructions.

### Build Dependencies
- CMake 3.10+
- Visual Studio (Windows) or GCC/Clang (Linux/macOS)
- SDL2

### Quick Start (Windows)
```
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Installation

Place the compiled DLLs (`jampgamex86.dll`, `cgamex86.dll`, `uix86.dll`) and the `zz_ojp_assets.pk3` from the `ojp/ojpenhanced/` directory into the `OpenOJP/` folder in your Jedi Academy installation directory.

## Credits

- **OpenJK Team** — Engine and base code
- **OJP Enhanced Team** — Original OJP modifications and assets
- **Raven Software** — Jedi Academy game
