project(openexr-thumbnailer)
cmake_minimum_required(VERSION 3.20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GDK REQUIRED gdk-pixbuf-2.0)

# Newer versions of OpenEXR (>= 3.0) ship with their own module
find_package(Imath CONFIG)
find_package(OpenEXR CONFIG)
if (OpenEXR_FOUND)
    set(OpenEXR_LIBRARIES 
        OpenEXR::OpenEXR
        Imath::Imath
    )
else()
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
    find_package(OpenEXR REQUIRED)
endif()


add_definitions(-DTHUMBNAILER_RETURNS_PIXBUF)

add_executable(openexr-thumbnailer
    src/openexr-thumbnailer.cpp
    src/thumbnailer-skeleton.c
    src/thumbnailer-skeleton.h
    )

target_include_directories(openexr-thumbnailer
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${GIO_INCLUDE_DIRS}
    ${GDK_INCLUDE_DIRS}
    )

target_link_libraries(openexr-thumbnailer PUBLIC 
    ${OpenEXR_LIBRARIES}
    ${GIO_LIBRARIES}
    ${GDK_LIBRARIES}
)

install(TARGETS openexr-thumbnailer)
install(FILES "openexr.thumbnailer"     DESTINATION "share/thumbnailers")
install(FILES "openexr-thumbnailer.xml" DESTINATION "share/mime/packages")