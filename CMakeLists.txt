cmake_minimum_required(VERSION 3.24)
project(untitled23)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
#set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(WIN32)
    set(CMAKE_PREFIX_PATH "R:/Qt/Qt5.9.0/5.9/msvc2017_64")
else()
    set(CMAKE_PREFIX_PATH "/home/vaca/Qt5.9.0/5.9/gcc_64")
endif()


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)


add_definitions(-DHAVE_STRUCT_TIMESPEC)

include_directories(${CMAKE_SOURCE_DIR}/libjuice-1.2.3/include)
include_directories(${CMAKE_SOURCE_DIR}/pthread/include)
include_directories(${CMAKE_SOURCE_DIR}/lws/include)
include_directories(${CMAKE_SOURCE_DIR}/cJSON-1.7.15)

add_subdirectory(${CMAKE_SOURCE_DIR}/libjuice-1.2.3)
add_subdirectory(${CMAKE_SOURCE_DIR}/cJSON-1.7.15)
aux_source_directory(chem CHEM_SRC)
add_executable(untitled23 ${CHEM_SRC} main.cpp
        ui/gaga.cpp ui/gaga.h ui/gaga.ui
        test_ws.cpp test_ws.h test_juice.c test_juice.h
        kcp/ikcp.c kcp/ikcp.h

        )
target_link_libraries(untitled23
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets

        ${CMAKE_SOURCE_DIR}/lws/websockets_static.lib
        ws2_32.lib
        userenv.lib
        psapi.lib
        iphlpapi.lib
        crypt32.lib
        ${CMAKE_SOURCE_DIR}/openssl_lib/libssl64MD.lib
        ${CMAKE_SOURCE_DIR}/openssl_lib/libcrypto64MD.lib
        kernel32.lib
        user32.lib
        gdi32.lib
        winspool.lib
        shell32.lib
        ole32.lib
        oleaut32.lib
        uuid.lib
        comdlg32.lib
        advapi32.lib

        ${CMAKE_SOURCE_DIR}/pthread/lib/x64/pthreadVC2.lib
        juice
        )

if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
