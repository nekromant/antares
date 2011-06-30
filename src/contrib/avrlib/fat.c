/*! \file fat.c \brief FAT16/32 file system driver. */
//*****************************************************************************
//
// File Name	: 'fat.c'
// Title		: FAT16/32 file system driver
// Author		: Pascal Stang
// Date			: 11/07/2000
// Revised		: 12/12/2000
// Version		: 0.3
// Target MCU	: ATmega103 (should work for Atmel AVR Series)
// Editor Tabs	: 4
//
// This code is based in part on work done by Jesper Hansen for his
//		YAMPP MP3 player project.
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "ata.h"
#include "rprintf.h"
#include "debug.h"

#include "fat.h"
#include "fatconf.h"

// globals
// buffers
unsigned char *SectorBuffer  =		(unsigned char *) FAT_SECTOR_BUFFER_ADDR;
unsigned char *FileNameBuffer =		(unsigned char *) FAT_FILENAME_BUFFER_ADDR;
unsigned char *PathNameBuffer =		(unsigned char *) FAT_PATHNAME_BUFFER_ADDR;

// filesystem constants/metrics
struct partrecord PartInfo;
unsigned char Fat32Enabled;
unsigned long FirstDataSector;
unsigned short BytesPerSector;
unsigned short SectorsPerCluster;
unsigned long FirstFATSector;
unsigned long RootDirStartCluster;

// operating variables
unsigned long CurrentDirStartCluster;	//< current directory starting cluster
struct FileInfoStruct FileInfo;			//< file information for last file accessed
unsigned long FatInCache = 0;


/*************************************************************************/
/*************************************************************************/


unsigned long fatClustToSect(unsigned long clust)
{
	return ((clust-2) * SectorsPerCluster) + FirstDataSector;
}

unsigned int fatClusterSize(void)
{
	// return the number of sectors in a disk cluster
	return SectorsPerCluster;
}

