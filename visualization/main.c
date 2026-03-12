#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_MESH_POINTS 10000
#define MAX_TRIANGLES 20000

typedef struct {
    Vector3 *vertices;
    int vertexCount;
    Vector3 *colors;
    int triangleCount;
} FlowFieldMesh;

typedef struct {
    Camera3D camera;
    FlowFieldMesh flowField;
    bool showGrid;
    bool showAxis;
    bool wireframe;
    float colorScale;
    float animationSpeed;
} VisualizationState;

void initVisualization(VisualizationState *state) {
    state->camera = (Camera3D){
        .position = (Vector3){3.0f, 2.0f, 3.0f},
        .target = (Vector3){0.0f, 0.0f, 0.0f},
        .up = (Vector3){0.0f, 1.0f, 0.0f},
        .fovy = 60.0f,
        .projection = CAMERA_PERSPECTIVE
    };
    
    state->showGrid = true;
    state->showAxis = true;
    state->wireframe = false;
    state->colorScale = 1.0f;
    state->animationSpeed = 1.0f;
    
    state->flowField.vertexCount = 0;
    state->flowField.triangleCount = 0;
    state->flowField.vertices = NULL;
    state->flowField.colors = NULL;
}

void generateFlowFieldMesh(VisualizationState *state, int nx, int ny, double *density, double *velocity_x, double *velocity_y) {
    int totalPoints = nx * ny;
    if (totalPoints > MAX_MESH_POINTS) totalPoints = MAX_MESH_POINTS;
    
    state->flowField.vertexCount = totalPoints;
    state->flowField.vertices = (Vector3 *)malloc(totalPoints * sizeof(Vector3));
    state->flowField.colors = (Vector3 *)malloc(totalPoints * sizeof(Vector3));
    
    double dx = 2.0 / (nx - 1);
    double dy = 2.0 / (ny - 1);
    double rho_max = 0.0;
    double rho_min = 1e10;
    
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            int idx = i + j * nx;
            if (idx >= totalPoints) break;
            
            double rho = density[idx];
            if (rho > rho_max) rho_max = rho;
            if (rho < rho_min) rho_min = rho;
        }
    }
    
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            int idx = i + j * nx;
            if (idx >= totalPoints) break;
            
            float x = (float)(i * dx - 1.0);
            float y = (float)(j * dy - 1.0);
            float z = 0.0f;
            
            double rho = density[idx];
            double u = velocity_x[idx];
            double v = velocity_y[idx];
            double magnitude = sqrt(u*u + v*v);
            
            float normalized = (float)((rho - rho_min) / (rho_max - rho_min + 1e-10));
            
            Vector3 color;
            if (normalized < 0.25f) {
                color = (Vector3){0.0f, normalized * 4.0f, 1.0f};
            } else if (normalized < 0.5f) {
                color = (Vector3){0.0f, 1.0f, 1.0f - (normalized - 0.25f) * 4.0f};
            } else if (normalized < 0.75f) {
                color = (Vector3){(normalized - 0.5f) * 4.0f, 1.0f, 0.0f};
            } else {
                color = (Vector3){1.0f, 1.0f - (normalized - 0.75f) * 4.0f, 0.0f};
            }
            
            state->flowField.vertices[idx] = (Vector3){x, z, y};
            state->flowField.colors[idx] = color;
        }
    }
    
    state->flowField.triangleCount = (nx - 1) * (ny - 1) * 2;
}

void renderFlowField(VisualizationState *state) {
    int nx = 50, ny = 50;
    int stride = 1;
    
    for (int j = 0; j < ny - 1; j += stride) {
        for (int i = 0; i < nx - 1; i += stride) {
            int idx = i + j * nx;
            int idx_r = (i + stride) + j * nx;
            int idx_t = i + (j + stride) * nx;
            int idx_tr = (i + stride) + (j + stride) * nx;
            
            Vector3 p0 = state->flowField.vertices[idx];
            Vector3 p1 = state->flowField.vertices[idx_r];
            Vector3 p2 = state->flowField.vertices[idx_t];
            
            Vector3 c0 = state->flowField.colors[idx];
            Color col0 = (Color){(unsigned char)(c0.x * 255), (unsigned char)(c0.y * 255), (unsigned char)(c0.z * 255), 255};
            
            DrawTriangle3D(p0, p1, p2, col0);
            
            if (idx_t < state->flowField.vertexCount && idx_tr < state->flowField.vertexCount) {
                Vector3 p3 = state->flowField.vertices[idx_tr];
                DrawTriangle3D(p1, p3, p2, col0);
            }
        }
    }
}

