import bpy

current_obj = bpy.context.active_object
f = open('Assets/Meshes/mesh_name.msh', 'w')

poly_list = current_obj.data.polygons[:]
vert_list = current_obj.data.vertices[:]

f.write(str(len(vert_list))+'\n')
f.write(str(len(poly_list))+'\n')

f.write('vertices\n')

for vert in current_obj.data.vertices:
    f.write(str(vert.co.x)+'\n')
    f.write(str(vert.co.y)+'\n')
    f.write(str(vert.co.z)+'\n')
    
f.write('triangles\n')

for poly in current_obj.data.polygons:
    f.write(str(poly.vertices[0])+'\n')
    f.write(str(poly.vertices[1])+'\n')
    f.write(str(poly.vertices[2])+'\n')
    
f.write('normals\n')

for vert in current_obj.data.vertices:
    f.write(str(vert.normal.x)+'\n')
    f.write(str(vert.normal.y)+'\n')
    f.write(str(vert.normal.z)+'\n') 