unsigned char fatInit( unsigned char device)
{
	//struct partrecord *pr;
	struct bpb710 *bpb;

	// read partition table
	// TODO.... error checking
	ataReadSectors(DRIVE0, 0, 1, SectorBuffer);
	// map first partition record	
	// save partition information to global PartInfo
	PartInfo = *((struct partrecord *) ((struct partsector *) SectorBuffer)->psPart);
//	PartInfo = *pr;
	
	// Read the Partition BootSector
	// **first sector of partition in PartInfo.prStartLBA
	ataReadSectors( DRIVE0, PartInfo.prStartLBA, 1, SectorBuffer );
	bpb = (struct bpb710 *) ((struct bootsector710 *) SectorBuffer)->bsBPB;

	// setup global disk constants
	FirstDataSector	= PartInfo.prStartLBA;
	if(bpb->bpbFATsecs)
	{
		// bpbFATsecs is non-zero and is therefore valid
		FirstDataSector	+= bpb->bpbResSectors + bpb->bpbFATs * bpb->bpbFATsecs;
	}
	else
	{
		// bpbFATsecs is zero, real value is in bpbBigFATsecs
		FirstDataSector	+= bpb->bpbResSectors + bpb->bpbFATs * bpb->bpbBigFATsecs;
	}
	SectorsPerCluster	= bpb->bpbSecPerClust;
	BytesPerSector		= bpb->bpbBytesPerSec;
	FirstFATSector		= bpb->bpbResSectors + PartInfo.prStartLBA;

	switch (PartInfo.prPartType)
	{
		case PART_TYPE_DOSFAT16:
		case PART_TYPE_FAT16:
		case PART_TYPE_FAT16LBA:
			// first directory cluster is 2 by default (clusters range 2->big)
			RootDirStartCluster = CLUST_FIRST;
			// push data sector pointer to end of root directory area
			//FirstDataSector += (bpb->bpbRootDirEnts)/DIRENTRIES_PER_SECTOR;
			Fat32Enabled = FALSE;
			break;
		case PART_TYPE_FAT32LBA:
		case PART_TYPE_FAT32:
			// bpbRootClust field exists in FAT32 bpb710, but not in lesser bpb's
			RootDirStartCluster = bpb->bpbRootClust;
			// push data sector pointer to end of root directory area
			// need this? FirstDataSector += (bpb->bpbRootDirEnts)/DIRENTRIES_PER_SECTOR;
			Fat32Enabled = TRUE;
			break;
		default:
			rprintfProgStrM("Found: No Partition!\r\n");
			//return 1;
			break;
	}

	// set current directory to root (\)
	CurrentDirStartCluster = RootDirStartCluster;
	PathNameBuffer[0] = '\\';
	PathNameBuffer[1] = 0;


	// do debug
#ifdef DEBUG_FAT
	switch (PartInfo.prPartType)
	{
		case PART_TYPE_DOSFAT16:
				rprintfProgStrM("Found: DOSFAT 16\r\n");
				break;
		case PART_TYPE_FAT16:
				rprintfProgStrM("Found: FAT16\r\n");
				break;
		case PART_TYPE_FAT16LBA:
				rprintfProgStrM("Found: FAT16 LBA\r\n");
				break;
		case PART_TYPE_FAT32LBA:
				rprintfProgStrM("Found: FAT32 LBA\r\n");
				break;
		case PART_TYPE_FAT32:
				rprintfProgStrM("Found: FAT32\r\n");
				//return 1;	
				break;
		default:
				rprintfProgStrM("Found: No Partition!\r\n");
				//return 1;
				break;
	}

	rprintfProgStrM("First sector    : ");	rprintfu32(PartInfo.prStartLBA);	rprintfCRLF();
	rprintfProgStrM("Size            : ");	rprintfu32(PartInfo.prSize);		rprintfCRLF();
	rprintfProgStrM("bytes/sector    : ");	rprintfu16(bpb->bpbBytesPerSec);	rprintfCRLF();
	rprintfProgStrM("sectors/cluster : ");	rprintfu08(bpb->bpbSecPerClust);	rprintfCRLF();
	rprintfProgStrM("reserved sectors: ");	rprintfu16(bpb->bpbResSectors);		rprintfCRLF();
	rprintfProgStrM("FatSectors      : ");	rprintfu16(bpb->bpbFATsecs);		rprintfCRLF();
	rprintfProgStrM("BigFatSectors   : ");	rprintfu32(bpb->bpbBigFATsecs);		rprintfCRLF();
	rprintfProgStrM("Number of Fats  : ");	rprintfu08(bpb->bpbFATs);			rprintfCRLF();
	rprintfProgStrM("First Fat Sector: ");	rprintfu32(FirstFATSector);			rprintfCRLF();
	rprintfProgStrM("First Data Sect : ");	rprintfu32(FirstDataSector);		rprintfCRLF();
	rprintfProgStrM("RootDirStartClus: ");	rprintfu32(RootDirStartCluster);	rprintfCRLF();
#endif

	return 0;	
}

//////////////////////////////////////////////////////////////

