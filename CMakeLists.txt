set(LLVM_LINK_COMPONENTS support)

set(CLANG_MUTE_VERSION "0.0.1")

configure_file(version.h.in version.h)

add_clang_executable(clang-mute main.cpp)
target_link_libraries(clang-mute
  PRIVATE
  clangTooling
  clangTransformer
)
