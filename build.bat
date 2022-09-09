@echo off
rem This file is generated from build.pbat, all edits will be lost
set PATH=C:\Ninja;C:\Program Files\Git\mingw64\bin;C:\Program Files\Git\usr\bin;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
if not exist llvm-project-15.0.0rc3.src.tar.xz curl -L -o llvm-project-15.0.0rc3.src.tar.xz https://github.com/llvm/llvm-project/releases/download/llvmorg-15.0.0-rc3/llvm-project-15.0.0rc3.src.tar.xz
if not exist llvm-project (
mkdir llvm-project
echo "extracting llvm-project-15.0.0rc3.src.tar.xz please wait"
tar xf llvm-project-15.0.0rc3.src.tar.xz --directory=llvm-project --strip-components=1
)
pushd llvm-project
pushd clang\tools
if not exist "clang-mute" mkdir "clang-mute"
echo add_clang_subdirectory(clang-mute) >> CMakeLists.txt
popd
copy /y ..\main.cpp clang\tools\clang-mute
copy /y ..\CMakeLists.txt clang\tools\clang-mute
copy /y ..\version.h.in clang\tools\clang-mute
if not exist "build" mkdir "build"
pushd build
cmake -DLLVM_ENABLE_PROJECTS="clang" -DCMAKE_BUILD_TYPE=Release -G Ninja -D CMAKE_C_COMPILER=cl -D CMAKE_CXX_COMPILER=cl ../llvm
cmake --build . --target clang-mute
popd
popd


