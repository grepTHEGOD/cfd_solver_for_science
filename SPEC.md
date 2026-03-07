# CFD Studio - Computational Fluid Dynamics Visualization Suite

## 1. Project Overview

**Project Name:** CFD Studio  
**Type:** Desktop Application (Pure C with GTK3 + OpenGL)  
**Core Functionality:** A ParaView-inspired CFD visualization application with integrated controls, multi-format 3D model import, and real-time parameter manipulation  
**Target Users:** Engineers, researchers, students working on aerodynamic analysis and fluid dynamics

---

## 2. UI/UX Specification

### 2.1 Window Structure

**Main Window:**
- Dimensions: 1600x900 minimum, resizable
- Style: Dark theme (like ParaView)
- Layout: Multi-panel with collapsible sidebars

**Layout Regions:**
```
+------------------------------------------------------------------+
|  Menu Bar                                                         |
+------------------------------------------------------------------+
|  Toolbar (Import, Export, Play, Pause, Reset)                   |
+------------------+-------------------------------+---------------+
|                  |                               |               |
|  Object Browser  |     3D Viewport               |  Properties   |
|  (Left Panel)    |     (OpenGL)                  |  Panel        |
|                  |                               |  (Right)      |
|  - Model tree    |                               |               |
|  - Layers        |                               |  - Materials  |
|  - Data arrays   |                               |  - Colors     |
|                  |                               |  - Filters    |
+------------------+-------------------------------+---------------+
|                    Control Panel (Bottom)                        |
|  [Sliders: Speed, Angle, Altitude, Pressure, Temperature...]    |
+------------------------------------------------------------------+
|  Status Bar: Units | Simulation Status | Performance Metrics     |
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
- Success: `#4EC9B0`
- Warning: `#DDB100`
- Error: `#F14C4C`

**Typography:**
- Font Family: "Segoe UI", "Cantarell", sans-serif
- Heading Size: 14px bold
- Body Size: 12px
- Small/Labels: 10px

**Spacing System:**
- Base unit: 4px
- Panel padding: 8px
- Element gap: 4px
- Section gap: 16px

### 2.3 Components

**Object Browser (Left Panel - 250px width):**
- Tree view with collapsible nodes
- Icons for different object types
- Checkboxes for visibility toggle
- Right-click context menu

**3D Viewport (Center):**
- OpenGL rendering context
- Mouse controls: rotate (left), pan (middle), zoom (scroll)
- Grid floor toggle
- Axis indicator (3D triad)
- Camera presets (Top, Front, Side, Iso)

**Properties Panel (Right Panel - 300px width):**
- Tabbed interface: Display, Data, Filters
- Color mapping controls
- Opacity slider
- Representation selector (Surface, Wireframe, Points)

**Control Panel (Bottom - 200px height, collapsible):**
```
+---------------------------------------------------------------+
|  [<<] Simulation Parameters                                   |
+---------------------------------------------------------------+
|  Airspeed:     [====|=========] 150.0 mph    [mph▼]           |
|  Altitude:     [==|===========] 10000 ft     [feet▼]          |
|  Angle of Att: [==|===========] 5.0 deg                      |
|  Pressure:     [====|=========] 101.325 kPa                   |
|  Temperature:  [======|=======] 288.15 K                       |
|  Density:      [====|=========] 1.225 kg/m³                   |
|  Mach Number:  0.20                                           |
|  Reynolds #:   2.5e6                                          |
+---------------------------------------------------------------+
```

**Unit Toggles:**
- Altitude: feet ↔ meters (toggle button with indicator light)
- Speed: mph ↔ mach (toggle button with indicator light)
- Temperature: K ↔ °C ↔ °F
- Pressure: Pa ↔ kPa ↔ psi ↔ atm

---

## 3. Functional Specification

### 3.1 Core Features

**3D Model Import:**
- Supported formats: STL (binary and ASCII), OBJ, custom CFD mesh format
- Drag-and-drop support
- Recent files list
- Model centering and scaling

**Visualization Types:**
1. **Pressure Contours** - Color-mapped pressure distribution
2. **Velocity Vectors** - Arrow glyphs showing flow direction/magnitude
3. **Streamlines** - Animated particle paths
4. **Heat Map** - Temperature distribution
5. **Force Vectors** - Lift, drag, moment arrows on model
6. **Wall Shear Stress** - Friction visualization
7. **Iso-surfaces** - 3D regions of constant value

**Data Display:**
- Drag coefficient (Cd)
- Lift coefficient (Cl)
- Pressure differential
- Heat flux (W/m²)
- Total forces (N)
- Moments (N·m)

### 3.2 Simulation Parameters (Sliders)

| Parameter | Range | Default | Unit |
|-----------|-------|---------|------|
| Airspeed | 0-2000 | 150 | mph/mach |
| Altitude | 0-100000 | 0 | feet/m |
| Angle of Attack | -45 to 45 | 0 | degrees |
| Freestream Pressure | 50-150 | 101.325 | kPa |
| Freestream Temperature | 150-400 | 288.15 | K |
| Turbulence Intensity | 0-100 | 5 | % |
| Viscosity | 1e-6 to 1e-3 | 1.8e-5 | Pa·s |

