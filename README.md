# CFD Studio

A comprehensive Computational Fluid Dynamics (CFD) software package for aerodynamic analysis, flow simulation, and hypersonic propulsion design.

## Project Overview

CFD Studio is a multi-component CFD suite featuring:
- 2D compressible Navier-Stokes solver with Roe flux splitting
- Real-time 3D visualization with raylib
- Interactive terminal and ncurses GUIs
- Scramjet design and analysis tools
- AI-powered design advisor
- C++ physics engine with 3D model support

## Architecture

```
cfd_solver_for_science/
├── solver/              # 2D Navier-Stokes CFD solver
│   ├── main.c           # CLI entry point
│   ├── cfd_solver.c/h   # Core solver implementation
│   └── physics/         # Physics modules
│       ├── atmosphere.c/h    # ISA atmosphere model
│       ├── forces.c/h        # Aerodynamic calculations
│       └── heat_transfer.c/h # Heat transfer analysis
├── tui/                 # Terminal user interface
├── gui/                 # Ncurses GUI with sliders
├── visualization/       # 3D raylib visualization
├── core/                # C++ physics engine
│   ├── physics_engine.cpp/h  # Core physics calculations
│   ├── model_loader.cpp/h    # 3D model (STL/OBJ) loading
│   └── physics_bridge.cpp/h  # C/C++ bridge
├── scramjet/            # Scramjet design suite
│   ├── main.c           # Scramjet configuration & analysis
│   └── scramjet_config.c/h   # Scramjet parameters
├── ai/                  # AI design advisor
│   ├── main.c           # C chatbot interface
│   └── main.py          # Python chatbot
├── CMakeLists.txt       # Root build configuration
├── SPEC.md              # Detailed specification
└── README.md
```

## Quick Start

### Building

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Running Components

```bash
# CFD Solver
./solver/cfd_solver

# Terminal UI
./tui/tui_executable

# Ncurses GUI
./gui/gui_executable

# 3D Visualization (requires raylib)
./visualization/visualization

# Scramjet Design Suite
./scramjet/scramjet_executable

# AI Advisor
./ai/ai_executable
```

## Components

### CFD Solver (`solver/`)

2D compressible Navier-Stokes solver using Roe flux splitting.

**Features:**
- Subsonic to hypersonic flow regimes (M 0.3-5.0+)
- Configurable grid resolution
- Angle of attack simulation
- Convergence monitoring
- ISA atmosphere model

**Usage:**
```bash
./solver/cfd_solver --mach 2.0 --alpha 5.0 --iter 5000
./solver/cfd_solver --mesh 200 --mach 0.8
```

### Terminal UI (`tui/`)

Interactive menu-driven interface for running simulations.

### Ncurses GUI (`gui/`)

Interactive GUI with sliders for real-time parameter adjustment:
- Velocity, altitude, distance
- Mass, gravity, wing area
- Drag/lift coefficients
- Angle of attack

Displays results: flight time, forces, energy, atmospheric properties.

### 3D Visualization (`visualization/`)

Raylib-based 3D visualization with:
- Density-based color mapping
- Camera orbit/pan/zoom controls
- Grid and axis toggles
- Wireframe mode

### C++ Physics Engine (`core/`)

Core physics calculations in C++:
- Atmospheric properties (ISA model)
- Aerodynamic coefficients
- Force calculations
- 3D model loading (STL/OBJ)
- C bridge for integration with C code

### Scramjet Design Suite (`scramjet/`)

Professional scramjet analysis tool:
- **Scramjet Types:** Pure, SHCRAMJET (Shrouded Helical Counterflow), Dual-Mode
- **Sizes:** Small, Medium, Large, X-Large
- **Fuel Options:** Hydrogen, Methane, Ethylene
- **Cooling Systems:** Regenerative, Film, Transpiration, Air Bleed
- **Inlet Types:** 2D Wedge, 3D Conical, 2D Digital, 3D Isometric

**Analysis Features:**
- Performance analysis (thrust, Isp, efficiency)
- Thermal management analysis
- Stress analysis
- Flow simulation
- Size comparison
- Design export

**Example Session:**
```
1. Configure Type & Size (SHCRAMJET Medium)
2. Set Fuel & Cooling (Hydrogen, Regenerative)
3. Configure Inlet (2D Wedge, 15°)
4. Set Flight Conditions (Mach 8, 35km)
5. Run Performance Analysis
```

### AI Advisor (`ai/`)

AI-powered design assistant with:
- Chatbot interface for design questions
- Parameter-based modification suggestions
- Python chatbot integration

## Dependencies

### Required
- GCC/Clang
- CMake 3.10+
- math library (libm)

### Optional
- **raylib** - 3D visualization
- **ncurses** - GUI interface
- **Python 3** - AI chatbot

### Installation

```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake libraylib-dev libncurses-dev

# Fedora
sudo dnf install gcc cmake raylib-devel ncurses-devel

# Arch Linux
sudo pacman -S gcc cmake raylib ncurses
```

## Test Cases

| Case | Mach | AoA | Description |
|------|------|-----|-------------|
| 1 | 0.3 | 0° | Subsonic flat plate |
| 2 | 0.8 | 0° | Transonic flow |
| 3 | 2.0 | 0° | Supersonic |
| 4 | 2.0 | 5° | Supersonic with lift |
| 5 | 5.0 | 10° | Hypersonic re-entry |

## Performance Notes

- Grid size O(N²) memory scaling
- 100x100 grid: ~1s per 100 iterations
- 200x200 grid: ~4s per 100 iterations

## License

MIT License
