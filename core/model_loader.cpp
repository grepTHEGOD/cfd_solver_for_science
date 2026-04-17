#include "model_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace ModelLoader {

static double computeAirDensity(double altitude) {
    const double R_GAS = 287.05;
    const double P0 = 101325.0;
    const double T0 = 288.15;
    const double LAPSE_RATE = 0.0065;
    const double H_TROPO = 11000.0;
    
    double T, P;
    if (altitude < H_TROPO) {
        T = T0 - LAPSE_RATE * altitude;
        double exponent = 9.80665 / (R_GAS * LAPSE_RATE);
        P = P0 * std::pow(1.0 - (LAPSE_RATE * altitude) / T0, exponent);
    } else {
        T = T0 - LAPSE_RATE * H_TROPO;
        double p_tropo = P0 * std::pow(1.0 - (LAPSE_RATE * H_TROPO) / T0, 9.80665 / (R_GAS * LAPSE_RATE));
        P = p_tropo * std::exp(-9.80665 * (altitude - H_TROPO) / (R_GAS * T));
    }
    
    return P / (R_GAS * T);
}

static double computeReynoldsNumber(double velocity, double length, double viscosity) {
    return (velocity * length) / viscosity;
}

bool ModelLoader::loadOBJ(const std::string& filename, Mesh& mesh) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    
    mesh.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "v") {
            double x, y, z;
            iss >> x >> y >> z;
            mesh.vertices.push_back(Vec3(x, y, z));
        } else if (prefix == "vn") {
            double x, y, z;
            iss >> x >> y >> z;
            mesh.normals.push_back(Vec3(x, y, z).normalize());
        } else if (prefix == "vt") {
            double u, v;
            iss >> u >> v;
            mesh.texcoords.push_back(Vec2(u, v));
        } else if (prefix == "f") {
            Face face;
            std::string vertex_str;
            int v_count = 0;
            while (iss >> vertex_str) {
                if (v_count >= 3) break;
                
                std::istringstream vss(vertex_str);
                std::string token;
                int idx = 0;
                
                while (std::getline(vss, token, '/')) {
                    if (!token.empty()) {
                        int val = std::stoi(token);
                        if (val < 0) {
                            val = mesh.vertices.size() + val + 1;
                        }
                        if (idx == 0) face.v[v_count] = val - 1;
                        else if (idx == 1) face.vt[v_count] = val - 1;
                        else if (idx == 2) face.vn[v_count] = val - 1;
                    }
                    idx++;
                }
                v_count++;
            }
            
            if (v_count >= 3) {
                mesh.faces.push_back(face);
            }
        } else if (prefix == "o") {
            iss >> mesh.name;
        }
    }
    
    file.close();
    
    if (mesh.vertices.empty()) {
        std::cerr << "No vertices loaded from: " << filename << std::endl;
        return false;
    }
    
    if (mesh.normals.empty()) {
        mesh.normals = computeNormal(mesh);
    }
    
    return true;
}

std::vector<Vec3> ModelLoader::computeNormal(const Mesh& mesh) {
    std::vector<Vec3> normals(mesh.vertices.size(), Vec3(0, 0, 0));
    
    for (const auto& face : mesh.faces) {
        if (face.v[0] < 0 || face.v[1] < 0 || face.v[2] < 0) continue;
        if (face.v[0] >= (int)mesh.vertices.size() || 
            face.v[1] >= (int)mesh.vertices.size() || 
            face.v[2] >= (int)mesh.vertices.size()) continue;
        
        Vec3 v0 = mesh.vertices[face.v[0]];
        Vec3 v1 = mesh.vertices[face.v[1]];
        Vec3 v2 = mesh.vertices[face.v[2]];
        
        Vec3 edge1 = v1 - v0;
        Vec3 edge2 = v2 - v0;
        Vec3 normal = edge1.cross(edge2);
        
        normals[face.v[0]] = normals[face.v[0]] + normal;
        normals[face.v[1]] = normals[face.v[1]] + normal;
        normals[face.v[2]] = normals[face.v[2]] + normal;
    }
    
    for (auto& n : normals) {
        n = n.normalize();
    }
    
    return normals;
}

