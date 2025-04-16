include(FetchContent)
FetchContent_Declare(fetch_github_content GIT_REPOSITORY https://github.com/CesarBerriot/fetch_github_content.git GIT_TAG 1.0.0)
FetchContent_MakeAvailable(fetch_github_content)
include(${fetch_github_content_SOURCE_DIR}/fetch_github_content.cmake)

fetch_github_content(
  discover_sources CesarBerriot 1.0.0 INCLUDE_DEFAULT
  make_string CesarBerriot 2.2.1
  hard_assert CesarBerriot 1.2.1
)

add_executable(Internal2)
target_compile_definitions(Internal2 PRIVATE MODULE_NAME="Internal2")
target_link_libraries(Internal2 PRIVATE make_string hard_assert Shlwapi)
target_include_directories(Internal2 PRIVATE source)
discover_sources(Internal2)

function(process_compile_options compiler_identifier)
  if(CMAKE_C_COMPILER_ID STREQUAL "${compiler_identifier}")
    target_compile_options(Internal2 PRIVATE ${ARGN})
  endif()
endfunction()

process_compile_options(MSVC /Oi-)
process_compile_options(GNU -Wno-builtin-declaration-mismatch)
process_compile_options(Clang -Wno-incompatible-library-redeclaration)