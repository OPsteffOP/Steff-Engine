/* THIS FILE IS AUTO-GENERATED BY STEFF-ENGINE. PLEASE DO NOT MODIFY. */

#pragma once
#include "pch.h"

#include "ScriptComponentExposing.h"
#include "ScriptComponentNamesStruct.h"
#include "ScriptManager.h"


using namespace SteffEngine::Core;
using namespace SteffEngine::Editor;

ENGINE_API
SteffEngine::Core::Components::Component* CreateComponent(size_t hash, SteffEngine::Core::GameObject* pGameObject)
{

	return nullptr;
}

ENGINE_API
String GetComponentName(SteffEngine::Core::Components::Component* pComponent)
{

	return nullptr;
}

ENGINE_API
ScriptComponentNamesStruct GetComponentNames()
{
	return ScriptComponentNamesStruct{ SteffEngine::Editor::GetComponentNamesSize(), SteffEngine::Editor::GetComponentNames() };
}
