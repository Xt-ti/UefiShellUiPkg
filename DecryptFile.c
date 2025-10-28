#include "Uihead.h"

STATIC
CHAR16*
AsciiToUnicode(IN CHAR8 *Ascii)
{
    if (Ascii == NULL) return NULL;
    UINTN Len = AsciiStrLen(Ascii);
    CHAR16 *Unicode = AllocateZeroPool((Len + 1) * sizeof(CHAR16));
    if (Unicode == NULL) return NULL;
    AsciiStrToUnicodeStrS(Ascii, Unicode, Len + 1);
    return Unicode;
}

STATIC
VOID
TrimSpaces(CHAR8 *Str)
{
    CHAR8 *p = Str;
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
            CopyMem(p, p + 1, AsciiStrLen(p));  // 删除空白
        else
            p++;
    }
}

EFI_STATUS
EFIAPI
ParseLogFile (
  IN CHAR16 *FilePath,
  OUT LOG_ITEM *Items,
  OUT UINTN *ItemCount
  )
{
    EFI_STATUS Status;
    SHELL_FILE_HANDLE FileHandle;
    UINTN FileSize;
    CHAR8 *Buffer;

    *ItemCount = 0;

    Status = ShellOpenFileByName(FilePath, &FileHandle, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
        Print(L"Open file failed: %r\n", Status);
        return Status;
    }

    Status = ShellGetFileSize(FileHandle, &FileSize);
    if (EFI_ERROR(Status)) {
        Print(L"Get size failed: %r\n", Status);
        ShellCloseFile(&FileHandle);
        return Status;
    }

    Buffer = AllocateZeroPool(FileSize + 1);
    if (!Buffer) {
        ShellCloseFile(&FileHandle);
        return EFI_OUT_OF_RESOURCES;
    }

    Status = ShellReadFile(FileHandle, &FileSize, Buffer);
    ShellCloseFile(&FileHandle);
    if (EFI_ERROR(Status)) {
        Print(L"Read file failed: %r\n", Status);
        FreePool(Buffer);
        return Status;
    }

    // 开始解析
    CHAR8 *Line = Buffer;
    for (UINTN i = 0; i < FileSize && *ItemCount < MAX_ROWS; i++) {
        if (Buffer[i] == '\n' || Buffer[i] == '\r') {
            Buffer[i] = '\0';
            if (*Line) {
                LOG_ITEM *Item = &Items[*ItemCount];
                TrimSpaces(Line);

                CHAR8 *Token = AsciiStrStr(Line, ":");
                if (Token) {
                    Token += 1;
                    // 提取每个 key=value
                    CHAR8 *KeyVal;
                    while ((KeyVal = AsciiStrStr(Token, "=")) != NULL) {
                        CHAR8 Key[32] = {0}, Value[64] = {0};
                        CHAR8 *Start = Token;
                        while (*Start == ' ' || *Start == ',') Start++;

                        CHAR8 *Eq = AsciiStrStr(Start, "=");
                        if (!Eq) break;
                        UINTN KeyLen = Eq - Start;
                        CopyMem(Key, Start, KeyLen);
                        Key[KeyLen] = '\0';

                        CHAR8 *ValStart = Eq + 1;
                        CHAR8 *Comma = AsciiStrStr(ValStart, ",");
                        UINTN ValLen = Comma ? (Comma - ValStart) : AsciiStrLen(ValStart);
                        CopyMem(Value, ValStart, ValLen);
                        Value[ValLen] = '\0';

                        // 去空格
                        TrimSpaces(Key);
                        TrimSpaces(Value);

                        // 存入结构体
                        CHAR16 *UniVal = AsciiToUnicode(Value);
                        if (AsciiStrCmp(Key, "SLOT_ID") == 0) StrCpyS(Item->SlotId, 8, UniVal);
                        else if (AsciiStrCmp(Key, "BDF") == 0) StrCpyS(Item->Bdf, 32, UniVal);
                        else if (AsciiStrCmp(Key, "PRESENT") == 0) StrCpyS(Item->Present, 8, UniVal);
                        else if (AsciiStrCmp(Key, "MAC") == 0) StrCpyS(Item->Mac, 32, UniVal);
                        else if (AsciiStrCmp(Key, "RESULT") == 0) StrCpyS(Item->Result, 8, UniVal);

                        if (Comma)
                            Token = Comma + 1;
                        else
                            break;
                    }
                    (*ItemCount)++;
                }
            }
            Line = &Buffer[i + 1];
        }
    }

    FreePool(Buffer);
    return EFI_SUCCESS;
}