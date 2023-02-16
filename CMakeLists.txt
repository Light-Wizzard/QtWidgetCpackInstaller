# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
#
cmake_minimum_required(VERSION 3.5)
###############################################################################
# Define Project and Version
project(QtWidgetCpackInstaller VERSION 0.1 LANGUAGES CXX)
###############################################################################
# Set Short Description Name
set(MY_DESCRIPTION "Qt Widget cpack Installer")
# Set GitHub PUBLISHER Name
set(MY_PUBLISHER "Light-Wizzard")
set(MY_PROJECT "QtWidgetCpackInstaller")
#
set(APP_URL "https://github.com/${MY_PUBLISHER}/${MY_PROJECT}")
set(APP_SUPPORT "https://github.com/${MY_PUBLISHER}/${MY_PROJECT}/wiki/Support")
set(APP_CONTACT "https://github.com/${MY_PUBLISHER}/${MY_PROJECT}/wiki/Contact")
set(APP_EMAIL "https://github.com/${MY_PUBLISHER}/${MY_PROJECT}/wiki/Email")
###############################################################################
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
###############################################################################
# Point CMake to the custom modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
###############################################################################
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
###############################################################################
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    # https://doc.qt.io/qt-6/windows-deployment.html
    set(OSS_WINDOWS TRUE)
    message(STATUS "Windows")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "AIX")
    message(STATUS "AIX Unsupported OS")
    set(OSS_UNSUPPORTED TRUE)
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux|CYGWIN.*|MSYS.*|^GNU$|Android")
    # https://github.com/probonopd/linuxdeployqt
    set(OSS_LINUX TRUE)
    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Android")
        message(STATUS "Android")
    elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
        message(STATUS "Linux")
    elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "CYGWIN.*")
        message(STATUS "CYGWIN.*")
    elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "MSYS.*")
        message(STATUS "MSYS.*")
    elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "^GNU$")
        message(STATUS "^GNU$")
    endif()
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "OpenBSD")
    set(OSS_LINUX TRUE)
    message(STATUS "OpenBSD")
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "FreeBSD")
    set(OSS_LINUX TRUE)
    message(STATUS "FreeBSD")
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "CrayLinuxEnvironment")
    set(OSS_LINUX TRUE)
    message(STATUS "CrayLinuxEnvironment")
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "MSYS")
    message(STATUS "MSYS")
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    # https://doc.qt.io/qt-6/macos-deployment.html
    set(OSS_MAC TRUE)
    message(STATUS "Mac")
else()
    set(OSS_UNSUPPORTED TRUE)
    message(FATAL_ERROR "Unsupported OS")
endif()
###############################################################################
if(OSS_LINUX)
  include(GNUInstallDirs)
  set(${CMAKE_INSTALL_LIBDIR} "lib")
  set(${CMAKE_INSTALL_DATADIR} "share")
  set(${CMAKE_INSTALL_INCLUDEDIR} "include")
  set(${CMAKE_INSTALL_BINDIR} "bin")
  message(STATUS "Setting installation destination on Unix to: ${CMAKE_INSTALL_PREFIX}")
elseif (OSS_WINDOWS)
    # Really not sure what to set these values at, but want them in same folder
    set(${CMAKE_INSTALL_LIBDIR} "bin")
    set(${CMAKE_INSTALL_DATADIR} "bin")
    set(${CMAKE_INSTALL_INCLUDEDIR} "bin")
    set(${CMAKE_INSTALL_BINDIR} "bin")
    message(STATUS "Setting installation destination on Windows to: ${CMAKE_INSTALL_PREFIX}")
elseif (OSS_MAC)
    set(${CMAKE_INSTALL_LIBDIR} "lib")
    set(${CMAKE_INSTALL_DATADIR} "share")
    set(${CMAKE_INSTALL_INCLUDEDIR} "include")
    set(${CMAKE_INSTALL_BINDIR} "bin")
    message(STATUS "Setting installation destination on Mac to: ${CMAKE_INSTALL_PREFIX}")
else()
    message(FATAL_ERROR "System not Unix, Linux, Apple nor Windows - not implemented yet")
endif()
###############################################################################
find_package(QT NAMES Qt6 REQUIRED COMPONENTS WebChannel WebEngineWidgets Widgets LinguistTools PrintSupport Network TextToSpeech)
find_package(Qt6 REQUIRED COMPONENTS WebChannel WebEngineWidgets Widgets LinguistTools PrintSupport Network TextToSpeech)
###############################################################################
# TS Files
set(TS_FILES
    translations/${PROJECT_NAME}_ar.ts
    translations/${PROJECT_NAME}_de.ts
    translations/${PROJECT_NAME}_en.ts
    translations/${PROJECT_NAME}_es.ts
    translations/${PROJECT_NAME}_fr.ts
    translations/${PROJECT_NAME}_it.ts
    translations/${PROJECT_NAME}_ja.ts
    translations/${PROJECT_NAME}_no.ts
    translations/${PROJECT_NAME}_ru.ts
    translations/${PROJECT_NAME}_sv.ts
    translations/${PROJECT_NAME}_zh_CN.ts
)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
        src/DialogAbout.cpp src/DialogAbout.h src/DialogAbout.ui
        src/MySettings.h src/MySettings.cpp
        src/document.cpp src/document.h
        src/previewpage.cpp src/previewpage.h
        ${TS_FILES}
        ${PROJECT_NAME}.qrc
)
###############################################################################
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
        # Define properties for Android with Qt 5 after find_package() calls as:
        # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
