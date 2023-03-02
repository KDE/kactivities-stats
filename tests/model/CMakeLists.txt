# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:
project (KActivitiesStatsTestApp)

find_package (Qt6 REQUIRED NO_MODULE COMPONENTS Core Gui Widgets Test Quick QuickWidgets)

if (NOT WIN32)

add_executable (KActivitiesStatsTestApp)

target_include_directories (KActivitiesStatsTestApp PRIVATE
   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/src
   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/autotests
   ${CMAKE_BINARY_DIR}/src
)

qt_wrap_ui(KActivitiesStatsTestApp_SRCS
   window.ui
)

qt_add_resources(KActivitiesStatsTestApp_SRCS
   main.qrc
)

target_sources(KActivitiesStatsTestApp PRIVATE
   ${KActivitiesStatsTestApp_SRCS}
   window.cpp
   main.cpp
)

target_link_libraries (KActivitiesStatsTestApp
   Qt6::Core
   Qt6::Gui
   Qt6::Widgets
   Qt6::Test
   Qt6::Quick
   Qt6::QuickWidgets

   KF6::Activities
   KF6::ActivitiesStats
)

endif ()