unsigned char fatGetDirEntry(unsigned short entry)
{
	unsigned long sector;
	struct direntry *de = 0;	// avoid compiler warning by initializing
	struct winentry *we;
	unsigned char haveLongNameEntry;
	unsigned char gotEntry;
	unsigned short b;
	int i,index;
	char *fnbPtr;
	unsigned short entrycount = 0;

	// read dir data
	sector = fatClustToSect(CurrentDirStartCluster);

	haveLongNameEntry = 0;
	gotEntry = 0;

	index = 16;	// crank it up
	
	//while(entrycount < entry) 
	while(1)
	{
		if(index == 16)	// time for next sector ?
		{
			ataReadSectors( DRIVE0, sector++, 1, SectorBuffer);
			de = (struct direntry *) SectorBuffer;
			index = 0;
		}
		
		// check the status of this directory entry slot
		if(de->deName[0] == 0x00)
		{
			// slot is empty and this is the end of directory
			gotEntry = 0;
			break;
		}
		else if(de->deName[0] == 0xE5)
		{
			// this is an empty slot
			// do nothing and move to the next one
		}
		else
		{
			// this is a valid and occupied entry
			// is it a part of a long file/dir name?
			if(de->deAttributes == ATTR_LONG_FILENAME)
			{
				// we have a long name entry
				// cast this directory entry as a "windows" (LFN: LongFileName) entry
				we = (struct winentry *) de;
				
				b = WIN_ENTRY_CHARS*( (we->weCnt-1) & 0x0f);		// index into string
				fnbPtr = &FileNameBuffer[b];
				for (i=0;i<5;i++)	*fnbPtr++ = we->wePart1[i*2];	// copy first part
				for (i=0;i<6;i++)	*fnbPtr++ = we->wePart2[i*2];	// second part
				for (i=0;i<2;i++)	*fnbPtr++ = we->wePart3[i*2];	// and third part
				if (we->weCnt & WIN_LAST) *fnbPtr = 0;				// in case dirnamelength is multiple of 13, add termination
				if ((we->weCnt & 0x0f) == 1) haveLongNameEntry = 1;	// flag that we have a complete long name entry set
			}
			else
			{
				// we have a short name entry
				
				// check if this is the short name entry corresponding
				// to the end of a multi-part long name entry
				if(haveLongNameEntry)
				{
					// a long entry name has been collected
					if(entrycount == entry)		
					{
						// desired entry has been found, break out
						gotEntry = 1;
						break;
					}
					// otherwise
					haveLongNameEntry = 0;	// clear long name flag
					entrycount++;			// increment entry counter		
				}
				else
				{
					// entry is a short name (8.3 format) without a
					// corresponding multi-part long name entry
					fnbPtr = FileNameBuffer;
					for (i=0;i<8;i++)	*fnbPtr++ = de->deName[i];		// copy name
					*fnbPtr++ = '.';									// insert '.'
					for (i=0;i<3;i++)	*fnbPtr++ = de->deExtension[i];	// copy extension
					*fnbPtr = 0;										// null-terminate

					if(entrycount == entry)		
					{
						// desired entry has been found, break out
						gotEntry = 1;
						break;
					}
					// otherwise
					entrycount++;			// increment entry counter		
				}
			}
		}
		// next directory entry
		de++;
		// next index
		index++;
	}
	
	// we have a file/dir to return
	// store file/dir starting cluster (start of data)
	FileInfo.StartCluster = (unsigned long) ((unsigned long)de->deHighClust << 16) + de->deStartCluster;
	// store file/dir size
	// (note: size field for subdirectory entries is always zero)
	FileInfo.Size = de->deFileSize;
	// store file/dir attributes
	FileInfo.Attr = de->deAttributes;
	// store file/dir creation time
	FileInfo.CreateTime = de->deCTime[0] | de->deCTime[1]<<8;
	// store file/dir creation date
	FileInfo.CreateTime = de->deCDate[0] | de->deCDate[1]<<8;

	return gotEntry;
}

// change directory into 
unsigned char fatChangeDirectory(unsigned short entry)
{
	// get the requested directory entry
	if(	fatGetDirEntry(entry) )
	{
		// make sure the entry is a directory
		if(FileInfo.Attr & ATTR_DIRECTORY)
		{
			// change directories into this directory
			// check to see if we are changing back to root directory
			if(FileInfo.StartCluster)
			{
				// standard change directory
				CurrentDirStartCluster = FileInfo.StartCluster;
			}
			else
			{
				// if startCluster pointer is zero,
				// a change to the root directory is intended
				// change directory to root
				CurrentDirStartCluster = RootDirStartCluster;
			}
			// TODO: handle pathname properly for going up a directory
			// set path string
			strcat(PathNameBuffer, FileNameBuffer);
			strcat(PathNameBuffer, "\\");
			// return success
			return TRUE;
		}
		else
		{
			// not a directory, cannot CD into a file!
			return FALSE;
		}
	}
	else
	{
		// not a valid entry, cannot CD!
		return FALSE;
	}
}