###############################################################################
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::WebChannel
    Qt6::WebEngineWidgets
    Qt6::PrintSupport
    Qt6::Network
    Qt6::TextToSpeech)
###############################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
###############################################################################
#install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "bin" COMPONENT applications)
###############################################################################
set(Vulkan_INCLUDE_DIR "C:/VulkanSDK/1.3.239.0/Bin")
# find_package(Vulkan REQUIRED)
###############################################################################
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
###############################################################################
set(COMPONENT_NAME_MAIN "DynamicLibrary")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN TRUE)
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED TRUE)
###############################################################################
if(OSS_WINDOWS)
    set(MY_FILE_EXTENSION ".exe")
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/DeployQt.cmake")
    windeployqt("${PROJECT_NAME}" "${INSTALL_BIN_PATH}")
elseif(OSS_LINUX)
    set(MY_FILE_EXTENSION ".run")
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/DeployQt.cmake")
    linuxdeployqt("${PROJECT_NAME}" "${INSTALL_BIN_PATH}")
elseif(OSS_MAC)
    set(MY_FILE_EXTENSION ".dmg")
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/DeployQt.cmake")
    macdeployqt("${PROJECT_NAME}" "${INSTALL_BIN_PATH}")
else()
    set(MY_FILE_EXTENSION "")
endif()
###############################################################################
include(InstallRequiredSystemLibraries)
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

set(CPACK_NSIS_COMPONENT_INSTALL On)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${MY_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "${MY_PUBLISHER}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/unlicense.txt")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} )
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR} )
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH} )


if(OSS_WINDOWS)

  # There is a bug in NSI that does not handle full UNIX paths properly.
  # Make sure there is at least one set of four backlashes.

  ##if (NOT EXISTS "${CMAKE_SOURCE_DIR}/usr/share/pixmaps/${PROJECT_NAME}.bmp" AND NOT IS_DIRECTORY "${CMAKE_SOURCE_DIR}/usr/share/pixmaps/${PROJECT_NAME}.bmp")
  ##    message(WARNING "File not found: ${CMAKE_SOURCE_DIR}/usr/share/pixmaps/${PROJECT_NAME}.bmp")
  ##endif()
  ##set(THIS_ICON "${CMAKE_SOURCE_DIR}/usr/share/pixmaps/${PROJECT_NAME}.bmp")

  set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/usr/share/pixmaps\\\\${PROJECT_NAME}.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${PROJECT_NAME}.${MY_FILE_EXTENSION}")
  set(CPACK_NSIS_DISPLAY_NAME "${MY_DESCRIPTION}")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\${APP_URL}")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\${APP_URL}")
  set(CPACK_NSIS_CONTACT "me@${APP_CONTACT}")
  set(CPACK_NSIS_MODIFY_PATH ON)
  #
  # This does not appear to work
  # QtWebEngineProcessd.exe
  #
#  message(STATUS "Looking for ${CMAKE_PREFIX_PATH}/bin/QtWebEngineProcessd.exe")
#  file(GLOB files "${CMAKE_PREFIX_PATH}/bin/QtWebEngineProcessd.exe")
#  foreach(file ${files})
#      message(STATUS "Installing QtWebEngineProcessd.exe file: ${file}")
#      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${COMPONENT_NAME_MAIN})
#  endforeach()
  message(STATUS "Looking for ${CMAKE_PREFIX_PATH}/bin/QtWebEngineProcess.exe")
  file(GLOB files "${CMAKE_PREFIX_PATH}/bin/QtWebEngineProcess.exe")
  foreach(file ${files})
      message(STATUS "Installing QtWebEngineProcess.exe file: ${file}")
      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${COMPONENT_NAME_MAIN})
  endforeach()
  # You have to set Sandbox off or set path to where QtWebEngineProcessd.exe is
  # qputenv("QTWEBENGINE_DISABLE_SANDBOX", 1);
  # QTWEBENGINE_DISABLE_SANDBOX=1
  # pass in --no-sandbox
  # qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox");
  # QTWEBENGINE_CHROMIUM_FLAGS="--no-sandbox" ${PROJECT_NAME}
  # qputenv("QTWEBENGINEPROCESS_PATH", "?");
  # QTWEBENGINEPROCESS_PATH="?"

  # \translations
