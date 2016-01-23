#pragma once

#include <3ds/types.h>

typedef struct
{
	u8 reserved[5];
	u8 flag;
	u8 remasterversion[2];
} exheader_systeminfoflags;

typedef struct
{
	u32 address;
	u32 nummaxpages;
	u32 codesize;
} exheader_codesegmentinfo;

typedef struct
{
	u8 name[8];
	exheader_systeminfoflags flags;
	exheader_codesegmentinfo text;
	u8 stacksize[4];
	exheader_codesegmentinfo ro;
	u8 reserved[4];
	exheader_codesegmentinfo data;
	u8 bsssize[4];
} exheader_codesetinfo;

typedef struct
{
	u64 programid[0x30];
} exheader_dependencylist;

typedef struct
{
	u8 savedatasize[4];
	u8 reserved[4];
	u8 jumpid[8];
	u8 reserved2[0x30];
} exheader_systeminfo;

typedef struct
{
	u8 extsavedataid[8];
	u8 systemsavedataid[8];
	u8 reserved[8];
	u8 accessinfo[7];
	u8 otherattributes;
} exheader_storageinfo;

typedef struct
{
	u8 programid[8];
	u8 flags[8];
	u16 resourcelimitdescriptor[0x10];
	exheader_storageinfo storageinfo;
	u64 serviceaccesscontrol[0x20];
	u8 reserved[0x1f];
	u8 resourcelimitcategory;
} exheader_arm11systemlocalcaps;

typedef struct
{
	u32 descriptors[28];
	u8 reserved[0x10];
} exheader_arm11kernelcapabilities;

typedef struct
{
	u8 descriptors[15];
	u8 descversion;
} exheader_arm9accesscontrol;

typedef struct
{
	// systemcontrol info {
	//   coreinfo {
	exheader_codesetinfo codesetinfo;
	exheader_dependencylist deplist;
	//   }
	exheader_systeminfo systeminfo;
	// }
	// accesscontrolinfo {
	exheader_arm11systemlocalcaps arm11systemlocalcaps;
	exheader_arm11kernelcapabilities arm11kernelcaps;
	exheader_arm9accesscontrol arm9accesscontrol;
	// }
	struct {
		u8 signature[0x100];
		u8 ncchpubkeymodulus[0x100];
		exheader_arm11systemlocalcaps arm11systemlocalcaps;
		exheader_arm11kernelcapabilities arm11kernelcaps;
		exheader_arm9accesscontrol arm9accesscontrol;
	} accessdesc;
} exheader_header;
