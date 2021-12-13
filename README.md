# ArPiRobot-CoreLib

Core library for ArPiRobot robots containing user-facing API, device "driver" code, etc.


## [cpp_library](cpp_library/)

C++ Library. This is where everything is implemented. This library is loaded by bindings to other languages, or used to program the robot in C++.


## [python_bindings](python_bindings/)

Python bindings for the ArPiRobot core library (C++). The python library matches the object oriented API of the C++ library.


## Development of Library (and Bindings)

- Any of the above folders can be opened in VSCode (or other IDEs as appropriate) and (provided correct extensions are installed) be easily worked with / built using VSCode
    - cpp_library 
        - recommended extensions
            - C/C++ Extension (Microsoft)
            - CMake Tools (Microsoft)
        - When building use the arpirobot-toolchain.cmake file (pre-configured as a CMake kit for VSCode)
    - python_bindings
        - recommended extensions
            - Python extension (Microsoft)
            - Pylance extension (Microsoft)
        - Recommended to use Pylance as the language server
    

- Deploying to robot for testing
    - In this (project root) folder there are several scripts
        - `dev-deploy.py`: Used to copy built library and language bindings to the robot (192.168.10.1) via SCP. Files deployed to `/home/pi/CoreLibTest`. *Make sure you build the C++ library before deploying or some files will be missing.*
        - `start-cpp.sh`: Used to start the C++ testrobot program (run this on the robot using SSH)
        - `start-py.sh`: Used to start the Python testrobot program (run this on the robot using SSH)