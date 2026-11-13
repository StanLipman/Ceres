#pragma once

#include <array>

struct PeDataDirectory
{
	uint32_t VirtualAddress{};
	uint32_t Size{};
};

struct PeOptionalHeader
{
	// II.25.2.3.1 PE Header Standard Fields
	uint16_t Magic{ 0x020B }; // PE32+
	uint8_t MajorLinkerVersion{ 6 };
	uint8_t MinorLinkerVersion{ 0 };
	uint32_t SizeOfCode{}; // TODO probably https://github.com/StanLipman/Ceres/issues/24
	uint32_t SizeOfInitializedData{ 0x00000800 }; // TODO figure out how to calculate this value, possibly https://github.com/StanLipman/Ceres/issues/3 , for now always 0x00000800 it seems
	uint32_t SizeOfUninitializedData{}; // TODO figure out how to calculate this value, possibly https://github.com/StanLipman/Ceres/issues/3
	uint32_t AddressOfEntryPoint{};
	uint32_t BaseOfCode{ 0x2000 };

	// II.25.2.3.2 PE header Windows NT-specific fields
	uint64_t ImageBase{ 0x10000000 };
	uint32_t SectionAlignment{ 0x2000 };
	uint32_t FileAlignment{ 0x0200 };
	uint16_t MajorOperatingSystemVersion{ 6 };
	uint16_t MinorOperatingSystemVersion{};
	uint16_t MajorImageVersion{ 1 };
	uint16_t MinorImageVersion{};
	uint16_t MajorSubsystemVersion{ 6 };
	uint16_t MinorSubsystemVersion{};
	uint32_t Win32VersionValue{};
	uint32_t SizeOfImage{}; // TODO calculate from the highest section VirtualAddress + VirtualSize rounded up to SectionAlignment, ticket unknown
	uint32_t SizeOfHeaders{}; // TODO calculate from DOS header + PE signature + COFF header + Optional Header + Section Headers rounded up to FileAlignment https://github.com/StanLipman/Ceres/issues/3
	uint32_t CheckSum{};
	uint16_t Subsystem{ 3 }; // IMAGE_SUBSYSTEM_WINDOWS_CUI
	uint16_t DllCharacteristics{ 0x8160 }; // IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA (0x0020) | IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE (0x0040) | IMAGE_DLLCHARACTERISTICS_NX_COMPAT (0x0100) | IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE (0x8000)
	uint64_t SizeOfStackReserve{ 0x0000000000100000 }; // 1MB
	uint64_t SizeOfStackCommit{ 0x0000000000001000 }; // 4KB
	uint64_t SizeOfHeapReserve{ 0x0000000000100000 }; // 1MB
	uint64_t SizeOfHeapCommit{ 0x0000000000001000 }; // 4KB
	uint32_t LoaderFlags{};
	uint32_t NumberOfRvaAndSizes{ 16 };

	// II.25.2.3.3 PE header data directories
	std::array<PeDataDirectory, 16> DataDirectories{}; // TODO https://github.com/StanLipman/Ceres/issues/24
};

inline PeOptionalHeader GetPeOptionalHeaderTemplate()
{
	return {};
}

inline void WritePeOptionalHeader(std::vector<uint8_t>& bytes, const PeOptionalHeader& header)
{
	WriteIntegerLittleEndian(bytes, header.Magic);
	WriteIntegerLittleEndian(bytes, header.MajorLinkerVersion);
	WriteIntegerLittleEndian(bytes, header.MinorLinkerVersion);
	WriteIntegerLittleEndian(bytes, header.SizeOfCode);
	WriteIntegerLittleEndian(bytes, header.SizeOfInitializedData);
	WriteIntegerLittleEndian(bytes, header.SizeOfUninitializedData);
	WriteIntegerLittleEndian(bytes, header.AddressOfEntryPoint);
	WriteIntegerLittleEndian(bytes, header.BaseOfCode);

	WriteIntegerLittleEndian(bytes, header.ImageBase);
	WriteIntegerLittleEndian(bytes, header.SectionAlignment);
	WriteIntegerLittleEndian(bytes, header.FileAlignment);
	WriteIntegerLittleEndian(bytes, header.MajorOperatingSystemVersion);
	WriteIntegerLittleEndian(bytes, header.MinorOperatingSystemVersion);
	WriteIntegerLittleEndian(bytes, header.MajorImageVersion);
	WriteIntegerLittleEndian(bytes, header.MinorImageVersion);
	WriteIntegerLittleEndian(bytes, header.MajorSubsystemVersion);
	WriteIntegerLittleEndian(bytes, header.MinorSubsystemVersion);
	WriteIntegerLittleEndian(bytes, header.Win32VersionValue);
	WriteIntegerLittleEndian(bytes, header.SizeOfImage);
	WriteIntegerLittleEndian(bytes, header.SizeOfHeaders);
	WriteIntegerLittleEndian(bytes, header.CheckSum);
	WriteIntegerLittleEndian(bytes, header.Subsystem);
	WriteIntegerLittleEndian(bytes, header.DllCharacteristics);
	WriteIntegerLittleEndian(bytes, header.SizeOfStackReserve);
	WriteIntegerLittleEndian(bytes, header.SizeOfStackCommit);
	WriteIntegerLittleEndian(bytes, header.SizeOfHeapReserve);
	WriteIntegerLittleEndian(bytes, header.SizeOfHeapCommit);
	WriteIntegerLittleEndian(bytes, header.LoaderFlags);
	WriteIntegerLittleEndian(bytes, header.NumberOfRvaAndSizes);

	for (const auto& directory : header.DataDirectories)
	{
		WriteIntegerLittleEndian(bytes, directory.VirtualAddress);
		WriteIntegerLittleEndian(bytes, directory.Size);
	}
}
