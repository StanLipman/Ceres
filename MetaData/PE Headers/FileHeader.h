#pragma once

// II.25.2.2 PE File Header
inline std::vector<uint8_t> GetPEFileHeader()
{
	std::vector<uint8_t> bytes = {
		// Not really sure where these bytes come from, but they seem to appear in every dll I (Stan) inspected
		0x50, 0x45, 0x00, 0x00,

		// Machine, Offset 0, Size 2
		0x4c, 0x01,

		// TODO correct size here, for now 0x03 as seen in other dll's, https://github.com/StanLipman/Ceres/issues/3
		// Number of Sections, Offset 2, Size 2
		0x03, 0x00,

		// Time/Date Stamp, Offset 4, Size 4
		0x00, 0x00, 0x00, 0x00,

		// Pointer to Symbol Table, Offset 8, Size 4, Always 0 II.24.1
		0x00, 0x00, 0x00, 0x00,

		// Number of Symbols, Offset 12, Size 4, Always 0 II.24.1
		0x00, 0x00, 0x00, 0x00,

		// Optional Header Size, Offset 16, Size 2, Temporary
		0x00, 0x00,

		// Characteristics, Offset 18, Size 2, https://learn.microsoft.com/en-us/dotnet/api/system.reflection.portableexecutable.characteristics?view=net-10.0
		0x22, 0x00,
	};

	return bytes;
}