#pragma once

#include "Parser/ParserManager.h"
#include "PeHeaders/MsDosHeader.h"
#include "PeHeaders/FileHeader.h"
#include "PeHeaders/OptionalHeader.h"

inline bool CreateMetaDataFromParseManager(ParseManager& parseManager, std::vector<uint8_t>& bytes)
{
	// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#coff-file-header-object-and-image
	PeFileHeader peFileHeader = GetPeFileHeaderTemplate();

	// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#optional-header-image-only
	PeOptionalHeader peOptionalHeader = GetPeOptionalHeaderTemplate();

	// calculate all metadata...

	// write metadata...

	// update headers...

	WriteMsDosStub(bytes); // https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#ms-dos-stub-image-only
	WriteIntegerLittleEndian(bytes, 0x00004550); // PE Signature
	WritePeFileHeader(bytes, peFileHeader);
	WritePeOptionalHeader(bytes, peOptionalHeader);
	// write section header ( https://github.com/StanLipman/Ceres/issues/3 )
	// ...

	return true;
}