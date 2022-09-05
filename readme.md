# clang-mute

Tool to comment out all function (and method) implementations in specific files.

```
echo 'void f(){ int x=1; }' > main.cpp
clang-mute main.cpp 
cat main.cpp
void f(){
#if 0
 int x=1; 
#endif
}
```


# Use

```
clang-mute target1.cpp [target2.cpp] [-- -Iincludepath -Ddefinition]
```

To use on real project, for example qt application, you need to create [compilation database](https://clang.llvm.org/docs/JSONCompilationDatabase.html) or compile flags file to provide all necessary includepaths to the tool.

## Sample session

```bash
git clone https://github.com/mugiseyebrows/cmake-qt-sample.git
cd cmake-qt-sample
mkdir build
cd build
# to fix file not found errors add missing includepaths to CMAKE_CXX_FLAGS variable
cmake -G Ninja -D CMAKE_CXX_FLAGS="-I/usr/lib/gcc/x86_64-linux-gnu/11/include" ..
# to generate ui_*.h files compile the app
ninja app
cmake . -DCMAKE_EXPORT_COMPILE_COMMANDS=1
clang-mute ../mainwindow.cpp
```

As alternative to compilation database you can specify includepaths and other compiler flags after `--`

```bash
clang-mute ../mainwindow.cpp -- -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG -DQT_WIDGETS_LIB -I/home/overloop/cmake-qt-sample/build -I/home/overloop/cmake-qt-sample -I/home/overloop/cmake-qt-sample/build/app_autogen/include -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -isystem /usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/usr/lib/gcc/x86_64-linux-gnu/11/include -fPIC
```

You can store flags in `compile_flags.txt` file, one per line.

```bash
echo -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG -DQT_WIDGETS_LIB -I/home/overloop/cmake-qt-sample/build -I/home/overloop/cmake-qt-sample -I/home/overloop/cmake-qt-sample/build/app_autogen/include -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -isystem /usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I/usr/lib/gcc/x86_64-linux-gnu/11/include -fPIC | tr ' ' '\n' > compile_flags.txt
clang-mute ../mainwindow.cpp
```

# Build

## Ubuntu

```bash
sudo apt install build-essential cmake ninja-build git
git clone --depth 1 https://github.com/llvm/llvm-project.git
cd llvm-project/clang/tools
git clone https://github.com/mugiseyebrows/clang-mute.git
echo "add_clang_subdirectory(clang-mute)" >> CMakeLists.txt
cd ../..
mkdir build
cd build
cmake -DLLVM_ENABLE_PROJECTS="clang" -DCMAKE_BUILD_TYPE=Release -G Ninja ../llvm
ninja clang-mute
sudo cp bin/clang-mute /usr/bin
```

## Windows

- Install [ninja](https://ninja-build.org/)
- Add ninja to `%PATH%`
- Clone sources

```bash
git clone --depth 1 https://github.com/llvm/llvm-project.git
cd llvm-project\clang\tools
git clone https://github.com/mugiseyebrows/clang-mute.git
echo add_clang_subdirectory(clang-mute) >> CMakeLists.txt
cd ..\..
mkdir build
cd build
```

### Visual Studio

```bash
cmake -DLLVM_ENABLE_PROJECTS="clang" -DCMAKE_BUILD_TYPE=Release -G Ninja ../llvm
ninja clang-mute
```

### MinGW

- Add compiler to `%PATH%`

```bash
cmake -DLLVM_ENABLE_PROJECTS="clang" -DCMAKE_BUILD_TYPE=Release -G Ninja -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ ../llvm
ninja clang-mute
```


# See also

[post on medium](https://medium.com/@mugiseyebrows/source-to-source-transformation-using-transformers-in-clang-b6507a191ca4)