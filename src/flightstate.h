#ifndef _FLIGHT_STATE_H_
#define _FLIGHT_STATE_H_

#include <functional>
#include <vector>

enum class FlightState {
    StartFlight,
	CockpitPreparation,
	BeforeStart,
	EngineStart,
	AfterStart,
	Taxi,
	BeforeTakeoffFlow,
	TakeoffFlow,
	AfterTakeoffFlow
};

class FlightStateChangedEvent {
public:
	void connect(std::function<void(FlightState)> handler) {
		eventHandlers.push_back(handler);
	}

	void emit(FlightState newFlightState) {
		for (auto handler : eventHandlers) {
			handler(newFlightState);
		}
	}

private:
	std::vector<std::function<void(FlightState)>> eventHandlers;
};

#endif // _FLIGHT_STATE_H_
