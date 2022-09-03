set(LLVM_LINK_COMPONENTS support)

add_clang_executable(clang-mute main.cpp)
target_link_libraries(clang-mute
  PRIVATE
  clangTooling
  clangTransformer
)
