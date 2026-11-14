#pragma once

// II.25.2.2 PE File Header
inline std::vector<uint8_t> GetPEFileHeaderTemplate()
{
	return {
		// Machine, Offset 0, Size 2, TODO make linux compatible when the time is ripe, https://github.com/StanLipman/Ceres/issues/22
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

		// Optional Header Size, Offset 16, Size 2
		0xE0, 0x00,

		// Characteristics, Offset 18, Size 2, IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_DLL https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#characteristics
		0x02, 0x20,
	};
}