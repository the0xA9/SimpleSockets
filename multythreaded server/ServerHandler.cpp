#include "ServerHandler.h"

std::vector<Server::operation_t> Server::operationsData = {
	{ "add", "add\\(([0-9.]+),([0-9.]+)\\)", "add(x,y) return x + y", Callable([](float x, float y) -> float { return x + y; })},
	{ "subtract", "subtract\\(([0-9.]+),([0-9.]+)\\)", "subtract(x,y) return x - y", Callable([](float x, float y) -> float { return x - y; })},
	{ "multiply", "multiply\\(([0-9.]+),([0-9.]+)\\)", "multiply(x,y) return x * y", Callable([](float x, float y) -> float { return x * y; })},
	{ "divide", "divide\\(([0-9.]+),([0-9.]+)\\)", "divide(x,y) return x / y", Callable([](float x, float y) -> float { return x / y; })},
};

bool Server::doHandle(operation_t& operation, const std::string& msg, std::string& response)
{
	std::vector<float> params;

	std::regex regular(operation.getPattern());
	std::cmatch result;
	if (!regex_match(msg.c_str(), result, regular))
		return false;

	for (size_t it = 1; it < result.size(); ++it)
		params.push_back(atof(result[it].str().c_str()));

	response.assign(std::to_string(operation.fn(params)));

	return true;
}

bool Server::Handle(const std::string& msg, std::string& response)
{
	if (msg.find("/help") == 0) {
		response += "Operations availible: " + std::to_string(operationsData.size()) + "\n";
		for (auto it : operationsData)
			response += "\t- " + it.getInfo() + "\t" + "\n";
		return true;
	}

	for (auto it : operationsData)
		if (msg.find(it.getName()) == 0)
			if(doHandle(it, msg, response))
				return true;

	return false;
}