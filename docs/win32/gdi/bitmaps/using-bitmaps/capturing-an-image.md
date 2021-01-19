<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/capturing-an-image -->

# Capturing an Image

You can use a bitmap to capture an image, and you can store the captured image in memory, display it at a different location in your application's window, or display it in another window.

In some cases, you may want your application to capture images and store them only temporarily. For example, when you scale or zoom a picture created in a drawing application, the application must temporarily save the normal view of the image and display the zoomed view. Later, when the user selects the normal view, the application must replace the zoomed image with a copy of the normal view that it temporarily saved.

To store an image temporarily, your application must call `CreateCompatibleDC` to create a DC that is compatible with the current window DC. After you create a compatible DC, you create a bitmap with the appropriate dimensions by calling the `CreateCompatibleBitmap` function and then select it into this device context by calling the `SelectObject` function.

After the compatible device context is created and the appropriate bitmap has been selected into it, you can capture the image. The `BitBlt` function captures images. This function performs a bit block transfer that is, it copies data from a source bitmap into a destination bitmap. However, the two arguments to this function are not bitmap handles. Instead, `BitBlt` receives handles that identify two device contexts and copies bitmap data from a bitmap selected into the source DC into a bitmap selected into the target DC.

The following example code is from an application that captures an image of the entire desktop, scales it down to the current window size and saves it to a file.

<!-- WIP -->
