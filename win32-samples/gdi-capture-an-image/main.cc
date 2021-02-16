#pragma once
#include <Windows.h>
#include <tchar.h>
#include <sal.h>

#include <stdio.h>
#include <stdlib.h>

#define VK_S_KEY 0x53

// Global Variables:
HINSTANCE hInst;                                              // current instance
static TCHAR szTitle[]       = TEXT("GDI_CAPTURE_AN_IMAGE");  // The title bar text
static TCHAR szWindowClass[] = TEXT("GDI_CAPTURE_AN_IMAGE");  // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// clang-format off
_Success_(return == 0)
int SaveBitmapImageToDisk(
    _In_ const char* filepath,
    _In_ BITMAPFILEHEADER bmfHeader,
    _In_ BITMAPINFOHEADER bi,
    _In_ char* lpbitmap,
    _In_ DWORD bmpSize)
{
  // clang-format on
  int retval = 0;

#pragma warning(disable : 4996)
  FILE* file_ptr = fopen(filepath, "wb");
  if (file_ptr == NULL)
  {
    fprintf(stderr, "fopen failed at %s:%d\n", __FILE__, __LINE__);
    fprintf(stderr, "failed to create file at %s\n", filepath);
    retval = -1;
  }
  else
  {
    // TODO convert bitmap to raw bytes

    size_t cb_written_total          = 0;
    size_t cb_expected_written_total = 0;
    size_t cb_written                = 0;
    size_t cb_expected_written       = 0;

    cb_expected_written = sizeof(BITMAPFILEHEADER);
    cb_written          = fwrite(&bmfHeader, 1, cb_expected_written, file_ptr);
    cb_expected_written_total += cb_expected_written;
    cb_written_total += cb_written;
    printf("[BITMAPFILEHEADER] wrote %d/%d bytes\n", cb_written, cb_expected_written);

    cb_expected_written = sizeof(BITMAPINFOHEADER);
    cb_written          = fwrite(&bi, 1, cb_expected_written, file_ptr);
    cb_expected_written_total += cb_expected_written;
    cb_written_total += cb_written;
    printf("[BITMAPINFOHEADER] wrote %d/%d bytes\n", cb_written, cb_expected_written);

    cb_expected_written = bmpSize;
    cb_written          = fwrite(lpbitmap, 1, cb_expected_written, file_ptr);
    cb_expected_written_total += cb_expected_written;
    cb_written_total += cb_written;
    printf("[BMP image data] wrote %d/%d bytes\n", cb_written, cb_expected_written);

    printf("wrote total %d/%d bytes\n", cb_written_total, cb_expected_written_total);

    fclose(file_ptr);
  }

  return retval;
}

