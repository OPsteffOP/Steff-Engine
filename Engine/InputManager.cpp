#include "pch.h"
#include "InputManager.h"

using namespace SteffEngine::Core;

InputManager* InputManager::m_pInstance{ nullptr };

PLUGIN_API
InputManager* InputManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new InputManager{};
	}

	return m_pInstance;
}

InputManager::InputManager()
	: m_IsEnabled(true)
{

}

void InputManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

void InputManager::ClearKeyInput()
{
	m_SingleKeyEvents = std::queue<KeyInput>{};

	std::unique_lock<std::mutex> lock{ m_Mutex };
	m_PressedKeys.clear();
	lock.unlock();
}

void InputManager::ClearMouseInput()
{
	m_SingleMouseEvents = std::queue<MouseInput>{};
}

void InputManager::AddKeyInput(const KeyInput& keyInput)
{
	if (!m_IsEnabled)
		return;

	// if the key type is down & the key is still being pressed down, do nothing
	if (keyInput.type == Type::DOWN && m_PressedKeys.find(keyInput.key) != m_PressedKeys.cend())
		return;

	// add the key information
	m_SingleKeyEvents.push(keyInput);
	
	// if the key type is up, remove the key from pressed down keys
	if (keyInput.type == Type::UP)
	{
		std::unique_lock<std::mutex> lock{ m_Mutex };

		std::unordered_set<Key>::const_iterator foundKeyIterator{ m_PressedKeys.find(keyInput.key) };
		if (foundKeyIterator != m_PressedKeys.cend())
			m_PressedKeys.erase(foundKeyIterator);

		lock.unlock();
	}
}

void InputManager::AddMouseInput(const MouseInput& mouseInput)
{
	m_SingleMouseEvents.push(mouseInput);
}

PLUGIN_API
void InputManager::SubscribeEvents(std::function<void(const KeyInput& keyInput)>& handler)
{
	SubscribeEvents(std::move(handler));
}

PLUGIN_API
void InputManager::SubscribeEvents(std::function<void(const KeyInput& keyInput)>&& handler)
{
	m_KeyEventSubscribers.push_back(handler);
}

PLUGIN_API
void InputManager::SubscribeEvents(std::function<void(const MouseInput& mouseInput)>& handler)
{
	SubscribeEvents(std::move(handler));
}

PLUGIN_API
void InputManager::SubscribeEvents(std::function<void(const MouseInput& mouseInput)>&& handler)
{
	m_MouseEventSubscribers.push_back(handler);
}

void InputManager::SendEvents()
{
	if (!m_IsEnabled)
		return;

	SendKeyEvents();
	SendMouseEvents();
}

void InputManager::SendKeyEvents()
{
	// loop over all pressed down keys & execute the subscribed functions
	std::unique_lock<std::mutex> lock{ m_Mutex };
	for (Key key : m_PressedKeys)
	{
		std::for_each(m_KeyEventSubscribers.cbegin(), m_KeyEventSubscribers.cend(),
			[&key](const std::function<void(const KeyInput& keyInput)>& handler) { handler(KeyInput{ key, Type::HOLD }); });
	}
	lock.unlock();

	// loop over all key events & execute the subscribed functions
	while(!m_SingleKeyEvents.empty())
	{
		const KeyInput& keyInput{ m_SingleKeyEvents.front() };
		std::for_each(m_KeyEventSubscribers.cbegin(), m_KeyEventSubscribers.cend(),
			[&keyInput](const std::function<void(const KeyInput& keyInput)>& handler) { handler(keyInput); });

		if (keyInput.type == Type::DOWN)
		{
			m_PressedKeys.insert(keyInput.key);
		}

		m_SingleKeyEvents.pop();
	}
}

void InputManager::SendMouseEvents()
{
	// loop over all mouse events & execute the subscribed functions
	while (!m_SingleMouseEvents.empty())
	{
		const MouseInput& mouseInput{ m_SingleMouseEvents.front() };
		std::for_each(m_MouseEventSubscribers.cbegin(), m_MouseEventSubscribers.cend(),
			[&mouseInput](const std::function<void(const MouseInput& mouseInput)>& handler) { handler(mouseInput); });

		m_SingleMouseEvents.pop();
	}
}

void InputManager::SetEnabled(bool isEnabled)
{
	ClearKeyInput();
	ClearMouseInput();

	m_IsEnabled = isEnabled;
}