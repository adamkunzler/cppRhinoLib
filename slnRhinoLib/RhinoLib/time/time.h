#ifndef TIME_H
#define TIME_H

#include <chrono>

namespace RhinoLib
{
	/// <summary>
	/// USAGE:
	/// auto start = std::chrono::stead_clock::now();
	/// ...do stuff
	/// std::cerr << "\n\nElapsed(ms)=" << since(start).count() << std::endl;
	/// </summary>
	template <
		class result_t = std::chrono::milliseconds,
		class clock_t = std::chrono::steady_clock,
		class duration_t = std::chrono::milliseconds>
	auto since(std::chrono::time_point<clock_t, duration_t> const& start)
	{
		return std::chrono::duration_cast<result_t>(clock_t::now() - start);
	}

}

#endif