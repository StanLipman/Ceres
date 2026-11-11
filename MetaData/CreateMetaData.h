#pragma once

#include "Parser/ParserManager.h"
#include "PE Headers/MsDosHeader.h"
#include "PE Headers/FileHeader.h"
#include "PE Headers/OptionalHeader.h"

inline bool CreateMetaDataFromParseManager(ParseManager& parseManager, std::vector<uint8_t>& bytes)
{
	// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#ms-dos-stub-image-only
	std::vector<uint8_t> msDosHeader = GetMsDosHeader();

	std::vector<uint8_t> peSignature = { 0x50, 0x45, 0x00, 0x00 };

	// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#coff-file-header-object-and-image
	std::vector<uint8_t> peFileHeader = GetPEFileHeaderTemplate();

	// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#optional-header-image-only
	std::vector<uint8_t> peOptionalHeader = GetPEOptionalHeaderTemplate();

	// calculate all metadata...

	bytes.reserve(
		msDosHeader.size() +
		peSignature.size() +
		peFileHeader.size() +
		peOptionalHeader.size()
	);

	bytes.insert(bytes.end(), msDosHeader.begin(), msDosHeader.end());
	bytes.insert(bytes.end(), peSignature.begin(), peSignature.end());
	bytes.insert(bytes.end(), peFileHeader.begin(), peFileHeader.end());
	bytes.insert(bytes.end(), peOptionalHeader.begin(), peOptionalHeader.end());

	return true;
}