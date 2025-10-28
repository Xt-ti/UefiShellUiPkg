#include "Uihead.h"
// Table data
STATIC TABLE_ROW Table[MAX_ROWS];
// items count
STATIC UINTN RowCount = 0;
// --- 简单 8x8 字模 (ASCII) ---
// 仅包含常用字符：space(32), '0'-'9', 'A'-'Z', ':' 和 '-' 等。
// 每行 8 位，从高位开始代表左侧像素。
// 未定义字符将显示为空格。
STATIC CONST UINT8 Font8x8[] = {
  // 32 ' ' (space)
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  // 33 '!' (not used)
  0x18,0x18,0x18,0x18,0x18,0x00,0x18,0x00,
  // 34 '"' (not used)
  0x6C,0x6C,0x24,0x00,0x00,0x00,0x00,0x00,
  // 35 '#'
  0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00,
  // 36 '$'
  0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00,
  // 37 '%'
  0x00,0xC6,0xCC,0x18,0x30,0x66,0xC6,0x00,
  // 38 '&'
  0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00,
  // 39 '''
  0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,
  // 40 '('
  0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00,
  // 41 ')'
  0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00,
  // 42 '*'
  0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,
  // 43 '+'
  0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,
  // 44 ','
  0x00,0x00,0x00,0x00,0x30,0x30,0x60,0x00,
  // 45 '-'
  0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,
  // 46 '.'
  0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,
  // 47 '/'
  0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00,
  // 48 '0'
  0x3C,0x66,0x6E,0x7E,0x76,0x66,0x3C,0x00,
  // 49 '1'
  0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00,
  // 50 '2'
  0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00,
  // 51 '3'
  0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00,
  // 52 '4'
  0x0C,0x1C,0x3C,0x6C,0xFE,0x0C,0x1E,0x00,
  // 53 '5'
  0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00,
  // 54 '6'
  0x3C,0x60,0x7C,0x66,0x66,0x66,0x3C,0x00,
  // 55 '7'
  0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0x00,
  // 56 '8'
  0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00,
  // 57 '9'
  0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00,
  // 58 ':'
  0x00,0x30,0x30,0x00,0x30,0x30,0x00,0x00,
  // 59 ';'
  0x00,0x30,0x30,0x00,0x30,0x30,0x60,0x00,
  // 60 '<'
  0x06,0x0C,0x18,0x30,0x18,0x0C,0x06,0x00,
  // 61 '='
  0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00,
  // 62 '>'
  0x60,0x30,0x18,0x0C,0x18,0x30,0x60,0x00,
  // 63 '?'
  0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00,
  // 64 '@'
  0x3C,0x66,0x6E,0x6E,0x6E,0x60,0x3E,0x00,
  // 65 'A'
  0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00,
  // 66 'B'
  0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00,
  // 67 'C'
  0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00,
  // 68 'D'
  0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00,
  // 69 'E'
  0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00,
  // 70 'F'
  0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00,
  // 71 'G'
  0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00,
  // 72 'H'
  0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00,
  // 73 'I'
  0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,
  // 74 'J'
  0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00,
  // 75 'K'
  0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00,
  // 76 'L'
  0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00,
  // 77 'M'
  0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00,
  // 78 'N'
  0x66,0x76,0x7E,0x6E,0x66,0x66,0x66,0x00,
  // 79 'O'
  0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00,
  // 80 'P'
  0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00,
  // 81 'Q'
  0x3C,0x66,0x66,0x66,0x6E,0x3C,0x0E,0x00,
  // 82 'R'
  0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00,
  // 83 'S'
  0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00,
  // 84 'T'
  0x7E,0x5A,0x18,0x18,0x18,0x18,0x3C,0x00,
  // 85 'U'
  0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00,
  // 86 'V'
  0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00,
  // 87 'W'
  0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00,
  // 88 'X'
  0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00,
  // 89 'Y'
  0x66,0x66,0x66,0x3C,0x18,0x18,0x3C,0x00,
  // 90 'Z'
  0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00,
  // 91 '['
  0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,
  // 92 '\'
  0x00,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,
  // 93 ']'
  0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,
  // 94 '^'
  0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,
  // 95 '_'
  0x00,0x00,0x00,0x00,0x00,0x18,0xFF,0x00,
};
// Font array length corresponds to ASCII 32..95 inclusive
#define FONT_FIRST_CH 32
#define FONT_LAST_CH  95
#define FONT_CHAR_COUNT (FONT_LAST_CH - FONT_FIRST_CH + 1)

