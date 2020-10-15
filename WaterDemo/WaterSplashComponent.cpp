#include "pch.h"
#include "WaterSplashComponent.h"
#include "GameObject.h"
#include "MeshRendererComponent.h"
#include "WaterPixelShader.h"
#include "InputManager.h"
#include "FrameTimer.h"

using namespace SteffEngine::Core::Components;
using namespace SteffEngine::Physics::Raycasting;
using namespace SteffEngine::Timers;

COMPONENT_DEFINITION(WaterSplashComponent);

WaterSplashComponent::WaterSplashComponent()
    : m_pMeshRendererComponent(nullptr)
    , m_SplashSizeIncrease(10.f, 10.f)
    , m_SplashMaxSize(10.f, 10.f)
    , m_SplashCurrentSize(-1.f, -1.f)
{

}

void WaterSplashComponent::Initialize()
{
    m_pMeshRendererComponent = m_pGameObject->GetComponent<MeshRendererComponent>();

    InputManager::GetInstance()->SubscribeEvents([this](const MouseInput& mouseInput)
        {
            Raycasting::HitRecord hitRecord{};
            if (Raycasting::Raycast(hitRecord, mouseInput.position))
            {
                HandleRayCollision(hitRecord);
            }
        });
}

void WaterSplashComponent::Update()
{
    if (m_SplashCurrentSize.x + 1.f < FLT_EPSILON || m_SplashCurrentSize.y + 1.f < FLT_EPSILON)
        return;

    m_SplashCurrentSize.x += m_SplashSizeIncrease.x * FrameTimer::GetInstance()->GetElapsedSec();
    m_SplashCurrentSize.y += m_SplashSizeIncrease.y * FrameTimer::GetInstance()->GetElapsedSec();

    if (m_SplashCurrentSize.x > m_SplashMaxSize.x || m_SplashCurrentSize.y > m_SplashMaxSize.y)
    {
        m_SplashCurrentSize = XMFLOAT2{ -1.f, -1.f };

        WaterPixelShader* pWaterPixelShader{ static_cast<WaterPixelShader*>(m_pMeshRendererComponent->GetPixelShader()) };
        pWaterPixelShader->SetSplashUVLeftTop(XMFLOAT2{ -1.f, -1.f });
    }

    UpdateSplashSize();
}

void WaterSplashComponent::HandleRayCollision(const HitRecord& hitRecord)
{
    m_SplashCurrentSize = XMFLOAT2{ 1.f, 1.f };
    UpdateSplashSize();

	const std::pair<XMFLOAT3, XMFLOAT3> collisionBox{ m_pMeshRendererComponent->GetCollisionBox() };
    const XMFLOAT2 planeSize{ std::fabsf(collisionBox.second.x - collisionBox.first.x), 
        std::fabsf(collisionBox.second.z - collisionBox.first.z) };
    const XMFLOAT2 splashPosition{ XMFLOAT2{ std::fabsf(hitRecord.intersectionPoint.x - (hitRecord.pGameObject->GetTransform().GetPosition().x - (planeSize.x / 2.f))),
        std::fabsf(hitRecord.intersectionPoint.z - (hitRecord.pGameObject->GetTransform().GetPosition().z - (planeSize.y / 2.f))) } };
	
    WaterPixelShader* pWaterPixelShader{ static_cast<WaterPixelShader*>(m_pMeshRendererComponent->GetPixelShader()) };
    pWaterPixelShader->SetSplashUVLeftTop(XMFLOAT2{ 1.f - splashPosition.x / planeSize.x, 1.f - splashPosition.y / planeSize.y });
}

void WaterSplashComponent::UpdateSplashSize()
{
    const XMFLOAT2 planeSize{ std::fabsf(m_pMeshRendererComponent->GetCollisionBox().second.x - m_pMeshRendererComponent->GetCollisionBox().first.x),
        std::fabsf(m_pMeshRendererComponent->GetCollisionBox().second.z - m_pMeshRendererComponent->GetCollisionBox().first.z) };
    const XMFLOAT2 splashUVSize{ m_SplashCurrentSize.x / planeSize.x, m_SplashCurrentSize.y / planeSize.y };
    static_cast<WaterPixelShader*>(m_pMeshRendererComponent->GetPixelShader())->SetSplashUVSize(splashUVSize);
}