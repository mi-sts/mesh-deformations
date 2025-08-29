include(FetchContent)

FetchContent_Declare(
        xxhash
        GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
        GIT_TAG v0.8.3
)

FetchContent_GetProperties(xxhash)
if (NOT xxhash_POPULATED)
    FetchContent_MakeAvailable(xxhash)

    add_library(xxhash STATIC
            ${xxhash_SOURCE_DIR}/xxhash.c
    )

    target_include_directories(xxhash PUBLIC
            ${xxhash_SOURCE_DIR}
    )

    target_compile_definitions(xxhash PUBLIC XXH_INLINE_ALL=0)
endif ()