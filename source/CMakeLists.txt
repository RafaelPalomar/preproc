cmake_minimum_required(VERSION 3.2.1)

project(preproc)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(preproc
  main.cxx
  )

target_link_libraries( preproc
  ${ITK_LIBRARIES}
)
