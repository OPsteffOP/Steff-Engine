#pragma once
namespace SteffEngine
{
	namespace Editor
	{
		struct GameObjectData
		{
			explicit GameObjectData(const char* pName, unsigned int id)
				: pName(pName)
				, id(id)
			{

			}

			const char* pName;
			unsigned int id;
		};
	}
}