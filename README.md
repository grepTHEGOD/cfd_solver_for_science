# CFD Solver for Science

A comprehensive Computational Fluid Dynamics (CFD) software package for aerodynamic analysis and flow simulation.

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Installation](#installation)
4. [Building from Source](#building-from-source)
5. [Running the Solver](#running-the-solver)
6. [Solver Options](#solver-options)
7. [Test Cases](#test-cases)
8. [Physics Modules](#physics-modules)
9. [Visualization](#visualization)
10. [Architecture](#architecture)
11. [Theory](#theory)
12. [Output](#output)
13. [Troubleshooting](#troubleshooting)

---

## Overview

CFD Solver for Science is a production-ready computational fluid dynamics package designed for aerodynamic analysis. It implements a 2D compressible Navier-Stokes solver using the finite volume method with Roe flux splitting, suitable for simulating flows from subsonic to hypersonic regimes.

### Applications

- Aerodynamic force and moment prediction
- Heat transfer analysis for high-speed vehicles
- Flow field visualization
- Educational tool for CFD fundamentals
- Preliminary design analysis for aerospace vehicles

---

## Features

### CFD Solver
- **2D Compressible Navier-Stokes Equations**: Full system of conservation equations for mass, momentum, and energy
- **Roe Flux Splitting**: First-order accurate Riemann solver for robust shock capturing
- **Structured Grid Support**: Configurable grid resolution up to 512x512 cells
- **Multiple Flow Regimes**: Subsonic, transonic, supersonic, and hypersonic
- **CFL-Based Time Stepping**: Automatic time step calculation for stability
- **Convergence Monitoring**: Real-time residual tracking with configurable tolerance
- **Angle of Attack**: Configurable inflow angle for lift analysis

### Physics Modules

#### Atmosphere Model (`atmosphere.c/h`)
- ISA (International Standard Atmosphere) model
- Temperature, pressure, density calculation
- Speed of sound computation
- Dynamic and kinematic viscosity
- Reynolds number calculation for given reference length

#### Aerodynamics (`forces.c/h`)
- Lift coefficient calculation with linear and stall regions
- Drag coefficient with induced drag modeling
- Dynamic pressure computation
- Reference area integration

#### Heat Transfer (`heat_transfer.c/h`)
- Stagnation temperature calculation
- Recovery factor computation (turbulent/laminar)
- Convective heat flux using correlation-based Nusselt number
- Wall temperature estimation

### Visualization

- Real-time 3D rendering with raylib
- Density-based color mapping (blue=low, red=high)
- Interactive camera: orbit (left mouse), pan (middle mouse), zoom (scroll)
- Toggle controls for grid and axis
- Color legend for flow field values

### User Interface

- Interactive terminal-based menu system (TUI)
- Professional color-coded output
- Animated progress indicators
- Clear status messages

---

## Installation

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake libraylib-dev

# Fedora
sudo dnf install gcc cmake raylib-devel

# Arch Linux
sudo pacman -S gcc cmake raylib

# macOS (Homebrew)
brew install cmake raylib
```

### Verification

```bash
# Check compiler
gcc --version

# Check CMake
cmake --version

# Check raylib (optional)
pkg-config --modversion raylib
```

---

## Building from Source

```bash
# Clone or navigate to project directory
cd cfd_solver_for_science

# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build all targets
make -j$(nproc)

# Verify executables
ls -la solver/cfd_solver tui/tui_executable visualization/visualization
```

### Build Options

| Option | Description | Default |
|--------|-------------|---------|
| `-DCMAKE_BUILD_TYPE=Release` | Optimized build | Debug |
| `-DCMAKE_C_FLAGS` | Compiler flags | -Wall -Wextra |

---

## Running the Solver

### Quick Start

```bash
# Run default case (subsonic)
./solver/cfd_solver

# Run specific test case
./solver/cfd_solver --mach 2.0 --iter 2000

# View help
./solver/cfd_solver --help
```

### Interactive Mode

```bash
$ ./solver/cfd_solver

  ╔══════════════════════════════════════════════════════════════╗
  ║          CFD SOLVER FOR SCIENCE - VERSION 2.1                ║
  ║     2D Compressible Navier-Stokes Flow Solver               ║
  ╚══════════════════════════════════════════════════════════════╝

  Available test cases:
  ─────────────────────────────────────────────────
  1.  Subsonic flow over flat plate (M=0.3, α=0°)
  2.  Transonic flow (M=0.8, α=0°)
  3.  Supersonic flow (M=2.0, α=0°)
  4.  Supersonic flow with angle of attack (M=2.0, α=5°)
  5.  Hypersonic flow (M=5.0, α=10°)
  6.  Custom parameters
  0.  Exit
  ─────────────────────────────────────────────────
```

### TUI Mode

```bash
./tui/tui_executable
```

---

## Solver Options

### Command-Line Arguments

| Argument | Description | Type | Default |
|----------|-------------|------|---------|
| `--mesh N` | Grid resolution (NxN) | integer | 100 |
| `--iter N` | Maximum iterations | integer | 5000 |
| `--mach M` | Freestream Mach number | float | 0.3 |
| `--alpha A` | Angle of attack (degrees) | float | 0 |
| `--temp T` | Freestream temperature (K) | float | 300 |
| `--pressure P` | Freestream pressure (Pa) | float | 101325 |
| `--help, -h` | Display help message | flag | - |

### Examples

```bash
# Subsonic flow (airfoil analysis)
./solver/cfd_solver --mesh 200 --mach 0.3 --iter 5000

# Transonic flow
./solver/cfd_solver --mesh 256 --mach 0.8 --iter 10000

# Supersonic flow (M=2)
./solver/cfd_solver --mesh 300 --mach 2.0 --alpha 5.0 --iter 10000

# Hypersonic re-entry (M=5)
./solver/cfd_solver --mesh 400 --mach 5.0 --alpha 10.0 --iter 20000

# Custom conditions
./solver/cfd_solver --mach 3.0 --temp 250 --pressure 85000
```

---

## Test Cases

### 1. Subsonic Flow (M = 0.3)

**Description**: Low-speed flow typical of aircraft cruise conditions.

```bash
./solver/cfd_solver --mach 0.3 --iter 2000
```

**Expected Results**:
- Nearly uniform flow field
- Minimal pressure gradients
- Low heat transfer

### 2. Transonic Flow (M = 0.8)

**Description**: Near-sonic flow with formation of shock waves.

```bash
./solver/cfd_solver --mach 0.8 --iter 5000
```

**Characteristics**:
- Mixed subsonic/supersonic regions
- Presence of shock waves
- Higher pressure gradients

### 3. Supersonic Flow (M = 2.0)

**Description**: Fully supersonic flow with oblique shocks.

```bash
./solver/cfd_solver --mach 2.0 --iter 5000
```

**Characteristics**:
- All streamlines supersonic
- Oblique shock waves possible
- Significant temperature rise

### 4. Hypersonic Flow (M = 5.0)

**Description**: High-Mach flow typical of re-entry vehicles.

```bash
./solver/cfd_solver --mach 5.0 --alpha 10.0 --iter 10000
```

**Characteristics**:
- Very high stagnation temperatures
- Strong shock waves
- Significant aerodynamic heating

---

## Physics Modules

### Atmosphere Model

The ISA (International Standard Atmosphere) model provides:

```
T = T₀ - L × h                      (troposphere)
p = p₀ × (1 - L×h/T₀)^(g/(R×L))    (pressure)
ρ = p/(R×T)                        (density)
a = √(γ×R×T)                       (speed of sound)
```

**Constants**:
- Sea level pressure: 101325 Pa
- Sea level temperature: 288.15 K
- Gas constant R: 287.05 J/(kg·K)
- Ratio of specific heats γ: 1.4

### Aerodynamic Forces

**Lift Coefficient** (linear region):
```
Cₗ = Cₗα × α
```

**Drag Coefficient**:
```
C₅ = C₅₀ + k × Cₗ²/(π×e×AR)
```

Where:
- Cₗα: lift curve slope (~2π per radian for thin airfoil)
- C₅₀: zero-lift drag
- k: induced drag factor
- AR: aspect ratio
- e: Oswald efficiency

### Heat Transfer

**Stagnation Temperature**:
```
T₀ = T∞ × (1 + (γ-1)/2 × M²)
```

**Recovery Factor**:
- Laminar: r = √Pr
- Turbulent: r = Pr^(1/3)

**Convective Heat Flux**:
```
q = h × (T₀ - Tₜₐₗₗ)
```

---

## Visualization

### Running Visualization

```bash
./visualization/visualization
```

### Controls

| Control | Action |
|---------|--------|
| Left Mouse | Orbit camera |
| Middle Mouse | Pan camera |
| Right Mouse | Zoom |
| Scroll Wheel | Zoom in/out |

### UI Panel

- **Show Grid**: Toggle ground grid
- **Show Axis**: Toggle 3D coordinate axes
- **Wireframe**: Toggle wireframe mode

### Color Legend

```
Blue    → Low density
Cyan    → Medium-low
Green   → Medium
Yellow  → Medium-high
Red     → High density
```

---

## Architecture

```
cfd_solver_for_science/
├── solver/
│   ├── main.c              # Entry point, CLI parser
│   ├── cfd_solver.c        # Navier-Stokes implementation
│   ├── cfd_solver.h        # Public API
│   ├── CMakeLists.txt      # Solver build config
│   └── physics/
│       ├── atmosphere.c/h   # ISA atmosphere model
│       ├── forces.c/h      # Aerodynamic calculations
│       └── heat_transfer.c/h  # Convective heat transfer
├── tui/
│   ├── main.c              # Terminal UI
│   └── CMakeLists.txt      # TUI build config
├── visualization/
│   ├── main.c              # raylib 3D visualization
│   └── CMakeLists.txt      # Visualization build config
├── CMakeLists.txt          # Root build configuration
└── README.md
```

### Data Flow

```
User Input → Parser → Initial Conditions → Solver Loop
                                              ↓
                                        Flux Calculation
                                              ↓
                                        Time Integration
                                              ↓
                                        Boundary Conditions
                                              ↓
                                         Convergence Check
                                              ↓
Output ← Forces ← Heat Transfer ← Flow Field Solution
```

---

## Theory

### Governing Equations

The 2D compressible Navier-Stokes equations in conservation form:

**Mass**:
```
∂ρ/∂t + ∂(ρu)/∂x + ∂(ρv)/∂y = 0
```

**Momentum x**:
```
∂(ρu)/∂t + ∂(ρu² + p)/∂x + ∂(ρuv)/∂y = 0
```

**Momentum y**:
```
∂(ρv)/∂t + ∂(ρuv)/∂x + ∂(ρv² + p)/∂y = 0
```

**Energy**:
```
∂E/∂t + ∂[(E + p)u]/∂x + ∂[(E + p)v]/∂y = 0
```

Where:
- ρ: density
- u, v: velocity components
- p: pressure
- E: total energy per unit volume

**Equation of State** (ideal gas):
```
p = (γ - 1) × (E - 0.5 × ρ × (u² + v²))
```

### Numerical Method

1. **Spatial Discretization**: Finite Volume Method (FVM)
2. **Flux Calculation**: Roe approximate Riemann solver
3. **Time Integration**: Explicit first-order Euler
4. **Time Step**: CFL condition with configurable safety factor

---

## Output

### Solver Output Example

```
  CFD Solver - 2D Compressible Navier-Stokes
  ============================================
  Grid: 100 x 100
  Max iterations: 5000
  Residual tolerance: 1.00e-06

  Iter     0: Residual = 1.234e-05, dt = 1.234e-06
  Iter   100: Residual = 5.678e-07, dt = 1.234e-06
  Iter   200: Residual = 2.345e-08, dt = 1.234e-06

  Solution converged in 234 iterations (1.45 seconds)

  Forces:
    Lift: 12345.67 N
    Drag: 2345.67 N

  Heat Transfer:
    Heat flux: 45678.90 W/m²
    Wall temperature: 543.21 K
```

### Output Parameters

| Parameter | Description | Unit |
|-----------|-------------|------|
| Lift | Aerodynamic lift force | N |
| Drag | Aerodynamic drag force | N |
| Heat Flux | Convective heat flux | W/m² |
| Wall Temp | Surface temperature | K |
| Stagnation Temp | Total temperature | K |

---

## Troubleshooting

### Build Errors

**raylib not found**:
```bash
# Install raylib
sudo apt-get install libraylib-dev

# Or use system library path
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

### Runtime Errors

**Segmentation fault**:
- Grid size too large for available memory
- Try reducing `--mesh` value

**No convergence**:
- Increase iterations with `--iter`
- Reduce CFL number in source code
- Check for proper boundary conditions

### Visualization Issues

**Window won't open**:
- Ensure X11 forwarding is enabled (SSH with -X)
- Check display environment: `echo $DISPLAY`

**Black screen**:
- Update graphics drivers
- Try reducing grid resolution

---

## Performance Notes

- Grid size scales memory as O(N²)
- 100x100 grid: ~1 second per 100 iterations
- 200x200 grid: ~4 seconds per 100 iterations
- 400x400 grid: ~16 seconds per 100 iterations

### Recommended Settings

| Flow Type | Grid Size | Iterations |
|-----------|-----------|------------|
| Subsonic | 100-200 | 2000-5000 |
| Transonic | 200-300 | 5000-10000 |
| Supersonic | 200-400 | 5000-10000 |
| Hypersonic | 300-512 | 10000-20000 |

---

## License

MIT License

Copyright (c) 2026 CFD Solver for Science

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

## References

### Fundamentals

1. Anderson, J. D. (1995). *Computational Fluid Dynamics: The Basics with Applications*. McGraw-Hill.
2. Hirsch, C. (2007). *Numerical Computation of Internal and External Flows*. Elsevier.
3. Blazek, J. (2015). *Computational Fluid Dynamics: Principles and Applications*. Elsevier.
4. Tannehill, J. C., Anderson, D. A., & Pletcher, R. H. (1997). *Computational Fluid Mechanics and Heat Transfer*. Taylor & Francis.
5. Ferziger, J. H., & Peric, M. (2002). *Computational Methods for Fluid Dynamics*. Springer.

### Numerical Methods

6. Roe, P. L. (1981). "Approximate Riemann Solvers, Parameter Vectors, and Difference Schemes". *Journal of Computational Physics*, 43(2), 357-372.
7. van Leer, B. (1979). "Towards the Ultimate Conservative Difference Scheme V. A Second-Order Sequel to Godunov's Method". *Journal of Computational Physics*, 32(1), 101-136.
8. Harten, A., Lax, P. D., & van Leer, B. (1983). "On Upstream Differencing and Godunov-Type Schemes for Hyperbolic Conservation Laws". *SIAM Review*, 25(1), 35-61.
9. Osher, S., & Solomon, F. (1982). "Upwind Difference Schemes for Hyperbolic Systems of Conservation Laws". *Mathematics of Computation*, 38(158), 339-374.
10. Toro, E. F. (2009). *Riemann Solvers and Numerical Methods for Fluid Dynamics*. Springer.

### Aerodynamics & Heat Transfer

11. Anderson, J. D. (2003). *Modern Compressible Flow: With Historical Perspective*. McGraw-Hill.
12. White, F. M. (2011). *Viscous Fluid Flow*. McGraw-Hill.
13. Schlichting, H., & Gersten, K. (2017). *Boundary-Layer Theory*. Springer.
14. John, J. E. A., & Haberman, W. L. (1988). *Introduction to Fluid Mechanics*. Prentice Hall.

### Atmosphere & Gas Dynamics

15. U.S. Standard Atmosphere (1976). *U.S. Standard Atmosphere*. U.S. Government Printing Office.
16. Courant, R., & Friedrichs, K. O. (1948). *Supersonic Flow and Shock Waves*. Interscience Publishers.
