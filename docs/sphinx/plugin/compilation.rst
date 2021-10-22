
Compilation
=============================



- Add the plugin to the *alien/CMakeLists.txt* file :

.. code-block:: cmake
	
	# Add compile option for plugin building
	option(ALIEN_PLUGIN_NAME "Whether or not to compile plugin backend" OFF)

	# Add the plugin directory	
	if (ALIEN_PLUGIN_NAME)
		add_subdirectory(plugins/plugin_name)
	endif (ALIEN_PLUGIN_NAME)


- Write an *alien/plugin/plugin_name/CMakeLists.txt* using another plugin's CMakeLists.txt file as a template:
	- you may need to write a findSolver.cmake file, which should be located in the alien/framework/arccon/build-system/Modules folder:

.. code-block:: cmake
	
	# Find the solver to be linked in the plugin
	find_package(Solver REQUIRED)


- Add the cmake target and files:
- Link against Arccore, Alien, and the Solver library, through arccon. note: arccon is used as a proxy to link external libraries.

.. code-block:: cmake

	target_link_libraries(alien_plugin PUBLIC
			Arccore::arccore_trace
			Arccore::arccore_collections
			Arccore::arccore_base
			Arccore::arccore_message_passing_mpi
			Alien::alien_core
			arccon::Solver
			)			

- Specify the solver's include directories

.. code-block:: cmake

	target_include_directories(alien_plugin PUBLIC
			$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
			$<BUILD_INTERFACE:${SOLVER_INCLUDE_DIRS}/..>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
			)
