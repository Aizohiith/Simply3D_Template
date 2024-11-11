#define STB_IMAGE_IMPLEMENTATION
#include "Simply3D.h"
Simply3D::Simply3D()
{
	gg_MainWindow = nullptr;
	gg_Camera = nullptr;

	gb_Debug = true;
	gb_RenderLights = false;
	gb_RenderCollition = false;
	gb_LockedCursor = true;
	gb_Wireframe = false;

	BasicShader = nullptr;
	TexturedShader =  nullptr;
	TexturedLightShader = nullptr;
	ShadowMapShader = nullptr;

	gg_Camera = nullptr;
	gg_Skybox = nullptr;
	gi_BufferWidth = 0;
	gi_BufferHeight = 0;

	gg_LeftMoveKey = KEY_NONE;
	gg_RightMoveKey = KEY_NONE;
	gg_ForwardMoveKey = KEY_NONE;
	gg_BackMoveKey = KEY_NONE;

	gi_Skybox = SkyboxTypes::NoSkyboxType;

	gm_ProjectionMatrix = glm::mat4(1.0f);
	gf_MoveSpeed = 0.0f;

	gg_UnitCube = nullptr;
	gg_UnitSphere = nullptr;

}

bool Simply3D::Init(const char* ps_Name, int pi_Width, int pi_Height)
{
	LOG_DEBUG3D(gb_Debug, "Engine Init Start");
//Initialize Window
	gg_MainWindow = new Window();
	if (!gg_MainWindow->Init(ps_Name, pi_Width, pi_Height))
	{
		LOG_DEBUG3D(gb_Debug, "Engine Init Failed");
		return false;
	}
	gi_BufferWidth = gg_MainWindow->GetBufferWidth();
	gi_BufferHeight = gg_MainWindow->GetBufferHeight();
	gg_MainWindow->SetLockMouse(true);
//
//Initialize GPU API
	if (!OpenGL::Init(gi_BufferWidth, gi_BufferHeight))
	{
		LOG_DEBUG3D(gb_Debug, "Engine Init Failed");
		return false;
	}
//
//Initialize Ambiet Light
	gg_AmbientLight = new AmbientLight();//Create Ambient Light
	gg_AmbientLight->SetColor(glm::vec3(1.0f));//Set Color
	gg_AmbientLight->SetIntencity(0.1f);//Set Intencity
//
//Initialize Directional Light	
	gg_DirectionalLight = new DirectionalLight();//Create Directional Light
	gg_DirectionalLight->SetColor(glm::vec3(1.0f));//Set Color
	gg_DirectionalLight->SetIntencity(0.8f);//Set Intencity
	gg_DirectionalLight->SetDirection(0.0f, 30.0f / 2.0f, 29.0f / 2.0f);//Set Direction
//
//Initialize Shaders
	BasicShader = new Shader();//Create Shaders
	TexturedShader = new Shader();//Create Shaders
	TexturedLightShader = new Shader();//Create Shaders
	ShadowMapShader = new Shader();//Create Shaders
//Initialize Camera
	gg_Camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));//Create Camera
//
//Initialize Movement
//Set Movement Keys
	gg_LeftMoveKey = KEY_A;
	gg_RightMoveKey = KEY_D;
	gg_ForwardMoveKey = KEY_W;
	gg_BackMoveKey = KEY_S;
//
	gf_MoveSpeed = 1.0f;//Set Movement Speed
//
//Initialize Skybox
	gi_Skybox = SkyboxTypes::NoSkyboxType;//Set Skybox Type
//
//Initialize Unit Objects
//Create Unit Objects
	gg_UnitCube = new Object();
	gg_UnitSphere = new Object();
//
//
	LOG_DEBUG3D(gb_Debug, "Engine Init Completed");
	return true;
}




void Simply3D::Start()
{
	LOG_DEBUG3D(gb_Debug, "Engine Started");
	Setup();//Setup

	Loop();//Loop
}
void Simply3D::Add(Object* pp_Object2Add)
{
	garr_Objects.push_back(pp_Object2Add);
}

