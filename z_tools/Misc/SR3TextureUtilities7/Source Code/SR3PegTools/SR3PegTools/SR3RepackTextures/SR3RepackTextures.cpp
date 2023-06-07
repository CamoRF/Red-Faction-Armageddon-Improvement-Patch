// SR3RepackTextures.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char TextureHeaderFile[MAX_PATH];
char TextureHeaderFileOut[MAX_PATH];
char TextureDataFile[MAX_PATH];
char TextureDataFileOut[MAX_PATH];
char TexturePath[MAX_PATH];
bool Silent=false;
char *TextureDump=NULL;

int main(int argc, char* argv[])
{
	if (argc<2)
	{
		std::cout << "Saints Row the Third texture repacker" << std::endl;
		std::cout << "To use:" << std::endl;
		std::cout << argv[0] << " [optional paramemters] <texture header filepath>" << std::endl << std::endl;
		std::cout << "The texture header file must be a *.cpeg_pc or *.cvbm_pc file.\n" << std::endl;
		std::cout << "The program will update the cpeg_pc/cvbm_pc and create a" << std::endl;
		std::cout << "new gpeg_pc/gvbm_pc file with your new textures." << std::endl << std::endl; 
		std::cout << "Optional parameters:" << std::endl;
		std::cout << "-o:<output file>" << std::endl;
		std::cout << "Change the name of the output file." << std::endl;
		std::cout << "Must end with .cpeg_pc or cvbm_pc." << std::endl;
		std::cout << std::endl;
		std::cout << "-i:<input directory>" << std::endl;
		std::cout << "Specify the directory that contains the texture file to repack." << std::endl;
		std::cout << "If you don't specify the input directory then the texture header file must" << std::endl;
		std::cout << "also have the unpacked textures in the same directory for this to work." << std::endl;
		std::cout << std::endl;
		std::cout << "-silent" << std::endl;
		std::cout << "Only display errors when running." << std::endl;
		
		return(1);
	}

	strcpy(TextureHeaderFile,argv[argc-1]);
	strcpy(TextureDataFileOut,TextureHeaderFile);
	if(toupper(TextureHeaderFile[strlen(TextureHeaderFile)-6])=='P')
		strcpy(TextureDataFileOut+strlen(TextureHeaderFile)-8,".gpeg_pc");
	else
		strcpy(TextureDataFileOut+strlen(TextureHeaderFile)-8,".gvbm_pc");
	strcpy(TextureHeaderFileOut,TextureHeaderFile);
	strcpy(TextureDataFile,TextureDataFileOut);
	
	FILE *TEXHEADER_HANDLE;
	FILE *TEXDATA_HANDLE;
	FILE *DDSFILE_HANDLE;

	TEXHEADER_HANDLE=fopen(TextureHeaderFile,"rb");
	if(TEXHEADER_HANDLE==NULL)
	{
		std::cout << "ERROR - Can't open " << TextureHeaderFile << " for reading." << std::endl;
		return(1);
	}

	strcpy(TexturePath,TextureHeaderFile);
	PathRemoveFileSpecA(TexturePath);
	
	if (argc>2)
	{
		for(int i=1;i<(argc-1);i++)
		{
			//std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
			if(strncmp(argv[i],"-i:",3)==0)
			{
				if(strlen(argv[i])==3)
				{
					std::cout << "No path specified for the input directory of the textures." << std::endl;
					std::cout << "Setting to current directory." << std::endl;
					strcpy(TexturePath,".");
				}
				else
				{
					strcpy(TexturePath,argv[i]+3);
					if (!Silent)
						std::cout << "Setting input path to " << TexturePath << std::endl;
				}
				continue;
			}

			if(strncmp(argv[i],"-o:",3)==0)
			{
				if(strlen(argv[i])==3)
				{
					std::cout << "No path specified for the output file. Ignoring." << std::endl;
				}
				else
				{
					strcpy(TextureHeaderFileOut,argv[i]+3);
					if((strcmp(TextureHeaderFileOut+(strlen(TextureHeaderFileOut)-8),".cpeg_pc")==0)||
						strcmp(TextureHeaderFileOut+(strlen(TextureHeaderFileOut)-8),".cvbm_pc")==0)
					{
						strcpy(TextureDataFileOut,TextureHeaderFileOut);
						if(toupper(TextureHeaderFile[strlen(TextureHeaderFileOut)-6])=='P')
							strcpy(TextureDataFileOut+strlen(TextureHeaderFileOut)-8,".gpeg_pc");
						else
							strcpy(TextureDataFileOut+strlen(TextureHeaderFileOut)-8,".gvbm_pc");
					}
					else
					{
						std::cout << "Not a valid peg filename for output file." << std::endl;
						std::cout << "Use *.cpeg_pc or *.cvbm_pc." << std::endl;
						return(1);
					}

					if (!Silent)
						std::cout << "Setting output file to " << TextureHeaderFileOut << std::endl;
				}
				continue;
			}

			if(strcmp(argv[i],"-silent")==0)
			{
				Silent=true;
				continue;
			}

			std::cout << "Unknown parameter : " << argv[i] << std::endl;
		}
	}
	if(strlen(TexturePath)==0)
		strcpy(TexturePath,".");

	//printf("Texture path is %s",TexturePath);

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
	fseek(TEXDATA_HANDLE,0,SEEK_END);
	unsigned int TextureFileSize=ftell(TEXDATA_HANDLE);
	fseek(TEXDATA_HANDLE,0,SEEK_SET);

	TextureDump=new char[TextureFileSize];
	fread(TextureDump,TextureFileSize,1,TEXDATA_HANDLE);

	fclose(TEXDATA_HANDLE);

	TEXDATA_HANDLE=fopen(TextureDataFileOut,"wb");
	if(TEXDATA_HANDLE==NULL)
	{
		std::cout << "ERROR - Can't open " << TextureDataFileOut << " for writing." << std::endl;
		return(1);
	}
	
	DDS_HEADER DDSHeader;
	char *RawDDSData;
	bool DeleteRawDDS=false;
	char DDSFileName[260];
	long DDSSize=0;

	if (!Silent)
		std::cout << "Textures aligned to " << TextureHeaderData.Alignment << " bytes." << std::endl;

	for(int i=0;i<TextureHeaderData.NumberOfTextures;i++)
	{
		strcpy(DDSFileName,TexturePath);
		strcat(DDSFileName,"\\");
		strcat(DDSFileName,TextureNames[i]);
		strcat(DDSFileName,".DDS");

		DDSFILE_HANDLE=fopen(DDSFileName,"rb");
		if(DDSFILE_HANDLE==NULL)
		{
			std::cout << "ERROR - Can't open " << DDSFileName << " for reading." << std::endl;
			std::cout << "Will use original texture." << std::endl;
			
			TEXCONTAINERRECORD *CurrentRecord=&TextureRecords[i];
			RawDDSData=TextureDump+CurrentRecord->FileOffsetOfData; // Get original offset of DDS data
			CurrentRecord->FileOffsetOfData=ftell(TEXDATA_HANDLE);	// Store new offset of DDS data
			if ((CurrentRecord->FileOffsetOfData%TextureHeaderData.Alignment)>0)
			{
				char padding=TextureHeaderData.Alignment-(CurrentRecord->FileOffsetOfData%TextureHeaderData.Alignment);
				for (char i=0;i<padding;i++)
					putc(0,TEXDATA_HANDLE);
				CurrentRecord->FileOffsetOfData=CurrentRecord->FileOffsetOfData+padding;
			}
			DDSSize=CurrentRecord->SizeOfData;	// Get size of original DDS data
			DeleteRawDDS=false;	// We don't want to delete cached original textures yet as it will break program
		}
		else
		{
			fseek(DDSFILE_HANDLE,0,SEEK_END);
			DDSSize=ftell(DDSFILE_HANDLE)-128;
			fseek(DDSFILE_HANDLE, 0, SEEK_SET);

			if(!fread(&DDSHeader,sizeof(DDSHeader),1,DDSFILE_HANDLE))
			{
				std::cout << "ERROR: Unable to read " << DDSFileName << " header data." << std::endl;
				fclose(DDSFILE_HANDLE);
				return(1);
			}

			if(DDSHeader.Magic!=' SDD')
			{
				std::cout << "ERROR: " << DDSFileName << " is not a valid DDS file." << std::endl;
				fclose(DDSFILE_HANDLE);
				return(1);
			}

			RawDDSData = new char[DDSSize];
			DeleteRawDDS=true;
		
			if(!fread(RawDDSData,DDSSize,1,DDSFILE_HANDLE))
			{
				std::cout << "ERROR: Trying to read past the end of the " << DDSFileName << " file." << std::endl;
				fclose(DDSFILE_HANDLE);
				return(1);
			}

			TEXCONTAINERRECORD *CurrentRecord=&TextureRecords[i];

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
					std::cout << "Error unsupported FOURCC texture format in file " << DDSFileName << std::endl;
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
						std::cout << "Error unsupported 16 bit texture format in file " << DDSFileName << std::endl;
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
						std::cout << "Error unsupported 16 bit texture format in file " << DDSFileName << std::endl;
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
						std::cout << "Error unsupported 16 bit texture format in file " << DDSFileName << std::endl;
						return(1);
					}
					break;
				default:
					std::cout << "Error unsupported 16 bit texture format in file " << DDSFileName << std::endl;
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
				std::cout << "Error unsupported " << DDSHeader.ddspf.dwRGBBitCount << " bit texture format in file " << DDSFileName << std::endl;
				return(1);
			}

			fclose(DDSFILE_HANDLE);
		}

		if(!Silent)
			std::cout << "Packing texture [" << i <<"] " << TextureNames[i] << " - (" << TextureRecords[i].Width << " x " << TextureRecords[i].Height << ")" << std::endl;
		
		if(!fwrite(RawDDSData,DDSSize,1,TEXDATA_HANDLE))
		{
			std::cout << "ERROR - Unable to write texture data to " << TextureDataFileOut << std::endl;
			fclose(TEXDATA_HANDLE);
			return(1);
		}
		if (DeleteRawDDS)
			delete[] RawDDSData;

	}
	
	TextureHeaderData.SizeOf_gpeg_pc=ftell(TEXDATA_HANDLE);
	
	TEXHEADER_HANDLE=fopen(TextureHeaderFileOut,"wb");
	if(TEXHEADER_HANDLE==NULL)
	{
		std::cout << "ERROR - Can't open " << TextureHeaderFileOut << " for writing." << std::endl;
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

	if(!fwrite(NameList,NameListSize,1,TEXHEADER_HANDLE))
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
	delete[] TextureDump;
	
	return 0;
}

