Plugin Folder Structure
=============================

.. code-block:: bash

    plugin                          
    ├── include/alien/plugin         
    │   ├── backend.h               # Backend configuration
    │   ├── options.h               # Preconditioners and Solvers 
    ├── src                    		    
    │   # Plugin's matrix class, 
    │   # with bindings to the solver's matrix type
    │ 	├── plugin_matrix.cpp        	
    │ 	├── plugin_matrix.h        		 
    │   # Plugin's vector class, 
    │   # with bindings to the solver's vector type
    │ 	├── plugin_vector.cpp
    │ 	├── plugin_vector.h 
    │   └── converters  
    │       # Converting methods between Alien SimpleCSR 
    │       # and Plugin storages
    │       ├── simplecsr_to_plugin_matrix.cpp 
    │	    ├── simplecsr_to_plugin_vector.cpp 
    │       └── plugin_to_simplecsr_vector.cpp
    │   # Plugin's Linear algebra class,
    │   # with bindings to the solver's linear algebra functions
    │ 	├── plugin_linear_algebra.cpp   
    │   # Plugin's Linear solver class,
    │   # with bindings to the solver's linear solver functions
    │ 	├── plugin_linear_solver.cpp 
    ├── test                      
    ├── example
    └── CMakeLists.txt  
