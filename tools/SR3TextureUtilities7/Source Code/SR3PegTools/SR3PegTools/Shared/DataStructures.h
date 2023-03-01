#pragma once

#pragma pack(push,1)			// Data seems to be aligned to 4 bytes but use this just in case it messes up the freads.

struct TEXCONTAINERHEADER {
	int Magic;								// Always GEKV
    short Version;							// 13 for SRTT and SRIV
	short platform;							// 0 = PC
    unsigned int SizeOf_cpeg_pc;			// Size of the header file.
    unsigned int SizeOf_gpeg_pc;			// Size of the data file.
    unsigned short NumberOfTextures;		// Number of textures in container.
	unsigned short flags;					// Various flags. Always 0?
    unsigned short NumberOfTexturesDup;		// Number of records in container. Same as NumberOfTextures.
    unsigned short Alignment;				// Always 16 for the PC.
};

enum TEXTURETYPES: unsigned short {
	D3DFMT_DXT1=400,
    D3DFMT_DXT3,
    D3DFMT_DXT5,
    D3DFMT_R5G6B5,
    D3DFMT_A1R5G5B5,
    D3DFMT_A4R4G4B4,
    D3DFMT_R8G8B8,
    D3DFMT_A8R8G8B8,
    D3DFMT_V8U8,
    D3DFMT_CxV8U8,
    D3DFMT_A8,
    D3DFMT_DXT5alt=701,
	FORCE16BITS=0x7fff
};

struct FLAGS {
	unsigned short alpha:1;
	unsigned short non_pow2:1;
	unsigned short alpha_test:1;
	unsigned short cube_map:1;
	unsigned short interleaved_mips:1;
	unsigned short interleaved_data:1;
	unsigned short debug_data:1;
	unsigned short dynamic:1;
	unsigned short anim_sheet:1;
	unsigned short linear_color_space:1;
	unsigned short high_mip:1;
	unsigned short high_mip_eligible:1;
	unsigned short inked_to_high_mip:1;
	unsigned short permamently_registered:1;
};

struct TEXCONTAINERRECORD {
	__int64 FileOffsetOfData;			// File position of texture data
    unsigned short Width;				// Width of texture
    unsigned short Height;				// Height of texture
    TEXTURETYPES TextureEnum;			// Texture type (see TEXTURETYPES enum).
	unsigned short pal_format;			// Not used by engine. Always 0.
	unsigned short anim_tiles_width;	// Not used by engine. Always 1.
	unsigned short anim_tile_height;	// Not used by engine. Always 1.
    unsigned short num_frames;			// Not used by engine. Always 1.
    FLAGS flags;						// Various flags for texture. (See FLAGS struct).
	__int64 filename;					// Not used by engine. Always 0.
	unsigned short pal_size;			// Not used by engine. Always 0.
    unsigned char fps;					// Not used by engine. Always 1.
    unsigned char NumberOfMipLevels;	// Number of mip levels in texture.
    unsigned int SizeOfData;			// Size of the texture data.
    unsigned int scratch[8];			// Runtime scratch pad. (Always filled with zeros).
};
#pragma pack(pop)


struct DDS_PIXELFORMAT {
  DWORD dwSize;
  DWORD dwFlags;
  DWORD dwFourCC;
  DWORD dwRGBBitCount;
  DWORD dwRBitMask;
  DWORD dwGBitMask;
  DWORD dwBBitMask;
  DWORD dwABitMask;
};

struct DDS_HEADER {
	DWORD			Magic;
	DWORD           dwSize;
	DWORD           dwFlags;
	DWORD           dwHeight;
	DWORD           dwWidth;
	DWORD           dwPitchOrLinearSize;
	DWORD           dwDepth;
	DWORD           dwMipMapCount;
	DWORD           dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	DWORD           dwCaps;
	DWORD           dwCaps2;
	DWORD           dwCaps3;
	DWORD           dwCaps4;
	DWORD           dwReserved2;
};