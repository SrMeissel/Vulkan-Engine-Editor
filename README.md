# Vulkan-Engine-Editor
Editor-Logic - A C++ DLL that launches the [Engine](https://github.com/SrMeissel/Vulkan-game-engine) and Editor-UI.
Editor-UI - A C# Class library that operates a WPF window.  

To make this work, the engine must have a DLL build configuration. the App class must be refactored to increase external control. this means the main function will be more involved in the EXE build of the engine.  

To display the rendered image within the editor there are two options:
* Use [HwndHost](https://learn.microsoft.com/en-us/dotnet/api/system.windows.interop.hwndhost?view=windowsdesktop-8.0) to capture the engine window within the editor.
* Do not create a GLFW window, but instead store a image buffer in a shared memory location accessible by the editor to be reconstructed into an image.
