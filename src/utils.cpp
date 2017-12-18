#include "stdafx.h"
#include "utils.h"

int utils::waitonmodules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules)
{
	bool signaled[32] = { 0 };
	bool success = false;

	std::uint32_t totalSlept = 0;

	if (timeout == 0)
	{
		for (auto& mod : modules)
		{
			if (GetModuleHandleW(std::data(mod)) == NULL)
				return WAIT_TIMEOUT;
		}
		return WAIT_OBJECT_0;
	}

	if (timeout < 0)
		timeout = INT32_MAX;

	while (true)
	{
		for (auto i = 0u; i < modules.size(); ++i)
		{
			auto& module = *(modules.begin() + i);
			if (!signaled[i] && GetModuleHandleW(std::data(module)) != NULL)
			{
				signaled[i] = true;

				//
				// Checks if all modules are signaled
				//
				bool done = true;
				for (auto j = 0u; j < modules.size(); ++j)
				{
					if (!signaled[j])
					{
						done = false;
						break;
					}
				}
				if (done)
				{
					success = true;
					goto exit;
				}
			}
		}
		if (totalSlept > std::uint32_t(timeout))
		{
			break;
		}
		Sleep(10);
		totalSlept += 10;
	}

exit:
	return success ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
}

std::uint8_t * utils::patternscan(const char* dll, const char * signature)
{
	auto module = GetModuleHandleA(dll);
	static auto pattern_to_byte = [](const char* pattern)
	{
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			} else
			{
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER) module;
	auto ntHeaders = (PIMAGE_NT_HEADERS) ((std::uint8_t*)module + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(signature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return &scanBytes[i];
		}
	}

	return nullptr;
}
