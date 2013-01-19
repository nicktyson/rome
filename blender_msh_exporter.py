import bpy
import os

# To run, Blender's working directory should be the rome folder.
# An easy way to set that is to right click this file -> Open With -> Blender
# (assuming you haven't moved this script)

# sharp edges need to be split for them to look correct
# quads must be converted to triangles


current_obj = bpy.context.active_object
f = open('Assets/Meshes/test_cube.msh', 'w')

poly_list = current_obj.data.polygons[:]
vert_list = current_obj.data.vertices[:]

f.write(str(len(vert_list))+'\n')
f.write(str(len(poly_list))+'\n')

f.write('vertices\n')

for vert in vert_list:
    f.write(str(round(vert.co.x, 4))+'\n')
    f.write(str(round(vert.co.y, 4))+'\n')
    f.write(str(round(vert.co.z, 4))+'\n')
    
f.write('triangles\n')

for poly in poly_list:
    f.write(str(poly.vertices[0])+'\n')
    f.write(str(poly.vertices[1])+'\n')
    f.write(str(poly.vertices[2])+'\n')
    
f.write('normals\n')

for vert in vert_list:
    f.write(str(round(vert.normal.x, 4))+'\n')
    f.write(str(round(vert.normal.y, 4))+'\n')
    f.write(str(round(vert.normal.z, 4))+'\n') 
