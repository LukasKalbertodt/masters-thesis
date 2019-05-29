use lox::{
    algo,
    ds::{HalfEdgeMesh, half_edge::TriConfig},
    fat::MiniMesh,
    io,
};

fn main() -> Result<(), io::Error> {
    type MyMesh = MiniMesh<HalfEdgeMesh<TriConfig>>;

    let mut mesh: MyMesh = io::read_file("input.stl")?;

    algo::subdivision::sqrt3(&mut mesh.mesh, &mut mesh.vertex_positions, 1);

    io::write_file(&mesh, "output.ply")?;

    Ok(())
}
