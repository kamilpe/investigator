cmake_minimum_required (VERSION 3.1)
project (Investigator)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Werror")

set(SRC_FILES
 src/CursesToolkit/Display.cpp
 src/CursesToolkit/Keyboard.cpp
 src/CursesToolkit/Window.cpp
 src/Logs/LogBuffer.cpp
 src/Logs/LogBufferView.cpp
 src/main.cpp
 src/ArgParser.cpp
 src/LogViewport.cpp
 src/LogViewportController.cpp
 src/MainController.cpp
 src/Pane.cpp
 src/PanesContainer.cpp
 src/Statusbar.cpp
 src/ListWindow.cpp
 src/PanesWindowController.cpp
 src/InputWindow.cpp
 src/InputWindowController.cpp
 src/LogLinePrinter.cpp
 src/ProgressWindow.cpp
 src/QuestionWindow.cpp
 src/QuestionWindowController.cpp
)

find_package(PkgConfig)
pkg_check_modules(NCURSES REQUIRED ncurses)

add_executable(inv ${SRC_FILES})
target_include_directories(inv PRIVATE ${NCURSES_INCLUDE_DIRS})
target_link_libraries(inv ${NCURSES_LIBRARIES} readline)