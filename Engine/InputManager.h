#pragma once
#include "KeyInput.h"
#include "MouseInput.h"
#include "InputType.h"
#include <queue>
#include <unordered_set>
#include <functional>
#include <mutex>

namespace SteffEngine
{
	namespace Core
	{
		class InputManager
		{
		public:
			PLUGIN_API static InputManager* GetInstance();
			static void Destroy();

			void ClearKeyInput();
			void ClearMouseInput();

			void AddKeyInput(const KeyInput& keyInput);
			void AddMouseInput(const MouseInput& mouseInput);

			PLUGIN_API void SubscribeEvents(std::function<void(const KeyInput& keyInput)>& handler);
			PLUGIN_API void SubscribeEvents(std::function<void(const KeyInput& keyInput)>&& handler);

			PLUGIN_API void SubscribeEvents(std::function<void(const MouseInput& mouseInput)>& handler);
			PLUGIN_API void SubscribeEvents(std::function<void(const MouseInput& mouseInput)>&& handler);

			void SendEvents();

			PLUGIN_API void SetEnabled(bool isEnabled);

		private:
			InputManager();

			void SendKeyEvents();
			void SendMouseEvents();

			static InputManager* m_pInstance;

			std::mutex m_Mutex;

			bool m_IsEnabled;

			std::queue<KeyInput> m_SingleKeyEvents;
			std::unordered_set<Key> m_PressedKeys;

			std::queue<MouseInput> m_SingleMouseEvents;

			std::vector<std::function<void(const KeyInput& keyInput)>> m_KeyEventSubscribers;
			std::vector<std::function<void(const MouseInput& mouseInput)>> m_MouseEventSubscribers;
		};
	}
}