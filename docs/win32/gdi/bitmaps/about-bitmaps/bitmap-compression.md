<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/bitmap-compression -->

# Bitmap Compression

Windows supports formats for compressing bitmaps that define their colors with 8 or 4 bits-per-pixel. Compression reduces the disk and memory storage required for the bitmap.

When the `Compression` member of the bitmap information header structure is `BI_RLE8`, a run-length encoding (RLE) format is used to compress an 8-bit bitmap. This format can be compressed in encoded or absolute modes. Both modes can occur anywhere in the same bitmap:

- _Encoded mode_ consists of two bytes: the first byte specifies the number of consecutive pixels to be drawn using the color index contained in the second byte. In addition, the first byte of the pair can be set to zero to indicate an escape character that denotes the end of a line, the end of a bitmap, or a delta, depending on the value of the second byte. The interpretation of the escape depends on the value of the second byte of the pair, which can be one of the following values.

| Values | Meaning |
|---|---|
| 0 | End of line. |
| 1 | End of bitmap. |
| 2 | Delta. The 2 bytes following the escape contain unsigned values indicating the horizontal and vertical offsets of the next pixel from the current position. |

- In _absolute mode_, the first byte is zero and the second byte is a value in the range `03H` through `FFH`. The second byte represents the number of bytes that follow, each of which contains the color index of a single pixel. When the second byte is two or less, the escape has the same meaning as encoded mode. In absolute mode, each run must be aligned on a word boundary.

The following examples shows the hexadecimal values of an 8-bit compressed bitmap:

```
[03 04] [05 06] [00 03 45 56 67] [02 78] [00 02 05 01]
[02 78] [00 00] [09 1E] [00 01]
```

The bitmap expands as follows (two-digit values represent a color index for a single pixel):

```
04 04 04
06 06 06 06 06
45 56 67
78 78
move current position 5 right and 1 down
78 78
end of line
1E 1E 1E 1E 1E 1E 1E 1E 1E
end of RLE bitmap
```

When the `Compression` member is `BI_RLE4`, the bitmap is compressed by using a run-length encoding format for a 4-bit bitmap, which also uses encoded and absolute modes:

- In encoded mode, the first byte of the pair contains the number of pixels to be drawn using the color indexes in the second byte. The second byte contains two color indexes, one in its high-order 4 bits and one in its low-order 4 bits, the second is drawn using the color in the low-order 4 bits, the third is drawn using the color in the high-order 4 bits, and so on, until all the pixels specified by the first byte have been drawn.
- In absolute mode, the first byte is zero. The second byte contains the number of color indexes that follow. Subsequent bytes contain color indexes in their high- and low-order 4 bits, one color index for each pixel. In absolute mode, each run must be aligned on a word boundary. The end-of-line, end-of-bitmap, and delta escapes described for `BI_RLE8` also apply to `BI_RLE4` compression.

The following example shows the hexadecimal values of a 4-bit compressed bitmap:

```
03 04 05 06 00 06 45 56 67 00 04 78 00 02 05 01
04 78 00 00 09 1E 00 01
```

The bitmap expands as follows (single-digit values represent a color index for a single pixel):

```
0 4 0
0 6 0 6 0
4 5 5 6 6 7
7 8 7 8
move current position 5 right and 1 down
7 8 7 8
end of line
1 E 1 E 1 E 1 E 1
end of RLE bitmap
```

<!-- END -->

[Next - Alpha Blending](./alpha-blending.md)

__shioko's note__

I understand how the `BI_RLE8` compression works but the `BI_RLE4` compression confuses me with high- and low-order 4 bits for something.
