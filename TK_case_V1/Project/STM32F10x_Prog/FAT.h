/*************************************************************/
/*                   FAT操作函数库                           */
/*  环境WinAVR 20060421                                      */
/*  作者：Bozai（章其波）                                    */
/*  E-mail：sudazqb@163.com                                  */
/*  2007年2月13日                                            */
/*************************************************************/


#ifndef __FAT_H__
#define __FAT_H__
#include<stdlib.h>
#include "T226.h"

#define CHAR	char
//#define BYTE	unsigned char
//#define WORD	unsigned short
//#define DWORD	unsigned int


#define FAT_BUFFERED 1

#define MAX_LONG_NAME_SIZE 132	/* 26*n+2   n=5 */
#define FAT_DEBUG 0		/* FAT debug flag */

#define FIX_DIRECTORY 0		/* 1 means use fix directory, 0 for any directory */


#if FIX_DIRECTORY==0

	#define  RECORD_SIZE 256	
	#define  RECORD_ADDR_START 0x0000	/* eeprom start address */
	#define  RECORD_ADDR_END  RECORD_SIZE*4	/* eeprom end address */
	
#endif

//extern BYTE (* SD_ReadBlock)(DWORD,BYTE *);
//extern BYTE (* SD_WriteBlock)(DWORD,BYTE *);
//extern DWORD (* FAT_ReadCapacity)(void);

//声明外部器件读写函数

extern BYTE CH375_ReadOneSector(DWORD LBA,BYTE *buffer);
extern BYTE CH375_WriteOneSector(DWORD LBA,BYTE *buffer);
extern BYTE SD_ReadSingleBlock(DWORD sector, BYTE* buffer);
extern BYTE SD_WriteSingleBlock(DWORD sector, BYTE* buffer);
extern BYTE IDE_ReadOneSector(DWORD LBA,BYTE *buffer);
extern BYTE IDE_WriteOneSector(DWORD LBA,BYTE *buffer);

extern DWORD SD_GetCapacity(void);



#define MSDOSFSROOT     0               // cluster 0 means the root dir
#define CLUST_FREE      0               // cluster 0 also means a free cluster
#define MSDOSFSFREE     CLUST_FREE
#define CLUST_FIRST     2             	// first legal cluster number
#define CLUST_RSRVD     0xfff6      	// reserved cluster range
#define CLUST_BAD       0xfff7     	// a cluster with a defect
#define CLUST_EOFS      0xfff8     	// start of eof cluster range
#define CLUST_EOFE      0xffff      	// end of eof cluster range


typedef __packed struct			// length 16 bytes
{			
	BYTE	prIsActive;		// 0x80 indicates active partition
	BYTE	prStartHead;		// starting head for partition
	WORD	prStartCylSect;		// starting cylinder and sector
	BYTE	prPartType;		// partition type (see above)
	BYTE	prEndHead;		// ending head for this partition
	WORD	prEndCylSect;		// ending cylinder and sector
	DWORD	prStartLBA;		// first LBA sector for this partition
	DWORD	prSize;			// size of this partition (bytes or sectors ?)
} partrecord;

        
typedef __packed struct
{
	CHAR	psPartCode[512-64-2];	// pad so struct is 512b
	BYTE	psPart[64];		// four partition records (64 bytes)
	BYTE	psBootSectSig0;		// two signature bytes (2 bytes)
	BYTE	psBootSectSig1;
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
} partsector;

typedef __packed struct
{
	CHAR	exDriveNumber;		// drive number (0x80)//0x00 for floopy disk 0x80 for hard disk
	CHAR	exReserved1;		// reserved should always set 0
	CHAR	exBootSignature;	// ext. boot signature (0x29)
#define EXBOOTSIG       0x29
	CHAR	exVolumeID[4];		// volume ID number
	CHAR	exVolumeLabel[11];	// volume label "NO NAME"
	CHAR	exFileSysType[8];	// fs type (FAT12 or FAT)
} extboot;

typedef __packed struct
{
	BYTE	bsJump[3];		// jump inst E9xxxx or EBxx90
	CHAR	bsOemName[8];		// OEM name and version
	CHAR	bsBPB[25];		// BIOS parameter block
	CHAR	bsExt[26];		// Bootsector Extension
	CHAR	bsBootCode[448];	// pad so structure is 512b
	BYTE	bsBootSectSig0;		// boot sector signature byte 0x55 
	BYTE	bsBootSectSig1;		// boot sector signature byte 0xAA
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
} bootsector50;


typedef __packed struct
{
        WORD	bpbBytesPerSec; 	// bytes per sector	//512 1024 2048 or 4096
        BYTE	bpbSecPerClust; 	// sectors per cluster	// power of 2
        WORD	bpbResSectors;  	// number of reserved sectors	//1 is recommend
        BYTE	bpbFATs;        	// number of FATs	// 2 is recommend
        WORD	bpbRootDirEnts; 	// number of root directory entries
        WORD	bpbSectors;     	// total number of sectors
        BYTE	bpbMedia;       	// media descriptor	//0xf8 match the fat[0]
        WORD	bpbFATsecs;     	// number of sectors per FAT
        WORD	bpbSecPerTrack; 	// sectors per track
        WORD	bpbHeads;       	// number of heads
        DWORD	bpbHiddenSecs;  	// # of hidden sectors
        DWORD	bpbHugeSectors; 	// # of sectors if bpbSectors == 0
} bpb50;

