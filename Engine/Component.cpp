#include "pch.h"
#include "Component.h"
#include "GameObject.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

unsigned int Component::nextComponentID{ 0 };

PLUGIN_API
Component::Component()
	: m_pGameObject(nullptr)
	, m_ComponentID(nextComponentID)
{
	++nextComponentID;
}

PLUGIN_API
TransformComponent* Component::GetTransform() const
{
	return &m_pGameObject->GetTransform();
}