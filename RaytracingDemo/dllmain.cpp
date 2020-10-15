// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <vld.h>

#include "GameManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "MeshRendererComponent.h"
#include "PosTexVertexShader.h"
#include "TexColPixelShader.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

ENGINE_API
void OnEnable()
{
    GameObject* pGameObject{ SceneManager::GetInstance()->GetActiveScene()->CreateGameObject() };
    pGameObject->AddComponent(new MeshRendererComponent{
        L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Models\\vehicle.obj",
        new PosTexVertexShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\MeshVertexShader.cso" },
        new TexColPixelShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\MeshPixelShader.cso",
            L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Textures\\vehicle_diffuse.png" }
    });

    InputManager::GetInstance()->SubscribeEvents([](const MouseInput& mouseInput) 
        {
            Raycasting::HitRecord hitRecord{};
            if (Raycasting::Raycast(hitRecord, mouseInput.position))
            {
                hitRecord.pGameObject->GetComponent<MeshRendererComponent>()->SetHighlightColor(XMFLOAT4{ 0.25f, 0.25f, 0.25f, 1.f });
                Debugging::DrawPermanentLine(ScreenToWorld(mouseInput.position), hitRecord.intersectionPoint);
            }
        });
}