// SR3AddTexture.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char TextureHeaderFile[MAX_PATH];
char TextureDataFile[MAX_PATH];
char TextureFilePath[MAX_PATH];
char TextureName[MAX_PATH];
bool Silent=false;
char *RawDDSData=NULL;
char *NameList=NULL;
char **TextureNames=NULL;

int main(int argc, char* argv[])
{

	if (argc<3)
	{
		std::cout << "Saints Row the Third texture adder" << std::endl;
		std::cout << "To use:" << std::endl;
		std::cout << argv[0] << " [optional paramemters] <texture header filepath> <texture to add>" << std::endl << std::endl;
		std::cout << "The texture header file must be a *.cpeg_pc or *.cvbm_pc file.\n" << std::endl;
		std::cout << "The program will update the cpeg_pc/cvbm_pc and create a" << std::endl;
		std::cout << "new gpeg_pc/gvbm_pc file with your new textures." << std::endl << std::endl; 
		std::cout << "Optional parameters:" << std::endl;
		std::cout << "-silent" << std::endl;
		std::cout << "Only display errors when running." << std::endl;
		
		return(1);
	}

	strcpy(TextureFilePath,argv[argc-1]);
	strcpy(TextureHeaderFile,argv[argc-2]);
	strcpy(TextureDataFile,TextureHeaderFile);
	if(toupper(TextureHeaderFile[strlen(TextureHeaderFile)-6])=='P')
		strcpy(TextureDataFile+strlen(TextureHeaderFile)-8,".gpeg_pc");
	else
		strcpy(TextureDataFile+strlen(TextureHeaderFile)-8,".gvbm_pc");
	
	FILE *TEXHEADER_HANDLE;
	FILE *TEXDATA_HANDLE;
	FILE *DDSFILE_HANDLE;

	TEXHEADER_HANDLE=fopen(TextureHeaderFile,"rb");
	if(TEXHEADER_HANDLE==NULL)
	{
		std::cout << "ERROR - Can't open " << TextureHeaderFile << " for reading." << std::endl;
		return(1);
	}

	strcpy(TextureName,TextureFilePath);
	PathStripPathA(TextureName);
	PathRemoveExtensionA(TextureName);
	
	if (argc>3)
	{
		for(int i=1;i<(argc-2);i++)
		{
			
			if(strcmp(argv[i],"-silent")==0)
			{
				Silent=true;
				continue;
			}

			std::cout << "Unknown parameter : " << argv[i] << std::endl;
		}
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

	TextureHeaderData.Alignment=16;		// Alignment must always be 16 bytes for PC.

	TEXCONTAINERRECORD *TextureRecords = new TEXCONTAINERRECORD[TextureHeaderData.NumberOfTextures+1];

	if(fread(TextureRecords,sizeof(TEXCONTAINERRECORD),TextureHeaderData.NumberOfTextures,TEXHEADER_HANDLE)!=TextureHeaderData.NumberOfTextures)
	{
		std::cout << "ERROR - Can't read texture records." << std::endl;
		fclose(TEXHEADER_HANDLE);
		return(1);
	}
		
	long NameListSize=TextureHeaderData.SizeOf_cpeg_pc-ftell(TEXHEADER_HANDLE);
	if (NameListSize>0)
	{
		NameList = new char[NameListSize];
		TextureNames = new char *[TextureHeaderData.NumberOfTextures];

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
	}
	
	fclose(TEXHEADER_HANDLE);				// OK all the data is loaded into memory. Let's close the file.
	
	TEXDATA_HANDLE=fopen(TextureDataFile,"r+b");
	if(TEXDATA_HANDLE==NULL)
	{
		std::cout << "ERROR - Can't open " << TextureDataFile << " for amending." << std::endl;
		return(1);
	}
	fseek(TEXDATA_HANDLE,0,SEEK_END);
	
	DDS_HEADER DDSHeader;
	long DDSSize=0;

	if (!Silent)
		std::cout << "Textures aligned to " << TextureHeaderData.Alignment << " bytes." << std::endl;

	DDSFILE_HANDLE=fopen(TextureFilePath,"rb");
	if(DDSFILE_HANDLE==NULL)
	{
		std::cout << "ERROR - Can't open " << TextureFilePath << " for reading." << std::endl;	
		return(1);
	}

	fseek(DDSFILE_HANDLE,0,SEEK_END);
	DDSSize=ftell(DDSFILE_HANDLE)-128;
	fseek(DDSFILE_HANDLE, 0, SEEK_SET);

	if(!fread(&DDSHeader,sizeof(DDSHeader),1,DDSFILE_HANDLE))
	{
		std::cout << "ERROR: Unable to read " << TextureFilePath << " header data." << std::endl;
		fclose(DDSFILE_HANDLE);
		return(1);
	}

	if(DDSHeader.Magic!=' SDD')
	{
		std::cout << "ERROR: " << TextureFilePath << " is not a valid DDS file." << std::endl;
		fclose(DDSFILE_HANDLE);
		return(1);
	}

	RawDDSData = new char[DDSSize];

	if(!fread(RawDDSData,DDSSize,1,DDSFILE_HANDLE))
	{
		std::cout << "ERROR: Trying to read past the end of the " << TextureFilePath << " file." << std::endl;
		fclose(DDSFILE_HANDLE);
		return(1);
	}

	TEXCONTAINERRECORD *CurrentRecord=&TextureRecords[TextureHeaderData.NumberOfTextures];

	ZeroMemory(CurrentRecord,sizeof(TEXCONTAINERRECORD));

	CurrentRecord->anim_tile_height=1;
	CurrentRecord->anim_tiles_width=1;
	CurrentRecord->fps=1;
	CurrentRecord->num_frames=1;

	for(int i=0;i<8;i++)
		CurrentRecord->scratch[i]=0;
	CurrentRecord->FileOffsetOfData=ftell(TEXDATA_HANDLE);
	if ((CurrentRecord->FileOffsetOfData%TextureHeaderData.Alignment)>0)
	{
		char padding=TextureHeaderData.Alignment-(CurrentRecord->FileOffsetOfData%TextureHeaderData.Alignment);
		for (char i=0;i<padding;i++)
			putc(0,TEXDATA_HANDLE);
		CurrentRecord->FileOffsetOfData=CurrentRecord->FileOffsetOfData+padding;
	}
	CurrentRecord->Height=(unsigned short)DDSHeader.dwHeight;
	if (DDSHeader.dwMipMapCount>0)
		CurrentRecord->NumberOfMipLevels=(unsigned char)DDSHeader.dwMipMapCount;
	else
		CurrentRecord->NumberOfMipLevels=1;
	CurrentRecord->SizeOfData=DDSSize;
	CurrentRecord->Width=(unsigned short)DDSHeader.dwWidth;

	if(DDSHeader.ddspf.dwFlags&0x4)		// Is a FourCC (DTXn) texture.
	{
		switch(DDSHeader.ddspf.dwFourCC)
		{
		case '1TXD':
			CurrentRecord->TextureEnum=D3DFMT_DXT1;
			CurrentRecord->flags.alpha=1;
			break;
		case '3TXD':
			CurrentRecord->TextureEnum=D3DFMT_DXT3;
			CurrentRecord->flags.alpha=1;
			break;
		case '5TXD':
			CurrentRecord->TextureEnum=D3DFMT_DXT5;
			CurrentRecord->flags.alpha=1;
			break;
		default:
			std::cout << "Error unsupported FOURCC texture format in file " << TextureFilePath << std::endl;
			return(1);
		}
	}
	else if (DDSHeader.ddspf.dwRGBBitCount==16)		// 16 bit textures
	{
		switch(DDSHeader.ddspf.dwRBitMask)
		{
		case 0xf800:
			if ((DDSHeader.ddspf.dwGBitMask==0x07e0)&&(DDSHeader.ddspf.dwBBitMask==0x001f)&&(DDSHeader.ddspf.dwABitMask==0))
			{
				CurrentRecord->TextureEnum=D3DFMT_R5G6B5;
				CurrentRecord->flags.alpha=0;
			}
			else
			{
				std::cout << "Error unsupported 16 bit texture format in file " << TextureFilePath << std::endl;
				return(1);
			}
			break;
		case 0x7c00:
			if ((DDSHeader.ddspf.dwGBitMask==0x03e0)&&(DDSHeader.ddspf.dwBBitMask==0x001f)&&(DDSHeader.ddspf.dwABitMask==0x8000))
			{
				CurrentRecord->TextureEnum=D3DFMT_A1R5G5B5;
				CurrentRecord->flags.alpha=1;
			}
			else
			{
				std::cout << "Error unsupported 16 bit texture format in file " << TextureFilePath << std::endl;
				return(1);
			}
		break;
		case 0x0f00:
			if ((DDSHeader.ddspf.dwGBitMask==0x00f0)&&(DDSHeader.ddspf.dwBBitMask==0x000f)&&(DDSHeader.ddspf.dwABitMask==0xf000))
			{
				CurrentRecord->TextureEnum=D3DFMT_A4R4G4B4;
				CurrentRecord->flags.alpha=1;
			}
			else
			{
				std::cout << "Error unsupported 16 bit texture format in file " << TextureFilePath << std::endl;
				return(1);
			}
			break;
		default:
			std::cout << "Error unsupported 16 bit texture format in file " << TextureFilePath << std::endl;
			return(1);
		}
	}
	else if ((DDSHeader.ddspf.dwRGBBitCount==24)&&(DDSHeader.ddspf.dwRBitMask==0x00ff0000)&&
			(DDSHeader.ddspf.dwGBitMask==0x0000ff00)&&(DDSHeader.ddspf.dwBBitMask==0x000000ff)&&
			(DDSHeader.ddspf.dwABitMask==0))
	{
		CurrentRecord->TextureEnum=D3DFMT_R8G8B8;
		CurrentRecord->flags.alpha=0;
	}
	else if ((DDSHeader.ddspf.dwRGBBitCount==32)&&(DDSHeader.ddspf.dwRBitMask==0x00ff0000)&&
		(DDSHeader.ddspf.dwGBitMask==0x0000ff00)&&(DDSHeader.ddspf.dwBBitMask==0x000000ff)&&
		(DDSHeader.ddspf.dwABitMask==0xff000000))
	{
		CurrentRecord->TextureEnum=D3DFMT_A8R8G8B8;
		CurrentRecord->flags.alpha=1;
	}
	else if ((DDSHeader.ddspf.dwRGBBitCount==8)&&(DDSHeader.ddspf.dwRBitMask==0)&&
		(DDSHeader.ddspf.dwGBitMask==0)&&(DDSHeader.ddspf.dwBBitMask==0)&&
		(DDSHeader.ddspf.dwABitMask==0xff))
	{
		CurrentRecord->TextureEnum=D3DFMT_A8;
		CurrentRecord->flags.alpha=1;
	}
	else
	{
		std::cout << "Error unsupported " << DDSHeader.ddspf.dwRGBBitCount << " bit texture format in file " << TextureFilePath << std::endl;
		return(1);
	}

	fclose(DDSFILE_HANDLE);

	if(!Silent)
		std::cout << "Packing texture " << TextureName << " - (" << TextureRecords[TextureHeaderData.NumberOfTextures].Width << " x " << TextureRecords[TextureHeaderData.NumberOfTextures].Height << ")" << std::endl;
	
	if(!fwrite(RawDDSData,DDSSize,1,TEXDATA_HANDLE))
	{
		std::cout << "ERROR - Unable to write texture data to " << TextureDataFile << std::endl;
		fclose(TEXDATA_HANDLE);
		return(1);
	}

	delete[] RawDDSData;

	TextureHeaderData.NumberOfTextures++;
	TextureHeaderData.NumberOfTexturesDup++;
	
	TextureHeaderData.SizeOf_gpeg_pc=ftell(TEXDATA_HANDLE);
	TextureHeaderData.SizeOf_cpeg_pc=TextureHeaderData.SizeOf_cpeg_pc+sizeof(TEXCONTAINERRECORD)+strlen(TextureName)+1;

	TEXHEADER_HANDLE=fopen(TextureHeaderFile,"wb");
	if(TEXHEADER_HANDLE==NULL)
	{
		std::cout << "ERROR - Can't open " << TextureHeaderFile << " for writing." << std::endl;
		return(1);
	}

	if(!fwrite(&TextureHeaderData,sizeof(TextureHeaderData),1,TEXHEADER_HANDLE))
	{
		std::cout << "ERROR - Can't write header data." << std::endl;
		fclose(TEXHEADER_HANDLE);
		return(1);
	}

	if(fwrite(TextureRecords,sizeof(TEXCONTAINERRECORD),TextureHeaderData.NumberOfTextures,TEXHEADER_HANDLE)!=TextureHeaderData.NumberOfTextures)
	{
		std::cout << "ERROR - Can't write texture record data." << std::endl;
		fclose(TEXHEADER_HANDLE);
		return(1);
	}

	if(NameListSize>0)
	{
		if(!fwrite(NameList,NameListSize,1,TEXHEADER_HANDLE))
		{
			std::cout << "ERROR - Can't write texture name data." << std::endl;
			fclose(TEXHEADER_HANDLE);
			return(1);
		}
	}

	if(!fwrite(TextureName,strlen(TextureName)+1,1,TEXHEADER_HANDLE))
	{
		std::cout << "ERROR - Can't write texture name data." << std::endl;
		fclose(TEXHEADER_HANDLE);
		return(1);
	}
	
	fclose(TEXHEADER_HANDLE);
	fclose(TEXDATA_HANDLE);
	
	delete[] TextureNames;			// Let's do some housekeeping.
	delete[] NameList;
	delete[] TextureRecords;
	
	return 0;
}