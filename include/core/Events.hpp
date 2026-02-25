#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

class EventManager{
	private:
			std::unordered_map<std::string, std::vector<std::function<void(float)>>> eventCallbacks;

	public:
			void registerEventCallback(const std::string& eventName, const std::function<void(float)>& callbackFunc);

			void triggerEvent(const std::string& eventName, float deltaTime);
};
