// GDI_CapturingAnImage.cpp : Defines the entry point for the application.
//

#include <Windows.h>
#include <tchar.h>

#include <stdio.h>
#include <stdlib.h>

// Global Variables:
HINSTANCE hInst;                                              // current instance
static TCHAR szTitle[]       = TEXT("GDI_CAPTURE_AN_IMAGE");  // The title bar text
static TCHAR szWindowClass[] = TEXT("GDI_CAPTURE_AN_IMAGE");  // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int SaveBitmapImageToDisk(LPCWSTR filepath, BITMAPFILEHEADER bmfHeader, BITMAPINFOHEADER bi, char* lpbitmap, DWORD bmpSize)
{
  int retval = 0;

  HANDLE fileHandle = CreateFileW(filepath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  if (fileHandle == NULL)
  {
    fprintf(stderr, "CreateFileW failed at %s:%d\n", __FILE__, __LINE__);
    fwprintf(stderr, L"Failed to create file at %s\n", filepath);
    retval = -1;
  }
  else
  {
    // TODO convert bitmap to bytes
    DWORD bytesWritten = 0;
    WriteFile(fileHandle, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &bytesWritten, NULL);
    WriteFile(fileHandle, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &bytesWritten, NULL);
    WriteFile(fileHandle, (LPSTR)lpbitmap, bmpSize, &bytesWritten, NULL);

    printf("%d bytes written\n", bytesWritten);

    CloseHandle(fileHandle);
  }

  return retval;
}

int APIENTRY CreateAndStartWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  MSG msg;

  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  // Main message loop:
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style         = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc   = WndProc;
  wcex.cbClsExtra    = 0;
  wcex.cbWndExtra    = 0;
  wcex.hInstance     = hInstance;
  wcex.hIcon         = LoadIcon(hInstance, IDI_APPLICATION);
  wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName  = NULL;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm       = LoadIcon(wcex.hInstance, IDI_APPLICATION);

  return RegisterClassEx(&wcex);
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
  HWND hWnd;

  hInst = hInstance;  // Store instance handle in our global variable

  hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

  if (!hWnd)
  {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

//
//   FUNCTION: CaptureAnImage(HWND hWnd)
//
//   PURPOSE: Captures a screenshot into a window and then saves it in a .bmp file.
//
//   COMMENTS:
//
//      Note: This sample will attempt to create a file called captureqwsx.bmp
//

int CaptureAnImage(HWND hWnd)
{
  HDC hdcScreen;
  HDC hdcWindow;
  HDC hdcMemDC      = NULL;
  HBITMAP hbmScreen = NULL;
  BITMAP bmpScreen;

  // Retrieve the handle to a display device context for the client
  // area of the window.
  hdcScreen = GetDC(NULL);
  hdcWindow = GetDC(hWnd);

  // Create a compatible DC which is used in a BitBlt from the window DC
  hdcMemDC = CreateCompatibleDC(hdcWindow);

  if (!hdcMemDC)
  {
    MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
    // goto done;
    exit(-1);
  }

  // Get the client area for size calculation
  RECT rcClient;
  GetClientRect(hWnd, &rcClient);

  // This is the best stretch mode
  SetStretchBltMode(hdcWindow, HALFTONE);

  // The source DC is the entire screen and the destination DC is the current window (HWND)
  if (!StretchBlt(hdcWindow, 0, 0, rcClient.right, rcClient.bottom, hdcScreen, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SRCCOPY))
  {
    MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
    // goto done;
    exit(-1);
  }

  // Create a compatible bitmap from the Window DC
  hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

  if (!hbmScreen)
  {
    MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
    // goto done;
    exit(-1);
  }

  // Select the compatible bitmap into the compatible memory DC.
  SelectObject(hdcMemDC, hbmScreen);

  // Bit block transfer into our compatible memory DC.
  if (!BitBlt(hdcMemDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hdcWindow, 0, 0, SRCCOPY))
  {
    MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
    // goto done;
    exit(-1);
  }

  // Get the BITMAP from the HBITMAP
  GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

  BITMAPFILEHEADER bmfHeader;
  BITMAPINFOHEADER bi;

  bi.biSize          = sizeof(BITMAPINFOHEADER);
  bi.biWidth         = bmpScreen.bmWidth;
  bi.biHeight        = bmpScreen.bmHeight;
  bi.biPlanes        = 1;
  bi.biBitCount      = 32;
  bi.biCompression   = BI_RGB;
  bi.biSizeImage     = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed       = 0;
  bi.biClrImportant  = 0;

  DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

  // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that
  // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc
  // have greater overhead than HeapAlloc.
  HANDLE hDIB    = GlobalAlloc(GHND, dwBmpSize);
  char* lpbitmap = (char*)GlobalLock(hDIB);

  // Gets the "bits" from the bitmap and copies them into a buffer
  // which is pointed to by lpbitmap.
  GetDIBits(hdcWindow, hbmScreen, 0, (UINT)bmpScreen.bmHeight, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

  // Add the size of the headers to the size of the bitmap to get the total file size
  DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

  // Offset to where the actual bitmap bits start.
  bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

  // Size of the file
  bmfHeader.bfSize = dwSizeofDIB;

  // bfType must always be BM for Bitmaps
  bmfHeader.bfType = 0x4D42;  // BM

  SaveBitmapImageToDisk(L"captureqwsx.bmp", bmfHeader, bi, lpbitmap, dwBmpSize);

  // Unlock and Free the DIB from the heap
  GlobalUnlock(hDIB);
  GlobalFree(hDIB);

  // Clean up
  // done:
  DeleteObject(hbmScreen);
  DeleteObject(hdcMemDC);
  ReleaseDC(NULL, hdcScreen);
  ReleaseDC(hWnd, hdcWindow);

  return 0;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND    - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY    - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message)
  {
    case WM_CREATE:
    {
      break;
    }
    case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      CaptureAnImage(hWnd);
      EndPaint(hWnd, &ps);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

int main()
{
  return CreateAndStartWindow(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
}
