#include <iostream>
#include "Simply3D.h" // Include the custom 3D engine header

/* Custom IDE or Compiler Setup Instructions

To compile and run this project, please configure your IDE with the following settings:

1. **Include Directories**:
   - `Libraries`
   - `Libraries/Assimp/include`
   - `Libraries/GLEW/include`
   - `Libraries/GLFW/include`
   - `Libraries/GLM`
   - `Simply3D`

2. **Source Files**:
   - Include all `.cpp` files from the following directories:
	 - `Simply3D`
	 - `Libraries/ImGui`

3. **Linker Directories**:
   - `Libraries/ASSIMP/lib/Release`
   - `Libraries/GLEW/lib/Release/Win32`
   - `Libraries/GLFW/lib-vc2022`

4. **Linker Flags**:
   - `assimp-vc140-mt.lib`
   - `opengl32.lib`
   - `glew32.lib`
   - `glfw3.lib`

5. **DLL Files**:
   - Place the following DLL files in the same directory as the executable (`.exe`):
	 - `Libraries/GLEW/bin/Release/Win32/glew32.dll`
	 - `Libraries/ASSIMP/bin/assimp-vc140-mt.dll`
*/


// Create an instance of the Simply3D engine
Simply3D Engine;

int main() {
	// Initialize the engine with the window title "Demo" and dimensions 640x480
	if (Engine.Init("Demo", 640, 480)) {
		std::cout << "Engine Init Completed" << std::endl;
	}
	else {
		std::cout << "Engine Init Failed" << std::endl;
		return -1; // Exit the program if initialization fails
	}

	// Set the skybox type to a predefined sky-blue backdrop
	Engine.SetSkyboxType(SkyboxTypes::SkyBlueSkyboxType);

	// Create a new object dynamically (a cube in this case)
	Object* Test_Cube = new Object();

	// Initialize the cube object with lighting and gold material
	Test_Cube->Init(ObjectTypes::CubeObjectType, ShaderTypes::TexturedLightShaderType, MaterialTypes::Gold);

	// Add the cube object to the engine for rendering
	Engine.Add(Test_Cube);

	// Start the engine to run the 3D scene
	Engine.Start();

	// Clean up memory before exiting
	delete Test_Cube;

	return 0; // Return 0 to indicate successful completion
}