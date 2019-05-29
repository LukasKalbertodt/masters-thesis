use lox::{
    ds::SharedVertexMesh,
    fat::MiniMesh,
    io,
};

fn main() -> Result<(), io::Error> {
    type MyMesh = MiniMesh<SharedVertexMesh>;

    let mesh: MyMesh = io::read_file("input.stl")?;
    io::write_file(&mesh, "output.ply")?;

    Ok(())
}
