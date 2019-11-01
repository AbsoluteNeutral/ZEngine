
#ifndef _ZG_ENGINE_H
#define _ZG_ENGINE_H

#ifdef _EXPORT_ENGINE
	#define ZG_API __declspec(dllexport)
#else
	#define ZG_API __declspec(dllimport)
#endif

#pragma warning(disable : 4996 4616 4251) //dllexport

#define SOL_CHECK_ARGUMENTS 1

//#define _CRT_NONSTDC_NO_DEPRECATE

#include <iostream>
#include <vector>
#include <map>
#include <string>

//3rd party
#include "GL/glew.h"
#include <GL/gl.h>

#include "fmod.hpp"

#ifdef USING_SOL2
#include <sol.hpp>
#endif

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/opengl_interop.hpp"

#include "zgconfig.h"

//Lib
#include "ZGMath.h"
#include "System.h"

#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)

#include "imgui.h"
#include "ImGuizmo.h"
#include "Engine.h"
#include "ImguiWrapper.h"

#endif

//UnitTest

#include "zgAllocator.h"
#include "MemoryManager.h"

//Assets
#include "AssetsManager.h"
//#include "../VideoLoader.h"

//Engine
#include "IEngine.h"
#include "ProjectPath.h"
#include "Logging.h"
#include "XMLWrap.h"

#include "AudioListener.h"
#include "inputkeydef.h"
#include "FrameRateController.h"
#include "XboxInput.h"

//Components
#include "GameObject.h"
#include "Component.h"
#include "Camera.h"
//#include "../Light.h"
#include "MeshRenderer.h"
//#include "../Particle.h"
//#include "../ParticleEmitter.h"
//#include "../Script.h"
//#include "../Sound.h"
//#include "../Sprite2D.h"
//#include "../TextRenderer.h"

////Components/Collider
//#include "../AABB.h"
//#include "../BC.h"
//#include "../Collision.h"
//#include "../OBB.h"
//#include "../Resolution.h"
//#include "../RigidBody.h"
//#include "../Detect.h"
//#include "../RayCast.h"
//


//
////Manager
#include "Gizmo.h"
#include "ResourceManager.h"
//
////GSM
//#include "../GameStateManager.h"
//#include "../SceneGraph.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#endif //_EFF_ENGINE_H
