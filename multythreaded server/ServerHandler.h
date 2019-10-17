#pragma once
#include <string>
#include <vector>
#include <functional>
#include <variant>
#include <regex>

class Server
{
private:
	class Callable {
	private:
		typedef std::function<float(float)> FuncUnary;
		typedef std::function<float(float,float)> FuncBinary;

		template <class... Fs> struct Overload : Fs... { using Fs::operator()...; };
		template <class... Fs> Overload(Fs...)->Overload<Fs...>;
	public:
		Callable(float(*fn)(float)) : _fn(fn) {}
		Callable(float(*fn)(float, float)) : _fn(fn) {}
		Callable(const Callable& rhs) : _fn(rhs._fn) {}
		Callable(Callable&& rhs) noexcept : _fn(rhs._fn) {}

		float operator()(const std::vector<float>& params) const {
			struct VH {
				VH(const std::vector<float>& p) : v(p) {}
				float operator()(FuncUnary f) const { return f(v[0]); }
				float operator()(FuncBinary f) const { return f(v[0], v[1]); }
				const std::vector<float>& v;
			};
			return std::visit(VH(params), _fn);
		}

	private:
		std::variant<FuncUnary, FuncBinary> _fn;
	};

	struct operation_t
	{
		operation_t(std::string name, std::string pattern, std::string info, Callable fn)
			: _name(name)
			, _pattern(pattern)
			, _info(info)
			, _fn(fn)
		{}

		const std::string getName() const { return _name; }
		const std::string getPattern() const { return _pattern; }
		const std::string getInfo() const { return _info; }
		const float fn(const std::vector<float>& params) { return _fn(params); }

	private:
		std::string _name;
		std::string _pattern;
		std::string _info;
		Callable _fn;
	};

public:
	static bool Handle(const std::string& msg, std::string& response);
private:
	static bool doHandle(operation_t& operation, const std::string& msg, std::string& response);
	static std::vector<Server::operation_t> operationsData;
};

