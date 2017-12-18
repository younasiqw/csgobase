#pragma once

namespace utils
{
	int waitonmodules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules);
	std::uint8_t* patternscan(const char* dll, const char* signature);
}