<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/scaling-an-image -->

# Scaling an Image

Some applications scale images: that is, they display zoomed or reduced views of an image. For example, a drawing application may provide a zoom feature that enables the user to view and edit a drawing on a pixel-by-pixel basis.

Application scale images by calling the `StretchBlt` function. Like the `BitBlt` function, `StretchBlt` copies bitmap data from a bitmap in a source device context (`DC`) into a bitmap in a target DC. However, unlike the `BitBlt` function, `StretchBlt` scales the image based on the specified dimensions of the source and target rectangles. If the source rectangle is larger than the target rectangle, the resultant image will appear to have shrunk; if the source rectangle is smaller than the target rectangle, the resultant image will appear to have expanded.

If the target rectangle is smaller than the source rectangle, `StretchBlt` removes color data from the image according to a specified stretch mode as shown in the following table.

| Stretch mode | Method |
|---|---|
| `BLACKONWHITE` | Performs a logical AND operation on the color data for the eliminated pixels and the color data for the remaining pixels. |
| `WHITEONBLACK` | Performs a logical OR operation on the color data for the eliminated pixels and the color data for the remaining pixels. |
| `COLORONCOLOR` | Eliminates the color data of the deleted pixels completely. |
| `HALFTONE` | Approximates the original (source) color data in the destination. |

You set the stretch mode by calling the `SetStretchBltMode` function. The following example code is taken from an application that demonstrates all four of the stretch modes available with the `StretchBlt` function.