### 3.3 Unit Conversion System

**Altitude:**
- 1 foot = 0.3048 meters
- Display toggle with live conversion

**Speed:**
- 1 mph = 0.44704 m/s
- 1 Mach (sea level, std) = 343 m/s
- Mach number calculated from altitude (sound speed varies)

**Derived Calculations:**
- Reynolds Number: Re = ρVL/μ
- Dynamic Pressure: q = 0.5ρV²
- Mach Number: M = V/a (where a = √(γRT))

### 3.4 Additional Feature Ideas

1. **Animation Mode** - Animate parameter changes over time
2. **Multi-case Comparison** - Side-by-side result comparison
3. **Cut Plane Widget** - Define arbitrary slicing planes
4. **Probe Tool** - Click to get exact values at point
5. **Report Generator** - Export PDF/HTML analysis reports
6. **Cloud Simulation** - Connect to remote HPC clusters
7. **VR View** - Immersive 3D inspection
8. **Video Export** - Record animations
9. **Scripting Console** - Python scripting for automation
10. **Parametric Study** - Sweep parameters automatically

### 3.5 Architecture (Pure C)

```
src/
├── main.c                    # Entry point, GTK app setup
├── cfd_app.c                 # Application state management
├── cfd_app.h                 
├── ui/
│   ├── main_window.c          # Window creation, layout
│   ├── main_window.h
│   ├── menu_bar.c             # Menu handlers
│   ├── menu_bar.h
│   ├── toolbar.c              # Toolbar buttons
│   ├── toolbar.h
│   ├── object_browser.c       # Left panel tree view
│   ├── object_browser.h
│   ├── properties_panel.c      # Right panel controls
│   ├── properties_panel.h
│   ├── control_panel.c        # Bottom sliders
│   ├── control_panel.h
│   ├── status_bar.c           # Status display
│   └── status_bar.h
├── viewport/
│   ├── gl_viewport.c          # OpenGL viewport widget
│   ├── gl_viewport.h
│   ├── camera.c               # Camera orbit/pan/zoom
│   ├── camera.h
│   ├── shader.c               # GLSL shaders
│   ├── shader.h
│   ├── mesh_renderer.c        # Mesh drawing
│   └── mesh_renderer.h
├── models/
│   ├── mesh.c                 # Mesh data structure
│   ├── mesh.h
│   ├── stl_loader.c           # STL file import
│   ├── stl_loader.h
│   ├── obj_loader.c           # OBJ file import
│   ├── obj_loader.h
│   └── model_manager.c        # Model CRUD
│   └── model_manager.h
├── viz/
│   ├── colormap.c             # Color mapping utilities
│   ├── colormap.h
│   ├── contour.c              # Contour generation
│   ├── contour.h
│   ├── vectors.c              # Vector field glyphs
│   ├── vectors.h
│   ├── streamlines.c          # Streamline computation
│   ├── streamlines.h
│   └── force_arrows.c         # Force visualization
│   └── force_arrows.h
├── physics/
│   ├── units.c                # Unit conversions
│   ├── units.h
│   ├── atmosphere.c           # ISA atmosphere model
│   ├── atmosphere.h
│   ├── forces.c               # Force calculations
│   └── forces.h
└── utils/
    ├── math_utils.c           # Vector/matrix math
    ├── math_utils.h
    ├── logger.c               # Logging
    └── logger.h
```

---

## 4. Technical Requirements

### 4.1 Dependencies

**Required Libraries:**
- GTK3 (>= 3.22) - GUI toolkit
- GLEW (>= 2.0) - OpenGL extension management
- GLM (header-only) - Math library OR custom math
- libepoxy - OpenGL function loading

**Build System:**
- CMake >= 3.10

### 4.2 OpenGL Requirements

- OpenGL 3.3 Core Profile minimum
- GLSL 330 shaders
- Vertex Buffer Objects (VBOs)
- VAO (Vertex Array Objects)

### 4.3 Acceptance Criteria

- [ ] CMakeLists.txt builds without errors
- [ ] Links against GTK3 and OpenGL libraries
- [ ] Executable launches on Linux
- [ ] Main window renders with dark theme
- [ ] Three-panel layout displays correctly
- [ ] All sliders respond to input
- [ ] Unit toggles switch values correctly
- [ ] Can import STL file and display in viewport
- [ ] Camera controls work (rotate, pan, zoom)
- [ ] Contour coloring displays on model
- [ ] Unit conversions are accurate
- [ ] Derived parameters update in real-time

---

## 5. Implementation Notes

### 5.1 OpenGL Shaders

**Vertex Shader:** Transform vertices, pass normals and positions to fragment
**Fragment Shader:** Simple Phong lighting + optional color mapping

### 5.2 Model Data Structure

```c
typedef struct {
    float *vertices;    // Position (x, y, z) per vertex
    float *normals;     // Normal (nx, ny, nz) per vertex
    float *colors;      // RGBA per vertex (for visualization)
    float *uvs;         // Texture coords if needed
    unsigned int *indices;
    int vertex_count;
    int face_count;
} Mesh;
```

### 5.3 Color Mapping

Rainbow colormap: Blue (low) → Cyan → Green → Yellow → Red (high)