void Simply3D::Add(Timer* pp_Timer2Add)
{
	garr_Timers.push_back(pp_Timer2Add);
}

void Simply3D::Add(PointLight* pp_PointLight2Add)
{
	garr_PointLights.push_back(pp_PointLight2Add);
}

void Simply3D::Remove(Object* Object2Remove)
{
	for (int C1 = 0; C1 < garr_Objects.size(); C1++)
	{
		if (garr_Objects[C1] == Object2Remove)
		{
			garr_Objects.erase(garr_Objects.begin() + C1);
			break;
		}
	}
}
void Simply3D::Remove(Timer* Timer2Remove)
{
	for (int C1 = 0; C1 < garr_Timers.size(); C1++)
	{
		if (garr_Timers[C1] == Timer2Remove)
		{
			garr_Timers.erase(garr_Timers.begin() + C1);
			break;
		}
	}
}
void Simply3D::Remove(PointLight* PointLight2Remove)
{
	for (int C1 = 0; C1 < garr_PointLights.size(); C1++)
	{
		if (garr_PointLights[C1] == PointLight2Remove)
		{
			garr_PointLights.erase(garr_PointLights.begin() + C1);
			break;
		}
	}
}

Simply3D::~Simply3D()
{
	/*for (auto& C1 : garr_Objects)
	{
		delete C1;
	}*/
	delete gg_MainWindow;
}

void Simply3D::Loop()
{
//Initilize Time
	uint64_t li_StartTimeFPS = Time::GetCurrentTime();//Set FPS Time Tracker
	uint64_t li_StartTimeRenderFPS = Time::GetCurrentTime();//Set FPS Time Tracker
	uint64_t li_StartTime = Time::GetCurrentTime();//Set Delta Time Tracker
	uint64_t li_LastTimeRenderd = Time::GetCurrentTime();//Set Render Time Tracker
	int li_FPS = 0;//Clear FPS
	int li_Render_FPS = 0;
//
	
	while (gg_MainWindow->ShouldClose() != true)
	{

	//Calculate Delta
		int li_Delta = Time::GetTimeDiffrenceSince(li_StartTime);//Get Delta
		li_StartTime = Time::GetCurrentTime();//Reset Delta Time Tracker
	//
		EventHandler(li_Delta);//Event Handler
		Update();//Update
	//Render
		if (Time::GetTimeDiffrenceSince(li_LastTimeRenderd) > (1000.0f / RENDER_FPS))
		{
			li_Render_FPS++;
			li_LastTimeRenderd = Time::GetCurrentTime();//Reset Render Time Tracker
			RenderShadows();//Render Shadows
			Render();//Render World
			

			if (Time::GetTimeDiffrenceSince(li_StartTimeRenderFPS) >= 1000)
			{
				li_StartTimeRenderFPS = Time::GetCurrentTime();
				LOG_DEBUG3D(gb_Debug, "Render FPS: " + std::to_string(li_Render_FPS));
				li_Render_FPS = 0;

			}
		}
	//

	//Calculate FPS
		li_FPS++;
		if (Time::GetTimeDiffrenceSince(li_StartTimeFPS) >= 1000)
		{
			li_StartTimeFPS = Time::GetCurrentTime();
			LOG_DEBUG3D(gb_Debug, "FPS: " + std::to_string(li_FPS) );
			LOG_DEBUG3D(gb_Debug, "Objects: " + std::to_string(garr_Objects.size()));
			li_FPS = 0;
			
		}
	//
	}
}

