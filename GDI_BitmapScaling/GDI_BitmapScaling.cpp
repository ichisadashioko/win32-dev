// GDI_BitmapScaling.cpp : Defines the entry point for the application.
//
#pragma once
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <CommCtrl.h>
#include <commdlg.h>

// Global Variables:
HINSTANCE hInst;                                    // current instance
WCHAR szTitle[]       = TEXT("GDI_BitmapScaling");  // The title bar text
WCHAR szWindowClass[] = TEXT("GDIBITMAPSCALING");   // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.

  // Initialize global strings
  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  MSG msg;

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style         = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc   = WndProc;
  wcex.cbClsExtra    = 0;
  wcex.cbWndExtra    = 0;
  wcex.hInstance     = hInstance;
  wcex.hIcon         = LoadIcon(hInstance, IDI_APPLICATION);
  wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName  = NULL;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm       = LoadIcon(wcex.hInstance, IDI_APPLICATION);

  return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance;  // Store instance handle in our global variable

  HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd)
  {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

WORD DIBNumColors(LPVOID lpv)
{
  INT bits;
  LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)lpv;
  LPBITMAPCOREHEADER lpbch = (LPBITMAPCOREHEADER)lpv;

  // With the BITMAPINFO format headers, the size of the palette is in biClrUsed, whereas in the BITMAPCORE - style headers, it is dependent on the bits per pixel (= 2 raised to the power of bits/pixel).

  if (lpbih->biSize != sizeof(BITMAPCOREHEADER))
  {
    if (lpbih->biClrUsed != 0)
    {
      return (WORD)lpbih->biClrUsed;
    }

    bits = lpbih->biBitCount;
  }
  else
  {
    bits = lpbch->bcBitCount;
  }

  if (bits > 8)
  {
    return 0;  // since biClrUsed is 0, we don't have an optimal palette
  }
  else
  {
    return (1 << bits);
  }
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DIBInfo(HANDLE hbi, LPBITMAPINFOHEADER lpbih)                *
 *                                                                            *
 *  PURPOSE    : Retrieves the DIB info associated with a CF_DIB              *
 *               format memory block.                                         *
 *                                                                            *
 *  RETURNS    : TRUE  - if successful.                                       *
 *               FALSE - otherwise                                            *
 *                                                                            *
 *****************************************************************************/
BOOL DIBInfo(HANDLE hbi, LPBITMAPINFOHEADER lpbih)
{
  if (hbi)
  {
    *lpbih = *(LPBITMAPINFOHEADER)hbi;

    // fill in the default fields
    if (lpbih->biSize != sizeof(BITMAPCOREHEADER))
    {
      if (lpbih->biSizeImage == 0L)
      {
        // explain the calculation
        // From wikipedia
        // The bits representing the bitmap pixels are packed in rows. The size of each row is rounded up to a multiple of 4 bytes (a 32-bit DWORD) by padding.
        // row_size = floor((bits_per_pixel * image_width + 31) / 32) * 4
        lpbih->biSizeImage = ((((lpbih->biWidth * lpbih->biBitCount) + 31) >> 5) << 2) * lpbih->biHeight;
      }

      if (lpbih->biClrUsed == 0L)
      {
        lpbih->biClrUsed = DIBNumColors(lpbih);
      }
    }

    return TRUE;
  }

  return FALSE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :OpenDIB(LPSTR szFilename)                                     *
 *                                                                            *
 *  PURPOSE    :Open a DIB file and create a memory DIB -- a memory handle    *
 *              containing BITMAPINFO, palette data and the bits.             *
 *                                                                            *
 *  RETURNS    :A handle to the DIB.                                          *
 *                                                                            *
 *****************************************************************************/
HANDLE OpenDIB(LPSTR szFilename)
{
  HFILE hFile;
  BITMAPINFOHEADER bih;
  LPBITMAPINFOHEADER lpbih;
  DWORD dwLen = 0;
  DWORD dwBits;
  HANDLE hDIB;
  HANDLE hMem;
  OFSTRUCT of;

  // Open the file and read DIB information
  hFile = OpenFile(szFilename, &of, (UINT)OF_READ);
  if (hFile == HFILE_ERROR)
  {
    return NULL;
  }

  DIBInfo(hDIB, &bih);
  // TODO - WIP
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int wmId, wmEvent;
  PAINTSTRUCT ps;
  // Handle to GDI device context
  HDC hDC;
  // Handle to DDB (device-dependent bitmap)
  HBITMAP hBitmap;

  HFONT hFont;
  NONCLIENTMETRICS ncm = {0};
  ncm.cbSize           = sizeof(NONCLIENTMETRICS);

  static HWND hwndButton;
  static HWND hwndButtonExit;
  static HANDLE hDIB = NULL;

  char szDirName[MAX_PATH];
  char szFilename[MAX_PATH]   = "\0";
  char szBitmapName[MAX_PATH] = "\\Waterfall.bmp";
  // char szBitmapName[MAX_PATH] = "\\tulips256.bmp";
  OPENFILENAMEA ofn;

  switch (message)
  {
    case WM_CREATE:
    {
      // Creates a font from the current theme's caption font
      SystemParametersInfo(SPI_GETNONCLIENTMETRICS, NULL, &ncm, NULL);
      hFont = CreateFontIndirect(&ncm.lfCaptionFont);

      // Gets the device context for the current window
      hDC = GetDC(hWnd);

      // Gets the directory of the current project and loads Waterfall.bmp
      GetCurrentDirectoryA(MAX_PATH, szDirName);
      strcat_s(szDirName, szBitmapName);
      strcat_s(szFilename, szDirName);
      hDIB = OpenDIB(szFilename);
    }
    case WM_COMMAND:
    {
      wmId = LOWORD(wParam);
      // Parse the menu selections:
      switch (wmId)
      {
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
      }
    }
    break;
    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      // TODO: Add any drawing code that uses hdc here...
      EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}
