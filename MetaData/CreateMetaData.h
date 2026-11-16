#pragma once

#include "Parser/ParserManager.h"
#include "PeHeaders/MsDosHeader.h"
#include "PeHeaders/FileHeader.h"
#include "PeHeaders/OptionalHeader.h"
#include "Sections/SectionHeader.h"

inline bool CreateMetaDataFromParseManager(ParseManager& parseManager, std::vector<uint8_t>& bytes)
{
	PeFileHeader peFileHeader = GetPeFileHeaderTemplate();
	PeOptionalHeader peOptionalHeader = GetPeOptionalHeaderTemplate();
	SectionHeader codeSectionHeader = GetCodeSectionHeaderTemplate();

	// calculate all metadata...

	// write metadata...

	// update headers...

	WriteMsDosStub(bytes); // https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#ms-dos-stub-image-only
	WriteIntegerLittleEndian(bytes, 0x00004550); // PE Signature
	WritePeFileHeader(bytes, peFileHeader);
	WritePeOptionalHeader(bytes, peOptionalHeader);
	WriteSectionHeader(bytes, codeSectionHeader);
	// ...

	return true;
}