void Simply3D::Setup()
{
//Load shader
	LOG_DEBUG3D(gb_Debug, "Loading Shaders");
	BasicShader->LoadFromFile("Simply3D/Shaders/Basic.vert", "Simply3D/Shaders/Basic.frag");
	TexturedShader->LoadFromFile("Simply3D/Shaders/TexturedBasic.vert", "Simply3D/Shaders/TexturedBasic.frag");
	TexturedLightShader->LoadFromFile("Simply3D/Shaders/TexturedLight.vert", "Simply3D/Shaders/TexturedLight.frag");
	ShadowMapShader->LoadFromFile("Simply3D/Shaders/DirectionalShadow.vert", "Simply3D/Shaders/DirectionalShadow.frag");
	LOG_DEBUG3D(gb_Debug, "Loading Shaders Completed");
//
//Load Unit Objects
	gg_UnitCube->Init(CubeObjectType, ShaderTypes::BasicShaderType, MaterialTypes::None);
	gg_UnitSphere->Init(IcosphereObjectType, ShaderTypes::BasicShaderType, MaterialTypes::None);
//	
	gg_No_Shadow = new Texture();
	gg_No_Shadow->Load("Simply3D/Textures/Plane.png", false);
	LOG_DEBUG3D(gb_Debug, "Engine Setup Completed");
}

void Simply3D::EventHandler(int pi_Delta)
{
//Setup Mouse Pos
	static float lf_LastMouseX = -1;
	static float lf_LastMouseY = -1;
	if (lf_LastMouseX == -1 && lf_LastMouseY == -1)
	{
		gg_MainWindow->GetMousePos(lf_LastMouseX, lf_LastMouseY);
	}
	float lf_MouseX = 0;
	float lf_MouseY = 0;
//
	gg_MainWindow->GetMousePos(lf_MouseX, lf_MouseY);//Get Current Mouse Pos
	//Calculate Delta Mouse
	float lf_DeltaMouseX = (lf_LastMouseX - lf_MouseX);
	float lf_DeltaMouseY = (lf_LastMouseY - lf_MouseY);
	//

	//Store Current Mouse Pos
	lf_LastMouseX = lf_MouseX;
	lf_LastMouseY = lf_MouseY;
	//


	gg_MainWindow->EventHandler();//Capture Events

	const bool* Keys = gg_MainWindow->GetKeys();//Get Key States

	float Speed = 0.01f * gf_MoveSpeed;//Scale Move Speed


//Handel Camera Controlls
//Move Camera Forward
	if (Keys[gg_ForwardMoveKey])
	{
		gg_Camera->SetPosition((gg_Camera->GetPosition() + (gg_Camera->GetForward() * static_cast<float>(pi_Delta) * Speed)));
	}
//
//Move Camera Left
	if (Keys[gg_LeftMoveKey])
	{
		gg_Camera->SetPosition((gg_Camera->GetPosition() - (gg_Camera->GetRight() * static_cast<float>(pi_Delta) * Speed)));
	}
//
//Move Camera Right
	if (Keys[gg_RightMoveKey])
	{
		gg_Camera->SetPosition((gg_Camera->GetPosition() + (gg_Camera->GetRight() * static_cast<float>(pi_Delta) * Speed)));
	}
//
//Move Camera Back
	if (Keys[gg_BackMoveKey])
	{
		gg_Camera->SetPosition((gg_Camera->GetPosition() - (gg_Camera->GetForward() * static_cast<float>(pi_Delta) * Speed)));
	}
//
//Rotate Camera
	if (gb_LockedCursor)
	{
		gg_Camera->SetRotation(gg_Camera->GetRotation().x + glm::radians(lf_DeltaMouseY * 0.1f), gg_Camera->GetRotation().y - glm::radians(lf_DeltaMouseX * 0.1f), 0.0f);
		if (gg_Camera->GetRotation().x > glm::radians(89.0f))
			gg_Camera->SetRotation(glm::radians(89.0f), gg_Camera->GetRotation().y, gg_Camera->GetRotation().z);
		if (gg_Camera->GetRotation().x < glm::radians(-89.0f))
			gg_Camera->SetRotation(glm::radians(-89.0f), gg_Camera->GetRotation().y, gg_Camera->GetRotation().z);
	}
//
//

//Handel Key Callback
	if ((gg_MainWindow->GetKeyHandlerTriggerd()) && (CallbackKey!= nullptr))
		CallbackKey(gg_MainWindow->GetKeys());
//

//Handel Mouse Move Callback
	if ((gg_MainWindow->GetMouseHandlerTriggerd()) && (CallbackMouseMove != nullptr))
	{
		float lf_MouseX, lf_MouseY;
		gg_MainWindow->GetMousePos(lf_MouseX, lf_MouseY);
		CallbackMouseMove(lf_MouseX, lf_MouseY);
	}
//

//Handel Mouse Key Callback
	if ((gg_MainWindow->GetMouseButtonHandlerTriggerd()) && (CallbackMouseButton != nullptr))
		CallbackMouseButton(gg_MainWindow->GetKeys());
//

//Handel Window Resize Callback
	if (gg_MainWindow->GetWindowResizeHandlerTriggerd())
	{
	//Capture new Window Size
		gi_BufferWidth = gg_MainWindow->GetBufferWidth();
		gi_BufferHeight = gg_MainWindow->GetBufferHeight();
	//
		float aspectRatio = static_cast<float>(gg_MainWindow->GetBufferWidth()) / static_cast<float>(gg_MainWindow->GetBufferHeight());//Calculate New Aspect Ratio
		if (aspectRatio > 0.0f)//Validate New Aspect Ratio
		{
			LOG_DEBUG3D(gb_Debug, "Aspect Ration Changed: " + std::to_string(aspectRatio));

			glViewport(0, 0, gg_MainWindow->GetBufferWidth(), gg_MainWindow->GetBufferHeight());//Set New Buffer Size
			gm_ProjectionMatrix = glm::perspective(glm::radians(FOV), (GLfloat)gi_BufferWidth / (GLfloat)gi_BufferHeight, MIN_VIEWDIST, MAX_VIEWDIST);//Calculate New Projection Matrix
			
			if (CallbackResize != nullptr)
				CallbackResize((float)gi_BufferWidth, (float)gi_BufferHeight);//Call Resize Callback
		}
		else
			LOG_DEBUG3D(gb_Debug,  "Invalid aspect ratio during resize: " + std::to_string(aspectRatio));

	}
//
}

