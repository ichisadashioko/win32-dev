# 20201021 - render and display image (bitmap)

I am working on a handwriting application. The application draws the user's "input" directly on the "window". GDI was used as rendering API. However, I also want to get the image (bitmap) representation of the "canvas/window" in order to do some image processing. I think I might need to use a separate API for drawing on a two-dimemsional array and render that array on the "window". I think I will learn about [bitmaps](https://docs.microsoft.com/en-us/windows/win32/gdi/bitmaps) in the next few days.

# Symbols in markdown

```md
<!-- END -->
```

denotes that I have done retyping that guide.

```md
<!-- WIP -->
```

denotes that where I need to pick up to continue following the guide since last time I visited these guides.

# Note

## 2021-01-24 Working with bitmaps

While trying to learn how to save a bitmap as image, we need quite a lot of structures to represent the bitmap.

- `BITMAP` - to store some bitmap infomation
- `HBITMAP` - the bitmap handler which we need to pass to the `GetObject` function to get the `BITMAP` structure and pass to the `GetDIBits` to get the "byte-encoded" bitmap data.
- `HDC` - the device context handler which has the bitmap (e.g. on a display device - probably).
- `BITMAPINFOHEADER`
- `BITMAPFILEHEADER`
- `char*`

I felt discouraged because I am still not sure which ones should I pass to a function.
