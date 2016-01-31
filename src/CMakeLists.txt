# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

set (
   KActivitiesStats_LIB_SRCS

   query.cpp
   terms.cpp
   resultset.cpp
   resultwatcher.cpp
   resultmodel.cpp
   activitiessync_p.cpp
   cleaning.cpp

   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/src/common/database/Database.cpp
   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/src/common/database/schema/ResourcesDatabaseSchema.cpp
   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/src/utils/qsqlquery_iterator.cpp

   version.cpp
   )

qt5_add_dbus_interface (
   KActivitiesStats_LIB_SRCS

   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/src/common/dbus/org.kde.ActivityManager.ResourcesScoring.xml
   resourcesscoring_interface
   )

qt5_add_dbus_interface (
   KActivitiesStats_LIB_SRCS

   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/src/common/dbus/org.kde.ActivityManager.ResourcesLinking.xml
   resourceslinking_interface
   )



add_library (
   KF5ActivitiesStats SHARED
   ${KActivitiesStats_LIB_SRCS}
   )
add_library(KF5::ActivitiesStats ALIAS KF5ActivitiesStats)

include_directories (
   ${KASTATS_CURRENT_ROOT_SOURCE_DIR}/src
   )

set_target_properties (
   KF5ActivitiesStats
   PROPERTIES
   VERSION "5.19.0"
   SOVERSION "5"
   EXPORT_NAME ActivitiesStats
   )

target_link_libraries (
   KF5ActivitiesStats
   PUBLIC
      Qt5::Core
   PRIVATE
      Qt5::DBus
      Qt5::Sql
      KF5::Activities
      KF5::ConfigCore
   )

target_include_directories (
   KF5ActivitiesStats
   INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KActivitiesStats>"
   )

# install
generate_export_header (KF5ActivitiesStats BASE_NAME KActivitiesStats)

ecm_generate_headers (
   KActivitiesStats_CamelCase_HEADERS
   HEADER_NAMES
   Query
   Terms
   ResultSet
   ResultWatcher
   ResultModel
   Cleaning
   Version

   PREFIX KActivitiesStats
   REQUIRED_HEADERS KActivitiesStats_HEADERS
)

install (
   TARGETS KF5ActivitiesStats
   EXPORT KF5ActivitiesStatsLibraryTargets
   ${SKIP_NAMELINK}
   ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
   )

install (
   FILES ${KActivitiesStats_CamelCase_HEADERS}
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KActivitiesStats/KActivities/Stats
   COMPONENT Devel
   )

install (
   FILES ${KActivitiesStats_HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/kactivitiesstats_export.h
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KActivitiesStats/kactivitiesstats
   COMPONENT Devel
   )

if (NOT WIN32)
   configure_file (
      ${CMAKE_CURRENT_SOURCE_DIR}/libKActivitiesStats.pc.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/libKActivitiesStats.pc
      )
   install (
      FILES ${CMAKE_CURRENT_BINARY_DIR}/libKActivitiesStats.pc
      DESTINATION ${KDE_INSTALL_LIBDIR}/pkgconfig
      )
endif ()

include (ECMGeneratePriFile)
ecm_generate_pri_file (
   BASE_NAME KActivitiesStats
   LIB_NAME KF5ActivitiesStats
   DEPS "KActivities"
   FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KF5_INCLUDE_INSTALL_DIR}/KActivitiesStats
   )
install (
   FILES ${PRI_FILENAME}
   DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
   )


