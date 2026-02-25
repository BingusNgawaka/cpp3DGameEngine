# 3D OpenGL Renderer in C++
Slightly cleaned up version of [older project](https://github.com/BingusNgawaka/cppProjects/tree/master/engine) with main difference being actually working portable asset paths.
[stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) used for image loading/decoding

# Dependencies
CMake
```
apt install libglfw3-dev
apt install libglew-dev
apt install libglm-dev
```

# Build
```
mkdir build
cd build
cmake ..
make
./main
```

# Notes
Left click on window to focus and use mouse to move first person camera view

Controls:
- WASD: Move
- Right Click: Zoom in camera
- LCtrl: Crouch
- Space: Jump

