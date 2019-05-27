using pmp::SurfaceMesh;
using pmp::Normal;
using pmp::Point;
using pmp::IOFlags;


Outcome write_bench(const char* infile, const char* outfile, const char* ext) {
    SurfaceMesh mesh;
    mesh.read(infile);

    // Generate face normals if we are writing STL
    if (string(ext) == "stl") {
        auto normals = mesh.add_face_property<Normal>("f:normal");
        auto points = mesh.get_vertex_property<Point>("v:point");

        auto vertices = vector<Point>();
        for (auto f: mesh.faces()) {
            vertices.clear();
            for (auto v: mesh.vertices(f)) {
                vertices.push_back(points[v]);
            }

            auto diff_a = vertices[1] - vertices[0];
            auto diff_b = vertices[2] - vertices[0];
            auto normal = pmp::normalize(pmp::cross(diff_a, diff_b));
            normals[f] = normal;
        }
    }

    auto flags = IOFlags();
    flags.use_binary = true;

    return run_bench([&mesh, outfile, flags]() {
        mesh.write(outfile, flags);
        return "dummy";
    });
}

#define GEN_WRITE_BENCH(name, mesh, ext) Outcome name() {       \
    return write_bench("/mnt/ramdisk/" mesh "." ext, "/mnt/ramdisk/_out_" mesh "." ext, ext);     \
}

GEN_WRITE_BENCH(write_file_ply_cat, "cat", "ply")
GEN_WRITE_BENCH(write_file_stl_cat, "cat", "stl")
GEN_WRITE_BENCH(write_file_ply_tiger, "tiger", "ply")
GEN_WRITE_BENCH(write_file_stl_tiger, "tiger", "stl")
