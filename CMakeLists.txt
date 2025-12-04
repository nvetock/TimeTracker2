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
        Svg
        SvgWidgets
)

# ---- Sources ----
# Add more source/header files here as you build out the app.
set(TIMETRACKER_SOURCES
        main.cpp

        # domain linking
        src/WorkSession.cpp

        # infrastructure linking
        src/AppSettingsRepository.cpp
        include/infra/AppSettingsRepository.h
        src/SessionLogRepository.cpp
        include/infra/SessionLogRepository.h

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
        src/ui/BaseCardPage.cpp
        src/ui/TrackWorkTimerPage.cpp
        src/ui/TrackWorkSetupPage.cpp
        src/ui/ExportPage.cpp
        src/ui/LogHistoryPage.cpp
        src/ui/SettingsPage.cpp
        src/ui/NewItemDialog.cpp
        src/ui/UiFlowController.h
        src/ui/UiFlowController.cpp
        src/ui/MainMenuPage.cpp
        src/ui/InlineEditableLabel.cpp
        src/ui/ClickableSvgWidget.h

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
        Qt6::Svg
        Qt6::SvgWidgets
)

# ---- Include dirs (optional if you keep everything in src/) ----
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        # ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---- Platform-specific deployment helpers ----
# NOTE: This is a simple dev-time helper so the app runs from the build tree.
# For real deployment, use windeployqt / macdeployqt.

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

    # Copy image format plugin for SVG (qsvg) – nice to have
    if (EXISTS "${QT_INSTALL_PATH}/plugins/imageformats/qsvg${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/"
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/imageformats/qsvg${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/"
        )
    endif ()

    # Copy core Qt DLLs this app depends on
    # 👇 Added Svg and SvgWidgets here
    foreach(QT_LIB Core Gui Widgets Svg SvgWidgets)
        if (EXISTS "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
            )
        endif ()
    endforeach()
endif ()

if (APPLE AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # Try to infer Qt install path from CMAKE_PREFIX_PATH
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()

    set(MACDEPLOYQT_EXECUTABLE "${QT_INSTALL_PATH}/bin/macdeployqt")

    if (EXISTS "${MACDEPLOYQT_EXECUTABLE}")
        # For Qt 6, CMake usually builds a bundle (.app) for a Widgets app.
        # This runs macdeployqt on the bundle after build.
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND "${MACDEPLOYQT_EXECUTABLE}"
                "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
                -verbose=1
                VERBATIM
        )
    else ()
        message(WARNING "macdeployqt not found at: ${MACDEPLOYQT_EXECUTABLE}")
    endif ()
endif ()
