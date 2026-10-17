cmake_minimum_required(VERSION 3.10.0)
project(fitten-banditten VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
    cpp-httplib
    GIT_REPOSITORY  https://github.com/yhirose/cpp-httplib
    GIT_TAG         v0.61.0 
)

FetchContent_MakeAvailable(cpp-httplib)

find_package(SQLite3)

set(CONFIG_DIR $ENV{HOME}/.config/${CMAKE_PROJECT_NAME})

make_directory($CONFIG_DIR)

file(GLOB files CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/config_defaults/*")

foreach(file IN LISTS files)
    get_filename_component(filename "${file}" NAME)
    file(COPY "${file}" DESTINATION "${CONFIG_DIR}")
endforeach()

file(GLOB files CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/static/*")

foreach(file IN LISTS files)
    get_filename_component(filename "${file}" NAME)
    configure_file(
        "${file}"
        "${CONFIG_DIR}/static/${filename}"
        @ONLY
    )
endforeach()

message(STATUS "${CMAKE_SOURCE_DIR}/config_defaults/config.yaml")

add_executable(fitten-banditten 
    main.cpp
    router/api.cpp
    router/pages.cpp
    libs/internals/internals.cpp
    libs/internals/sqlite_wrapper.cpp
)
target_link_libraries(fitten-banditten PRIVATE SQLite3::SQLite3)
target_link_libraries(fitten-banditten PRIVATE httplib::httplib)