void Simply3D::Update()
{
	gg_Camera->Update();// Update Camera
//Update Object Collition
	for (auto& C1 : garr_Objects)
	{
		C1->Update();
		if (C1->GetColliderType() == NoColliderType)
			continue;
		bool lb_CollitionFound = false;
		for (auto& C2 : garr_Objects)
		{
			if (C2->GetColliderType() == NoColliderType)
				continue;
			if (C1 == C2)
				continue;
			lb_CollitionFound = C1->GetCollider()->IsInCollision(*C2->GetCollider()) || lb_CollitionFound;
		}
		C1->GetCollider()->SetInCollision(lb_CollitionFound);
	}
//
//Update Timers
	for (auto& C1 : garr_Timers)
	{
		C1->CallTimeout(Time::GetCurrentTime());
	}
//

}

void Simply3D::RenderShadows()
{
	ShadowMapShader->StartProgram();//Start Shadow Shader
	glViewport(0, 0, gg_DirectionalLight->GetShadowMap()->GetWidth(), gg_DirectionalLight->GetShadowMap()->GetHeight());//Set Render Buffer Size
	
	gg_DirectionalLight->GetShadowMap()->Write();//Enable Directional Light For Writing

	glClear(GL_DEPTH_BUFFER_BIT);//Clear Buffer

	GLuint lu_Model = ShadowMapShader->GetAttributeID("model");//Get Model ID
	GLuint lu_Projection_View = ShadowMapShader->GetAttributeID("projectionview");//Get Project View ID
	glm::mat4 lm4_Light_Project = glm::perspective(glm::radians(FOV), (GLfloat)gi_BufferWidth / (GLfloat)gi_BufferHeight, MIN_VIEWDIST, gg_DirectionalLight->GetShadowRenderDistance());
	ShadowMapShader->SetAttribute(lu_Projection_View, gg_DirectionalLight->GetTransform(lm4_Light_Project, gg_Camera->GetViewMatrix()));//Set Projection View

	//improved shadow mapping
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(0.0001f, 0.0001f);

	glCullFace(GL_FRONT);//Set Culling

//Render Object Shadows
	for (auto& C1 : garr_Objects)
	{
		if (!C1->GetCastShadow())
			continue;
		
		ShadowMapShader->SetAttribute(lu_Model, C1->GetTransformation());//Set Model
		C1->Render();//Render Object
	}
//

	glDisable(GL_POLYGON_OFFSET_FILL);
	glCullFace(GL_BACK);//Set Culling

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//Bind Frame Buffer
}
void Simply3D::RenderObjects(glm::mat4 pm4_Projection, glm::mat4 pm4_View)
{
	for (auto& C1 : garr_Objects)
	{
		Shader* ll_ActiveShader;
	//Determine Shader
		switch (C1->GetShaderType())
		{
			case BasicShaderType: ll_ActiveShader = BasicShader; break;

			case TexturedBasicShaderType: ll_ActiveShader = TexturedShader; break;

			case TexturedLightShaderType: ll_ActiveShader = TexturedLightShader; break;

			default:
			{
				std::cerr << "Invalid Shader On Object" << std::endl;
				ll_ActiveShader = BasicShader;
			}break;
		}
	//

		GLuint lu_Shadow_Map;
		GLuint lu_Directional_Light_Perspective_View;

		GLuint lu_Model = ll_ActiveShader->GetAttributeID("model");//Get Model ID
		GLuint lu_Perspective = ll_ActiveShader->GetAttributeID("projection");//Get Projection ID
		GLuint lu_View = ll_ActiveShader->GetAttributeID("view");//Get View ID
		GLuint lu_Color = ll_ActiveShader->GetAttributeID("u_Color");//Get Color ID
		GLuint lu_Texture = ll_ActiveShader->GetAttributeID("theTexture");//Get Texture ID

		GLuint lu_Ambient_Light_Intencity = ll_ActiveShader->GetAttributeID("ALight.Intencity");//Get Ambient Light Intencity ID
		GLuint lu_Ambient_Light_Color = ll_ActiveShader->GetAttributeID("ALight.color");//Get Ambient Light Color ID

		GLuint lu_Directional_Light_Intencity = ll_ActiveShader->GetAttributeID("DLight.Intencity");//Get Directional Light Intencity ID
		GLuint lu_Directional_Light_Color = ll_ActiveShader->GetAttributeID("DLight.color");//Get Directional Light Color ID
		GLuint lu_Directional_Light_Direction = ll_ActiveShader->GetAttributeID("DLight.Direction");// Get Directional Light Directional ID
		

		GLint lu_Material_Shine = ll_ActiveShader->GetAttributeID("u_mat.Shiness");//Get Materail Shine ID
		GLint lu_Materail_Intencity = ll_ActiveShader->GetAttributeID("u_mat.Intencity");//Get Materail Intencity ID

		GLint lu_Camera_Position = ll_ActiveShader->GetAttributeID("u_CameraPos");//Get Camera Position ID
		GLint lu_Point_Light_Count = ll_ActiveShader->GetAttributeID("LightCount");//Get Point Light Count ID
		
		ll_ActiveShader->StartProgram();//Activate Shader

		if (C1->GetShaderType() == TexturedLightShaderType)
		{
			gg_AmbientLight->Use(lu_Ambient_Light_Intencity, lu_Ambient_Light_Color);//Set Ambient Light Settigns
			gg_DirectionalLight->Use(lu_Directional_Light_Intencity, lu_Directional_Light_Color, lu_Directional_Light_Direction);//Set Directional Light Settigns
			int li_Light = 0;
		//Set Point Lights Settings
			for (auto& C2 : garr_PointLights)
			{
				if (glm::length(C2->GetPosition() - C1->GetPosition()) > 45.0f)
					continue;
				GLuint lu_Point_Light_Color = ll_ActiveShader->GetAttributeID(("PLight[" + std::to_string(li_Light) + "].color").c_str());
				GLuint lu_Point_Light_Position = ll_ActiveShader->GetAttributeID(("PLight[" + std::to_string(li_Light) + "].Position").c_str());
				GLuint lu_Point_Light_Intencity = ll_ActiveShader->GetAttributeID(("PLight[" + std::to_string(li_Light) + "].Intencity").c_str());
				GLuint lu_Point_Light_Attenuation = ll_ActiveShader->GetAttributeID(("PLight[" + std::to_string(li_Light) + "].AttenuationFormula").c_str());
				C2->Use(lu_Point_Light_Intencity, lu_Point_Light_Color, lu_Point_Light_Position, lu_Point_Light_Attenuation);
				li_Light++;

			}
		//
		}
		lu_Directional_Light_Perspective_View = ll_ActiveShader->GetAttributeID("lightprojectionview");//Get Directional Light Perspective View ID
		glm::mat4 lm4_Light_Project = glm::perspective(glm::radians(FOV), (GLfloat)gi_BufferWidth / (GLfloat)gi_BufferHeight, MIN_VIEWDIST, gg_DirectionalLight->GetShadowRenderDistance());
		ll_ActiveShader->SetAttribute(lu_Directional_Light_Perspective_View, gg_DirectionalLight->GetTransform(lm4_Light_Project, gg_Camera->GetViewMatrix()));//Set Directional Light Perspective View Settigns

	//Set Shadow Texture
		if (C1->GetRecieveShadow())
			gg_DirectionalLight->GetShadowMap()->Read(GL_TEXTURE1);
		else
			gg_No_Shadow->Use(GL_TEXTURE1);
	//
		lu_Shadow_Map = ll_ActiveShader->GetAttributeID("DirectionalShadowMap");//Get Shadow Map ID
		ll_ActiveShader->SetAttribute(lu_Shadow_Map, static_cast<GLuint>(1));//Set Shadow Map Settings

		ll_ActiveShader->SetAttribute(lu_Camera_Position, gg_Camera->GetPosition());//Set Camera Position
		ll_ActiveShader->SetAttribute(lu_Point_Light_Count, static_cast<GLuint>(garr_PointLights.size()));//Set Point Light Count
		ll_ActiveShader->SetAttribute(lu_Texture, static_cast<GLuint>(0));//Set Texture
		ll_ActiveShader->SetAttribute(lu_View, pm4_View);//Set View
		ll_ActiveShader->SetAttribute(lu_Perspective, pm4_Projection);//Set Perspective
		ll_ActiveShader->SetAttribute(lu_Color, C1->GetColor());//Set Color
		ll_ActiveShader->SetAttribute(lu_Model, C1->GetTransformation());//Set Model

		C1->GetMaterial().Use(lu_Materail_Intencity, lu_Material_Shine);//Set Material Settigns
		C1->Render();//Render Model

		ll_ActiveShader->StopProgram();//Stop Shader Program
	}
}

