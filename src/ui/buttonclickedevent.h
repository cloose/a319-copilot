#ifndef _BUTTON_CLICKED_EVENT_H_
#define _BUTTON_CLICKED_EVENT_H_

#include <functional>
#include <vector>

class ButtonClickedEvent {
public:
	void connect(std::function<void(void)> handler) {
		eventHandlers.push_back(handler);
	}

	void emit() {
		for (auto handler : eventHandlers) {
			handler();
		}
	}

private:
	std::vector<std::function<void(void)>> eventHandlers;
};

#endif // _BUTTON_CLICKED_EVENT_H_
