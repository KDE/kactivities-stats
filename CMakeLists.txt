# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

cmake_minimum_required(VERSION 3.5)

set(KF5_VERSION "5.54.0") # handled by release scripts
set(KF5_DEP_VERSION "5.54.0") # handled by release scripts
project (KActivitiesStats VERSION ${KF5_VERSION})

set (REQUIRED_QT_VERSION 5.10.0)

# We don't build in-source
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   message (
      FATAL_ERROR
      "kactivities-stats require an out of source build. Please create a separate build directory and run 'cmake path_to_kactivities-stats [options]' there."
   )
endif ()

set (KASTATS_CURRENT_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Extra CMake stuff
include (FeatureSummary)
find_package (ECM 5.54.0  NO_MODULE)
set_package_properties (ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary (WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include (KDEInstallDirs)
include (KDECMakeSettings)
include (KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include (GenerateExportHeader)
include (ECMGenerateHeaders)
include (ECMAddQch)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

# Qt
set (CMAKE_AUTOMOC ON)
find_package (Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED COMPONENTS Core DBus Sql)

# KDE Frameworks
# find_package (KF5DBusAddons ${KF5_DEP_VERSION} CONFIG REQUIRED)
# find_package (KF5I18n ${KF5_DEP_VERSION} CONFIG REQUIRED)
find_package (KF5Config ${KF5_DEP_VERSION} CONFIG REQUIRED)
find_package (KF5Activities ${KF5_DEP_VERSION} CONFIG REQUIRED)

find_package (Boost 1.49 REQUIRED)

# Basic includes
include (CPack)

include (CMakePackageConfigHelpers)
include (ECMSetupVersion)


# Adding local CMake modules
set (
   CMAKE_MODULE_PATH
   ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
   ${CMAKE_MODULE_PATH}
   )

# add_definitions (-DTRANSLATION_DOMAIN=\"kactivitiesstats5\")
# if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
#     ki18n_install (po)
# endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   set(CMAKE_CXX_VISIBILITY_PRESET default)
   set(CMAKE_VISIBILITY_INLINES_HIDDEN 0)
endif ()

# libKActivitiesStats

ecm_setup_version (
   PROJECT
   VARIABLE_PREFIX KACTIVITIESSTATS
   VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kactivitiesstats_version.h"
   PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5ActivitiesStatsConfigVersion.cmake"
   SOVERSION 1
   )

add_subdirectory (src)
if(BUILD_TESTING)
  add_subdirectory (autotests)
  add_subdirectory (tests)
endif (BUILD_TESTING)

set (CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5ActivitiesStats")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5ActivitiesStats_QCH
        FILE KF5ActivitiesStatsLibraryQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5ActivitiesStatsLibraryQchTargets.cmake\")")
endif()

install (
   EXPORT KF5ActivitiesStatsLibraryTargets
   DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
   FILE KF5ActivitiesStatsLibraryTargets.cmake
   NAMESPACE KF5::
   )

configure_package_config_file (
   "${CMAKE_CURRENT_SOURCE_DIR}/KF5ActivitiesStatsConfig.cmake.in"
   "${CMAKE_CURRENT_BINARY_DIR}/KF5ActivitiesStatsConfig.cmake"
   INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
   PATH_VARS  KF5_INCLUDE_INSTALL_DIR CMAKE_INSTALL_PREFIX
   )

install (
   FILES "${CMAKE_CURRENT_BINARY_DIR}/KF5ActivitiesStatsConfig.cmake"
         "${CMAKE_CURRENT_BINARY_DIR}/KF5ActivitiesStatsConfigVersion.cmake"
   DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
   COMPONENT Devel
   )

install (
   FILES ${CMAKE_CURRENT_BINARY_DIR}/kactivitiesstats_version.h
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
   )

# Write out the features
feature_summary (WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