typedef __packed struct
{
	BYTE	bsJump[3];		// jump inst E9xxxx or EBxx90
	CHAR	bsOemName[8];		// OEM name and version
	CHAR	bsBPB[53];		// BIOS parameter block
	CHAR	bsExt[26];		// Bootsector Extension
	CHAR	bsBootCode[418];	// pad so structure is 512b
	BYTE	bsBootSectSig2;		// boot sector signature byte 0x00 
	BYTE	bsBootSectSig3;		// boot sector signature byte 0x00
	BYTE	bsBootSectSig0;		// boot sector signature byte 0x55 
	BYTE	bsBootSectSig1;		// boot sector signature byte 0xAA
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
#define BOOTSIG2        0x00
#define BOOTSIG3        0x00
} bootsector710 ;

typedef __packed struct
{
	WORD	bpbBytesPerSec;		// bytes per sector
	BYTE	bpbSecPerClust;		// sectors per cluster
	WORD	bpbResSectors;		// number of reserved sectors
	BYTE	bpbFATs;		// number of FATs
	WORD	bpbRootDirEnts;		// number of root directory entries
	WORD	bpbSectors;		// total number of sectors
	BYTE	bpbMedia;		// media descriptor
	WORD	bpbFATsecs;		// number of sectors per FAT
	WORD	bpbSecPerTrack;		// sectors per track
	WORD	bpbHeads;		// number of heads
	DWORD	bpbHiddenSecs;		// # of hidden sectors
// 3.3 compat ends here
	DWORD	bpbHugeSectors;		// # of sectors if bpbSectors == 0
// 5.0 compat ends here
	DWORD     bpbBigFATsecs;	// like bpbFATsecs for FAT32
	WORD      bpbExtFlags;		// extended flags:
#define FATNUM    0xf			// mask for numbering active FAT
#define FATMIRROR 0x80			// FAT is mirrored (like it always was)
	WORD      bpbFSVers;		// filesystem version
#define FSVERS    0			// currently only 0 is understood
	DWORD     bpbRootClust;		// start cluster for root directory
	WORD      bpbFSInfo;		// filesystem info structure sector
	WORD      bpbBackup;		// backup boot sector
	// There is a 12 byte filler here, but we ignore it
} bpb710;


// Structure of a dos directory entry.	 FAT32的目录项定义 http://blog.csdn.net/kv110/archive/2009/09/26/4596875.aspx
typedef __packed struct
{
	BYTE		deName[8];      // filename, blank filled
#define SLOT_EMPTY      0x00            // slot has never been used
#define SLOT_E5         0x05            // the real value is 0xE5
#define SLOT_DELETED    0xE5            // file in this slot deleted
#define SLOT_DIR		0x2E	// a directorymmm
	BYTE		deExtension[3]; // extension, blank filled
	BYTE		deAttributes;   // file attributes	文件属性（最高两位保留未用，0至5位分别是b0只读位、b1隐藏位、b2系统位、b3卷标位、b4子目录位、b5归档位 00读写）
#define ATTR_NORMAL     0x00            // normal file
#define ATTR_READONLY   0x01            // file is readonly
#define ATTR_HIDDEN     0x02            // file is hidden
#define ATTR_SYSTEM     0x04            // file is a system file
#define ATTR_VOLUME     0x08            // entry is a volume label
#define ATTR_LONG_FILENAME	0x0F	// this is a long filename entry			    
#define ATTR_DIRECTORY  0x10            // entry is a directory name
#define ATTR_ARCHIVE    0x20            // file is new or modified
	BYTE        deLowerCase;    	// NT VFAT lower case flags  (set to zero) --仅长文件名目录项用，用来存储其对应的短文件名目录项的文件名字节校验和等?
#define LCASE_BASE      0x08            // filename base in lower case
#define LCASE_EXT       0x10            // filename extension in lower case
	BYTE        deCHundredth;   	// hundredth of seconds in CTime   24位二进制的文件建立时间，其中的高5位为小时，次6位为分钟
	BYTE        deCTime[2];     	// create time
	BYTE        deCDate[2];     	// create date 16位二进制的文件建立日期，其中的高7位为相对于1980年的年份值，次4位为月份，后5位为月内日期
	BYTE        deADate[2];     	// access date	16位二进制的文件最新访问日期，定义同上
	WORD        deHighClust; 	// high bytes of cluster number	起始簇号的高16位
	BYTE        deMTime[2];     	// last update time	   16位二进制的文件最新修改时间，其中的高5位为小时，次6位为分钟，后5位的二倍为秒数
	BYTE        deMDate[2];     	// last update date	 16位二进制的文件建立日期，其中的高7位为相对于1980年的年份值，次4位为月份，后5位为月内日期
	WORD        deStartCluster; 	// starting cluster of file	起始簇号的低16位
	DWORD       deFileSize;  	// size of file in bytes32位的文件字节长度（对于子目录，此项为0）
} direntry;

