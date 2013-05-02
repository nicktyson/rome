import bpy
import os

# I'd rather not use an absolute file path, but it's a little simpler.

# To use relative path, Blender's working directory should be the rome folder.
# import os
# os.chdir()

# sharp edges need to be split for them to look correct
# the same is true for UV seams
# quads are automatically converted to triangles

current_obj = bpy.context.active_object
f = open('C:/Users/Nick/Documents/Visual Studio 2010/Projects/rome/Assets/Meshes/test_cube2.msh', 'w')

bpy.ops.object.mode_set(mode='EDIT')
bpy.ops.mesh.select_all() 
bpy.ops.mesh.quads_convert_to_tris()
bpy.ops.object.mode_set(mode='OBJECT')

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

f.write('texcoords\n')

uvdata = current_obj.data.uv_layers.active.data
txcoords = [0] * (2*len(vert_list))

for i, uv in enumerate(uvdata):
    index = current_obj.data.loops[i].vertex_index
    tx = uvdata[i].uv
    txcoords[2*index] = tx.x
    txcoords[2*index + 1] = tx.y
    
for tex in txcoords:
    f.write(str(tex)+'\n')