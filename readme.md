# clang-mute

Tool to comment out all function (and method) implementation in specific files.

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
clang-mute target1.cpp [target2.cpp] [-- -I/includepath -Ddefinition]
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


