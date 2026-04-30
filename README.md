# Awoken-Engine
Awoken Engine is a C++ library 3D Game Engine that uses GLFW, GLM, Assimp, freetype, OpenAL and nlohmann_JSON 
as dependencies. 

## Usage
Awoken Engine works out of the box, with all C++ and Header Files available to be edited to fit your Game's needs.
The default Assets for Awoken Engine are in the assets folder in this repository. You should be able to set up your
project however you want but the default assets look for the assets folder with the exe. This can be edited in the engine

### Installing Assimp
To use Awoken Engine on Windows or Linux first clone the repository. All dependencies are included in the Dependencies 
folder however Assimp needs to be installed for the engine to run. To install Assimp on Windows go to the assimp itch 
page and get the installer. To install on Linux find the appropriate installer for your distro or build from source on 
the Assimp repository.

### Building Awoken Engine
After Assimp is installed build the project with CMake

#### Build with Linux
```` 
cd Awoken-Engine
cmake -B build
cd build
make
````
This will give you the .a file and Linux executable to test that the engine is running correctly

#### Build with Windows Visual Studio
````
cd Awoken-Engine
cmake -B build
````
Then open the solution and build the solution to get the .lib file and .exe file for the Test

## To-Do List
Here is a to-do this of the features being added to Awoken Engine not in any particular order

1. Emission in the shader
2. Collision Components
3. Physics
4. Networking
5. Animation System
6. Shadows
7. Additional Input Events (Controllers and others)