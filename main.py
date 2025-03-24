import trimesh
import numpy as np

# Load your 3D model
MODEL_PATH = 'C:\\Users\\tt\\Desktop\\hair\\source\\Hair 2\\hair.obj'
mesh = trimesh.load(MODEL_PATH)  # Replace with your file
if len(mesh.vertex_normals) == 0:
    mesh = mesh.copy()  # Make a copy to avoid altering the original
    mesh.vertex_normals = mesh.face_normals[mesh.faces]

# Extract vertex positions (x, y, z), normals, and texture coordinates (u, v)
vertices = mesh.vertices  # Vertex positions
normals = mesh.vertex_normals  # Normals for each vertex
uvs = mesh.visual.uv  # Texture coordinates (u, v)

# Check if the mesh has texture coordinates
if uvs is None:
    print("No UV texture coordinates found!")
    uvs = np.zeros((vertices.shape[0], 2))  # Placeholder uvs if none are found

# Extract triangle indices to arrange vertices, normals, and uvs by triangle
faces = mesh.faces

# Prepare data in [x, y, z, nx, ny, nz, u, v] format for each vertex in each triangle
with open('output.asc', 'w') as f:
    for face in faces:
        f.write("triangle\n")  # Write "triangle" identifier for each face
        for vertex_idx in face:
            x, y, z = vertices[vertex_idx]
            nx, ny, nz = normals[vertex_idx]
            u, v = uvs[vertex_idx]
            f.write(f"{x} {y} {z} {nx} {ny} {nz} {u} {v}\n")