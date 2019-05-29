use lox::{
    prelude::*,
    ds::DirectedEdgeMesh,
    fat::MiniMesh,
    io,
};

fn main() -> Result<(), io::Error> {
    type MyMesh = MiniMesh<DirectedEdgeMesh>;

    let mut mesh: MyMesh = io::read_file("input.stl")?;

    mesh.vertex_positions = mesh.mesh.vertices().map(|v| {
        let old_pos = mesh.vertex_positions[v.handle()];
        let new_pos = if v.is_boundary() {
            old_pos
        } else {
            v.adjacent_vertices()
                .map(|n| mesh.vertex_positions[n.handle()])
                .centroid()
                .unwrap()
        };

        (v.handle(), new_pos)
    }).collect();

    io::write_file(&mesh, "output.ply")?;

    Ok(())
}