void Simply3D::Render()
{
	RenderSkybox();
	RenderObjects(gm_ProjectionMatrix, gg_Camera->GetViewMatrix());//Render Objects
	RenderDebugLights();
	RenderDebugColliders();
	gg_MainWindow->SwapBuffers();//Swap Buffers
}

void Simply3D::RenderDebugColliders()
{
	//Render Colliders
	bool lb_Wireframe = GetWireframe();
	if (gb_RenderCollition)
	{
		glDisable(GL_CULL_FACE);//Disable Face Culling
		OpenGL::SetWireframe(true);//Enable Wireframe
		//Start Shader Program
		Shader* ll_ActiveShader = BasicShader;
		ll_ActiveShader->StartProgram();
		//

		GLuint lu_Model = ll_ActiveShader->GetAttributeID("model");//Get Model ID
		GLuint lu_Projection = ll_ActiveShader->GetAttributeID("projection");//Get Projection ID
		GLuint lu_View = ll_ActiveShader->GetAttributeID("view");//Get View ID
		GLuint lu_Color = ll_ActiveShader->GetAttributeID("u_Color");//Get Color ID

		ll_ActiveShader->SetAttribute(lu_View, gg_Camera->GetViewMatrix());
		ll_ActiveShader->SetAttribute(lu_Projection, gm_ProjectionMatrix);

		//Render Objects's Collider
		for (auto& C1 : garr_Objects)
		{
			if (C1->GetColliderType() == NoColliderType)
				continue;

			Object* ll_RenderObject = nullptr;
			if (C1->GetColliderType() == BoxColliderType)
				ll_RenderObject = gg_UnitCube;
			if (C1->GetColliderType() == SphereColliderType)
				ll_RenderObject = gg_UnitSphere;

			if (ll_RenderObject == nullptr)
			{
				std::cerr << "Invalid Collider On Object" << std::endl;
				return;
			}

			ll_RenderObject->SetPosition(C1->GetCollider()->GetTotalPosition());
			ll_RenderObject->SetScale(C1->GetCollider()->GetTotalScale());
			ll_RenderObject->SetRotation(glm::vec3(0.0f));

			if (!C1->GetCollider()->GetInCollision())
				ll_ActiveShader->SetAttribute(lu_Color, glm::vec3(1.0f, 1.0f, 1.0f));
			else
				ll_ActiveShader->SetAttribute(lu_Color, glm::vec3(1.0f, 0.0f, 0.0f));

			ll_ActiveShader->SetAttribute(lu_Model, ll_RenderObject->GetTransformation());

			ll_RenderObject->Render();
		}
		//
		OpenGL::SetWireframe(lb_Wireframe);//Disable Wireframe
		glEnable(GL_CULL_FACE);//Enable Face Culling
		ll_ActiveShader->StopProgram();//Stop Shader Program
	}
	//
}
void Simply3D::RenderDebugLights()
{
	//Render Light Objects
	if (gb_RenderLights)
	{
		Shader* ll_ActiveShader = BasicShader;
		ll_ActiveShader->StartProgram();

		GLuint uniformModel = ll_ActiveShader->GetAttributeID("model");
		GLuint uniformPerspective = ll_ActiveShader->GetAttributeID("projection");
		GLuint uniformView = ll_ActiveShader->GetAttributeID("view");
		GLuint uniformColorID = ll_ActiveShader->GetAttributeID("u_Color");

		ll_ActiveShader->SetAttribute(uniformView, gg_Camera->GetViewMatrix());
		ll_ActiveShader->SetAttribute(uniformPerspective, gm_ProjectionMatrix);

		for (auto& C1 : garr_PointLights)
		{
			gg_UnitCube->SetPosition(C1->GetPosition());
			gg_UnitCube->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
			gg_UnitCube->SetColor(C1->GetColor());

			ll_ActiveShader->SetAttribute(uniformColorID, C1->GetColor());
			ll_ActiveShader->SetAttribute(uniformModel, gg_UnitCube->GetTransformation());

			gg_UnitCube->Render();

		}
		ll_ActiveShader->StopProgram();
	}
	//
}

