# CFD Studio

**Computational Fluid Dynamics Visualization Suite**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![raylib](https://img.shields.io/badge/raylib-6.0-green.svg)](https://www.raylib.com/)

CFD Studio is a desktop application for computational fluid dynamics visualization, written in pure C using the raylib graphics library. It provides real-time aerodynamic analysis, ISA atmospheric modeling, and interactive 3D visualization with an Onshape-inspired dark theme UI.

---

## Overview

CFD Studio enables engineers, researchers, and students to perform aerodynamic analysis and visualize fluid dynamics concepts in real-time. The application features a modern dark-themed interface with a three-panel layout, interactive parameter controls, and instant feedback on aerodynamic forces and heat transfer calculations.

Built entirely in C11 using raylib for hardware-accelerated 3D rendering, CFD Studio delivers a responsive desktop experience for analyzing aircraft performance characteristics at various flight conditions.

---

## Features

- **3D Viewport** - Hardware-accelerated rendering with raylib/OpenGL
- **Interactive Camera Controls** - Orbit, pan, and zoom with mouse
- **ISA Atmosphere Model** - International Standard Atmosphere calculations for pressure, density, and temperature at altitude
- **Aerodynamic Force Analysis** - Real-time calculation of lift, drag, and side forces
- **Heat Transfer Analysis** - Convective heat flux, stagnation temperature, and surface temperature calculations
- **Unit Conversions** - Toggle between mph/Mach for speed and feet/meters for altitude
- **Parameter Sliders** - Adjustable simulation parameters: airspeed, altitude, distance, fuel, angle of attack
- **Derived Values Display** - Mach number, Reynolds number, dynamic pressure, sound speed
- **Dark Theme UI** - Onshape-inspired professional dark color scheme
- **Model Import Support** - Framework for OBJ and STL mesh format loading (stub implementations)
- **Visualization Tools** - Wireframe/solid rendering modes, grid floor, axis indicator
- **Force Vector Visualization** - Visual representation of lift and drag vectors

---

## Quick Start

```bash
# Clone and build
mkdir build && cd build
cmake ..
make

# Run (set library path first)
export LD_LIBRARY_PATH=/home/grep/local/home/grep/local/lib:$LD_LIBRARY_PATH
./cfd_studio
```

---

## Requirements

| Component | Version | Notes |
|-----------|---------|-------|
| raylib | 6.0 | Graphics and windowing library |
| CMake | 3.10+ | Build system |
| C Compiler | C11 | GCC, Clang, or MSVC |
| X11 | - | Windowing system (Linux) |
| pthread | - | Threading support |
| libm | - | Math library |
| libdl | - | Dynamic loading |

**Platform Support:** Linux (X11), macOS, Windows

---

## Building

### Step 1: Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install cmake build-essential libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install cmake gcc libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel mesa-libGL-devel
```

**macOS:**
```bash
brew install raylib cmake
```

### Step 2: Clone and Build

```bash
git clone <repository-url>
cd cfd_solver_for_science
mkdir build && cd build
cmake ..
make
```

### Build Options

**Debug Build (default):**
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

**Release Build (optimized):**
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

---

## Running

### Set Library Path

Before running, ensure raylib can be found:

```bash
export LD_LIBRARY_PATH=/home/grep/local/home/grep/local/lib:$LD_LIBRARY_PATH
```

Add this to your `.bashrc` or `.zshrc` for persistence:
```bash
echo 'export LD_LIBRARY_PATH=/home/grep/local/home/grep/local/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
```

### Launch Application

```bash
./build/cfd_studio
```

---

## Usage Guide

### Camera Controls

| Action | Control |
|--------|---------|
| Rotate View | Left Mouse Button + Drag |
| Pan View | Middle Mouse Button + Drag |
| Zoom In/Out | Mouse Scroll Wheel |
| Reset Camera | Click Reset button in toolbar |

### Parameter Adjustment

Use the sliders in the bottom Control Panel to adjust simulation parameters:

1. **Airspeed** - Drag slider or click and type value (0-2000)
2. **Altitude** - Adjust flight altitude (0-100,000 ft/m)
3. **Distance** - Set distance parameter (0-50,000 km)
4. **Fuel** - Configure fuel load (0-100,000 kg)
5. **Angle of Attack** - Set AoA (-45° to +45°)

### Unit Toggles

- **Speed Unit** - Toggle between mph and Mach number
- **Altitude Unit** - Toggle between feet and meters

### Visualization Toggles

Access toolbar buttons to toggle:
- **Grid** - Show/hide ground grid
- **Axis** - Show/hide 3D axis indicator
- **Wireframe** - Toggle wireframe vs solid rendering
- **Import** - Open file dialog (placeholder)

---

## Simulation Parameters

| Parameter | Range | Default | Unit | Description |
|-----------|-------|---------|------|-------------|
| Airspeed | 0 - 2000 | 150 | mph / Mach |freestream velocity |
| Altitude | 0 - 100,000 | 0 | ft / m | Flight altitude |
| Distance | 0 - 50,000 | 1,000 | km | Reference distance |
| Fuel | 0 - 100,000 | 5,000 | kg | Fuel mass |
| Angle of Attack | -45 - 45 | 0 | degrees | Pitch angle relative to airflow |

---

## Output Values

The Results Panel displays the following calculated values:

### Aerodynamic Forces

| Output | Unit | Description |
|--------|------|-------------|
| Lift Force | N | Vertical aerodynamic force |
| Drag Force | N | Horizontal resistive force |
| Side Force | N | Lateral aerodynamic force |
| Lift Coefficient (Cl) | - | Dimensionless lift coefficient |
| Drag Coefficient (Cd) | - | Dimensionless drag coefficient |

### Heat Transfer

| Output | Unit | Description |
|--------|------|-------------|
| Heat Flux | W/m² | Convective heat flux at stagnation point |
| Surface Temperature | K | Calculated surface temperature |
| Stagnation Temperature | K | Total temperature at stagnation point |
| Thermal Load | W | Total thermal energy transfer |

### Derived Parameters

| Output | Unit | Description |
|--------|------|-------------|
| Mach Number | - | Ratio of velocity to local speed of sound |
| Reynolds Number | - | Ratio of inertial to viscous forces |
| Air Density | kg/m³ | Ambient air density at altitude |
| Dynamic Pressure | Pa | Aerodynamic pressure (½ρv²) |
| Sound Speed | m/s | Local speed of sound at altitude |

---

## Project Structure

```
cfd_solver_for_science/
├── SPEC.md                 # Detailed specification document
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── build/                  # Build output directory
│   └── cfd_studio          # Compiled executable
├── src/                    # Source code
│   ├── main.c              # Application entry point
│   ├── cfd_app.c           # Application state management
│   ├── cfd_app.h           # Application header
│   ├── ui/                 # User interface components
│   │   ├── gui_framework.c # Custom UI widgets (buttons, sliders, toggles)
│   │   ├── gui_framework.h
│   │   ├── control_panel.c # Bottom panel with parameter controls
│   │   ├── control_panel.h
│   │   ├── main_window.c   # Main window management (stub)
│   │   ├── main_window.h
│   │   ├── menu_bar.c      # Menu bar (stub)
│   │   ├── menu_bar.h
│   │   ├── toolbar.c       # Toolbar with action buttons (stub)
│   │   ├── toolbar.h
│   │   ├── status_bar.c    # Status bar (stub)
│   │   ├── status_bar.h
│   │   ├── object_browser.c # Left panel model tree (stub)
│   │   ├── object_browser.h
│   │   ├── properties_panel.c # Properties panel (stub)
│   │   └── properties_panel.h
│   ├── viewport/           # 3D rendering
│   │   ├── viewport_3d.c   # Main 3D viewport rendering
│   │   ├── viewport_3d.h
│   │   ├── gl_viewport.h   # OpenGL viewport (stub)
│   │   ├── camera.h        # Camera utilities
│   │   ├── mesh_renderer.h # Mesh rendering
│   │   └── shader.h        # Shader management
│   ├── physics/            # Physics calculations
│   │   ├── atmosphere.c    # ISA atmosphere model
│   │   ├── atmosphere.h
│   │   ├── forces.c        # Aerodynamic force calculations
│   │   ├── forces.h
│   │   ├── heat_transfer.c # Convective heat transfer
│   │   ├── heat_transfer.h
│   │   ├── units.c         # Unit conversion utilities
│   │   └── units.h
│   ├── models/             # Model/mesh handling
│   │   ├── model_manager.c # Model and scene management
│   │   ├── model_manager.h
│   │   ├── mesh.h          # Mesh data structures
│   │   ├── obj_loader.c    # OBJ format loader (stub)
│   │   ├── obj_loader.h
│   │   ├── stl_loader.c    # STL format loader (stub)
│   │   └── stl_loader.h
│   ├── viz/                # Visualization utilities
│   │   ├── colormap.h      # Color mapping for scalar fields
│   │   ├── contour.h       # Contour visualization (stub)
│   │   ├── force_arrows.c  # Force vector arrows
│   │   ├── force_arrows.h
│   │   ├── streamlines.c   # Streamline computation (stub)
│   │   ├── streamlines.h
│   │   ├── vectors.c       # Vector field visualization
│   │   └── vectors.h
│   └── utils/              # Utility functions
│       ├── logger.c        # Logging system
│       ├── logger.h
│       ├── math_utils.c    # Vector and matrix math
│       └── math_utils.h
└── shaders/                # GLSL shader files (reserved)
```

---

## Architecture

### Module Overview

CFD Studio follows a modular architecture with clear separation of concerns:

#### UI Layer (`src/ui/`)

The user interface is built with custom GUI widgets using raylib's rendering primitives:

- **gui_framework** - Core UI component library providing buttons, sliders, toggles, text inputs, and panel layouts
- **control_panel** - Bottom panel containing simulation parameter sliders and derived value displays
- **toolbar** - Top toolbar with action buttons (import, grid, axis, wireframe, reset)
- **status_bar** - Bottom status bar showing control hints
- **object_browser** - Left panel for model hierarchy and layers (stub)
- **properties_panel** - Right panel for detailed object properties (stub)

#### Viewport Layer (`src/viewport/`)

3D visualization powered by raylib's OpenGL backend:

- **viewport_3d** - Main 3D rendering context, camera management, and scene composition
- **camera** - Camera controls for orbit, pan, zoom operations
- **mesh_renderer** - Mesh and geometry rendering
- **shader** - Custom shader management for advanced rendering

#### Physics Layer (`src/physics/`)

Real-time aerodynamic and thermodynamic calculations:

- **atmosphere** - ISA (International Standard Atmosphere) model providing pressure, density, temperature, and sound speed at any altitude
- **forces** - Aerodynamic force calculations including lift, drag, and side forces using coefficient-based methods
- **heat_transfer** - Convective heat transfer analysis including stagnation temperature and heat flux calculations
- **units** - Comprehensive unit conversion utilities for speed (mph ↔ Mach), altitude (ft ↔ m), distance, pressure, and temperature

#### Model Layer (`src/models/`)

Mesh and geometry management:

- **model_manager** - Central manager for loaded models and scene objects
- **mesh** - Mesh data structures for CFD geometry
- **obj_loader** - Wavefront OBJ format support (stub)
- **stl_loader** - STL stereolithography format support (stub)

#### Visualization Layer (`src/viz/`)

Advanced visualization features:

- **colormap** - Color mapping for scalar field visualization
- **contour** - Contour line generation (stub)
- **force_arrows** - 3D arrow visualization for force vectors
- **streamlines** - Streamline computation for flow visualization (stub)
- **vectors** - General vector field visualization

#### Utilities Layer (`src/utils/`)

Common utility functions:

- **logger** - Configurable logging system with debug, info, warning, and error levels
- **math_utils** - Vector and matrix mathematics utilities

---

## Technical Stack

| Component | Technology |
|-----------|------------|
| Language | C11 |
| Graphics | raylib 6.0 (OpenGL) |
| Build System | CMake 3.10+ |
| UI Framework | Custom (raylib primitives) |
| Target Platforms | Linux, macOS, Windows |

### Color Palette

The UI follows an Onshape-inspired dark theme:

| Element | Color |
|---------|-------|
| Background Dark | `#1E1E1E` |
| Panel Background | `#252526` |
| Panel Border | `#3E3E42` |
| Accent Primary | `#007ACC` |
| Accent Secondary | `#0E639C` |
| Text Primary | `#CCCCCC` |
| Text Secondary | `#858585` |

---

## Roadmap

Planned future enhancements:

- [ ] Full OBJ/STL mesh import with validation
- [ ] Pressure distribution visualization
- [ ] Streamline and pathline rendering
- [ ] Contour plots for scalar fields
- [ ] Pressure coefficient mapping
- [ ] Boundary layer visualization
- [ ] Multiple angle of attack analysis
- [ ] Drag polar generation
- [ ] Export results to CSV
- [ ] Additional atmosphere models
- [ ] Compressible flow corrections
- [ ] CFD mesh generation basics

---

## License

This project is licensed under the MIT License.

MIT License

Copyright (c) 2024 CFD Studio

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

### Development Guidelines

- Follow C11 standard
- Use meaningful variable and function names
- Add comments for complex calculations
- Test on multiple platforms when possible

---

## Acknowledgments

- [raylib](https://www.raylib.com/) - Simple and easy-to-use graphics library
- [International Standard Atmosphere](https://en.wikipedia.org/wiki/International_Standard_Atmosphere) - Atmospheric model reference
