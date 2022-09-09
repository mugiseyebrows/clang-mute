if [ ! -f llvm-project-15.0.0rc3.src.tar.xz ]; then
    curl -L -o llvm-project-15.0.0rc3.src.tar.xz https://github.com/llvm/llvm-project/releases/download/llvmorg-15.0.0-rc3/llvm-project-15.0.0rc3.src.tar.xz
fi
if [ ! -d llvm-project ]; then
    mkdir llvm-project
    echo "extracting llvm-project-15.0.0rc3.src.tar.xz please wait"
    tar xf llvm-project-15.0.0rc3.src.tar.xz --directory=llvm-project --strip-components=1
    mkdir llvm-project/clang/tools/clang-mute
    mkdir llvm-project/build
    echo "add_clang_subdirectory(clang-mute)" >> llvm-project/clang/tools/CMakeLists.txt
fi
cp *.cpp CMakeLists.txt *.h.in llvm-project/clang/tools/clang-mute
cd llvm-project/build
cmake -DLLVM_ENABLE_PROJECTS="clang" -DCMAKE_BUILD_TYPE=Release -G Ninja ../llvm
cmake --build . --target clang-mute
cd ../..