void drawUI(VisualizationState *state) {
    int screenWidth = GetScreenWidth();
    int panelWidth = 250;
    
    DrawRectangle(0, 0, panelWidth, GetScreenHeight(), (Color){37, 37, 38, 255});
    DrawRectangle(panelWidth, 0, 1, GetScreenHeight(), (Color){62, 62, 66, 255});
    
    int y = 20;
    int x = 15;
    
    DrawText("CFD Visualization", x, y, 20, (Color){0, 122, 204, 255});
    y += 40;
    
    DrawText("Controls:", x, y, 16, (Color){204, 204, 204, 255});
    y += 25;
    
    DrawText("Left Mouse - Rotate", x, y, 14, (Color){133, 133, 133, 255});
    y += 20;
    DrawText("Middle Mouse - Pan", x, y, 14, (Color){133, 133, 133, 255});
    y += 20;
    DrawText("Right Mouse - Zoom", x, y, 14, (Color){133, 133, 133, 255});
    y += 20;
    DrawText("Scroll - Zoom In/Out", x, y, 14, (Color){133, 133, 133, 255});
    y += 35;
    
    DrawText("Options:", x, y, 16, (Color){204, 204, 204, 255});
    y += 25;
    
    Rectangle toggleRect = {x, (float)y, 20, 20};
    if (CheckCollisionPointRec(GetMousePosition(), toggleRect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->showGrid = !state->showGrid;
        }
    }
    DrawRectangleRec(toggleRect, state->showGrid ? (Color){0, 122, 204, 255} : (Color){80, 80, 80, 255});
    DrawText("Show Grid", x + 30, y + 2, 14, (Color){204, 204, 204, 255});
    y += 30;
    
    Rectangle toggleRect2 = {x, (float)y, 20, 20};
    if (CheckCollisionPointRec(GetMousePosition(), toggleRect2)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->showAxis = !state->showAxis;
        }
    }
    DrawRectangleRec(toggleRect2, state->showAxis ? (Color){0, 122, 204, 255} : (Color){80, 80, 80, 255});
    DrawText("Show Axis", x + 30, y + 2, 14, (Color){204, 204, 204, 255});
    y += 30;
    
    Rectangle toggleRect3 = {x, (float)y, 20, 20};
    if (CheckCollisionPointRec(GetMousePosition(), toggleRect3)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->wireframe = !state->wireframe;
        }
    }
    DrawRectangleRec(toggleRect3, state->wireframe ? (Color){0, 122, 204, 255} : (Color){80, 80, 80, 255});
    DrawText("Wireframe", x + 30, y + 2, 14, (Color){204, 204, 204, 255});
    y += 40;
    
    DrawText("Legend:", x, y, 16, (Color){204, 204, 204, 255});
    y += 25;
    
    for (int i = 0; i < 10; i++) {
        float t = i / 9.0f;
        Color col;
        if (t < 0.25f) {
            col = (Color){0, (unsigned char)(t * 4 * 255), 255, 255};
        } else if (t < 0.5f) {
            col = (Color){0, 255, (unsigned char)((1.0f - (t - 0.25f) * 4.0f) * 255), 255};
        } else if (t < 0.75f) {
            col = (Color){(unsigned char)((t - 0.5f) * 4.0f * 255), 255, 0, 255};
        } else {
            col = (Color){255, (unsigned char)((1.0f - (t - 0.75f) * 4.0f) * 255), 0, 255};
        }
        DrawRectangle(x + i * 20, y, 20, 15, col);
    }
    y += 20;
    DrawText("Low", x, y, 10, (Color){133, 133, 133, 255});
    DrawText("High", x + 180, y, 10, (Color){133, 133, 133, 255});
}

void runVisualization(int argc, char **argv) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "CFD Solver - Flow Visualization");
    
    VisualizationState state;
    initVisualization(&state);
    
    int nx = 50, ny = 50;
    int totalPoints = nx * ny;
    double *density = (double *)malloc(totalPoints * sizeof(double));
    double *vel_x = (double *)malloc(totalPoints * sizeof(double));
    double *vel_y = (double *)malloc(totalPoints * sizeof(double));
    
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            int idx = i + j * nx;
            double x = (double)i / (nx - 1);
            double y = (double)j / (ny - 1);
            
            double rho = 1.0 + 0.5 * sin(3.0 * M_PI * x) * sin(3.0 * M_PI * y);
            density[idx] = rho;
            vel_x[idx] = 1.0 + 0.2 * cos(2.0 * M_PI * y);
            vel_y[idx] = 0.1 * sin(2.0 * M_PI * x);
        }
    }
    
    generateFlowFieldMesh(&state, nx, ny, density, vel_x, vel_y);
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        UpdateCamera(&state.camera, CAMERA_ORBITAL);
        
        BeginDrawing();
        ClearBackground((Color){30, 30, 30, 255});
        
        BeginMode3D(state.camera);
        
        if (state.showGrid) {
            DrawGrid(20, 1.0f);
        }
        
        if (state.showAxis) {
            DrawLine3D((Vector3){0, 0, 0}, (Vector3){2, 0, 0}, RED);
            DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 2, 0}, GREEN);
            DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 0, 2}, BLUE);
        }
        
        renderFlowField(&state);
        
        EndMode3D();
        
        drawUI(&state);
        
        EndDrawing();
    }
    
    free(density);
    free(vel_x);
    free(vel_y);
    if (state.flowField.vertices) free(state.flowField.vertices);
    if (state.flowField.colors) free(state.flowField.colors);
    
    CloseWindow();
}

int main(int argc, char **argv) {
    runVisualization(argc, argv);
    return 0;
}
