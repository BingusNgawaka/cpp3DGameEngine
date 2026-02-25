#include "../../include/core/Events.hpp"
#include <iostream>

void EventManager::registerEventCallback(const std::string& eventName, const std::function<void(float)>& callbackFunc){
	eventCallbacks[eventName].push_back(callbackFunc);
}

void EventManager::triggerEvent(const std::string& eventName, float deltaTime){
	if(eventCallbacks.find(eventName) != eventCallbacks.end()){
		for(const auto& callbackFunc:eventCallbacks[eventName]){
			callbackFunc(deltaTime);
		}
	}
}

