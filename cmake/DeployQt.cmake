# The MIT License (MIT)
#
# Copyright (c) 2018 Nathan Osman
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# https://github.com/nitroshare/nitroshare-desktop/
#
###############################################################################
# Alterted by Light-Wizzard https://github.com/Light-Wizzard
# Need to add Support for:
#  Expected uname -s output:
#
# AIX                           AIX
# BSD/OS                        BSD/OS
# FreeBSD                       FreeBSD
# HP-UX                         HP-UX
# Linux                         Linux            LinuxDeployQt
# GNU/kFreeBSD                  GNU/kFreeBSD
# NetBSD                        NetBSD
# OpenBSD                       OpenBSD
# OFS/1 (Digital Unix)          OSF1
# SCO OpenServer 5              SCO_SV
# SCO UnixWare 7                UnixWare
# SCO UnixWare (pre release 7)  UNIX_SV
# SCO XENIX                     Xenix
# Solaris                       SunOS
# SunOS                         SunOS
# Tru64                         Tru64
# Ultrix                        ULTRIX
# cygwin                        CYGWIN_NT-5.1
# MSYS                          MSYS_NT-6.1
# MacOSX                        Darwin            MacDeployQt
###############################################################################
find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)
###############################################################################
# Retrieve the absolute path to qmake and then use that path to find the
# windeployqt and macdeployqt binaries
get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
###############################################################################
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    # https://doc.qt.io/qt-6/windows-deployment.html
    set(OSS_WINDOWS TRUE)
    message(STATUS "Windows WinDeployQt")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "AIX")
    message(STATUS "AIX Unsupported OS")
    set(OSS_UNSUPPORTED TRUE)
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux|CYGWIN.*|MSYS.*|^GNU$|Android")
    # https://github.com/probonopd/linuxdeployqt
    set(OSS_LINUX TRUE)
    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Android")
        message(STATUS "Android LinuxDeployQt")
    elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
        message(STATUS "Linux LinuxDeployQt")
    elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "CYGWIN.*")
        message(STATUS "CYGWIN.* LinuxDeployQt")
    elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "MSYS.*")
        message(STATUS "MSYS.* LinuxDeployQt")
    elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "^GNU$")
        message(STATUS "^GNU$ LinuxDeployQt")
    endif()
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "OpenBSD")
    set(OSS_LINUX TRUE)
    message(STATUS "OpenBSD LinuxDeployQt")
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "FreeBSD")
    set(OSS_LINUX TRUE)
    message(STATUS "FreeBSD LinuxDeployQt")
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "CrayLinuxEnvironment")
    set(OSS_LINUX TRUE)
    message(STATUS "CrayLinuxEnvironment LinuxDeployQt")
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "MSYS")
    message(STATUS "MSYS")
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    # https://doc.qt.io/qt-6/macos-deployment.html
    set(OSS_MAC TRUE)
    message(STATUS "Mac MacDeployQt")
else()
    set(OSS_UNSUPPORTED TRUE)
    message(FATAL_ERROR "Unsupported OS")
endif()
#
if(OSS_WINDOWS)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    if (NOT WINDEPLOYQT_EXECUTABLE)
        message(FATAL_ERROR "windeployqt not found")
    endif()
elseif(OSS_LINUX)
    # FIXME not sure what folder this will be in, but it needs to be here to work
    find_program(LINUXDEPLOYQT_EXECUTABLE linuxdeployqt HINTS "${_qt_bin_dir}")
    if (NOT LINUXDEPLOYQT_EXECUTABLE)
        message(FATAL_ERROR "linuxdeployqt not found")
    endif()
elseif(OSS_MAC)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
    if (NOT MACDEPLOYQT_EXECUTABLE)
        message(FATAL_ERROR "macdeployqt not found")
    endif()
endif()
###############################################################################
# Add commands that copy the required Qt files to the same directory as the
# target after being built as well as including them in final installation
function(windeployqt target)
    # Run windeployqt immediately after build
    # removed --no-angle due to indigestion
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                --verbose 0
                --no-compiler-runtime
                --no-opengl-sw
                \"$<TARGET_FILE:${target}>\"
        COMMENT "Deploying Windows Qt WinDeployQt..."
    )
    ###########################################################################
    # windeployqt doesn't work correctly with the system runtime libraries,
    # so we fall back to one of CMake's own modules for copying them over
    # Doing this with MSVC 2015 requires CMake 3.6+
    if((MSVC_VERSION VERSION_EQUAL 1900 OR MSVC_VERSION VERSION_GREATER 1900)
            AND CMAKE_VERSION VERSION_LESS "3.6")
        message(WARNING "Deploying with MSVC 2015+ requires CMake 3.6+")
    endif()

    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    include(InstallRequiredSystemLibraries)
    foreach(lib ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
        get_filename_component(filename "${lib}" NAME)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                copy_if_different "${lib}" \"$<TARGET_FILE_DIR:${target}>\"
            COMMENT "Copying ${filename}..."
        )
    endforeach()
endfunction()
###############################################################################
# Add commands that copy the required Qt files to the application bundle
# represented by the target.
function(macdeployqt target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            \"$<TARGET_FILE_DIR:${target}>/../..\"
            -always-overwrite
        COMMENT "Deploying Mac Qt..."
    )
endfunction()
###############################################################################
function(linuxdeployqt target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${LINUXDEPLOYQT_EXECUTABLE}"
            \"$<TARGET_FILE_DIR:${target}>/../..\"
            -always-overwrite
        COMMENT "Deploying Linux Qt..."
    )
endfunction()
###############################################################################
mark_as_advanced(WINDEPLOYQT_EXECUTABLE MACDEPLOYQT_EXECUTABLE LINUXDEPLOYQT_EXECUTABLE)
