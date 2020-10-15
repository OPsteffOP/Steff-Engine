// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <vld.h>

#include "GameManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "MeshRendererComponent.h"
#include "InstancedMeshRendererComponent.h"
#include "CameraManager.h"
#include "PosTexInstanceVertexShader.h"
#include "TexColPixelShader.h"
#include "PosTexVertexShader.h"

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
    CameraComponent* pCamera{ CameraManager::GetInstance()->GetActiveCamera() };
    pCamera->GetTransform()->SetPosition(0.f, 10.f, 0.f);
    pCamera->GetTransform()->Rotate(0.f, 45.f, 0.f);

    GameObject* pGameObject{ SceneManager::GetInstance()->GetActiveScene()->CreateGameObject() };
    InstancedMeshRendererComponent* pInstancedMeshRendererComponent{ pGameObject->CreateComponent<InstancedMeshRendererComponent>(
        100u,
        L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Models\\cube.obj",
        new PosTexInstanceVertexShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\InstancedMeshVertexShader.cso" },
        new TexColPixelShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\MeshPixelShader.cso",
            L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Textures\\vehicle_diffuse.png" }) };

    for (int i{ 0 }; i < 10; ++i)
    {
        for (int j{ 0 }; j < 10; ++j)
        {
            // WITHOUT INSTANCING
            //GameObject* pGameObject{ SceneManager::GetInstance()->GetActiveScene()->CreateGameObject(XMFLOAT3{ 50.f * j, 0.f, 50.f * i }) };
            //pGameObject->AddComponent(new MeshRendererComponent{
            //    L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Models\\cube.obj",
            //    new PosTexVertexShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\MeshVertexShader.cso" },
            //    new TextureColorPixelShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\MeshPixelShader.cso",
            //        L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Textures\\vehicle_diffuse.png" } });
            //pGameObject->GetTransform().Move(50.f * j, 0.f, 50.f * i);

            // WITH INSTANCING
            pInstancedMeshRendererComponent->AddInstance(
                L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Textures\\vehicle_diffuse.png",
                XMFLOAT3{ 50.f * j, 0.f, 50.f * i }
            );
        }
    }
}