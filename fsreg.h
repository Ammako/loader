#pragma once

#include <3ds/types.h>

Result fsregInit(void);
void fsregExit(void);
Result FSREG_CheckHostLoadId(u64 *prog_handle);
Result FSREG_LoadProgram(u64 *prog_handle, FS_ProgramInfo *title);
Result FSREG_GetProgramInfo(exheader_header *exheader, u32 entry_count, u64 prog_handle);
Result FSREG_UnloadProgram(u64 prog_handle);
