Goals / Milestones

* GLFW3 / C++ threads update
	* clean up threads to work C++-style
	* possibly change key handling
	* investigate GLFW static linking
* actual game engine capability
	* object lifecycle
		* multithreading fixes below
		* remove objects from graph
		* RAII
	* Lua
		* finish/design above first
		* scripts to create scenes / place nodes
		* scripts to control scene graph node
		* console / dynamic manipulation of graph
		* overall game logic
* example game
	* ui
		* font rendering
		* buttons
	* add/improve states
	* add/improve node/entity classes
		* player object
		* camera management - switching camera tye at one location
		* look into entity/component architectures
	* the actual lua scripts
* miscellaneous graphics features


Features

* ssao / hbao
* render settings
	* bloom
* better bloom
* depth of field
* volumetric lighting / area lights
* basic terrain
* improve scene creation
	* data-driven via XML parsing
* resource management
	* textures and meshes


Fixes

* look into changing key handling
* consider making material methods static
* multithreading fixes
	* fix meshnode materialproperties methods
	* removeable nodes
* better error/exception handling
	* openGL
	* file reading
* centralize constants
* make file structure of source files better
	* move dll's?
