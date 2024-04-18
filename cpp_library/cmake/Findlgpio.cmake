################################################################################
### Find the lgpio shared libraries.
################################################################################

# Find the path to the lgpio includes.
find_path(lgpio_INCLUDE_DIR 
	NAMES lgpio.h 
	HINTS /usr/include)
	
# Find the lgpio libraries.
find_library(lgpio_LIBRARY 
	NAMES liblgpio.so
	HINTS /usr/lib)

    
# Set the lgpio variables to plural form to make them accessible for 
# the paramount cmake modules.
set(lgpio_INCLUDE_DIRS ${lgpio_INCLUDE_DIR})
set(lgpio_INCLUDES     ${lgpio_INCLUDE_DIR})

# Handle REQUIRED, QUIET, and version arguments 
# and set the <packagename>_FOUND variable.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(lgpio 
    DEFAULT_MSG 
    lgpio_INCLUDE_DIR lgpio_LIBRARY)
