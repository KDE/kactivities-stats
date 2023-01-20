# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:
project (KActivitiesStatsTestApp)

find_package (Qt${QT_MAJOR_VERSION} REQUIRED NO_MODULE COMPONENTS Core Gui Widgets Test Quick QuickWidgets)

include_directories (
   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/src
   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/autotests
   ${CMAKE_BINARY_DIR}/src
   )

set (
   KActivitiesStatsTestApp_SRCS
   window.cpp
   main.cpp
   )

qt_wrap_ui(
   KActivitiesStatsTestApp_SRCS
   window.ui
   )

qt_add_resources(
   KActivitiesStatsTestApp_SRCS
   main.qrc
   )

if (NOT WIN32)

   add_executable (
      KActivitiesStatsTestApp
      ${KActivitiesStatsTestApp_SRCS}
      )

   target_link_libraries (
      KActivitiesStatsTestApp

      Qt${QT_MAJOR_VERSION}::Core
      Qt${QT_MAJOR_VERSION}::Gui
      Qt${QT_MAJOR_VERSION}::Widgets
      Qt${QT_MAJOR_VERSION}::Test
      Qt${QT_MAJOR_VERSION}::Quick
      Qt${QT_MAJOR_VERSION}::QuickWidgets

      KF6::Activities
      KF6::ActivitiesStats
      )

endif ()
