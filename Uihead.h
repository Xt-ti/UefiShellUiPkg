#ifndef _GRAPHIC_TOP_H_
#define _GRAPHIC_TOP_H_
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/ShellLib.h>
#include <Protocol/SimpleTextInEx.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/PrintLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/ShellParameters.h>

typedef struct {
    CHAR16 SlotId[8];
    CHAR16 Bdf[32];
    CHAR16 Present[8];
    CHAR16 Mac[32];
    CHAR16 Result[8];
} LOG_ITEM;

#define MAX_ROWS     50   // 表格容纳最大行数
#define PAGE_ROWS    6    // 每页可见行数（可按 FONT_SCALE 调整）
#define COL_COUNT    5    // 列数成员
#define FONT_SCALE   2  // 放大倍数（每个点阵像素映射为 Scale×Scale 的方块）
#define CHAR_W       8
#define CHAR_H       8
#define CELL_PAD_X   8
#define CELL_PAD_Y   8

typedef struct {
  CHAR16 *Cols[COL_COUNT];
} TABLE_ROW;

EFI_STATUS
EFIAPI
ParseLogFile (
  IN CHAR16 *FilePath,
  OUT LOG_ITEM *Items,
  OUT UINTN *ItemCount
  );

#endif