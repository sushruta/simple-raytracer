# simple-raytracer
A simple raytracer written in modern c++

Using CMake, you can create and run the application in windows as well nix based platforms.

### Setting it up

First install:

- [Git](https://git-scm.com/)

- [CMake](https://cmake.org) (`>= 3.12`)

- [Visual Studio](https://visualstudio.microsoft.com/downloads/)

Then type the following in your [terminal](https://hyper.is/) or open a terminal in visual studio code.

```bash
# 🐑 Clone the repo
git clone https://github.com/sushruta/simple-raytracer --recurse-submodules

# 💿 go inside the folder
cd simple-raytracer

# 👯 If you forget to `recurse-submodules` you can always run:
git submodule update --init

# 👷 Make a build folder
mkdir build
cd build

# 🖼️ To build your Visual Studio solution on Windows x64
cmake .. -A x64

# 🔨 Build project
cmake --build .
```

Open the `sln` file in `build` folder. Run it in debug mode or release mode

-------------------------------------------

For comments and fanmail, just open an issue
🧡
