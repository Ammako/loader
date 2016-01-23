#include <3ds.h>
#include "fsldr.h"

#define SDK_VERSION 0xA0000C8

static Handle fsldrHandle;
static int fsldrRefCount;

Result fsldrInit(void)
{
  Result ret = 0;

  if (AtomicPostIncrement(&fsldrRefCount)) return 0;

  ret = srvGetServiceHandle(&fsldrHandle, "fs:LDR");
  if (R_SUCCEEDED(ret))
  {
    ret = FSLDR_InitializeWithSdkVersion(fsldrHandle, SDK_VERSION);
    if (R_SUCCEEDED(ret)) ret = FSLDR_SetPriority(0);
    if (R_FAILED(ret)) svcCloseHandle(fsldrHandle);
  }

  if (R_FAILED(ret)) AtomicDecrement(&fsldrRefCount);
  return ret;
}

void fsldrExit(void)
{
  if (AtomicDecrement(&fsldrRefCount)) return;
  svcCloseHandle(fsldrHandle);
}

Result FSLDR_InitializeWithSdkVersion(Handle session, u32 version)
{
  u32 *cmdbuf = getThreadCommandBuffer();

  cmdbuf[0] = IPC_MakeHeader(0x861,1,2); // 0x8610042
  cmdbuf[1] = version;
  cmdbuf[2] = 32;

  Result ret = 0;
  if(R_FAILED(ret = svcSendSyncRequest(session))) return ret;

  return cmdbuf[1];
}

Result FSLDR_SetPriority(u32 priority)
{
  u32 *cmdbuf = getThreadCommandBuffer();

  cmdbuf[0] = IPC_MakeHeader(0x862,1,0); // 0x8620040
  cmdbuf[1] = priority;

  Result ret = 0;
  if(R_FAILED(ret = svcSendSyncRequest(fsldrHandle))) return ret;

  return cmdbuf[1];
}

Result FSLDR_OpenFileDirectly(Handle* out, FS_Archive archive, FS_Path path, u32 openFlags, u32 attributes)
{
  u32 *cmdbuf = getThreadCommandBuffer();

  cmdbuf[0] = IPC_MakeHeader(0x803,8,4); // 0x8030204
  cmdbuf[1] = 0;
  cmdbuf[2] = archive.id;
  cmdbuf[3] = archive.lowPath.type;
  cmdbuf[4] = archive.lowPath.size;
  cmdbuf[5] = path.type;
  cmdbuf[6] = path.size;
  cmdbuf[7] = openFlags;
  cmdbuf[8] = attributes;
  cmdbuf[9] = IPC_Desc_StaticBuffer(archive.lowPath.size, 2);
  cmdbuf[10] = (u32) archive.lowPath.data;
  cmdbuf[11] = IPC_Desc_StaticBuffer(path.size, 0);
  cmdbuf[12] = (u32) path.data;

  Result ret = 0;
  if(R_FAILED(ret = svcSendSyncRequest(fsldrHandle))) return ret;

  if(out) *out = cmdbuf[3];

  return cmdbuf[1];
}