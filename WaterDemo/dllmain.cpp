// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <vld.h>

#include "SceneManager.h"
#include "MeshRendererComponent.h"
#include "MeshManager.h"
#include "TimePosTexVertexShader.h"
#include "WaterPixelShader.h"
#include "WaterSplashComponent.h"

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
    // Generating plane
    const float planeCenterX{ 0.f };
    const float planeY{ 0 };
    const float planeCenterZ{ 0.f };

    const float planeWidth{ 50.f };
    const float planeDepth{ 50.f };
    
    const unsigned int squaresAmountX{ 10 };
    const unsigned int squaresAmountZ{ 10 };

    const float squareWidth{ planeWidth / squaresAmountX };
    const float squareDepth{ planeDepth / squaresAmountZ };

    const float planeOffsetX{ planeCenterX - (planeWidth / 2.f) };
    const float planeOffsetZ{ planeCenterZ - (planeDepth / 2.f) };

    std::vector<PosTexVertex> vertices;
    std::vector<uint32_t> indices;
    for (unsigned int z{ 0 }; z < squaresAmountZ; ++z)
    {
        for (unsigned int x{ 0 }; x < squaresAmountX; ++x)
        {
            const XMFLOAT3 squareBottomLeft{ 
                planeOffsetX + (x * squareWidth),
                planeY,
                planeOffsetZ + (z * squareDepth)
            };

            const XMFLOAT3 squareTopLeft{ 
                squareBottomLeft.x,
                squareBottomLeft.y,
                squareBottomLeft.z + squareDepth
            };

            const XMFLOAT3 squareBottomRight{
                squareBottomLeft.x + squareWidth,
                squareBottomLeft.y,
                squareBottomLeft.z
            };

            const XMFLOAT3 squareTopRight{
                squareBottomLeft.x + squareWidth,
                squareBottomLeft.y,
                squareBottomLeft.z + squareDepth
            };

            vertices.push_back(PosTexVertex{ squareBottomLeft, XMFLOAT2{ std::fabsf((squareBottomLeft.x + planeOffsetX) / planeWidth), std::fabsf((squareBottomLeft.z + planeOffsetZ) / planeDepth) } });
            vertices.push_back(PosTexVertex{ squareTopLeft, XMFLOAT2{ std::fabsf((squareTopLeft.x + planeOffsetX) / planeWidth), std::fabsf((squareTopLeft.z + planeOffsetZ) / planeDepth) } });
            vertices.push_back(PosTexVertex{ squareBottomRight, XMFLOAT2{ std::fabsf((squareBottomRight.x + planeOffsetX) / planeWidth), std::fabsf((squareBottomRight.z + planeOffsetZ) / planeDepth) } });
            vertices.push_back(PosTexVertex{ squareBottomRight, XMFLOAT2{ std::fabsf((squareBottomRight.x + planeOffsetX) / planeWidth), std::fabsf((squareBottomRight.z + planeOffsetZ) / planeDepth) } });
            vertices.push_back(PosTexVertex{ squareTopLeft, XMFLOAT2{ std::fabsf((squareTopLeft.x + planeOffsetX) / planeWidth), std::fabsf((squareTopLeft.z + planeOffsetZ) / planeDepth) } });
            vertices.push_back(PosTexVertex{ squareTopRight, XMFLOAT2{ std::fabsf((squareTopRight.x + planeOffsetX) / planeWidth), std::fabsf((squareTopRight.z + planeOffsetZ) / planeDepth) } });

            indices.push_back((z * (squaresAmountX * 6)) + (x * 6) + 0);
            indices.push_back((z * (squaresAmountX * 6)) + (x * 6) + 1);
            indices.push_back((z * (squaresAmountX * 6)) + (x * 6) + 2);
            indices.push_back((z * (squaresAmountX * 6)) + (x * 6) + 3);
            indices.push_back((z * (squaresAmountX * 6)) + (x * 6) + 4);
            indices.push_back((z * (squaresAmountX * 6)) + (x * 6) + 5);
        }
    }

    // TODO: DO THE NORMAL CALCULATIONS IN THE GEOMETRY SHADER

    GameObject* pGameObject{ SceneManager::GetInstance()->GetActiveScene()->CreateGameObject(XMFLOAT3{ planeCenterX, 0.f, planeCenterZ }) };
    pGameObject->CreateComponent<MeshRendererComponent>(
        MeshManager::GetInstance()->RegisterMeshData(L"WaterPlane", vertices, indices),
        new TimePosTexVertexShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\WaterVertexShader.cso" },
        new WaterPixelShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\WaterPixelShader.cso",
            L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Textures\\Water.png",
            L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Textures\\Splash.png" },
        nullptr,
        true);
    pGameObject->CreateComponent<WaterSplashComponent>();
}