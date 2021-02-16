# The modified and improve sample from the Microsoft one

The original program starts a GUI application, captures the screen and writes the captured image to a bitmap file in local disk every single time it received the `WM_PAINT` message. Because of that, it overwrites the image file everytime user resize the application, writes a lot of data to disk (which potentially reduces SSD health).

- This modified version aims to capture the desktop and only render it in the application window.
- It will give user a shortcut key to command the writing image to disk action.
- It will keep the capture image aspect ratio while rendering instead of stretching to fill the application window.
