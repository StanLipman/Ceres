#pragma once

// II.25.2.3 PE Optional Header
inline std::vector<uint8_t> GetPEOptionalHeader()
{
	std::vector<uint8_t> standardFields = GetPEHeaderStandardFields();

	std::vector<uint8_t> bytes = {};

	return bytes;
}

// II.25.2.3.1 PE Header Standard Fields
inline std::vector<uint8_t> GetPEHeaderStandardFields()
{
	std::vector<uint8_t> bytes = {
		// Magic, Offset 0, Size 2
		0x0b, 0x01,

		// LMajor, Offset 2, Size 1
		0x06,

		// LMinor, Offset 3, Size 1
		0x00,

		// Code Size, Offset 4, Size 4, Temporary (TODO calculate code size, ticket unknown)
		0x00, 0x00, 0x00, 0x00,

		// Initialized Data Size, Offset 8, Size 4, TODO figure out how to calculate this value, possibly https://github.com/StanLipman/Ceres/issues/3
		0x00, 0x00, 0x00, 0x00,

		// Uninitialized Data Size, Offset 12, Size 4, TODO figure out how to calculate this value, possibly https://github.com/StanLipman/Ceres/issues/3
		0x00, 0x00, 0x00, 0x00,

		// Entry Point RVA, Offset 16, Size 4, TODO figure out how to calculcate this value
		0x00, 0x00, 0x00, 0x00,

		// Base Of Code, Offset 20, Size 4, TODO figure out how to calculcate this value
		0x00, 0x00, 0x00, 0x00,

		// Base Of Data, Offset 24, Size 4, TODO figure out how to calculcate this value
		0x00, 0x00, 0x00, 0x00
	};

	return bytes;
}