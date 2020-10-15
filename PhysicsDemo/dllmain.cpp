// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <vld.h>

#include "CameraComponent.h"
#include "CameraManager.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "MeshRendererComponent.h"
#include "PosTexVertexShader.h"
#include "TexColPixelShader.h"
#include "RigidBodyComponent.h"
#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "PlaneColliderComponent.h"
#include "CharacterControllerComponent.h"

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
    pCamera->GetTransform()->SetPosition(-75.f, 10.f, -75.f);
    pCamera->GetTransform()->Rotate(0.f, 45.f, 0.f);

    GameObject* pGameObject{ SceneManager::GetInstance()->GetActiveScene()->CreateGameObject() };
    pGameObject->AddComponent(new MeshRendererComponent{
        L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Models\\cube.obj",
        new PosTexVertexShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\MeshVertexShader.cso" },
        new TexColPixelShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\MeshPixelShader.cso",
            L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Textures\\vehicle_diffuse.png" } });
    //pGameObject->AddComponent(new RigidBodyComponent{});
    //pGameObject->AddComponent(new BoxColliderComponent{ 50.f, 50.f, 50.f, 0.f, 0.f, 0.f });
    //pGameObject->AddComponent(new SphereColliderComponent{ 25.f, 0.f, 0.f, 0.f });
    //pGameObject->AddComponent(new CapsuleColliderComponent{ 25.f, 100.f, 0.f, 0.f, 0.f });
    //pGameObject->AddComponent(new PlaneColliderComponent{ 0.f, 0.f, 0.f });
    
    //pGameObject->AddComponent(new CharacterControllerComponent{ 25.f, 50.f, 0.f, 0.f, 0.f, 45.f, 50.f });

    //pGameObject->GetComponent<RigidBodyComponent>()->AddForce(physx::PxVec3{ 500.f, 0.f, 0.f }, physx::PxForceMode::eACCELERATION);
}