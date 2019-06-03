<h2 align="center">
    Designing and Implementing a Polygon Mesh Library: Can Rust Improve the
    Status Quo in the Domain of Geometry Processing?
</h2>

My master's thesis. Abstract:

> Polygon meshes can be represented in various ways using different data structures, each varying in its capabilities, memory consumption and performance characteristics.
> However, instead of attempting to abstract over several data structures, most popular geometry processing libraries only implement one.
> The open-source library `lox`, developed as part of this thesis, provides such an abstraction using the trait system of the Rust programming language.
> Comparing `lox` to the existing libraries OpenMesh and PMP regarding factors such as execution speed shows that the trait-based abstraction did not incur any significant runtime overhead, making `lox`'s core design a viable option for geometry processing libraries.
> While choosing Rust as the programming language mostly benefited the project, some of its limitations, like the lack of generic associated types, notably slowed down the development of `lox`.


**Links**:
- **Download thesis as PDF** TODO
- [Repository of polygon library `lox`](https://github.com/LukasKalbertodt/lox)