#  message(STATUS "Looking for translations qm files")
#  file(GLOB files "${CMAKE_BINARY_DIR}/translations/*.qm")
#  foreach(file ${files})
#      #message(STATUS "Installing translations qm file: ${file}")
#      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR}/translations COMPONENT ${COMPONENT_NAME_MAIN})
#  endforeach()
  install(DIRECTORY ${CMAKE_BINARY_DIR}/translations/ DESTINATION ${CMAKE_INSTALL_BINDIR}/translations COMPONENT ${COMPONENT_NAME_MAIN})
  # These are all the files WinDeployQt found
  message(STATUS "Looking for WinDeployQt dll files")
  file(GLOB files "${CMAKE_BINARY_DIR}/*.dll")
  foreach(file ${files})
      #message(STATUS "Installing WinDeployQt dll file: ${file}")
      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${COMPONENT_NAME_MAIN})
  endforeach()
  # Platform dlls
#  message(STATUS "Looking for platforms dll files")
#  file(GLOB files "${CMAKE_BINARY_DIR}/platforms/*.dll")
#  foreach(file ${files})
#      #message(STATUS "Installing platforms dll file: ${file}")
#      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR}/platforms COMPONENT ${COMPONENT_NAME_MAIN})
#  endforeach()
  install(DIRECTORY ${CMAKE_BINARY_DIR}/platforms/ DESTINATION ${CMAKE_INSTALL_BINDIR}/platforms COMPONENT ${COMPONENT_NAME_MAIN})
  if (MSVC)
    message(STATUS "Looking for MSVC plugins platform dll files ${CMAKE_PREFIX_PATH}")
    file(GLOB files " ${CMAKE_PREFIX_PATH}/plugins/platforms/*.dll")
    foreach(file ${files})
        #message(STATUS "Installing plugins platform dll file: ${file}")
        install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR}/platforms COMPONENT ${COMPONENT_NAME_MAIN})
    endforeach()
  endif()
  if (MINGW)
      message(STATUS "MinGW not supported in Web Engine project")
#      file(GLOB files "${CMAKE_PREFIX_PATH}/plugins/platforms/*.dll")
#      foreach(file ${files})
#          #message(STATUS "Installing MinGW plugins platform dll file: ${file}")
#          install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR}/platforms COMPONENT ${COMPONENT_NAME_MAIN})
#      endforeach()
  endif()

  install(DIRECTORY ${CMAKE_BINARY_DIR}/resources/ DESTINATION ${CMAKE_INSTALL_BINDIR}/resources COMPONENT ${COMPONENT_NAME_MAIN})
  install(DIRECTORY ${CMAKE_BINARY_DIR}/texttospeech/ DESTINATION ${CMAKE_INSTALL_BINDIR}/texttospeech COMPONENT ${COMPONENT_NAME_MAIN})

#  # iconengines
#install(DIRECTORY iconengines/ DESTINATION ${CMAKE_INSTALL_BINDIR}/iconengines COMPONENT ${COMPONENT_NAME_MAIN})
#  file(GLOB files "${CMAKE_BINARY_DIR}/iconengines/*.dll")
#  foreach(file ${files})
#      message(STATUS "Installing dll file: ${file}")
#      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${COMPONENT_NAME_MAIN})
#  endforeach()
#  # imageformats
#install(DIRECTORY imageformats/ DESTINATION ${CMAKE_INSTALL_BINDIR}/imageformats COMPONENT ${COMPONENT_NAME_MAIN})
#  file(GLOB files "${CMAKE_BINARY_DIR}/imageformats/*.dll")
#  foreach(file ${files})
#      message(STATUS "Installing dll file: ${file}")
#      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${COMPONENT_NAME_MAIN})
#  endforeach()
#  # networkinformation
#install(DIRECTORY networkinformation/ DESTINATION ${CMAKE_INSTALL_BINDIR}/networkinformation COMPONENT ${COMPONENT_NAME_MAIN})
#  file(GLOB files "${CMAKE_BINARY_DIR}/networkinformation/*.dll")
#  foreach(file ${files})
#      message(STATUS "Installing dll file: ${file}")
#      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${COMPONENT_NAME_MAIN})
#  endforeach()
#  # styles
#install(DIRECTORY styles/ DESTINATION ${CMAKE_INSTALL_BINDIR}/styles COMPONENT ${COMPONENT_NAME_MAIN})
#  file(GLOB files "${CMAKE_BINARY_DIR}/styles/*.dll")
#  foreach(file ${files})
#      message(STATUS "Installing dll file: ${file}")
#      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${COMPONENT_NAME_MAIN})
#  endforeach()
#  # tls
#install(DIRECTORY tls/ DESTINATION ${CMAKE_INSTALL_BINDIR}/tls COMPONENT ${COMPONENT_NAME_MAIN})
#  file(GLOB files "${CMAKE_BINARY_DIR}/tls/*.dll")
#  foreach(file ${files})
#      message(STATUS "Installing dll file: ${file}")
#      install(FILES "${file}" DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${COMPONENT_NAME_MAIN})
#  endforeach()
else()
  set(CPACK_STRIP_FILES "bin/${PROJECT_NAME}")
  set(CPACK_SOURCE_STRIP_FILES "")
endif()

set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")


include(CPack)
CPACK_ADD_COMPONENT(${PROJECT_NAME} ${COMPONENT_NAME_MAIN})
################################ End of File ##################################
