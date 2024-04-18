################################################################################
### Find the pigpio shared libraries.
################################################################################

# Find the path to the pigpio includes.
find_path(pigpio_INCLUDE_DIR 
    NAMES pigpio.h pigpiod_if.h pigpiod_if2.h
    HINTS /usr/include /usr/local/include)
    
# Find the pigpio libraries.
find_library(pigpio_LIBRARY 
    NAMES libpigpio.so
    HINTS /usr/lib /usr/local/lib)
find_library(pigpiod_if_LIBRARY 
    NAMES libpigpiod_if.so
    HINTS /usr/lib /usr/local/lib)
find_library(pigpiod_if2_LIBRARY 
    NAMES libpigpiod_if2.so
    HINTS /usr/lib /usr/local/lib)
    
# Set the pigpio variables to plural form to make them accessible for 
# the paramount cmake modules.
set(pigpio_INCLUDE_DIRS ${pigpio_INCLUDE_DIR})
set(pigpio_INCLUDES     ${pigpio_INCLUDE_DIR})

# Handle REQUIRED, QUIET, and version arguments 
# and set the <packagename>_FOUND variable.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(pigpio REQUIRED_VARS pigpio_INCLUDE_DIR pigpio_LIBRARY pigpiod_if_LIBRARY pigpiod_if2_LIBRARY)

if(pigpio_FOUND)
    if(NOT TARGET pigpio::pigpio)
        add_library(pigpio::pigpio INTERFACE IMPORTED)
        set_target_properties(pigpio::pigpio PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${pigpio_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${pigpio_LIBRARY}")
    endif()
	if(NOT TARGET pigpio::pigpio_if)
        add_library(pigpio::pigpio_if INTERFACE IMPORTED)
        set_target_properties(pigpio::pigpio_if PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${pigpio_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${pigpiod_if_LIBRARY}")
    endif()
	if(NOT TARGET pigpio::pigpio_if2)
        add_library(pigpio::pigpio_if2 INTERFACE IMPORTED)
        set_target_properties(pigpio::pigpio_if2 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${pigpio_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${pigpiod_if2_LIBRARY}")
    endif()
endif()