void fatPrintDirEntry(void)
{
	// print a formatted dir-style output for most recent file
	// print date
	rprintfNum(10, 2, FALSE, '0', (FileInfo.CreateDate&DD_MONTH_MASK)>>DD_MONTH_SHIFT );	// month
	rprintfChar('/');
	rprintfNum(10, 2, FALSE, '0', (FileInfo.CreateDate&DD_DAY_MASK)>>DD_DAY_SHIFT );		// day
	rprintfChar('/');
	rprintfNum(10, 4, FALSE, '0', (FileInfo.CreateDate&DD_YEAR_MASK)>>DD_YEAR_SHIFT );	// year
	rprintfChar(' ');

	// print time
	rprintfNum(10, 2, FALSE, '0', (FileInfo.CreateTime&DT_HOURS_MASK)>>DT_HOURS_SHIFT );	// month
	rprintfChar(':');
	rprintfNum(10, 2, FALSE, '0', (FileInfo.CreateTime&DT_MINUTES_MASK)>>DT_MINUTES_SHIFT );		// day
	rprintfChar(':');
	rprintfNum(10, 2, FALSE, '0', 2*(FileInfo.CreateTime&DT_2SECONDS_MASK)>>DT_2SECONDS_SHIFT );	// seconds
	rprintfChar(' ');

	// print attributes
	if(FileInfo.Attr & ATTR_VOLUME)		rprintfChar('V'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_DIRECTORY)	rprintfChar('D'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_READONLY)	rprintfChar('R'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_HIDDEN)		rprintfChar('H'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_SYSTEM)		rprintfChar('S'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_ARCHIVE)	rprintfChar('A'); else rprintfChar('-');
	rprintfChar(' ');

	// print filesize
	rprintfNum(10, 8, FALSE, ' ', FileInfo.Size);	// filesize
	rprintfChar(' ');

	// print filename
	rprintfStr(FileNameBuffer);
}

void fatDumpDirSlot(unsigned short slot)
{
	unsigned long sector;
	// load correct sector
	sector = fatClustToSect(CurrentDirStartCluster);
	sector += slot/DIRENTRIES_PER_SECTOR;
	// print the entry as a hex table
	debugPrintHexTable(32, SectorBuffer+(slot<<5) );
}

struct FileInfoStruct* fatGetFileInfo(void)
{
	return &FileInfo;
}

// return the size of the last directory entry
unsigned long fatGetFilesize(void)
{
	return FileInfo.Size;
}

// return the long name of the last directory entry
char* fatGetFilename(void)
{	
	return FileNameBuffer;
}

// return the directory of the last directory entry
char* fatGetDirname(void)
{	
	return PathNameBuffer;
}

// load a clusterfull of data
void fatLoadCluster(unsigned long cluster, unsigned char *buffer)
{
	register unsigned char i;
	// read cluster
	//while ( ataReadSectors( DRIVE0, clust2sect(cluster), SectorsPerCluster, buffer) != 0);
	for(i=0; i<SectorsPerCluster; i++)
	{
		ataReadSectors( DRIVE0, fatClustToSect(cluster)+i, 1, buffer+(i<<9) );
		// temporary fix for wierd misaligned cluster problem
		// (only when using FAT16?)
//		ataReadSectors( DRIVE0, fatClustToSect(cluster+8)+i, 1, buffer+(i<<9) );
	}
}


// find next cluster in the FAT chain
unsigned long fatNextCluster(unsigned long cluster)
{
	unsigned long nextCluster;
	unsigned long fatMask;
	unsigned long fatOffset;
	unsigned long sector;
	unsigned int offset;
	
	// get fat offset in bytes
	if(Fat32Enabled)
	{
		// four FAT bytes (32 bits) for every cluster
		fatOffset = cluster << 2;
		// set the FAT bit mask
		fatMask = FAT32_MASK;
	}
	else
	{
		// two FAT bytes (16 bits) for every cluster
		fatOffset = cluster << 1;
		// set the FAT bit mask
		fatMask = FAT16_MASK;
	}
	
	// calculate the FAT sector that we're interested in
	sector = FirstFATSector + (fatOffset / BytesPerSector);
	// calculate offset of the our entry within that FAT sector
	offset = fatOffset % BytesPerSector;

	// if we don't already have this FAT chunk loaded, go get it
	if (sector != FatInCache)
	{
		// read sector of FAT table
		while (ataReadSectors( DRIVE0, sector, 1, (unsigned char*)FAT_CACHE_ADDR) != 0);
		FatInCache = sector;
	}

	// read the nextCluster value
	nextCluster = (*((unsigned long*) &((char*)FAT_CACHE_ADDR)[offset])) & fatMask;

	// check to see if we're at the end of the chain
	if (nextCluster == (CLUST_EOFE & fatMask))
		nextCluster = 0;

#ifdef DEBUG_FAT
	rprintfProgStrM(">");
	rprintfu32(nextCluster);
	rprintfCRLF();
#endif
	
	return nextCluster;
}