void Simply3D::RenderSkybox()
{
	glViewport(0, 0, gg_MainWindow->GetBufferWidth(), gg_MainWindow->GetBufferHeight());//Resize Buffer
	OpenGL::ClearScreen();//Clear Buffer
	//Render Skybox
	if (gg_Skybox != nullptr && gi_Skybox != NoSkyboxType)
		gg_Skybox->Render(gg_Camera->GetViewMatrix(), gm_ProjectionMatrix);
	//

}

void Simply3D::SetLockedCursor(bool lb_Locked)
{
	gb_LockedCursor = lb_Locked;
	gg_MainWindow->SetLockMouse(gb_LockedCursor);
}

bool Simply3D::GetLockedCursor() const
{
	return gb_LockedCursor;
}

void Simply3D::SetDebug(bool Debug)
{
	gb_Debug = Debug;
}
bool Simply3D::GetDebug() const
{
	return gb_Debug;
}

void Simply3D::SetSkyboxType(const SkyboxTypes& pp_Skybox)
{
	if (gi_Skybox == pp_Skybox)
		return;
	LOG_DEBUG3D(gb_Debug, "Loading Skybox");

	gi_Skybox = pp_Skybox;
//Create Skybox Name List
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Simply3D/Textures/Skybox/left" + std::to_string(gi_Skybox) + ".png");
	skyboxFaces.push_back("Simply3D/Textures/Skybox/right" + std::to_string(gi_Skybox) + ".png");
	skyboxFaces.push_back("Simply3D/Textures/Skybox/down" + std::to_string(gi_Skybox) + ".png");
	skyboxFaces.push_back("Simply3D/Textures/Skybox/up" + std::to_string(gi_Skybox) + ".png");
	skyboxFaces.push_back("Simply3D/Textures/Skybox/back" + std::to_string(gi_Skybox) + ".png");
	skyboxFaces.push_back("Simply3D/Textures/Skybox/front" + std::to_string(gi_Skybox) + ".png");
//
//Clear Skybox
	if (gg_Skybox != nullptr)
	{
		delete gg_Skybox;
		gg_Skybox = nullptr;
	}
//
//Create New Skybox
	if (gi_Skybox == 4 or gi_Skybox == 5)
		gg_Skybox = new Skybox(skyboxFaces, true);
	else
		gg_Skybox = new Skybox(skyboxFaces);

	LOG_DEBUG3D(gb_Debug, "Loading Skybox Completed");
//
}

