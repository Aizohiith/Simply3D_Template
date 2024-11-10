#pragma once


#include "OpenGL.h"
#include "Window.h"
#include "Time.hpp"
#include "Shader.h"
#include "Camera.h"
#include "CommonValues.hpp"
#include "Object.h"
#include "Skybox.h"
#include "Timer.h"
#include "Light.h"
#include "string"
#include "Enumerators.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>


//#########################################################################
//Use These settings in The Main Linker After Linkie the Libraries As Well
//
// opengl32.lib
// glew32.lib
// glfw3.lib
// 
//#########################################################################

class Simply3D
{
public:
	Simply3D();
	bool Init(const char* Name, int Width, int Height);//Initialize Engine

	void Start();//Start Engine

	void Add(Object* Object2Add);
	void Add(Timer* Timer2Add);
	void Add(PointLight* PointLight2Add);

	void Remove(Object* Object2Remove);
	void Remove(Timer* Timer2Remove);
	void Remove(PointLight* PointLight2Remove);

	void SetSkyboxType(const SkyboxTypes& Skybox);//Set Skybox Type
	SkyboxTypes GetSkyboxType() const;

	void SetMoveSpeed(float NewMoveSpeed);//Set Movement Speed
	float GetMoveSpeed() const;

	void SetMoveKey(MoveTypes TypeToChange, int NewKey);//Set Movement Keys
	int GetMoveKey(MoveTypes Key)  const;

	void SetRenderLights(bool RenderLights);
	bool GetRenderLights() const;

	void SetRenderCollision(bool RenderCollision);
	bool GetRenderCollision() const;

	void SetCamera(Camera& NewCamera);
	Camera* GetCamera() const;
	
	void SetWindow(Window& NewWindow);
	Window* GetWindow() const;

	void SetAmbientLight(AmbientLight& newAmbientLight);
	AmbientLight* GetAmbientLight() const;
	void SetDirectionalLight(DirectionalLight& newDirectionalLight);
	DirectionalLight* GetDirectionalLight() const;

	void SetWireframe(bool Wireframe);
	bool GetWireframe() const;

	void SetLockedCursor(bool Locked);
	bool GetLockedCursor() const;
	
	void SetDebug(bool Debug);
	bool GetDebug() const;

	void (*CallbackKey)(const bool* Keys) = nullptr;
	void (*CallbackResize)(const float& Width, const float& Height) = nullptr;
	void (*CallbackMouseMove)(const float& x, const float& y) = nullptr;
	void (*CallbackMouseButton)(const bool* Keys) = nullptr;

	~Simply3D();

protected:

	Window* gg_MainWindow;
	Camera* gg_Camera;
	
	glm::mat4 gm_ProjectionMatrix;

	Shader* BasicShader;
	Shader* TexturedShader;
	Shader* TexturedLightShader;
	Shader* ShadowMapShader;

	std::vector<Object*> garr_Objects;
	std::vector<Timer*> garr_Timers;
	std::vector<PointLight*> garr_PointLights;

	GLint gi_BufferWidth;
	GLint gi_BufferHeight;

	Skybox* gg_Skybox;

	AmbientLight* gg_AmbientLight;
	DirectionalLight* gg_DirectionalLight;

	Texture* gg_No_Shadow;

	SkyboxTypes gi_Skybox;
	bool gb_LockedCursor;
	float gf_MoveSpeed;

	bool gb_RenderLights;
	bool gb_RenderCollition;
	bool gb_Debug;
	bool gb_Wireframe;

	Object* gg_UnitCube;
	Object* gg_UnitSphere;

	int gg_LeftMoveKey;
	int gg_RightMoveKey;
	int gg_ForwardMoveKey;
	int gg_BackMoveKey;

	void Loop();
	void Setup();
	void Update();
	void EventHandler(int Delta);
	void Render();
	void RenderDebugColliders();
	void RenderDebugLights();
	void RenderSkybox();
	void RenderShadows();
	void RenderObjects(glm::mat4 pmat4_Projection, glm::mat4 pmat4_View);
};