// --- Draw a pixel to framebuffer ---
STATIC VOID DrawPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINTN X, UINTN Y, UINT32 Color)
{
  if (X >= Gop->Mode->Info->HorizontalResolution || Y >= Gop->Mode->Info->VerticalResolution) return;
  UINT8 *Fb = (UINT8*)(UINTN)Gop->Mode->FrameBufferBase;
  UINTN PixelSize = 4; // we assume 32-bit BGRA
  UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine * PixelSize;
  UINT8 *P = Fb + Y * Pitch + X * PixelSize;
  // color is 0x00RRGGBB we map to B G R order in memory (common)
  P[0] = (UINT8)(Color & 0xFF);        // Blue
  P[1] = (UINT8)((Color >> 8) & 0xFF); // Green
  P[2] = (UINT8)((Color >> 16) & 0xFF);// Red
  P[3] = 0x00; // Reserved/alpha (ignored)
}

// --- Draw one character using the 8x8 bitmap, scaled by 'scale' ---
STATIC VOID DrawCharScaled(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, CHAR16 Ch, INTN X, INTN Y, UINTN Scale, UINT32 Color)
{
  UINT8 const *bitmap;
  INTN idx = (INTN)Ch - FONT_FIRST_CH;
  if (idx < 0 || idx >= FONT_CHAR_COUNT) {
    // unsupported -> use space
    bitmap = &Font8x8[0];
  } else {
    bitmap = &Font8x8[idx * 8];
  }

  for (UINTN row = 0; row < CHAR_H; row++) {
    UINT8 rowbits = bitmap[row];
    for (UINTN col = 0; col < CHAR_W; col++) {
      if (rowbits & (1 << (7 - col))) {
        // draw a Scale x Scale block
        for (UINTN sy = 0; sy < Scale; sy++) {
          for (UINTN sx = 0; sx < Scale; sx++) {
            DrawPixel(Gop, X + col * Scale + sx, Y + row * Scale + sy, Color);
          }
        }
      }
    }
  }
}

// --- Draw a string (we convert to uppercase to match font) ---
STATIC VOID DrawStringScaled(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, CHAR16 *Str, INTN X, INTN Y, UINTN Scale, UINT32 Color)
{
  INTN curX = X;
  while (*Str) {
    CHAR16 ch = *Str++;
    // map lowercase to uppercase to simplify font usage
    if (ch >= L'a' && ch <= L'z') ch = (CHAR16)(ch - L'a' + L'A');
    DrawCharScaled(Gop, ch, curX, Y, Scale, Color);
    curX += (CHAR_W + 1) * Scale; // char width + 1 pixel spacing
  }
}

// --- Fill rect with color ---
STATIC VOID FillRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, INTN W, INTN H, UINT32 Color)
{
  for (INTN yy = 0; yy < H; yy++) {
    for (INTN xx = 0; xx < W; xx++) {
      DrawPixel(Gop, X + xx, Y + yy, Color);
    }
  }
}

// --- Save / Restore full framebuffer ---
// Note: for simplicity we save the full framebuffer.
// Could be optimized to save only window region.
STATIC VOID *SavedFrameBuffer = NULL;
STATIC UINTN SavedFbSize = 0;

STATIC
EFI_STATUS
EFIAPI
SaveFullFrame (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
  )
{
  SavedFbSize = Gop->Mode->FrameBufferSize;
  SavedFrameBuffer = AllocatePool(SavedFbSize);
  if (SavedFrameBuffer == NULL) return EFI_OUT_OF_RESOURCES;
  CopyMem(SavedFrameBuffer, (VOID*)(UINTN)Gop->Mode->FrameBufferBase, SavedFbSize);
  return EFI_SUCCESS;
}

STATIC VOID RestoreFullFrame(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop)
{
  if (SavedFrameBuffer != NULL) {
    CopyMem((VOID*)(UINTN)Gop->Mode->FrameBufferBase, SavedFrameBuffer, SavedFbSize);
    FreePool(SavedFrameBuffer);
    SavedFrameBuffer = NULL;
    SavedFbSize = 0;
  }
}

