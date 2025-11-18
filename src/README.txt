# U233(gamma,n)U232 — Geant4 Electron-Beam Yield App

Simulate a **U-233 metallic cylinder** irradiated by a **40 MeV electron beam**. The electrons produce bremsstrahlung γ’s in the same target (“converter + target”), and we **count U-232 nuclei** produced via the **photonuclear** process inside the target. At the end of each run we report:

- total **U-232** atoms created,
- **probability per primary electron**,
- **yield [atoms/s]** for a given beam current,
- **projected atoms** for a given irradiation time.

This is meant for quick yield estimates (not full depletion/decay chains).

---

## Physics & Model

- **Physics list**: `FTFP_BERT` + `G4EmExtraPhysics` (enables γ-nuclear) + `G4DecayPhysics`.
- **Process counted**: tracks whose *creator process name* is exactly **`"photonNuclear"`** and whose creation volume is the **target**.
- **Geometry**:
  - Target: U-233 metal (pure isotope), cylinder: **Ø1 cm × 1 cm** (axis +Z).
  - World: air box (10 cm half-size).
- **Beam (default)**:
  - Primary particle: **electron**.
  - Energy: **40 MeV**.
  - Spot: uniform disk of **radius 2.5 mm**.
  - Direction: +Z, source positioned **1 cm** upstream of target center.

**Yield formula**  
Let `N_232` be the total U-232 counted and `N_e` the number of primary electrons:
- Per-primary probability: `p = N_232 / N_e`
- Electrons per second: `Ṅ_e = I / e`  (with `e = 1.602176634e-19 C`)
- **Yield [atoms/s]**: `Y = p * Ṅ_e`
- **Projected atoms** after time `T`: `N_proj = Y * T`

Assumptions: thin-target estimate for yield; ignores burn-up, re-interactions, and decay during irradiation.

---

## Repository Layout

include/
ActionInitialization.hh
DetectorConstruction.hh
PrimaryGeneratorAction.hh
RunAction.hh
TrackingAction.hh
macros/
init_vis.mac
run.mac
src/
ActionInitialization.cc
DetectorConstruction.cc
PrimaryGeneratorAction.cc
RunAction.cc
TrackingAction.cc
main.cc
CMakeLists.txt (provided by you; DO NOT edit)

## Build (Windows 11, CMake GUI + Visual Studio 2022)

**Prerequisites**
- Geant4 **built with UI & visualization** (`ui_all`, `vis_all`).  
  Run the Geant4 environment script before building:
  - `geant4-v[...]-install\bin\Geant4-[...]-win64-Setup.bat` (or `Geant4Vars.bat`)
- CMake ≥ **3.16**, Visual Studio **2022**, x64 toolset.

**Steps (CMake GUI)**
1. **Where is the source code**: path to this repo.  
   **Where to build**: choose a separate `build` folder.
2. Configure:
   - Generator: **Visual Studio 17 2022**
   - Platform: **x64**
3. Ensure `Geant4_DIR` is detected (from your Geant4 install).
4. **Configure** → **Generate** → **Open Project**.
5. In VS, set configuration (**Release** recommended) → **Build** the target `u233_gamma_n_u232`.

The build copies `macros/` next to the built executable automatically.

---

## Run

### Option A — Interactive (with visualization)
From VS (Debugging→Working Dir set to the exe folder) or a terminal:

u233_gamma_n_u232.exe

This launches the UI and runs `macros/init_vis.mac`. In the UI, you can type:

/control/execute macros/run.mac

shell
Copy code

### Option B — Batch
u233_gamma_n_u232.exe macros/run.mac

yaml
Copy code

---

## Configuration (macros)

**`macros/init_vis.mac`**: sets up OpenGL viewer and defaults:
```tcl
/run/beamCurrent 0.00025 A   # 0.25 mA
/run/irradTime   3600 s      # 1 hour
macros/run.mac: example batch:

tcl
Copy code
/run/beamCurrent 0.00025 A
/run/irradTime   86400 s
/run/initialize
/run/beamOn 10000
You can change beamCurrent and irradTime without recompiling.