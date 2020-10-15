#include "pch.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "FrameTimer.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;
using namespace SteffEngine::Timers;

CameraManager* CameraManager::m_pInstance{ nullptr };

PLUGIN_API
CameraManager* CameraManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CameraManager{};
	}

	return m_pInstance;
}

CameraManager::CameraManager()
	: m_pActiveCamera(nullptr)
{
	SubscribeInput();
}

void CameraManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

PLUGIN_API
void CameraManager::SetActiveCamera(CameraComponent* pCamera)
{
	if (m_pActiveCamera != nullptr)
		m_pActiveCamera->m_IsActive = false;

	m_pActiveCamera = pCamera;
	m_pActiveCamera->m_IsActive = true;
}

void CameraManager::SubscribeInput()
{
	InputManager::GetInstance()->SubscribeEvents([this](const KeyInput& keyInput)
	{
		const float moveSpeed{ 10.f * FrameTimer::GetInstance()->GetElapsedSec() };
		const float rotateSpeed{ 10.f * FrameTimer::GetInstance()->GetElapsedSec() };
		if (keyInput.key == Key::A && (keyInput.type == Type::DOWN || keyInput.type == Type::HOLD))
		{
			CameraManager::GetInstance()->GetActiveCamera()->GetTransform()->Move(-moveSpeed, 0.f, 0.f);
		}
		else if (keyInput.key == Key::D && (keyInput.type == Type::DOWN || keyInput.type == Type::HOLD))
		{
			CameraManager::GetInstance()->GetActiveCamera()->GetTransform()->Move(moveSpeed, 0.f, 0.f);
		}
		else if (keyInput.key == Key::Z && (keyInput.type == Type::DOWN || keyInput.type == Type::HOLD))
		{
			CameraManager::GetInstance()->GetActiveCamera()->GetTransform()->Move(0.f, 0.f, moveSpeed);
		}
		else if (keyInput.key == Key::S && (keyInput.type == Type::DOWN || keyInput.type == Type::HOLD))
		{
			CameraManager::GetInstance()->GetActiveCamera()->GetTransform()->Move(0.f, 0.f, -moveSpeed);
		}
		else if (keyInput.key == Key::Space && (keyInput.type == Type::DOWN || keyInput.type == Type::HOLD))
		{
			CameraManager::GetInstance()->GetActiveCamera()->GetTransform()->Move(0.f, moveSpeed, 0.f);
		}
		else if (keyInput.key == Key::Right && (keyInput.type == Type::DOWN || keyInput.type == Type::HOLD))
		{
			CameraManager::GetInstance()->GetActiveCamera()->GetTransform()->Rotate(0.f, rotateSpeed, 0.f);
		}
		else if (keyInput.key == Key::Left && (keyInput.type == Type::DOWN || keyInput.type == Type::HOLD))
		{
			CameraManager::GetInstance()->GetActiveCamera()->GetTransform()->Rotate(0.f, -rotateSpeed, 0.f);
		}
		else if (keyInput.key == Key::Up && (keyInput.type == Type::DOWN || keyInput.type == Type::HOLD))
		{
			CameraManager::GetInstance()->GetActiveCamera()->GetTransform()->Rotate(-rotateSpeed, 0.f, 0.f);
		}
		else if (keyInput.key == Key::Down && (keyInput.type == Type::DOWN || keyInput.type == Type::HOLD))
		{
			CameraManager::GetInstance()->GetActiveCamera()->GetTransform()->Rotate(rotateSpeed, 0.f, 0.f);
		}
	});
}