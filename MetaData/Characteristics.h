#pragma once

#include <cstdint>

namespace Characteristics
{
    // https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#characteristics
    namespace COFF
    {
        constexpr uint16_t ExecutableImage = 0x0002;
        constexpr uint16_t LargeAddressAware = 0x0020;
        constexpr uint16_t FileDLL = 0x2000;
    }

    // https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#dll-characteristics
    namespace DLL
    {
        constexpr uint16_t HighEntropy = 0x0020;
        constexpr uint16_t DynamicBase = 0x0040;
        constexpr uint16_t TerminalServerAware = 0x8000;
    }

    // https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#section-flags
	namespace Section
	{
        constexpr uint32_t Code = 0x00000020;
        constexpr uint32_t InitializedData = 0x00000040;
        constexpr uint32_t UninitializedData = 0x00000080;

        constexpr uint32_t Execute = 0x20000000;
        constexpr uint32_t Read = 0x40000000;
        constexpr uint32_t Write = 0x80000000;
	}
}