```cpp
#include "stdafx.h"
#include "GDIBitmapScaling.h"
#include <commctrl.h>
#include <CommDlg.h>

#define MAX_LOADSTRING 100


HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG msg;
    HACCEL hAccelTable;

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_GDIBITMAPSCALING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINRESOURCE(IDC_GDIBITMAPSCALING));

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstace, MAKEINRESOURCE(IDI_GDIBITMAPSCALING));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_GDIBITMAPSCALING);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

#define NEW_DIB_FORMAT(hpbih) (lphib->biSize != sizeof(BITMAPCOREHEADER))
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;

    hInst = hInstance;

    hWnd = CreateWindow(szWindowClass, szTittle, WS_SYSMENU, CW_USEDEFAULT, 0, 1024, 768, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

static HCURSOR hcurSave;

WORD DIBNumColors (LPVOID lpv)
{
    INT bits;
    LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)lpv;
    LPBITMAPCOREHEADER lpbch = (LPBITMAPCOREHEADER)lpv;

    /* With the BITMAPINFO format headers, the size of the palette is in biClrUsed, whereas in the BITMAPCORE - style headers, it is dependent on the bits per pixel ( = 2 raised to the power of bits/pixel).
     */
    if(NEW_DIB_FORMAT(lpbih)) {
        if (lpbih->biClrUsed != 0){
            return (WORD)lpbih->biClrUsed;
        }
        bits = lpbih->biBitCount;
    }else{
        bits = lpcbch->bcBitCount;
    }

    if (bits > 8){
        /* Since biClrUsed is 0, we don't have an optimal palette */
        return 0;
    }else {
        return (1 << bits);
    }
}

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i) ((((i) + 31) >> 5) << 2)

//
// FUNCTION : DIBInfo(HANDLE hbi, LPBITMAPINFOHEADER lpbih)
//
// PURPOSE : Retrieves the DIB info associated with a CF_DIB format memory block.
//
// RETURN : TRUE  - if successful
//          FALSE - otherwise
//
BOOL DIBInfo (HANDLE hbi, LPBITMAPINFOHEADER lpbih)
{
    if (hbi){
        *lpbih = *(LPBITMAPINFORHEADER)hbi;

        /* fill in the default fields */
        if (NEW_DIB_FORMAT(lpbih)){
            if(lpbih->biSizeImage == 0L){
                lpbih->biSizeImage = WIDTHBYTES(lpbih->biWidth*lpbih->biBitCount) * lpbih->biHeight;
            }

            if(lpbih->biClrUsed == 0L){
                lpbih->biClrUsed == DIBNumColors(lpbih);
            }
        }

        return TRUE;
    }

    return FALSE;
}

/* flags for mmioSeek() */
#ifndef SEEK_SET
#define SEEK_SET 0 /* seek to an absolute position */
#define SEEK_CUR 1 /* seek relative to current position */
#define SEEK_END 2 /* seek relative to the end of file */
#endif /* ifndef SEEK_SET */
VOID ReadPackedFileHeader(HFILE hFile, LPBITMAPFILEHEADER lpbmfhdr, LPDWORD lpdwOffset)
{
    *lpdwOffset = _llseek(hFile, 0L, (UINT) SEEK_CUR);
    _hread(hFile, (LPSTR) &lpbmfhdr->bfType, sizeof(WORD)); /* read in bfType */
    _hread(hFile, (LPSTR) &lpbmfhdr->bfSize, sizeof(DWORD) * 3); /* read in last 3 dwords */
}

/* macro to determine if resource id a DIB */

/* Header signatures for various resources */
#define BFT_ICON 0x4349 /* 'IC' */
#define BFT_BITMAP 0x4d42 /* 'BM' */
#define BFT_CURSOR 0x5450 /* 'PT' */
HANDLE ReadDIBBitmapInfo(INT hFile)
{
    DWORD dwOffset;
    HANDLE hbi = NULL;
    INT szie;
    INT i;
    WORD nNumColor;
    LPRGBQUAD lprgbq;
    BITMAPINFOHEADER bih;
    BITMAPCOREHEADER bch;
    LPBITMAPINFOHEADER lpbih;
    BITMAPFILEHEADER bf;
    DWORD dwDWMasks = 0;
    DWORD dwWidth = 0;
    DWORD dwHeight = 0;
    WORD wPlanes, wBitCount;

    if(hFile == HFILE_ERROR){
        return NULL;
    }

    /* Read the bitmap file header */
    ReadPackedFileHeader(hFile, &bf, &dwOffset);

    /* Do we have a RC HEADER? */
    if(!ISDIB(bf.bfType)){
        bf.bfOffBits = 0L;
        _llseek(hFile, dwOffset, (UINT)SEEK_SET); /* seek back to beginning of file */
    }

    if(sizeof(bih) != _hread(hFile, (LPSTR)&bih, (UINT)sizeof(bih))){
        return FALSE;
    }

    nNumColors = DIBNumColors(&bih);

    /* Check the nature (BITMAPINFO or BITMAPCORE) of the info block and extract the field information accordingly. If a BITMAPCOREHEADER, transfer it's field information to a BITMAPINFOHEADER-style block
    */
    switch(size=(INT)bih.biSize){
        case sizeof(BITMAPINFOHEADER):
            break;
        case sizeof(BITMAPCOREHEADER):
            bch = *(LPBITMAPCOREHEADER)&bih;

            dwWidth = (DWORD)bch.bcWidth;
            dwHeight = (DWORD)bch.bcHeight;
            wPlanes = bch.bcPlanes;
            wBitCount = bch.bcBitCount;

            bih.biSize = sizeof(BITMAPINFOHEADER);
            bih.biWidth = dwWidth;
            bih.biHeight = dwHeight;
            bih.biPlanes = wPlanes;
            bih.biBitCount = wBitCount;
            bih.biCompression = BI_RGB;
            bih.biSizeImage = 0;
            bih.biXPelsPerMeter = 0;
            bih.biYPelsPerMeter = 0;
            bih.biClrUsed = nNumColors;
            bih.biClrImportant = nNumColors;

            _llseek(hFile, (LONG)sizeof(BITMAPCOREHEADER) - sizeof(BITMAPINFOHEADER), (UINT)SEEK_CUR);
            break;
        default:
            /* Not a DIB! */
            return NULL;
    }

    /* Fill in some default values if they are zero */
    if(bih.biSizeImage == 0){
       bih.biSizeImage = WIDTHBYTES((DWORD)bih.biWidth * bih.biBitCount) * bih.biHeight;
    }

    if (bih.biClrUsed == 0){
        bih.biClrUsed = DIBNumColors(&bih);
    }

    /* Allocate for the BITMAPINFO structure and the color table. */
    if((bih.biBitCount == 16) || (bih.biBitCount == 32)){
        dwDWMasks = sizeof(DWORD) * 3;
    }

    hbi = GlobalAlloc(GPTR, (LONG)bih.biSize + nNumColors * sizeof(RGBQUAD) + dwDWMasks);

    if (!hbi){
        return NULL;
    }

    lpbih = (LPBITMAPINFORHEADER)hbi;
    *lpbih = bih;

    /* Get a pointer to the color table */
    lprgbq = (LPRGBQUAD)((LPSTR)lpbih + bih.biSize);
    if(nNumColors){
        if(size == sizeof(BITMAPCOREHEADER)){
            /* convert a old color table (3 bytes RGBTRIPLEs) to a new color table (4 bytes RGBQUADs)
             */
            _hread(hFile, (LPSTR)lprgbq, (UINT)nNumColors*sizeof(RGBTRIPLE));

            for(i = nNumColors - 1; i >= 0; i--){
                RGBQUAD rgbq;

                rgbq.rgbRed = ((RGBTRIPLE*)lprgbq)[i].rgbtRed;
                rgbq.rgbBlue = ((RGBTRIPLE*)lprgbq)[i].rgbtBlue;
                rgbq.rgbGreen = ((RGBTRIPLE*)lprgbq)[i].rgbtGreen;
                rgbq.rgbReserved = (BYTE)0;

                lprgbq[i] = rgbq;
            }
        }else{
            _hread(hFile, (LPSTR)lprgbq, (UINT)nNumColors * sizeof(RGBQUAD));
        }
    }else{
        if(dwDWMasks){
            _hread(hFile, (LPSTR)lprgbq, dwDWMasks);
        }
    }

    if(bf.bfOffBits != 0L){
        _llseek(hFile, dwOffset + bf.bfOffBits, (UINT)SEEK_SET);
    }

    return hbi;
}
```

<!-- WIP -->