// number of directory entries in one sector
#define DIRENTRIES_PER_SECTOR	0x10	//when the bpbBytesPerSec=512 

// Structure of a Win95 long name directory entry

typedef __packed struct
{
	BYTE		weCnt;		// 
#define WIN_LAST        0x40
#define WIN_CNT         0x3f
	BYTE		wePart1[10];
	BYTE		weAttributes;
#define ATTR_WIN95      0x0f
	BYTE		weReserved1;
	BYTE		weChksum;
	BYTE		wePart2[12];
	WORD       	weReserved2;
	BYTE		wePart3[4];
} winentry;

#define WIN_ENTRY_CHARS	13      	// Number of chars per winentry

// Maximum filename length in Win95
// Note: Must be < sizeof(dirent.d_name)
#define WIN_MAXLEN      255

// This is the format of the contents of the deTime field in the direntry
// structure.
// We don't use bitfields because we don't know how compilers for
// arbitrary machines will lay them out.
#define DT_2SECONDS_MASK        0x1F    // seconds divided by 2
#define DT_2SECONDS_SHIFT       0
#define DT_MINUTES_MASK         0x7E0   // minutes
#define DT_MINUTES_SHIFT        5
#define DT_HOURS_MASK           0xF800  // hours
#define DT_HOURS_SHIFT          11

// This is the format of the contents of the deDate field in the direntry
// structure.
#define DD_DAY_MASK		0x1F	// day of month
#define DD_DAY_SHIFT		0
#define DD_MONTH_MASK		0x1E0	// month
#define DD_MONTH_SHIFT		5
#define DD_YEAR_MASK		0xFE00	// year - 1980
#define DD_YEAR_SHIFT		9



// Stuctures
typedef __packed struct
{
	DWORD StartCluster;		//< file starting cluster for last file accessed
	DWORD Size;			//< file size for last file accessed
	BYTE Attr;			//< file attr for last file accessed
	//unsigned short CreateTime;	//< file creation time for last file accessed
	//unsigned short CreateDate;	//< file creation date for last file accessed
	DWORD Sector;			//<file record place
	WORD Offset;			//<file record offset
} FileInfoStruct;


//MP3后添加的函数

#if FIX_DIRECTORY
	extern BYTE Search(BYTE *dir, direntry *MusicInfo,WORD *Count,BYTE *type);//查找音乐文件
#else
	extern BYTE SearchInit(void);
	extern BYTE Search(WORD *music_record_addr, direntry *MusicInfo,WORD *Count,BYTE *type);//查找音乐文件
	extern BYTE SearchLrc(BYTE *dir,BYTE * longnamebuffer,direntry *LrcInfo,WORD music_record_addr);//serch lrc file
#endif

extern BYTE FAT_LoadPartCluster(DWORD cluster,BYTE part,BYTE * buffer);//加载文件



//FAT原有的函数
extern BYTE FAT_Init(void);//初始化
//extern BYTE FAT_LoadCluster(DWORD cluster,BYTE * buffer);//读一个扇区
extern DWORD FAT_NextCluster(DWORD cluster);//查找下一簇号
//extern DWORD FAT_FindFreeCluster(void);//查找空簇
extern BYTE FAT_DisDir(BYTE *dir);//显示目录内容
extern WORD FAT_FindItem(DWORD cluster, BYTE *name, FileInfoStruct *  FileInfo);//查找文件
extern DWORD FAT_Open(BYTE * dir);//打开文件
extern DWORD FAT_OpenDir(BYTE * dir);//打开目录
extern BYTE FAT_Read(DWORD pointer, DWORD size);//读文件，size=0代表整个文件
extern BYTE FAT_Rename(BYTE *dir,BYTE *newname);//重命名
extern BYTE FAT_ModifyFAT(DWORD cluster,DWORD val);//修改FAT项
extern BYTE FAT_Delete(BYTE *dir);//删除文件
extern BYTE FAT_DelItem( FileInfoStruct  *FileInfo);//删除记录项
extern WORD FAT_FindFreeItem(DWORD cluster, FileInfoStruct  *FileInfo);//查找空余项
extern BYTE FAT_MkDir(BYTE * dir);//建目录
extern BYTE FAT_Write(DWORD cluster,BYTE *data,DWORD size);//写文件
extern DWORD FAT_Create(BYTE * dir,DWORD size);//建文件，必须指定文件大小
extern BYTE FAT_RmDir(BYTE * dir);//删除目录
extern WORD FAT_Close(DWORD * p);//关闭文件

extern void CopyDirentruyItem( direntry  *Desti,direntry *Source);
extern BYTE Search_man( direntry  *MusicInfo,WORD *Count,BYTE *type);

#endif