// clang-format off
_Success_(return == 0)
int PrepareBitmapStructuresForSaving(
    _In_ HDC hdc,
    _In_ HBITMAP hbm,
    _Inout_ BITMAPFILEHEADER* out_bitmap_file_header,
    _Inout_ BITMAPINFOHEADER* out_bitmap_info_header,
    _Inout_ BITMAP* out_bitmap,
    _Outptr_ char** out_bitmap_image_data,
    _Inout_ DWORD* out_bitmap_image_data_cb)
{
  // clang-format on
  int retval = 0;

  BITMAPFILEHEADER bmfHeader;
  BITMAPINFOHEADER bi;
  BITMAP bm;

  GetObject(hbm, sizeof(BITMAP), &bm);

  bi.biSize          = sizeof(BITMAPINFOHEADER);
  bi.biWidth         = bm.bmWidth;
  bi.biHeight        = bm.bmHeight;
  bi.biPlanes        = 1;
  bi.biBitCount      = 32;
  bi.biCompression   = BI_RGB;
  bi.biSizeImage     = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed       = 0;
  bi.biClrImportant  = 0;

  DWORD cb_bitmap_image_data = ((bm.bmWidth * bi.biBitCount + 31) / 32) * 4 * bm.bmHeight;

  char* bitmap_image_data = (char*)malloc(cb_bitmap_image_data);
  if (bitmap_image_data == NULL)
  {
    fprintf(stderr, "malloc failed at %s:%d\n", __FILE__, __LINE__);
    retval = -1;
  }
  else
  {
    // Gets the "bits" from the bitmap and copies them into a buffer
    // which is pointed to by bitmap_image_data.
    GetDIBits(hdc, hbm, 0, (UINT)bm.bmHeight, bitmap_image_data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // Add the size of the headers to the size of the bitmap to get the total file size
    DWORD dwSizeofDIB = cb_bitmap_image_data + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    // Size of the file
    bmfHeader.bfSize = dwSizeofDIB;

    // bfType must always be BM for Bitmaps
    bmfHeader.bfType = 0x4D42;  // BM

    // memcpy(out_bitmap_info_header, (&bi), sizeof(BITMAPINFOHEADER));
    // memcpy(out_bitmap_file_header, (&bmfHeader), sizeof(BITMAPFILEHEADER));
    // memcpy(out_bitmap, (&bm), sizeof(BITMAP));
    (*out_bitmap_info_header) = bi;
    (*out_bitmap_file_header) = bmfHeader;
    (*out_bitmap)             = bm;

    // return the bitmap bytes
    // free(bitmap_image_data);
    (*out_bitmap_image_data)    = bitmap_image_data;
    (*out_bitmap_image_data_cb) = cb_bitmap_image_data;
  }

  return retval;
}

int CaptureAndSaveScreenAsBitmap()
{
  int retval = 0;

  // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdc

  // call GetDC(NULL) to get the device context for the "entire screen"
  HDC hdcScreen = GetDC(NULL);

  // TODO implement the function

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

  // Retrieve the handle to a display device context for the client
  // area of the window.
  hdcScreen = GetDC(NULL);
  hdcWindow = GetDC(hWnd);

  // Create a compatible DC which is used in a BitBlt from the window DC
  hdcMemDC = CreateCompatibleDC(hdcWindow);

  if (!hdcMemDC)
  {
    MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
  }
  else
  {
    // Get the client area for size calculation
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    // This is the best stretch mode
    SetStretchBltMode(hdcWindow, HALFTONE);

    // The source DC is the entire screen and the destination DC is the current window (HWND)
    if (!StretchBlt(hdcWindow, 0, 0, rcClient.right, rcClient.bottom, hdcScreen, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SRCCOPY))
    {
      MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
    }
    else
    {
      // Create a compatible bitmap from the Window DC
      hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

      if (!hbmScreen)
      {
        MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
      }
      else
      {
        // Select the compatible bitmap into the compatible memory DC.
        SelectObject(hdcMemDC, hbmScreen);

        // Bit block transfer into our compatible memory DC.
        if (!BitBlt(hdcMemDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hdcWindow, 0, 0, SRCCOPY))
        {
          MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
        }
        else
        {
          BITMAPINFOHEADER bitmap_info_header;
          BITMAPFILEHEADER bitmap_file_header;
          BITMAP bitmap;
          char* bitmap_image_data = NULL;
          DWORD cb_bitmap_image_data;

          // clang-format off
          int return_code = PrepareBitmapStructuresForSaving(
            hdcWindow,
            hbmScreen,
            &bitmap_file_header,
            &bitmap_info_header,
            &bitmap,
            &bitmap_image_data,
            &cb_bitmap_image_data);
          // clang-format on
          if (return_code == 0)
          {
            SaveBitmapImageToDisk("captureqwsx.bmp", bitmap_file_header, bitmap_info_header, bitmap_image_data, cb_bitmap_image_data);
            free(bitmap_image_data);
          }
          else
          {
            fprintf(stderr, "PrepareBitmapStructuresForSaving failed at %s:%d\n", __FILE__, __LINE__);
          }

          // TODO disable image saving and keep the capture image aspect ratio while rendering
        }

        DeleteObject(hbmScreen);
      }
    }

    DeleteObject(hdcMemDC);
  }

  // Clean up
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
    case WM_KEYUP:
    {
      int virtualKeyCode = (int)wParam;
      if (virtualKeyCode == VK_S_KEY)
      {
        // TODO capture and write image to disk
      }
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