// --- Draw table window (header + visible rows) ---
STATIC VOID DrawTableWindow(
  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
  INTN WinX, INTN WinY, INTN WinW, INTN WinH,
  UINTN startRow, UINTN fontScale,
  UINTN visibleRows
  )
{
  // colors: use 0x00RRGGBB
  UINT32 bgColor = 0x00FFFFFF; // white
  UINT32 titleColor = 0x00CCCCCC;
  UINT32 borderColor = 0x00000000;
  UINT32 textColor = 0x00000000;

  // background
  FillRect(Gop, WinX, WinY, WinW, WinH, bgColor);
  // title bar
  FillRect(Gop, WinX, WinY, WinW, 24 * fontScale, titleColor);
  // border
  for (INTN i = 0; i < 2; i++) {
    // top
    FillRect(Gop, WinX + i, WinY + i, WinW - 2*i, 1, borderColor);
    // bottom
    FillRect(Gop, WinX + i, WinY + WinH - 1 - i, WinW - 2*i, 1, borderColor);
    // left
    FillRect(Gop, WinX + i, WinY + i, 1, WinH - 2*i, borderColor);
    // right
    FillRect(Gop, WinX + WinW - 1 - i, WinY + i, 1, WinH - 2*i, borderColor);
  }




  
  // header text
  DrawStringScaled(Gop, L"Slot", WinX + CELL_PAD_X, WinY + CELL_PAD_Y/2, fontScale, textColor);
  DrawStringScaled(Gop, L"BDF", WinX + 7*(WinW/44) + CELL_PAD_X, WinY + CELL_PAD_Y/2, fontScale, textColor);
  DrawStringScaled(Gop, L"Present", WinX + 14*(WinW/44) + CELL_PAD_X, WinY + CELL_PAD_Y/2, fontScale, textColor);
  DrawStringScaled(Gop, L"MAC", WinX + 28*(WinW/44) + CELL_PAD_X, WinY + CELL_PAD_Y/2, fontScale, textColor);
  DrawStringScaled(Gop, L"Result", WinX + 38*(WinW/44) + CELL_PAD_X, WinY + CELL_PAD_Y/2, fontScale, textColor);

  // rows area origin
  INTN rowStartY = WinY + 24 * fontScale + CELL_PAD_Y;
  INTN rowHeight = (CHAR_H * fontScale) + CELL_PAD_Y;

  // draw rows
  for (UINTN r = 0; r < visibleRows; r++) {
    UINTN idx = startRow + r;
    if (idx >= RowCount) break;
    INTN y = rowStartY + r * rowHeight;
    // optional alternate row background
    if ((r & 1) == 0) {
      FillRect(Gop, WinX + 1, y, WinW - 2, rowHeight, 0x00F8F8F8);
    }
    // draw six columns
    DrawStringScaled(Gop, Table[idx].Cols[0], WinX + CELL_PAD_X, y, fontScale, textColor);
    DrawStringScaled(Gop, Table[idx].Cols[1], WinX + 5*(WinW/44) + CELL_PAD_X, y, fontScale, textColor);
    DrawStringScaled(Gop, Table[idx].Cols[2], WinX + 16*(WinW/44) + CELL_PAD_X, y, fontScale, textColor);
    DrawStringScaled(Gop, Table[idx].Cols[3], WinX + 21*(WinW/44) + CELL_PAD_X, y, fontScale, textColor);
    DrawStringScaled(Gop, Table[idx].Cols[4], WinX + 39*(WinW/44) + CELL_PAD_X, y, fontScale, textColor);
  }

  // footer hints
  DrawStringScaled(Gop, L"[UP/DOWN] Scroll  [ESC] Exit", WinX + CELL_PAD_X, WinY + WinH - (CHAR_H + CELL_PAD_Y) * fontScale, fontScale, 0x0000AA00);
}



