#pragma once
#include "VariableData.h"
#include "EditorType.h"

namespace SteffEngine
{
	namespace Editor
	{
		struct ExposedVariableData
		{
			ExposedVariableData()
				: ExposedVariableData(nullptr, EditorType(0), VariableAccessibility(0), VariableType(0), nullptr)
			{

			}

			explicit ExposedVariableData(const char* name, EditorType editorType, VariableAccessibility accessibility, VariableType type, const void* pData)
				: name(name)
				, editorType(editorType)
				, accessibility(accessibility)
				, type(type)
				, pData(pData)
			{
			}

			const char* name;
			EditorType editorType;
			VariableAccessibility accessibility;
			VariableType type;
			const void* pData;
		};
	}
}