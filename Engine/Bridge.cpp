#include "pch.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "FrameTimer.h"
#include "EditorComponentNames.h"
#include "EditorSceneStorage.h"
#include "SceneManager.h"
#include "DeviceManager.h"
#include "ShaderType.h"
#include "EditorShaderStorage.h"
#include "ScriptManager.h"
#include "ScriptComponentNamesStruct.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Timers;

#pragma region GAMEMANAGER
ENGINE_API
void __stdcall CleanUp()
{
	GameManager::Destroy();
}

ENGINE_API
void __stdcall Initialize(HWND windowHandle, uint32_t windowWidth, uint32_t windowHeight)
{
	GameManager::GetInstance()->Initialize(windowHandle, windowWidth, windowHeight);
}

ENGINE_API
void __stdcall Tick()
{
	GameManager::GetInstance()->Tick();
}

ENGINE_API
void __stdcall ToggleDebuggingTools()
{
	DEBUG_TOOLS_ENABLED = !DEBUG_TOOLS_ENABLED;
}
#pragma endregion

#pragma region INPUT
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
ENGINE_API
bool __stdcall ProcessWindowsMessage(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(windowHandle, msg, wParam, lParam))
		return false;

	return true;
}

ENGINE_API
void __stdcall SetInputEnabled(bool isEnabled)
{
	InputManager::GetInstance()->SetEnabled(isEnabled);
}

ENGINE_API
void __stdcall AddKeyInput(const KeyInput& keyInput)
{
	InputManager::GetInstance()->AddKeyInput(keyInput);
}

ENGINE_API
void __stdcall AddMouseInput(const MouseInput& mouseInput)
{
	InputManager::GetInstance()->AddMouseInput(mouseInput);
}
#pragma endregion

#pragma region GAMEOBJECTS
ENGINE_API
size_t GetGameObjectsData(const SteffEngine::Editor::GameObjectData*& pArrayBegin)
{
	const std::vector<SteffEngine::Editor::GameObjectData>& gameObjectNames{ SteffEngine::Editor::EditorSceneStorage::GetGameObjectsData() };

	pArrayBegin = gameObjectNames.data();
	return gameObjectNames.size();
}

ENGINE_API
size_t GetGameObjectComponents(unsigned int gameObjectID, const unsigned int*& pArrayBegin)
{
	const std::vector<unsigned int>& componentsList{ SteffEngine::Editor::EditorSceneStorage::GetGameObjectComponents(gameObjectID) };

	pArrayBegin = componentsList.data();
	return componentsList.size();
}

ENGINE_API
unsigned int CreateGameObject(const char*& pGameObjectName)
{
	const GameObject* pGameObject{ SceneManager::GetInstance()->GetActiveScene()->CreateGameObject() };

	pGameObjectName = pGameObject->GetName();
	return pGameObject->GetGameObjectID();
}
#pragma endregion

#pragma region COMPONENTS
ENGINE_API
size_t __stdcall GetComponentNames(const char**& ptr)
{
	std::vector<ScriptComponentNamesStruct> scriptsComponentNameData{ SteffEngine::Scripting::ScriptManager::GetInstance()->CallFunction<ScriptComponentNamesStruct>("GetComponentNames") };
	scriptsComponentNameData.push_back(ScriptComponentNamesStruct{ SteffEngine::Editor::GetComponentNamesSize(), SteffEngine::Editor::GetComponentNames() });

	size_t length{};
	for (const ScriptComponentNamesStruct& scriptComponentNameData : scriptsComponentNameData)
	{
		length += scriptComponentNameData.length;
	}

	ptr = static_cast<const char**>(::operator new(length * sizeof(void*))); // Create the array of char pointers (to combine the names of the engine + scripts)
	size_t currentInsertPoint{};
	for (const ScriptComponentNamesStruct& scriptComponentNameData : scriptsComponentNameData)
	{
		// Copying the array data to the combined array
		std::memcpy(ptr + currentInsertPoint, scriptComponentNameData.array, scriptComponentNameData.length * sizeof(void*));
		currentInsertPoint += scriptComponentNameData.length;
	}

	return length;
}

ENGINE_API
void __stdcall FreeComponentNamesArray(size_t size, const char** ptr)
{
	::operator delete(ptr, size * sizeof(void*));
}

ENGINE_API
size_t __stdcall GetComponentData(unsigned int id, const SteffEngine::Editor::ExposedVariableData*& pArrayBegin, const char*& pComponentName)
{
	const std::pair<size_t, const SteffEngine::Editor::ExposedVariableData*> componentData{ std::move(SteffEngine::Editor::EditorSceneStorage::GetComponentData(id)) };

	pArrayBegin = componentData.second;
	pComponentName = SteffEngine::Editor::EditorSceneStorage::GetComponent(id)->GetComponentName();
	return componentData.first;
}

ENGINE_API
int __stdcall CreateComponent(unsigned int gameObjectID, const char* componentName)
{
	GameObject* pGameObject{ SteffEngine::Editor::EditorSceneStorage::GetGameObject(gameObjectID) };
	if (pGameObject == nullptr)
		return -1;

	return int(pGameObject->CreateComponent(std::hash<std::string>{}(componentName))->GetComponentID());
}

ENGINE_API
void __stdcall VariableChanged(unsigned int componentID, const char* variableName)
{
	SteffEngine::Core::Components::Component* pComponent{ SteffEngine::Editor::EditorSceneStorage::GetComponent(componentID) };
	if (pComponent != nullptr)
		pComponent->VariableChanged(variableName);
}
#pragma endregion

#pragma region SHADERS
ENGINE_API
int __stdcall CompileShader(const char* shaderPath, ShaderType type, const char*& errorMessage)
{
	const std::pair<const char*, int> errorMessagePair{ SteffEngine::Editor::EditorShaderStorage::CompileShader(shaderPath, type) };
	errorMessage = errorMessagePair.first;
	return errorMessagePair.second;
}

ENGINE_API
unsigned int LoadShader(const char* shaderName)
{
	return SteffEngine::Editor::EditorShaderStorage::LoadShader(shaderName);
}

ENGINE_API
unsigned int CreateShaderData(const char* shaderName, const char* compiledShaderPath)
{
	return SteffEngine::Editor::EditorShaderStorage::AddShaderData(shaderName, compiledShaderPath);
}

ENGINE_API
void AddShaderData(unsigned int shaderID, const char* propertyValue, ShaderType shaderType)
{
	SteffEngine::Editor::EditorShaderStorage::AddProperty(shaderID, propertyValue, shaderType);
}
#pragma endregion