SkyboxTypes Simply3D::GetSkyboxType() const
{
	return gi_Skybox;
}

void Simply3D::SetMoveSpeed(float pf_NewMoveSpeed)
{
	gf_MoveSpeed = pf_NewMoveSpeed;
}
float Simply3D::GetMoveSpeed() const
{
	return gf_MoveSpeed;
}

void Simply3D::SetMoveKey(MoveTypes TypeToChange, int NewKey)
{
	switch (TypeToChange)
	{
		case ForwardMoveType:
			gg_ForwardMoveKey = NewKey;
			break;
		case BackMoveType:
			gg_BackMoveKey = NewKey;
			break;
		case LeftMoveType:
			gg_LeftMoveKey = NewKey;
			break;
		case RightMoveType:
			gg_RightMoveKey = NewKey;
			break;
		default:
			std::cerr << "Invalid Move Type" << std::endl;
			break;
	}
}
int Simply3D::GetMoveKey(MoveTypes Key) const
{
	switch (Key)
	{
		case ForwardMoveType:
			return gg_ForwardMoveKey;
			break;
		case BackMoveType:
			return gg_BackMoveKey;
			break;
		case LeftMoveType:
			return gg_LeftMoveKey;
			break;
		case RightMoveType:
			return gg_RightMoveKey;
			break;
		default:
			return KEY_NONE;
			break;
	}
}

