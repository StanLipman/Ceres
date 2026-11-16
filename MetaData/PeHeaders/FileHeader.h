#pragma once

#include <cstdint>
#include <Helpers/LittleEndianHelper.h>
#include <MetaData/Characteristics.h>

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
	uint16_t Characteristics{};
};

// II.25.2.2 PE File Header
inline PeFileHeader GetPeFileHeaderTemplate()
{
	PeFileHeader fileHeader{};

	fileHeader.Characteristics = Characteristics::COFF::ExecutableImage
		| Characteristics::COFF::LargeAddressAware
		| Characteristics::COFF::FileDLL;

	return fileHeader;
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
