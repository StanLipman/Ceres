#pragma once

#include "Parser/ParserManager.h"
#include "PE Headers/MsDosHeader.h"
#include "PE Headers/FileHeader.h"

inline bool CreateMetaDataFromParseManager(ParseManager& parseManager, std::vector<uint8_t>& bytes)
{
	std::vector<uint8_t> msDosHeader = GetMsDosHeader();

	std::vector<uint8_t> peFileHeader = GetPEFileHeader();



	bytes.insert(
		bytes.end(),
		msDosHeader.begin(),
		msDosHeader.end()
	);

	bytes.insert(
		bytes.end(),
		peFileHeader.begin(),
		peFileHeader.end()
	);

	return true;
}