void Simply3D::SetRenderLights(bool RenderLights)
{
	gb_RenderLights = RenderLights;
}
bool Simply3D::GetRenderLights() const
{
	return gb_RenderLights;
}

void Simply3D::SetRenderCollision(bool RenderCollision)
{
	gb_RenderCollition = RenderCollision;
}
bool Simply3D::GetRenderCollision() const
{
	return gb_RenderCollition;
}

Camera* Simply3D::GetCamera() const
{
	return gg_Camera;
}

void Simply3D::SetCamera(Camera& pp_NewCamera)
{
	gg_Camera = &pp_NewCamera;
}

void Simply3D::SetWindow(Window& NewWindow)
{
	gg_MainWindow = &NewWindow;
}
Window* Simply3D::GetWindow() const
{
	return gg_MainWindow;
}

void Simply3D::SetAmbientLight(AmbientLight& newAmbientLight)
{
	gg_AmbientLight = &newAmbientLight;
}
AmbientLight* Simply3D::GetAmbientLight() const
{
	return gg_AmbientLight;
}

void Simply3D::SetDirectionalLight(DirectionalLight& newDirectionalLight)
{
	gg_DirectionalLight = &newDirectionalLight;
}
DirectionalLight* Simply3D::GetDirectionalLight() const
{
	return gg_DirectionalLight;
}


void Simply3D::SetWireframe(bool pb_Wireframe)
{
	gb_Wireframe = pb_Wireframe;
	OpenGL::SetWireframe(gb_Wireframe);
}
bool Simply3D::GetWireframe() const
{
	return gb_Wireframe;
}
