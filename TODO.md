Features

* ssao / hbao
* render settings
	* bloom
* better bloom
* depth of field
* volumetric lighting / area lights
* basic terrain
* further improve scene creation
	* data-driven via XML parsing
* lua integration
	* create scenes by running script that places nodes
	* object behavior - link node to lua script
	* add things dynamically, preferably with a console
* make more node/entity classes
	* more-capable player object
* cameras
	* make camera movements affect all cameras?
	* swoop-down camera
* resource management
	* textures and meshes
* add / improve states
* ui
	* font rendering
	* buttons

Fixes

* consider making material methods static
* multithreading fixes
	* fix meshnode materialproperties methods
	* removeable nodes
* better error/exception handling
	* openGL
	* file reading
* centralize constants
* make file structure of source files less horrible
	* move dll's