MeshProperties ModelLoader::computeProperties(const Mesh& mesh) {
    MeshProperties props;
    props.vertex_count = (int)mesh.vertices.size();
    props.face_count = (int)mesh.faces.size();
    props.surface_area = 0.0;
    props.projected_area_x = 0.0;
    props.projected_area_y = 0.0;
    props.projected_area_z = 0.0;
    props.volume = 0.0;
    props.wetted_area = 0.0;
    
    double cx = 0, cy = 0, cz = 0;
    double min_x = 1e100, max_x = -1e100;
    double min_y = 1e100, max_y = -1e100;
    double min_z = 1e100, max_z = -1e100;
    
    for (const auto& v : mesh.vertices) {
        cx += v.x;
        cy += v.y;
        cz += v.z;
        min_x = std::min(min_x, v.x);
        max_x = std::max(max_x, v.x);
        min_y = std::min(min_y, v.y);
        max_y = std::max(max_y, v.y);
        min_z = std::min(min_z, v.z);
        max_z = std::max(max_z, v.z);
    }
    
    if (!mesh.vertices.empty()) {
        cx /= mesh.vertices.size();
        cy /= mesh.vertices.size();
        cz /= mesh.vertices.size();
    }
    
    for (const auto& face : mesh.faces) {
        if (face.v[0] < 0 || face.v[1] < 0 || face.v[2] < 0) continue;
        if (face.v[0] >= (int)mesh.vertices.size() || 
            face.v[1] >= (int)mesh.vertices.size() || 
            face.v[2] >= (int)mesh.vertices.size()) continue;
        
        Vec3 v0 = mesh.vertices[face.v[0]];
        Vec3 v1 = mesh.vertices[face.v[1]];
        Vec3 v2 = mesh.vertices[face.v[2]];
        
        Vec3 edge1 = v1 - v0;
        Vec3 edge2 = v2 - v0;
        Vec3 cross = edge1.cross(edge2);
        double area = cross.length() * 0.5;
        
        props.surface_area += area;
        props.wetted_area += area;
        
        Vec3 normal = cross.normalize();
        props.projected_area_x += std::fabs(normal.x * area);
        props.projected_area_y += std::fabs(normal.y * area);
        props.projected_area_z += std::fabs(normal.z * area);
        
        Vec3 centroid = (v0 + v1 + v2) * (1.0 / 3.0);
        props.volume += (1.0 / 6.0) * cross.dot(centroid);
    }
    
    props.centroid_x = cx;
    props.centroid_y = cy;
    props.centroid_z = cz;
    
    props.bounding_box_x = max_x - min_x;
    props.bounding_box_y = max_y - min_y;
    props.bounding_box_z = max_z - min_z;
    
    props.planform_area = props.projected_area_z;
    
    double length = props.bounding_box_x;
    double max_dim = std::max({props.bounding_box_x, props.bounding_box_y, props.bounding_box_z});
    double min_dim = std::min({props.bounding_box_x, props.bounding_box_y, props.bounding_box_z});
    
    props.aspect_ratio = (max_dim > 0) ? (length * length / props.planform_area) : 1.0;
    props.fineness_ratio = (min_dim > 0) ? (length / min_dim) : 1.0;
    
    return props;
}

double ModelLoader::computeDragCoefficient(const Mesh& mesh, double velocity, double altitude) {
    MeshProperties props = computeProperties(mesh);
    
    double Cd0 = 0.02;
    
    double frontal_area = props.projected_area_x;
    double wetted_area = props.wetted_area;
    
    double Re = computeReynoldsNumber(velocity, props.bounding_box_x, 1.5e-5);
    
    double Cf = 0.0;
    if (Re > 0) {
        Cf = 0.074 / std::pow(Re, 0.2);
    }
    
    double form_factor = 1.0 + 0.5 * props.fineness_ratio;
    
    double Cd = Cd0 + form_factor * Cf * (wetted_area / frontal_area);
    
    double rho = computeAirDensity(altitude);
    double q = 0.5 * rho * velocity * velocity;
    
    return Cd;
}

double ModelLoader::computeLiftCoefficient(const Mesh& mesh, double angle_of_attack) {
    MeshProperties props = computeProperties(mesh);
    
    double alpha_rad = angle_of_attack * M_PI / 180.0;
    
    double Cl_alpha = 2.0 * M_PI;
    
    double Cl = Cl_alpha * alpha_rad * (props.planform_area / props.wetted_area);
    
    double alpha_stall = 15.0 * M_PI / 180.0;
    if (std::fabs(alpha_rad) > alpha_stall) {
        double stall_factor = std::exp(-std::pow((std::fabs(alpha_rad) - alpha_stall) / (5.0 * M_PI / 180.0), 2));
        Cl *= stall_factor;
    }
    
    return Cl;
}

}