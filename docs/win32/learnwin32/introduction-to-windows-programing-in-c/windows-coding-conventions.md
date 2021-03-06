<!-- https://docs.microsoft.com/en-us/windows/win32/learnwin32/windows-coding-conventions -->
# Windows Coding Conventions

If you are new to Windows programming, it can be disconcerting when you first see a Windows program. The code is filled with strange type definitions like `DWORD_PTR` and `LPRECT`, and variables have names like `hWnd` and `pwsz` (called Hungarian notation). It's worth taking a moment to learn some of the Windows coding conventions.

The vast majority of Windows APIs consist of either functions or Component Object Model (COM) interfaces. Very few Windows APIs are provided as C++ classes. (A notable exception is GDI+, one of the 2-D graphics APIs.)

## Typedefs

The Windows headers contain a lot of typedefs. Many of these are defined in the header file `WinDef.h`. Here are some that you will encounter often.

### Integer types

| Data type | Size | Signed? |
|-----------|------|---------|
| `BYTE` | 8 bits | Unsigned |
| `DWORD` | 32 bits | Unsigned |
| `INT32` | 32 bits | Signed |
| `INT64` | 64 bits | Signed |
| `LONG` | 32 bits | Signed |
| `LONGLONG` | 64 bits | Signed |
| `UINT32` | 32 bits | Unsigned |
| `UINT64` | 64 bits | Unsigned |
| `ULONG` | 32 bits | Unsigned |
| `ULONGLONG` | 64 bits | Unsigned |
| `WORD` | 16 bits | Unsigned |

As you can see, there is a certain amount of redundancy is these typedefs. Some of this overlap is simply due to the history of the Windows APIs. The types listed here have fixed size, and the sizes are the same in both 32-bit and 64-applications. For example, the `DWORD` type is always 32 bits wide.

### Boolean Type

`BOOL` is a typedef for an integer value that is used in a Boolean context. The header file `WinDef.h` also defines two values for use with `BOOL`.

```cpp
#define FALSE 0
#define TRUE  1
```

Despite this definition of `TRUE`, however, most functions that return a `BOOL` type can return any non-zero value to indicate Boolean truth. Therefore, you should always write this:

```cpp
// Right way.
BOOL result = SomeFunctionThatReturnsBoolean();
if (result)
{
    // ...
}
```

and not this:

```cpp
// Wrong!
if (result == TRUE)
{
    // ...
}
```

Be aware that `BOOL` is an integer type and is not interchangeable with the C++ `bool` type.

### Pointer Types

Windows defines many data types of the form _pointer-to-X_. These usually have the prefix _P-_ or _LP-_ in the name. For example, `LPRECT` is a pointer to a `RECT`, where `RECT` is a structure that describes a rectangle. The following variable declarations are equivalent.

```cpp
RECT*  rect; // Pointer to a RECT structure.
LPRECT rect; // The same
PRECT  rect; // Also the same.
```

Historically, _P_ stands for "pointer" and _LP_ stands for "long pointer". Long pointers (also called _far pointers_) are a holdover from 16-bit Windows, when they were needed to address memory ranges outside the current segment. The _LP_ prefix was preserved to make it easier to port 16-bit code to 32-bit Windows. Today there is no distinction - a pointer is a pointer.

### Pointer Precision Types

The following data types are always the size of a pointer - that is, 32 bits wide in 32-bit applications, and 64 bits wide in 64-bit applications. The size is determined at compile time. When a 32-bit application runs on 64-bit Windows, these data types are still 4 bytes wide. (A 64-bit application cannot run on 32-bit Windows, so the reverse situation does not occur.)

- `DWORD_PTR`
- `INT_PTR`
- `LONG_PTR`
- `ULONG_PTR`
- `UINT_PTR`

These types are used in situations where an integer might be cast to a pointer. They are also used to defined variables for pointer arithmetic and to define loop counters that iterate over the full range of bytes in memory buffers. More generally, they appear in places where an existing 32-bit value was expanded to 64 bits on 64-bit Windows.

## Hungarian Notation

_Hungarian notation_ is the practice of adding prefixes to the names of variables, to give additional information about the variable. (The notation's inventor, Charles Simonyi, was Hungarian, hence its name.)

In its original form, Hungarian notation gives _semantic_ information about a variable, telling you the intended use. For example, _i_ means an index, _cb_ means a size in bytes ("count of bytes"), and _rw_ and _col_ mean row and column numbers. These prefixes are designed to avoid the accidental use of a variable in the wrong context. For example, if you saw the expression `rwPosition + cbTable`, you would know that a row number is being added to a size, which is almost certainly a bug in the code.

A more common form of Hungarian notation uses prefixes to give _type_ information - for example, _dw_ for `DWORD` and _w_ for `WORD`.

If you search the Web for "Hungarian notation", you will find a lot of opinions about whether Hungarian notation is good or bad. Some programmers have an intense dislike for Hungarian notation. Others find it helpful. Regardless, many of the code examples on MSDN use Hungarian notation, but you don't need to memorize the prefixes to understand the code.

## Next

[Working with String](./working-with-strings.md)
