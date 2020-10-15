#pragma once

#pragma region MACROS
#if (defined(ENGINE_EXPORTS) || defined(PLUGIN_EXPORTS))
#define ENGINE_API extern "C" __declspec(dllexport)
#define PLUGIN_API __declspec(dllexport)
#else
#define ENGINE_API extern "C" __declspec(dllimport)
#define PLUGIN_API __declspec(dllimport)
#endif

#define SAFE_DELETE(object) if(object != nullptr) { delete object; object = nullptr; }
#define SAFE_DELETE_CONST(object) if(object != nullptr) { delete object; }
#define SAFE_DELETE_DIRECTX(object) if(object != nullptr) { object->Release(); object = nullptr; }
#define SAFE_DELETE_PHYSX(object) if(object != nullptr) { object->release(); object = nullptr; }

#define IDENTITY_MATRIX XMFLOAT4X4{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f }
#define DEFAULT_GAMEOBJECT_NAME "GameObject"

#define EDITOR_READONLY
#define EDITOR_READWRITE
#define EDITOR_HIDDEN
#define EDITOR_RESERVED(type)
#pragma endregion

#pragma region DIRECTX
#define NOMINMAX  //for directx

#pragma comment(lib, "dxgi.lib")

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma endregion

#pragma region DIRECTX_MATH
#include <DirectXMath.h>
using namespace DirectX;
#pragma endregion

#pragma region PHYSX
#include <PxPhysicsAPI.h>
#pragma endregion

#define _USE_MATH_DEFINES

#pragma region INCLUDES
#include "Tools.h"
#include "Logging.h"
#include "ConfigManager.h"
#include "Raycast.h"
#include "Debugging.h"
#include "Typedefs.h"
#include "GameObject.h"

#include "imgui.h"

#include "PhysxHelper.h"

#include <intrin.h>
#include <vector>
#include <string_view>
#include <algorithm>
#include <math.h>
#include <string>
#pragma endregion

#pragma region NAMESPACES
using namespace SteffEngine;
using namespace SteffEngine::Tools;
using namespace SteffEngine::Physics;
#pragma endregion

#pragma region GLOBAL_VARIABLES
inline extern unsigned int WINDOW_WIDTH = 0;
inline extern unsigned int WINDOW_HEIGHT = 0;

#ifdef _DEBUG
inline extern bool DEBUG_TOOLS_ENABLED = true;
#else
inline extern bool DEBUG_TOOLS_ENABLED = false;
#endif

inline extern const std::wstring CONFIG_FOLDER = L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Configs\\";
inline extern const std::wstring SHADERS_FOLDER = L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Shaders\\";
inline extern const std::wstring BINARY_MESHES_FOLDER = L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Binary\\";
inline extern const std::wstring EDITOR_DATA_FOLDER = L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\EditorData\\";
#pragma endregion