rome
====

A graphics project/renderer written in C++.  It originally used OpenGL 2.x, but has now been ported to 3.3.  
  
In its current form, it consists of a deferred renderer with geometry, shading, and post-processing passes.  One thread renders the scene, while another updates object positions.  It includes both first-person and top-down third-person cameras.
  
Renderer and SimState are some of the more interesting classes.
  
Requires Windows.  
  
Libraries used:  
GLEW - helps use OpenGL extensions  
GLFW - significantly reduces the boilerplate required to create an OpenGL window; also provides cross-platform key input functionality  
GLM - provides matrix/vector classes that help replace deprecated OpenGL functions
