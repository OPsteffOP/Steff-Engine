// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <vld.h>

#include "SceneManager.h"
#include "PostProcessingEffect.h"

using namespace SteffEngine::Core;

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
    SceneManager::GetInstance()->GetActiveScene()->AddPostProcessingEffect(new PostProcessingEffect{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\GrayscalePixelShader.cso" });
}