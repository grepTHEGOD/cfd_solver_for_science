#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <vector>
#include <string>
#include <cmath>

namespace ModelLoader {

struct Vec2 {
    double u, v;
    Vec2() : u(0), v(0) {}
    Vec2(double u_, double v_) : u(u_), v(v_) {}
};

struct Vec3 {
    double x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(double s) const { return Vec3(x * s, y * s, z * s); }
    double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    double length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3 normalize() const {
        double len = length();
        if (len > 0) return *this * (1.0 / len);
        return *this;
    }
};

struct Face {
    int v[3];
    int vn[3];
    int vt[3];
    Face() { v[0]=v[1]=v[2]=vn[0]=vn[1]=vn[2]=vt[0]=vt[1]=vt[2]=-1; }
};

struct Mesh {
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> texcoords;
    std::vector<Face> faces;
    std::string name;
    
    void clear() {
        vertices.clear();
        normals.clear();
        texcoords.clear();
        faces.clear();
    }
};

struct MeshProperties {
    double surface_area;
    double projected_area_x;
    double projected_area_y;
    double projected_area_z;
    double volume;
    double centroid_x;
    double centroid_y;
    double centroid_z;
    double bounding_box_x;
    double bounding_box_y;
    double bounding_box_z;
    int vertex_count;
    int face_count;
    double aspect_ratio;
    double wetted_area;
    double fineness_ratio;
    double planform_area;
};

class ModelLoader {
public:
    static bool loadOBJ(const std::string& filename, Mesh& mesh);
    static MeshProperties computeProperties(const Mesh& mesh);
    static double computeDragCoefficient(const Mesh& mesh, double velocity, double altitude);
    static double computeLiftCoefficient(const Mesh& mesh, double angle_of_attack);
    static std::vector<Vec3> computeNormal(const Mesh& mesh);
};

}

#endif