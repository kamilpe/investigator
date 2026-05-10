cmake_minimum_required (VERSION 3.31)
project (Investigator)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_FILES
 src/Toolkit/Display.cpp
 src/Toolkit/Keyboard.cpp
 src/Toolkit/Window.cpp
 src/Logs/LogBuffer.cpp
 src/Logs/LogBufferView.cpp
 src/Logs/MemoryMappedFile.cpp
 src/main.cpp
 src/ArgParser.cpp
 src/LogViewportWindow.cpp
 src/LogViewportController.cpp
 src/MainController.cpp
 src/Pane.cpp
 src/PanesContainer.cpp
 src/Statusbar.cpp
 src/PanesWindow.cpp
 src/PanesWindowController.cpp
 src/Bookmark.cpp
 src/BookmarksWindow.cpp
 src/BookmarksWindowController.cpp
 src/InputWindow.cpp
 src/InputWindowController.cpp
 src/LogLinePrinter.cpp
 src/ProgressWindow.cpp
 src/QuestionWindow.cpp
 src/QuestionWindowController.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSES REQUIRED ncurses)
pkg_check_modules(readline REQUIRED readline)

add_executable(inv ${SRC_FILES})
target_compile_options(inv PRIVATE -Wall -pedantic -Werror)
target_compile_features(inv PRIVATE cxx_std_17)
target_include_directories(inv PRIVATE ${NCURSES_INCLUDE_DIRS})
target_link_libraries(inv PRIVATE ${NCURSES_LIBRARIES} readline)
