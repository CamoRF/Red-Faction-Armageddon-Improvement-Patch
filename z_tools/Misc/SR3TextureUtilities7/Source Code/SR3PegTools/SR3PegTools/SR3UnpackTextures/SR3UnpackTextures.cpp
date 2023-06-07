// SR3UnpackTextures.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

char TextureHeaderFile[MAX_PATH];
char TextureDataFile[MAX_PATH];
char TexturePath[MAX_PATH];
bool Silent=false;
bool fix=false;

int main(int argc, char* argv[])
{
	if (argc<2)
	{
		std::cout << "Saints Row the Third texture unpacker" << std::endl;
		std::cout << "To use:" << std::endl << std::endl;
		std::cout << argv[0] << " [optional arguements] <texture header filepath>" << std::endl << std::endl;
		std::cout << "The texture header file must be a *.cpeg_pc or *.cvbm_pc file." << std::endl;
		std::cout << "The texture header file must also have a corresponding" << std::endl;
		std::cout << "<filename>.gpeg_pc or <filename>.gvbm_pc file for this" << std::endl;
		std::cout << "utility to work." << std::endl << std::endl;
		std::cout << "The optional arguements are:" << std::endl;
		std::cout << "-o:<output directory>" << std::endl;
		std::cout << "Specify the ouput directory of the texture files in <output directory>." << std::endl;
		std::cout << "For directories with spaces in wrap in double quotes." << std::endl;
		std::cout << "If no directory is specified it will use the directory " << argv[0] << "was called from." << std::endl;
		return(1);
	}

	strcpy_s(TextureHeaderFile,MAX_PATH,argv[argc-1]);
	strcpy_s(TextureDataFile,MAX_PATH,TextureHeaderFile);
	if(toupper(TextureHeaderFile[strlen(TextureHeaderFile)-6])=='P')
		strcpy(TextureDataFile+strlen(TextureHeaderFile)-8,".gpeg_pc");
	else
		strcpy(TextureDataFile+strlen(TextureHeaderFile)-8,".gvbm_pc");

	strcpy(TexturePath,TextureHeaderFile);
	PathRemoveFileSpecA(TexturePath);

	if (argc>2)
	{
		for(int i=1;i<(argc-1);i++)
		{
			//std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
			if(strncmp(argv[i],"-o:",3)==0)
			{
				if(strlen(argv[i])==3)
				{
					std::cout << "No path specified for the output directory." << std::endl;
					std::cout << "Setting to current directory." << std::endl;
					strcpy(TexturePath,".");
				}
				else
				{
					strcpy(TexturePath,argv[i]+3);
					if(!Silent)
						std::cout << "Setting output path to " << TexturePath << std::endl;
				}
				continue;
			}

			if(strcmp(argv[i],"-silent")==0)
			{
				Silent=true;
				continue;
			}

			if (strcmp(argv[i],"-fix")==0)
			{
				fix=true;
				continue;
			}

			std::cout << "Unknown parameter : " << argv[i] << std::endl;
		}
	}

	if (strlen(TexturePath)==0)
		strcpy(TexturePath,".");

	//printf("Texture data file %s\n",TextureDataFile);

	FILE *TEXHEADER_HANDLE;
	FILE *TEXDATA_HANDLE;
	FILE *DDSFILE_HANDLE;

	TEXHEADER_HANDLE=fopen(TextureHeaderFile,"rb");
	if(TEXHEADER_HANDLE==NULL)
	{
		std::cout << "ERROR - Can't open " << TextureHeaderFile << " for reading." << std::endl;
		return(1);
	}

	TEXCONTAINERHEADER TextureHeaderData;

	if(!fread(&TextureHeaderData,sizeof(TextureHeaderData),1,TEXHEADER_HANDLE))
	{
		std::cout << "ERROR - Can't read header data." << std::endl;
		fclose(TEXHEADER_HANDLE);
		return(1);
	}

	if(TextureHeaderData.Magic!='VKEG')
	{
		std::cout << "ERROR - Not a valid cpeg_pc or cvbm_pc file." << std::endl;
		fclose(TEXHEADER_HANDLE);
		return(1);
	}

	if(TextureHeaderData.Version!=13)
	{
		std::cout << "ERROR - Unknown version number. Will attempt to decode file but bad" << std::endl;
		std::cout << "things will probably happen." << std::endl;
	}

	if(!Silent)
		std::cout << "Number of textures stored = " << TextureHeaderData.NumberOfTextures << std::endl;

	TEXCONTAINERRECORD *TextureRecords = new TEXCONTAINERRECORD[TextureHeaderData.NumberOfTextures];

	if(fread(TextureRecords,sizeof(TEXCONTAINERRECORD),TextureHeaderData.NumberOfTextures,TEXHEADER_HANDLE)!=TextureHeaderData.NumberOfTextures)
	{
		std::cout << "ERROR - Can't read texture records." << std::endl;
		fclose(TEXHEADER_HANDLE);
		return(1);
	}
		
	long NameListSize=TextureHeaderData.SizeOf_cpeg_pc-ftell(TEXHEADER_HANDLE);
	char *NameList = new char[NameListSize];
	char **TextureNames = new char *[TextureHeaderData.NumberOfTextures];

	if(!fread(NameList,NameListSize,1,TEXHEADER_HANDLE))
	{
		std::cout << "ERROR - Filename list data truncated. May get errors." << std::endl;
	}
	
	
	TextureNames[0]=NameList;
	for(int i=1;i<TextureHeaderData.NumberOfTextures;i++)
	{
		char *t;							// I hate manipulating pointers.
		t=TextureNames[i-1];
		t=t+strlen(TextureNames[i-1]);
		t++;
		TextureNames[i]=t;
	}
	
	fclose(TEXHEADER_HANDLE);				// OK all the data is loaded into memory. Let's close the file.
	
	TEXDATA_HANDLE=fopen(TextureDataFile,"rb");
	if(TEXDATA_HANDLE==NULL)
	{
		std::cout << "ERROR - Can't open " << TextureDataFile << " for reading." << std::endl;
		return(1);
	}

// Fix for broken hex edited header files.

	if (fix && (TextureHeaderData.NumberOfTextures==1))
	{
		fseek(TEXDATA_HANDLE,0,SEEK_END);
		if((TextureRecords[0].SizeOfData+TextureRecords[0].FileOffsetOfData)<ftell(TEXDATA_HANDLE))
		{
			std::cout << "Fixing texture size." << std::endl;
			TextureRecords[0].SizeOfData=ftell(TEXDATA_HANDLE)-TextureRecords[0].FileOffsetOfData;
		}
		fseek(TEXDATA_HANDLE,0,SEEK_SET);
	}

	DDS_HEADER DDSHeader;
	char *RawDDSData;
	char DDSFileName[260];

	for(int i=0;i<TextureHeaderData.NumberOfTextures;i++)
	{
		if(ftell(TEXDATA_HANDLE)!=TextureRecords[i].FileOffsetOfData)
		{
			fseek(TEXDATA_HANDLE,TextureRecords[i].FileOffsetOfData,SEEK_SET);
		}
		RawDDSData = new char[TextureRecords[i].SizeOfData];
		if(!fread(RawDDSData,TextureRecords[i].SizeOfData,1,TEXDATA_HANDLE))
		{
			std::cout << "ERROR: Trying to read past the end of the " << TextureDataFile << " file." << std::endl;
			fclose(TEXDATA_HANDLE);
			return(1);
		}
	
		strcpy(DDSFileName,TexturePath);
		strcat(DDSFileName,"\\");
		strcat(DDSFileName,TextureNames[i]);
		strcat(DDSFileName,".DDS");
		
		DDSFILE_HANDLE=fopen(DDSFileName,"wb");
		if(DDSFILE_HANDLE==NULL)
		{
			std::cout << "ERROR - Can't create " << DDSFileName << " for writing." << std::endl;
			return(1);
		}
		
		memset(&DDSHeader,0,sizeof(DDSHeader));

		DDSHeader.Magic=' SDD';
		DDSHeader.dwSize=124;
		DDSHeader.dwFlags=0x081007;
		if(TextureRecords[i].NumberOfMipLevels>1)
		{
			DDSHeader.dwFlags=0x0A1007;
			DDSHeader.dwMipMapCount=TextureRecords[i].NumberOfMipLevels;
			DDSHeader.dwCaps=0x401008;
		}
		else
			DDSHeader.dwCaps=0x1000;
		DDSHeader.dwHeight=TextureRecords[i].Height;
		DDSHeader.dwWidth=TextureRecords[i].Width;
		DDSHeader.ddspf.dwSize=32;

		switch (TextureRecords[i].TextureEnum)
		{
		case D3DFMT_DXT1:
			DDSHeader.ddspf.dwFlags=0x4;
			DDSHeader.ddspf.dwFourCC='1TXD';
			DDSHeader.dwPitchOrLinearSize=((TextureRecords[i].Height/4)*(TextureRecords[i].Width/4))*8;
			break;
		case D3DFMT_DXT3:
			DDSHeader.ddspf.dwFlags=0x4;
			DDSHeader.ddspf.dwFourCC='3TXD';
			DDSHeader.dwPitchOrLinearSize=((TextureRecords[i].Height/4)*(TextureRecords[i].Width/4))*16;
			break;
		case D3DFMT_DXT5:
		case D3DFMT_DXT5alt:
			DDSHeader.ddspf.dwFlags=0x4;
			DDSHeader.ddspf.dwFourCC='5TXD';
			DDSHeader.dwPitchOrLinearSize=((TextureRecords[i].Height/4)*(TextureRecords[i].Width/4))*16;
			break;
		case D3DFMT_R5G6B5:
			DDSHeader.ddspf.dwFlags=0x40;
			DDSHeader.ddspf.dwRGBBitCount=16;
			DDSHeader.ddspf.dwRBitMask=0xf800;
			DDSHeader.ddspf.dwGBitMask=0x07e0;
			DDSHeader.ddspf.dwBBitMask=0x001f;
			DDSHeader.ddspf.dwABitMask=0x0000;
			DDSHeader.dwPitchOrLinearSize=(TextureRecords[i].Height*TextureRecords[i].Width)*2;
			break;
		case D3DFMT_A1R5G5B5:
			DDSHeader.ddspf.dwFlags=0x41;
			DDSHeader.ddspf.dwRGBBitCount=16;
			DDSHeader.ddspf.dwRBitMask=0x7c00;
			DDSHeader.ddspf.dwGBitMask=0x03e0;
			DDSHeader.ddspf.dwBBitMask=0x001f;
			DDSHeader.ddspf.dwABitMask=0x8000;
			DDSHeader.dwPitchOrLinearSize=(TextureRecords[i].Height*TextureRecords[i].Width)*2;
			break;
		case D3DFMT_A4R4G4B4:
			DDSHeader.ddspf.dwFlags=0x41;
			DDSHeader.ddspf.dwRGBBitCount=16;
			DDSHeader.ddspf.dwRBitMask=0x0f00;
			DDSHeader.ddspf.dwGBitMask=0x00f0;
			DDSHeader.ddspf.dwBBitMask=0x000f;
			DDSHeader.ddspf.dwABitMask=0xf000;
			DDSHeader.dwPitchOrLinearSize=(TextureRecords[i].Height*TextureRecords[i].Width)*2;
			break;
		case D3DFMT_R8G8B8:
			DDSHeader.ddspf.dwFlags=0x40;
			DDSHeader.ddspf.dwRGBBitCount=24;
			DDSHeader.ddspf.dwRBitMask=0x00ff0000;
			DDSHeader.ddspf.dwGBitMask=0x0000ff00;
			DDSHeader.ddspf.dwBBitMask=0x000000ff;
			DDSHeader.ddspf.dwABitMask=0x00000000;
			DDSHeader.dwPitchOrLinearSize=(TextureRecords[i].Height*TextureRecords[i].Width)*3;
			break;
		case D3DFMT_A8R8G8B8:
			DDSHeader.ddspf.dwFlags=0x41;
			DDSHeader.ddspf.dwRGBBitCount=32;
			DDSHeader.ddspf.dwRBitMask=0x00ff0000;
			DDSHeader.ddspf.dwGBitMask=0x0000ff00;
			DDSHeader.ddspf.dwBBitMask=0x000000ff;
			DDSHeader.ddspf.dwABitMask=0xff000000;
			DDSHeader.dwPitchOrLinearSize=(TextureRecords[i].Height*TextureRecords[i].Width)*4;
			break;
		case D3DFMT_V8U8:
			DDSHeader.ddspf.dwFlags=0x00800000;
			DDSHeader.ddspf.dwRGBBitCount=16;
			DDSHeader.ddspf.dwRBitMask=0x00ff;
			DDSHeader.ddspf.dwGBitMask=0xff00;
			DDSHeader.ddspf.dwBBitMask=0x0000;
			DDSHeader.ddspf.dwABitMask=0x0000;
			DDSHeader.dwPitchOrLinearSize=(TextureRecords[i].Height*TextureRecords[i].Width)*2;
			break;
		case D3DFMT_CxV8U8:
			DDSHeader.ddspf.dwFlags=0x4;
			DDSHeader.ddspf.dwFourCC=0x75;
			DDSHeader.ddspf.dwRGBBitCount=16;
			DDSHeader.ddspf.dwRBitMask=0x00ff;
			DDSHeader.ddspf.dwGBitMask=0xff00;
			DDSHeader.ddspf.dwBBitMask=0x0000;
			DDSHeader.ddspf.dwABitMask=0x0000;
			DDSHeader.dwPitchOrLinearSize=(TextureRecords[i].Height*TextureRecords[i].Width)*2;
			break;
		case D3DFMT_A8:
			DDSHeader.ddspf.dwFlags=0x2;
			DDSHeader.ddspf.dwRGBBitCount=8;
			DDSHeader.ddspf.dwRBitMask=0x00;
			DDSHeader.ddspf.dwGBitMask=0x00;
			DDSHeader.ddspf.dwBBitMask=0x00;
			DDSHeader.ddspf.dwABitMask=0xff;
			DDSHeader.dwPitchOrLinearSize=(TextureRecords[i].Height*TextureRecords[i].Width);
			break;
		default:
			std::cout << "ERROR - Unknown texture type." << std::endl;
			fclose(DDSFILE_HANDLE);
			return(1);
		}

		if(!Silent)
			std::cout << "Creating texture [" << i << "] " << TextureNames[i] << " - (" << TextureRecords[i].Width <<" x " << TextureRecords[i].Height << ")" << std::endl;
		
		if(!fwrite(&DDSHeader,sizeof(DDSHeader),1,DDSFILE_HANDLE))
		{
			std::cout << "ERROR - Unable to write texture header." << std::cout;
			fclose(DDSFILE_HANDLE);
			return(1);
		}
		
		if(!fwrite(RawDDSData,TextureRecords[i].SizeOfData,1,DDSFILE_HANDLE))
		{
			std::cout << "ERROR - Unable to write texture data." << std::endl;
			fclose(DDSFILE_HANDLE);
			return(1);
		}
		
		delete[] RawDDSData;	
		fclose(DDSFILE_HANDLE);
	}
	
	fclose(TEXDATA_HANDLE);
	
	delete[] TextureNames;			// Let's do some housekeeping.
	delete[] NameList;
	delete[] TextureRecords;

	return 0;
}

