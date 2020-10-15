#pragma once
#include "Renderer.h"
#include "ExposedVariableData.h"
#include "EditorSceneStorage.h"

namespace SteffEngine
{
	namespace Core
	{
		class GameObject;
		class Scene;

		namespace Components
		{
			class TransformComponent;

			class Component
			{
			public:
				PLUGIN_API Component();
				Component(const Component& component) = delete;
				Component(Component&& component) = delete;

				virtual ~Component() = default;

				Component& operator=(const Component& component) = delete;
				Component& operator=(Component&& component) = delete;

				inline unsigned int GetComponentID() const { return m_ComponentID; };
				PLUGIN_API inline TransformComponent* GetTransform() const;

				virtual void Initialize() {};
				virtual void PostInitialize() {};

				virtual void FixedUpdate() {};
				virtual void Update() {};
				virtual void LateUpdate() {};
				virtual void Render(Renderer& renderer) const {};

				// Events
				virtual void SceneSwitched(Scene* pOldScene) {};
				virtual void VariableChanged(const std::string_view& variableName) {};
				virtual void TransformChanged() {};

				template<typename T>
				inline bool IsSameType() const { return IsType(T::hash); };

				String GetComponentName() const;

			protected:
				virtual bool IsType(size_t hash) const = 0;
				virtual std::vector<SteffEngine::Editor::ExposedVariableData> GetEditorData() const = 0;

				static unsigned int nextComponentID;
				const unsigned int m_ComponentID;

				GameObject* m_pGameObject;

				friend class GameObject;
				friend class SteffEngine::Editor::EditorSceneStorage;
			};
		}
	}
}

// Faster way for RTTI
#define TYPE_TO_STRING(type) #type

#ifdef PLUGIN_EXPORTS
	#define COMPONENT_REGISTRATION(type)																						\
	inline const size_t type::hash{ std::hash<std::string_view>{}(TYPE_TO_STRING(type)) };												\
	inline const String type::name{ TYPE_TO_STRING(type) };
#else
	#define COMPONENT_REGISTRATION(type)
#endif

#define COMPONENT_DECLARATION()																									\
friend class Component;																											\
friend class GameObject;																										\
																																\
protected:																														\
	virtual bool IsType(size_t hash) const override;																			\
	virtual std::vector<SteffEngine::Editor::ExposedVariableData> GetEditorData() const override;								\
	static const size_t hash;																									\
	static const String name;																									\
																																\
public:																															\
	static size_t GetHash() { return hash; }																					\
	static String GetName() { return name; }

#define COMPONENT_DEFINITION(type)																								\
const size_t type::hash{ std::hash<std::string_view>{}(TYPE_TO_STRING(type)) };													\
const String type::name{ TYPE_TO_STRING(type) };																				\
																																\
bool type::IsType(size_t hash) const																							\
{																																\
	return this->hash == hash;																									\
}

// Use this one if the component inherits from another component (not the base Component class)
#define COMPONENT_DEFINITION_INHERITANCE(type, base)																			\
const size_t type::hash{ std::hash<std::string_view>{}(TYPE_TO_STRING(type)) };													\
const String type::name{ TYPE_TO_STRING(type) };																				\
																																\
bool type::IsType(size_t hash) const																							\
{																																\
	return this->hash == hash || base::hash == hash;																			\
}