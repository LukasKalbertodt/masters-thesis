#![feature(proc_macro_hygiene)]

use lox::{
    mesh,
    cgmath::Point3,
    ds::SharedVertexMesh,
    fat::MiniMesh,
    io,
};

fn main() -> Result<(), io::Error> {
    let (mesh, vertex_positions) = mesh! {
        type: SharedVertexMesh,
        vertices: [
            v0: Point3::new(0.0, 0.0, 0.0),
            v1: Point3::new(0.0, 1.0, 0.0),
            v2: Point3::new(1.0, 0.0, 0.0),
            v3: Point3::new(1.0, 1.0, 0.0),
        ],
        faces: [
            [v0, v2, v1],
            [v2, v3, v1],
        ]
    };

    let mesh = MiniMesh { mesh, vertex_positions };
    io::write_file(&mesh, "output.ply")?;

    Ok(())
}
