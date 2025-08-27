#----------------------------------------------------------------------------
# Geant4 Project CMakeLists.txt for CPNR_OMEG_colab (Final Compatibility Version)
#----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
project(CPNR_OMEG_colab)

#--- Find Geant4 and ROOT packages ---
find_package(Geant4 REQUIRED ui_all vis_all)
find_package(ROOT REQUIRED COMPONENTS Core Graf Tree)

#--- Setup Include Directories and Compile Definitions ---
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#--- Locate sources ---
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

#--- Add the executable and link to libraries ---
add_executable(CPNR_OMEG_colab CPNR_OMEG_colab.cc ${sources})
target_link_libraries(CPNR_OMEG_colab PRIVATE ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#--- Copy macro files to the build directory ---
set(PROJECT_SCRIPTS
  init_vis.mac
  run.mac
  source.mac
  label.mac
  batch_vis.mac
)
foreach(_script ${PROJECT_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()
