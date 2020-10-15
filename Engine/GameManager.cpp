#include "pch.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "DeviceManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "ScriptManager.h"
#include "SceneManager.h"
#include "MeshManager.h"
#include "PhysxManager.h"
#include "EditorShaderStorage.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Timers;
using namespace SteffEngine::Scripting;
using namespace SteffEngine::Editor;

GameManager* GameManager::m_pInstance{ nullptr };

PLUGIN_API
GameManager* GameManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new GameManager{};
	}

	return m_pInstance;
}

GameManager::GameManager()
	: m_pRenderer(nullptr)
{

}

void GameManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

GameManager::~GameManager()
{
	SAFE_DELETE(m_pRenderer);

	InputManager::GetInstance()->SetEnabled(false);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SceneManager::Destroy();
	PhysxManager::Destroy();
	MeshManager::Destroy();
	TextureManager::Destroy();
	ConfigManager::Destroy();
	ShaderManager::Destroy();
	InputManager::Destroy();
	CameraManager::Destroy();
	FrameTimer::Destroy();
	ScriptManager::Destroy();

	Debugging::CleanUp();

	DeviceManager::Destroy();
}

void GameManager::Initialize(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight)
{
	Logging::Log(LogType::INFORMATION, "Started initialization...");

	WINDOW_WIDTH = windowWidth;
	WINDOW_HEIGHT = windowHeight;

#ifdef _DEBUG
	// =-=-=-=-= INITIALIZING CONSOLE =-=-=-=-= //
	AllocConsole();
	freopen_s((FILE**)stdin, "CONIN$", "r", stdout);
	freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

	// =-=-=-=-= INITIALIZING DIRECTX DEVICE =-=-=-=-= //
	DeviceManager::Initialize();

	// =-=-=-=-= INITIALIZING RENDERER =-=-=-=-= //
	m_pRenderer = new Renderer{ windowHandle, WINDOW_WIDTH, WINDOW_HEIGHT };
	m_pRenderer->SetRenderState(RenderState::SOLID);

	// =-=-=-=-= INITIALIZING PHYSX =-=-=-=-= //
	PhysxManager::GetInstance()->Initialize();

	// =-=-=-=-= INITIALIZING DEAR IMGUI =-=-=-=-= //
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplDX11_Init(DeviceManager::m_pDevice, DeviceManager::m_pDeviceContext);
	ImGui_ImplWin32_Init(windowHandle);
	ImGui::StyleColorsDark();

	// =-=-=-=-= INITIALIZING SHADER EDITOR =-=-=-=-= //
	EditorShaderStorage::Initialize();

	// =-=-=-=-= CREATING SCENE =-=-=-=-= //
	SceneManager::GetInstance()->CreateScene("testing");
	SceneManager::GetInstance()->SetActiveScene("testing");

	// =-=-=-=-= CREATING CAMERA =-=-=-=-= //
	GameObject* pGameObject{ SceneManager::GetInstance()->GetActiveScene()->CreateGameObject("Default Camera") };
	Components::CameraComponent* pCameraComponent{ pGameObject->CreateComponent<Components::CameraComponent>(60.f) };
	pCameraComponent->GetTransform()->SetPosition(0.f, 0.f, -50.f);
	pCameraComponent->Activate();

	// =-=-=-=-= LOADING SCRIPTS =-=-=-=-= //
	ScriptManager::GetInstance()->LoadScripts();
}

void GameManager::Tick()
{
	FrameTimer::GetInstance()->SetEndFrame();
	FrameTimer::GetInstance()->SetStartFrame();

	Update();
	Render();
}

void GameManager::Update()
{
	InputManager::GetInstance()->SendEvents();

	SceneManager::GetInstance()->FixedUpdate(); // TODO: MAKE THE FIXED UPDATE (ONLY EXECUTE THIS FUNCTION AT SPECIFIC TIMES)
	SceneManager::GetInstance()->Update();
	SceneManager::GetInstance()->LateUpdate();

	if (DEBUG_TOOLS_ENABLED)
		Debugging::Update();
}

void GameManager::Render()
{
	// Initialize frame
	m_pRenderer->InitializeFrame();

	// Initialize imgui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		// Frame processing
		if (CameraManager::GetInstance()->GetActiveCamera() != nullptr)
		{
			SceneManager::GetInstance()->Render(*m_pRenderer);

			if (DEBUG_TOOLS_ENABLED)
				Debugging::Render(*m_pRenderer);
		}
	}

	// Render imgui frame
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Render frame
	m_pRenderer->PresentFrame();
}