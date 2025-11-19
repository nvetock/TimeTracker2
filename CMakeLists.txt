cmake_minimum_required(VERSION 3.21)

project(TimeTracker
        VERSION 0.1.0
        LANGUAGES CXX
)

# ---- C++ standard ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Qt automatic tools ----
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# If you ever need Qt5 fallback, this can be extended,
# but for now we assume Qt6.
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
)

# ---- Sources ----
# Add more source/header files here as you build out the app.
set(TIMETRACKER_SOURCES
        main.cpp

        # domain linking
        src/WorkSession.cpp

        # infrastructure linking

        # idle detector linking
        include/idle/IdleDetectorFactory.h
        src/MacIdleDetector.cpp
        src/WinIdleDetector.cpp
        src/IdleDetectorFactory.cpp

        # application service linking
        include/appservice/ActivityMonitor.h
        src/ActivityMonitor.cpp
        include/appservice/AppController.h
        src/AppController.cpp
        include/appservice/ReminderScheduler.h
        src/ReminderScheduler.cpp
        include/appservice/SessionManager.h
        src/SessionManager.cpp

        # ui linking
        src/ui/TimeTrackerPanel.h
        src/ui/TimeTrackerPanel.cpp
        src/ui/SideTab.h
        src/ui/SideTab.cpp
        src/ui/GradientFrame.h
)


# ---- Executable / bundle settings ----
add_executable(${PROJECT_NAME}
        ${TIMETRACKER_SOURCES}
)
# ---- Add qt font resources path
qt_add_resources(APP_RESOURCES resources.qrc)
target_sources(${PROJECT_NAME} PRIVATE main.cpp ${APP_RESOURCES})

# On Windows, WIN32_EXECUTABLE hides the console window.
# On macOS, MACOSX_BUNDLE builds a .app bundle.
if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE FALSE
    )
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.timetracker"
            MACOSX_BUNDLE_BUNDLE_NAME "TimeTracker"
    )
endif()

# ---- Link Qt ----
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# ---- Include dirs (optional if you keep everything in src/) ----
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        # ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---- Platform-specific deployment helpers ----
# Note: deployment (copying Qt DLLs / frameworks next to the app)
# is usually handled by Qt's windeployqt / macdeployqt tools after build.
# The block below is a simple Windows helper that copies the main Qt DLLs
# into the build folder so the EXE runs from the build tree.

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX "")
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()

    # Try to infer Qt install path from CMAKE_PREFIX_PATH
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()

    # Copy platform plugin (qwindows)
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
        )
    endif ()

    # Copy core Qt DLLs this app depends on
    foreach(QT_LIB Core Gui Widgets)
        if (EXISTS "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
            )
        endif ()
    endforeach()
endif ()
