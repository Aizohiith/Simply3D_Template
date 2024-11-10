#pragma once
#include "Material.h"

enum ShaderTypes { NoShaderType = 0, BasicShaderType, TexturedBasicShaderType, TexturedLightShaderType, MaxShaderType};
enum ObjectTypes { CubeObjectType = 0, PlaneObjectType, WallObjectType, SphereObjectType, DonutObjectType, ConeObjectType, CylinderObjectType, IcosphereObjectType, MaxObjectType};
enum SkyboxTypes { NoSkyboxType = -1, GraySkyboxType, OuterSpaceSkyboxType, SkyBlueSkyboxType, OceanVallySkyboxType, SnowyMountainsSkyboxType, ManHattain, MaxSkyboxType};
enum ColliderTypes { NoColliderType, BoxColliderType, SphereColliderType};
enum MoveTypes {ForwardMoveType, BackMoveType, LeftMoveType, RightMoveType, UpMoveType, DownMoveType};

namespace MaterialTypes
{
	static const Material None(0.0f, 1, false);
	static const Material NoneSmooth(0.0f, 1, true);
	static const Material SuperMetal(10.f, 256, false);
	static const Material SuperMetalSmooth(10.f, 256, true);

	// Highly Reflective Metals
	static const Material ChromeSmooth(0.90f, 225, true);
	static const Material Chrome(0.90f, 225, false);
	static const Material TitaniumSmooth(0.85f, 220, true);
	static const Material Titanium(0.85f, 220, false);
	static const Material SilverSmooth(0.85f, 200, true);
	static const Material Silver(0.85f, 200, false);
	static const Material AluminiumSmooth(0.75f, 175, true);
	static const Material Aluminium(0.75f, 175, false);
	static const Material GoldSmooth(0.75f, 150, true);
	static const Material Gold(0.75f, 150, false);
	static const Material CopperSmooth(0.70f, 150, true);
	static const Material Copper(0.70f, 150, false);
	static const Material BrassSmooth(0.65f, 125, true);
	static const Material Brass(0.65f, 125, false);
	static const Material SteelSmooth(0.60f, 120, true);
	static const Material Steel(0.60f, 120, false);
	static const Material IronSmooth(0.55f, 100, true);
	static const Material Iron(0.55f, 100, false);
	static const Material BronzeSmooth(0.50f, 100, true);
	static const Material Bronze(0.50f, 100, false);

	// Moderately Reflective Non-Metals
	static const Material GlassSmooth(0.50f, 150, true);
	static const Material Glass(0.50f, 150, false);
	static const Material CeramicSmooth(0.45f, 125, true);
	static const Material Ceramic(0.45f, 125, false);
	static const Material MarbleSmooth(0.40f, 100, true);
	static const Material Marble(0.40f, 100, false);
	static const Material RubberSmooth(0.35f, 75, true);
	static const Material Rubber(0.35f, 75, false);
	static const Material PlasticSmooth(0.30f, 50, true);
	static const Material Plastic(0.30f, 50, false);
	static const Material ConcreteSmooth(0.25f, 50, true);
	static const Material Concrete(0.25f, 50, false);
	static const Material BrickSmooth(0.20f, 40, true);
	static const Material Brick(0.20f, 40, false);
	static const Material WoodSmooth(0.15f, 25, true);
	static const Material Wood(0.15f, 25, false);

	// Low Reflective Non-Metals
	static const Material LeatherSmooth(0.10f, 20, true);
	static const Material Leather(0.10f, 20, false);
	static const Material FabricSmooth(0.08f, 15, true);
	static const Material Fabric(0.08f, 15, false);
	static const Material PaperSmooth(0.05f, 5, true);
	static const Material Paper(0.05f, 5, false);
	static const Material SandSmooth(0.03f, 10, true);
	static const Material Sand(0.03f, 10, false);
	static const Material SoilSmooth(0.02f, 8, true);
	static const Material Soil(0.02f, 8, false);
	static const Material ClaySmooth(0.02f, 5, true);
	static const Material Clay(0.02f, 5, false);

	// Organic Materials
	static const Material BarkSmooth(0.01f, 5, true);
	static const Material Bark(0.01f, 5, false);
	static const Material GrassSmooth(0.01f, 3, true);
	static const Material Grass(0.01f, 3, false);
	static const Material LeavesSmooth(0.01f, 3, true);
	static const Material Leaves(0.01f, 3, false);
	static const Material MossSmooth(0.01f, 2, true);
	static const Material Moss(0.01f, 2, false);
	static const Material FurSmooth(0.01f, 2, true);
	static const Material Fur(0.01f, 2, false);

	// Additional Materials
	static const Material AsphaltSmooth(0.10f, 20, true);
	static const Material Asphalt(0.10f, 20, false);
	static const Material IceSmooth(0.70f, 150, true);
	static const Material Ice(0.70f, 150, false);
	static const Material SnowSmooth(0.05f, 10, true);
	static const Material Snow(0.05f, 10, false);
	static const Material StoneSmooth(0.20f, 40, true);
	static const Material Stone(0.20f, 40, false);
	static const Material SandstoneSmooth(0.15f, 35, true);
	static const Material Sandstone(0.15f, 35, false);
	static const Material GraniteSmooth(0.30f, 60, true);
	static const Material Granite(0.30f, 60, false);
	static const Material LimestoneSmooth(0.25f, 50, true);
	static const Material Limestone(0.25f, 50, false);
	static const Material MudSmooth(0.05f, 10, true);
	static const Material Mud(0.05f, 10, false);

}
