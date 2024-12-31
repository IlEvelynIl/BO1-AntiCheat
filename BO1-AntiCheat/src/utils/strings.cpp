#include "strings.hpp"

#include <chrono>

namespace utils {
	namespace strings {
		std::string ToLower(std::string str)
		{
			std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
			{
				return std::tolower(c);
			});
			return str;
		}

		bool Contains(std::string& str, std::string& sub)
		{
			return str.find(sub) != std::string::npos;
		}

		bool ContainsIgnoreCase(const std::string& str, const std::string& sub)
		{
			return ToLower(str).find(ToLower(sub)) != std::string::npos;
		}
		
		std::string GetCurrentEpoch()
		{
			auto now = std::chrono::system_clock::now();
			auto epoch_time = std::chrono::time_point_cast<std::chrono::seconds>(now).time_since_epoch();
			return std::to_string(epoch_time.count());
		}
	} // strings
} // utils