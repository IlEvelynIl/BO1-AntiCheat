#include "strings.hpp"

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
	} // strings
} // utils