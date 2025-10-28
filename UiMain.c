#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>


#define MAX_LOG_LINES  40
#define PAGE_LINES     8

typedef struct {
  CHAR16 *Text;
  BOOLEAN Highlight;
} LOG_ITEM;

LOG_ITEM *Logs;
UINTN LogCount = 0;


/* 绘制一个简易边框（使用 Box Drawing 字符） */
VOID DrawFrame() 
{
  gST->ConOut->ClearScreen(gST->ConOut);
  Print(L"╔══════════════════════════════════════════════════════╗\n");
  for (UINTN i = 0; i < PAGE_LINES + 2; i++) {
    Print(L"║                                                      ║\n");
  }
  Print(L"╚══════════════════════════════════════════════════════╝\n");
  gST->ConOut->SetCursorPosition(gST->ConOut, 2, 1);
  Print(L"UEFI Log Viewer (Press ↑ ↓ to scroll, Q to exit)");
}


/* 初始化一些示例日志 */
VOID InitSampleLogs() 
{
  Logs = AllocatePool(MAX_LOG_LINES * sizeof(LOG_ITEM));
  for (UINTN i = 0; i < MAX_LOG_LINES; i++) {
    CHAR16 Buf[64];
    UnicodeSPrint(Buf, sizeof(Buf), L"Log line #%02d", i + 1);
    Logs[i].Text = AllocateCopyPool(StrSize(Buf), Buf);
    Logs[i].Highlight = (i % 5 == 0); // 每 5 行高亮
  }
  LogCount = MAX_LOG_LINES;
}


/* 显示一页日志内容 */

VOID DisplayPage (UINTN StartIndex) 
{
  UINTN EndIndex = StartIndex + PAGE_LINES;
  if (EndIndex > LogCount) EndIndex = LogCount;

  for (UINTN line = 0; line < PAGE_LINES; line++) {
    UINTN Row = 3 + line;
    gST->ConOut->SetCursorPosition(gST->ConOut, 3, Row);

    if (StartIndex + line < EndIndex) {
      if (Logs[StartIndex + line].Highlight)
        gST->ConOut->SetAttribute(gST->ConOut, EFI_YELLOW | EFI_BACKGROUND_BLACK);
      else
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);

      Print(L"%s", Logs[StartIndex + line].Text);
    } else {
      Print(L"                                                          ");
    }
  }

  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
}


/* 主函数 */
EFI_STATUS
EFIAPI
UiMainEntryPoint (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  ) 
{
  EFI_INPUT_KEY Key;
  EFI_STATUS Status;
  UINTN StartIndex = 0;

  InitSampleLogs();
  DrawFrame();
  DisplayPage(StartIndex);

  while (TRUE) {
    Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
    if (EFI_ERROR(Status)) continue;

    if (Key.UnicodeChar == L'q' || Key.UnicodeChar == L'Q') {
      break;
    }

    if (Key.ScanCode == SCAN_DOWN && StartIndex + PAGE_LINES < LogCount) {
      StartIndex++;
      DisplayPage(StartIndex);
    } else if (Key.ScanCode == SCAN_UP && StartIndex > 0) {
      StartIndex--;
      DisplayPage(StartIndex);
    }
  }

  for (UINTN i = 0; i < LogCount; i++)
    if (Logs[i].Text) FreePool(Logs[i].Text);
  FreePool(Logs);

  gST->ConOut->ClearScreen(gST->ConOut);
  Print(L"Exit Log Viewer.\n");
  return EFI_SUCCESS;
}