/**
 * @brief Shell EntryPoint.
 * 
 * @param ImageHandle
 * @param SystemTable
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
UiMainEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_SHELL_PARAMETERS_PROTOCOL  *ShellParameters;
  EFI_GRAPHICS_OUTPUT_PROTOCOL   *Gop;
  EFI_STATUS                      Status;
  CHAR16                        **Argv;
  UINTN                           Argc;
  LOG_ITEM                        Items[MAX_ROWS];

  Status = EFI_SUCCESS;
  Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
  if (EFI_ERROR(Status)) {
    Print(L"GOP not present: %r\n", Status);
    return EFI_UNSUPPORTED;
  }

  // load shell protocol
  Status = gBS->OpenProtocol(
                            ImageHandle,
                            &gEfiShellParametersProtocolGuid,
                            (VOID **)&ShellParameters,
                            ImageHandle,
                            NULL,
                            EFI_OPEN_PROTOCOL_GET_PROTOCOL
                            );
  if (EFI_ERROR(Status)) {
    Print(L"Shell Protocol not present: %r\n", Status);
    return EFI_UNSUPPORTED;
  }

  if (ShellParameters->Argc < 1) {
    Print(L"No arguments provided. Use --help for usage information.\n");
    gBS->CloseProtocol(
                      ImageHandle,
                      &gEfiShellParametersProtocolGuid,
                      ImageHandle,
                      NULL
                      );
    Status = EFI_INVALID_PARAMETER;
    goto invalid;
  }
  Argv = ShellParameters->Argv;
  Argc = ShellParameters->Argc;
  if (Argc <= 1) {
    Print(L"Usage: NicGraphicsPkg.efi fs0:\\log.txt\n");
    return EFI_INVALID_PARAMETER;
  }

  Status = ParseLogFile (Argv[1], Items, &RowCount);
  if (!EFI_ERROR(Status)) {
    Print(L"Parsed %u items\n", RowCount);
    for (UINTN i = 0; i < RowCount; i++) {
        Print(L"Item%u: Slot=%s, BDF=%s, Present=%s, MAC=%s, Result=%s\n",
              i,
              Items[i].SlotId,
              Items[i].Bdf,
              Items[i].Present,
              Items[i].Mac,
              Items[i].Result);
    }
}

  // prepare data
  for (UINTN i = 0; i < RowCount; i++) {
    for (UINTN c = 0; c < COL_COUNT; c++) {
      CHAR16 buf[128];
      if (c == 0) UnicodeSPrint(buf, sizeof(buf), Items[i].SlotId);
      if (c == 1) UnicodeSPrint(buf, sizeof(buf), Items[i].Bdf);
      if (c == 2) UnicodeSPrint(buf, sizeof(buf), Items[i].Present);
      if (c == 3) UnicodeSPrint(buf, sizeof(buf), Items[i].Mac);
      if (c == 4) UnicodeSPrint(buf, sizeof(buf), Items[i].Result);
      Table[i].Cols[c] = AllocateCopyPool((StrLen(buf)+1) * sizeof(CHAR16), buf);
    }
  }

  // compute window geometry
  UINTN scrW = Gop->Mode->Info->HorizontalResolution;
  UINTN scrH = Gop->Mode->Info->VerticalResolution;
  // 90% * 70%
  INTN WinW = (scrW * 100) / 100;
  INTN WinH = (scrH * 70) / 100;
  // 水平居中
  INTN WinX = (scrW - WinW) / 2;
  INTN WinY = (scrH - WinH) / 2;
  UINTN fontScale = FONT_SCALE;
  UINTN visibleRows = PAGE_ROWS;

  // save full framebuffer
  Status = SaveFullFrame(Gop);
  if (EFI_ERROR(Status)) {
    Print(L"SaveFullFrame failed: %r\n", Status);
    goto cleanup;
  }

  // main loop: draw window, handle keys
  UINTN startRow = 0;
  BOOLEAN exit = FALSE;
  while (!exit) {
    // clear window area to background (we restore only window region per frame for simplicity)
    FillRect(Gop, WinX, WinY, WinW, WinH, 0x00FFFFFF);
    DrawTableWindow(Gop, WinX, WinY, WinW, WinH, startRow, fontScale, visibleRows);

    // wait for key
    UINTN idx;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &idx);
    EFI_INPUT_KEY Key;
    Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
    if (EFI_ERROR(Status)) continue;

    if (Key.ScanCode == SCAN_UP) {
      if (startRow > 0) startRow--;
    } else if (Key.ScanCode == SCAN_DOWN) {
      if (startRow + visibleRows < RowCount) startRow++;
    } else if (Key.ScanCode == SCAN_ESC) {
      exit = TRUE;
    } else if (Key.UnicodeChar == L'+' ) {
      // enlarge font
      fontScale++;
      if (fontScale > 6) fontScale = 6;
    } else if (Key.UnicodeChar == L'-') {
      if (fontScale > 1) fontScale--;
    }
  }

  // restore saved framebuffer (returns to shell view)
  RestoreFullFrame(Gop);

cleanup:
  // free table strings
  for (UINTN i = 0; i < RowCount; i++) {
    for (UINTN c = 0; c < COL_COUNT; c++) {
      if (Table[i].Cols[c]) {
        FreePool(Table[i].Cols[c]);
        Table[i].Cols[c] = NULL;
      }
    }
  }
invalid:
  return Status;
}
