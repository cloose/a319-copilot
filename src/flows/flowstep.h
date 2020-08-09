#ifndef _FLOW_STEP_H_
#define _FLOW_STEP_H_

#include <functional>
#include <string>
#include <vector>

#include <color.h>

struct FlowStep
{
	std::string description;
	std::string state;
    std::function<Color(void)> condition;

	FlowStep(const std::string& description_, const std::string& state_, std::function<Color(void)> condition_ = []() constexpr { return color_white; }) {
		description = description_;
		state = state_;
        condition = condition_;
	}
};

#endif // _FLOW_STEP_H_
