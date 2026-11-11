#pragma once

inline std::vector<uint8_t> GetPEHeaderStandardFields();
inline std::vector<uint8_t> GetPEHeaderWindowsNtSpecificFields();
inline std::vector<uint8_t> GetPEHeaderDataDirectories();

// II.25.2.3 PE Optional Header
inline std::vector<uint8_t> GetPEOptionalHeaderTemplate()
{
	std::vector<uint8_t> standardFields = GetPEHeaderStandardFields();
	std::vector<uint8_t> windowsNtFields = GetPEHeaderWindowsNtSpecificFields();
	std::vector<uint8_t> dataDirectories = GetPEHeaderDataDirectories();

	std::vector<uint8_t> bytes;

	bytes.reserve(
		standardFields.size() +
		windowsNtFields.size() +
		dataDirectories.size()
	);

	bytes.insert(bytes.end(), standardFields.begin(), standardFields.end());
	bytes.insert(bytes.end(), windowsNtFields.begin(), windowsNtFields.end());
	bytes.insert(bytes.end(), dataDirectories.begin(), dataDirectories.end());

	return bytes;
}

// II.25.2.3.1 PE Header Standard Fields
inline std::vector<uint8_t> GetPEHeaderStandardFields()
{
	return {
		// Magic, Offset 0, Size 2
		0x0b, 0x02,

		// LMajor, Offset 2, Size 1
		0x06,

		// LMinor, Offset 3, Size 1
		0x00,

		// Code Size, Offset 4, Size 4, TODO calculate code size, ticket unknown
		0x00, 0x00, 0x00, 0x00,

		// Initialized Data Size, Offset 8, Size 4, TODO figure out how to calculate this value, possibly https://github.com/StanLipman/Ceres/issues/3 , for now always 0x00080000 it seems
		0x00, 0x08, 0x00, 0x00,

		// Uninitialized Data Size, Offset 12, Size 4, TODO figure out how to calculate this value, possibly https://github.com/StanLipman/Ceres/issues/3
		0x00, 0x00, 0x00, 0x00,

		// Entry Point RVA, Offset 16, Size 4, NOTE possibly we need to point to the entry point, but it is not needed for dll's
		0x00, 0x00, 0x00, 0x00,

		// Base Of Code, Offset 20, Size 4, TODO figure out how to calculcate this value, ticket unknown, for now always 0x00200000 it seems
		0x00, 0x20, 0x00, 0x00,

		// Base Of Data, Offset 24, Size 4, NOTE not needed as we set the 'Magic' to PE32+
		/*0x00, 0x00, 0x00, 0x00*/
	};
}

// II.25.2.3.2 PE header Windows NT-specific fields 
inline std::vector<uint8_t> GetPEHeaderWindowsNtSpecificFields()
{
	// Offset and size of these fields can differ from PE32 and PE32+, the comments above these bytes will describe them for both
	// For now we only set the bytes for PE32+
	return {
		// ImageBase, Offset 28/24, Size 4/8
		0x00, 0x00, 0x00, 0x10,

		// SectionAlignment, Offset 32/32, Size 4
		0x00, 0x20, 0x00, 0x00,

		// FileAlignment, Offset 36/36, Size 4
		0x00, 0x02, 0x00, 0x00,

		// MajorOperatingSystemVersion, Offset 40/40, Size 2
		0x06, 0x00,

		// MinorOperatingSystemVersion, Offset 42/42, Size 2
		0x00, 0x00,

		// MajorImageVersion, Offset 44/44, Size 2
		0x01, 0x00,

		// MinorImageVersion, Offset 46/46, Size 2
		0x00, 0x00,

		// MajorSubsystemVersion, Offset 48/48, Size 2
		0x06, 0x00,

		// MinorSubsystemVersion, Offset 50/50, Size 2
		0x00, 0x00,

		// Win32VersionValue, Offset 52/52, Size 4
		0x00, 0x00, 0x00, 0x00,

		// SizeOfImage, Offset 56/56, Size 4, TODO calculate from the highest section VirtualAddress + VirtualSize rounded up to SectionAlignment, ticket unknown
		0x00, 0x00, 0x00, 0x00,

		// SizeOfHeaders, Offset 60/60, Size 4
		// TODO calculate from DOS header + PE signature + COFF header + Optional Header + Section Headers rounded up to FileAlignment https://github.com/StanLipman/Ceres/issues/3
		0x00, 0x00, 0x00, 0x00,

		// CheckSum, Offset 64/64, Size 4
		0x00, 0x00, 0x00, 0x00,

		// Subsystem, Offset 68/68, Size 2, IMAGE_SUBSYSTEM_WINDOWS_CUI (Console application)
		0x03, 0x00,

		// DllCharacteristics, Offset 70/70, Size 2
		// IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA (0x0020)
		// | IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE (0x0040)
		// | IMAGE_DLLCHARACTERISTICS_NX_COMPAT (0x0100)
		// | IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE (0x8000)
		0x60, 0x81,

		// SizeOfStackReserve, Offset 72/72, Size 8, 1MB
		0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,

		// SizeOfStackCommit, Offset 80/80, Size 8, 4KB
		0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// SizeOfHeapReserve, Offset 88/88, Size 8, 1MB
		0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,

		// SizeOfHeapCommit, Offset 96/96, Size 8, 4KB
		0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// LoaderFlags, Offset 104/104, Size 4
		0x00, 0x00, 0x00, 0x00,

		// NumberOfRvaAndSizes, Offset 108/108, Size 4
		0x10, 0x00, 0x00, 0x00
	};
}

// II.25.2.3.3 PE header data directories
inline std::vector<uint8_t> GetPEHeaderDataDirectories()
{
	// Offset of these fields can differ from PE32 and PE32+, the comments above these bytes will describe them for both
	// For now we only set the bytes for PE32+
	return {
		// Export Table, Offset 96/112, Size 8
		// No native exports (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Import Table, Offset 104/120, Size 8
		// No native imports (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Resource Table, Offset 112/128, Size 8
		// No resources (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Exception Table, Offset 120/136, Size 8
		// No native exception table (.pdata) (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Certificate Table, Offset 128/144, Size 8
		// No Authenticode signature (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Base Relocation Table, Offset 136/152, Size 8
		// No native base relocations (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Debug, Offset 144/160, Size 8
		// No PE debug directory (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Architecture, Offset 152/168, Size 8
		// Reserved, must be zero
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Global Ptr, Offset 160/176, Size 8
		// Not used (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// TLS Table, Offset 168/184, Size 8
		// No native TLS directory. (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Load Config Table, Offset 176/192, Size 8
		// No native load configuration directory (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Bound Import, Offset 184/200, Size 8
		// No bound imports (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// IAT, Offset 192/208, Size 8
		// No native import address table (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Delay Import Descriptor, Offset 200/216, Size 8
		// No delay-loaded imports (yet)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// CLR Runtime Header, Offset 208/224, Size 8, TODO https://github.com/StanLipman/Ceres/issues/24
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		// Reserved, Offset 216/232, Size 8
		// Reserved, must be zero
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
}