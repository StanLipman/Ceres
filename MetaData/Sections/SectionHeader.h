#pragma once

#include <cstdint>
#include <array>
#include <MetaData/Characteristics.h>

// II.25.3 Section headers
// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#section-table-section-headers
struct SectionHeader
{
	std::array<char, 8> Name{};
	uint32_t VirtualSize{};
	uint32_t VirtualAddress{};
	uint32_t SizeOfRawData{};
	uint32_t PointerToRawData{};
	uint32_t PointerToRelocations{};
	uint32_t PointerToLinenumbers{};
	uint16_t NumberOfRelocations{};
	uint16_t NumberOfLinenumbers{};
	uint32_t Characteristics{};
};

inline SectionHeader GetCodeSectionHeaderTemplate()
{
	SectionHeader header{};

	header.Name = { '.', 't', 'e', 'x', 't' };

	header.Characteristics = Characteristics::Section::Code 
		| Characteristics::Section::Execute 
		| Characteristics::Section::Read;

	return header;
};

inline void WriteSectionHeader(std::vector<uint8_t>& bytes, const SectionHeader& header)
{
	for (const auto& character : header.Name)
		WriteIntegerLittleEndian(bytes, character);

	WriteIntegerLittleEndian(bytes, header.VirtualSize);
	WriteIntegerLittleEndian(bytes, header.VirtualAddress);
	WriteIntegerLittleEndian(bytes, header.SizeOfRawData);
	WriteIntegerLittleEndian(bytes, header.PointerToRawData);
	WriteIntegerLittleEndian(bytes, header.PointerToRelocations);
	WriteIntegerLittleEndian(bytes, header.PointerToLinenumbers);
	WriteIntegerLittleEndian(bytes, header.NumberOfRelocations);
	WriteIntegerLittleEndian(bytes, header.NumberOfLinenumbers);
	WriteIntegerLittleEndian(bytes, header.Characteristics);
};
