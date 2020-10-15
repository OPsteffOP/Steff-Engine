#include "pch.h"
#include "Script.h"
#include "ScriptComponentNamesStruct.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;
using namespace SteffEngine::Scripting;

Script::Script(HMODULE library)
	: m_Library(library)
{
	LoadFunctions();
	CallFunction<>("OnEnable");
}

Script::~Script()
{
	CallFunction<>("OnDisable");
	FreeLibrary(m_Library);
}

void Script::LoadFunctions()
{
	LoadFunction<void>("OnEnable");
	LoadFunction<void>("OnDisable");
	LoadFunction<Component*, size_t, GameObject*>("CreateComponent");
	LoadFunction<String, const Component*>("GetComponentName");
	LoadFunction<ScriptComponentNamesStruct>("GetComponentNames");
}