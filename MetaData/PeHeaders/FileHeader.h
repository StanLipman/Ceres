#pragma once

#include <cstdint>
#include <Helpers/LittleEndianHelper.h>

// II.25.2.2 PE File Header
// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#coff-file-header-object-and-image
struct PeFileHeader
{
	uint16_t Machine{ 0x8664 }; // IMAGE_FILE_MACHINE_AMD64
	uint16_t NumberOfSections{ 1 }; // for now just the .text section (IL/metadata etc.)
	uint32_t TimeDateStamp{};
	uint32_t PointerToSymbolTable{};
	uint32_t NumberOfSymbols{};
	uint16_t SizeOfOptionalHeader{ 240 }; // 240 bytes for PE32+ optional header
	uint16_t Characteristics{ 0x2002 }; // IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_DLL https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#characteristics
};

// II.25.2.2 PE File Header
inline PeFileHeader GetPeFileHeaderTemplate()
{
	return {};
};

inline void WritePeFileHeader(std::vector<uint8_t>& bytes, const PeFileHeader& header)
{
    WriteIntegerLittleEndian(bytes, header.Machine);
    WriteIntegerLittleEndian(bytes, header.NumberOfSections);
    WriteIntegerLittleEndian(bytes, header.TimeDateStamp);
    WriteIntegerLittleEndian(bytes, header.PointerToSymbolTable);
    WriteIntegerLittleEndian(bytes, header.NumberOfSymbols);
    WriteIntegerLittleEndian(bytes, header.SizeOfOptionalHeader);
    WriteIntegerLittleEndian(bytes, header.Characteristics);
};
