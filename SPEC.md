# CFD Studio - Computational Fluid Dynamics Visualization Suite

## 1. Project Overview

**Project Name:** CFD Studio  
**Type:** Desktop Application (Pure C with raylib)  
**Core Functionality:** A CFD visualization application with Onshape-inspired UI, 3D model import, and real-time aerodynamic analysis
**Target Users:** Engineers, researchers, students working on aerodynamic analysis and fluid dynamics

---

## 2. UI/UX Specification

### 2.1 Window Structure

**Main Window:**
- Dimensions: 1600x900 minimum, resizable
- Style: Dark theme (Onshape-inspired)
- Layout: Multi-panel with toolbar

**Layout Regions:**
```
+------------------------------------------------------------------+
|  Toolbar (Import, Grid, Axis, Wireframe, Reset)                 |
+------------------+-------------------------------+---------------+
|                  |                               |               |
|  Object Browser  |     3D Viewport              |  Results      |
|  (Left Panel)   |     (raylib/OpenGL)           |  Panel        |
|                  |                               |  (Right)      |
|  - Model tree    |     Mouse controls:           |               |
|  - Layers        |     - Left: Rotate            |  - Forces     |
|                  |     - Middle: Pan             |  - Heat       |
|                  |     - Scroll: Zoom            |               |
+------------------+-------------------------------+---------------+
|                    Control Panel (Bottom)                        |
|  [Sliders: Speed | Altitude | Distance | Fuel | AoA]           |
|  [Toggles: mph/Mach | ft/m]]                                     |
+------------------------------------------------------------------+
|  Status: Controls help                                           |
+------------------------------------------------------------------+
```

### 2.2 Visual Design

**Color Palette:**
- Background Dark: `#1E1E1E`
- Panel Background: `#252526`
- Panel Border: `#3E3E42`
- Accent Primary: `#007ACC` (blue)
- Accent Secondary: `#0E639C`
- Text Primary: `#CCCCCC`
- Text Secondary: `#858585`

**Typography:**
- Font Family: System default (raylib)
- Heading Size: 16px bold
- Body Size: 14px

### 2.3 Components

**Toolbar:**
- Import button (placeholder for file dialog)
- Grid toggle
- Axis toggle
- Wireframe toggle
- Camera reset

**3D Viewport:**
- raylib OpenGL rendering
- Mouse controls: rotate (left), pan (middle), zoom (scroll)
- Grid floor toggle
- Axis indicator (3D triad)

**Control Panel:**
- Airspeed slider (0-2000)
- Altitude slider (0-100000 ft/m)
- Distance slider (0-50000 km)
- Fuel slider (0-100000 kg)
- Angle of Attack slider (-45 to 45 deg)
- Unit toggles: mph/Mach, ft/m
- Derived values display: Mach, Reynolds, Density, Dynamic Pressure, Sound Speed

**Results Panel:**
- Lift force (N)
- Drag force (N)
- Side force (N)
- Lift coefficient (Cl)
- Drag coefficient (Cd)
- Heat flux (W/m²)
- Surface temperature (K)
- Stagnation temperature (K)
- Thermal load (W)

---

## 3. Functional Specification

### 3.1 Core Features

**3D Model Import:**
- Supported formats: STL, OBJ
- Placeholder for drag-and-drop

**Visualization:**
- 3D viewport with raylib
- Wireframe and solid modes
- Grid and axis display
- Camera orbit/pan/zoom

**Physics Calculations:**
- Lift and drag coefficients
- Forces (lift, drag, side)
- Heat transfer (convective heat flux, stagnation temperature)
- Atmospheric properties (density, pressure, sound speed)
- Mach number and Reynolds number

### 3.2 Simulation Parameters

| Parameter | Range | Default | Unit |
|-----------|-------|---------|------|
| Airspeed | 0-2000 | 150 | mph/Mach |
| Altitude | 0-100000 | 0 | ft/m |
| Distance | 0-50000 | 1000 | km |
| Fuel | 0-100000 | 5000 | kg |
| Angle of Attack | -45 to 45 | 0 | degrees |

### 3.3 Unit Conversion

**Speed:**
- mph ↔ Mach
- Display with toggle

**Altitude:**
- feet ↔ meters
- Display with toggle

---

## 4. Technical Implementation

### 4.1 Architecture (Pure C + raylib)

```
src/
├── main.c                    # Entry point, raylib window and game loop
├── cfd_app.c                # Application state management
├── cfd_app.h                 
├── ui/
│   ├── gui_framework.c       # Custom UI components (buttons, sliders, toggles)
│   ├── gui_framework.h
│   └── control_panel.c       # Bottom panel with sliders
│   └── control_panel.h
├── viewport/
│   ├── viewport_3d.c         # 3D rendering with raylib
│   └── viewport_3d.h
├── physics/
│   ├── units.c               # Unit conversions
│   ├── units.h
│   ├── atmosphere.c          # ISA atmosphere model
│   ├── atmosphere.h
│   ├── forces.c              # Force calculations
│   ├── forces.h
│   └── heat_transfer.c      # Heat transfer calculations
│   └── heat_transfer.h
└── models/
    ├── mesh.h
    └── model_manager.c
```

### 4.2 Dependencies

- raylib (6.0) - Graphics and windowing
- math library (libm)
- dl library (dynamic loading)
- pthread (threading)
- X11 (windowing system)

---

## 5. Build Instructions

```bash
mkdir build && cd build
cmake ..
make
./cfd_studio
```

**Runtime library path:**
```bash
export LD_LIBRARY_PATH=/home/grep/local/home/grep/local/lib:$LD_LIBRARY_PATH
```

---

## 6. Acceptance Criteria

- [x] CMakeLists.txt builds without errors
- [x] Executable links and builds
- [x] Main window renders with dark theme
- [x] Three-panel layout displays correctly
- [x] All sliders respond to input
- [x] Unit toggles switch values correctly
- [x] Camera controls work (rotate, pan, zoom)
- [x] Force calculations display in results panel
- [x] Heat transfer calculations display
- [x] Derived parameters update in real-time
