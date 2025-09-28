# Vulkan-Engine-Editor

This project WILL be a win32 UI that includes the Vulkan-game-engine as a static library. I have done my research, this is it. Progress will be made, no more backtracking. :)  

## Project Setup

For this project to be built, all dependencies need to be built and included.  
First [Srmeissel/Vulkan-game-engine](https://github.com/SrMeissel/Vulkan-game-engine) has to be built.
Second, to include all static libs go to **View**->**Other Windows**->**Property Manager**, then edit the Dependencies.props sheet.
Replace template directories in both **C/C++**->**General**->**Additional Include Directories**, and **Linker**->**input**->**Additional Dependencies**.
The project should now be able to be built and ran.
