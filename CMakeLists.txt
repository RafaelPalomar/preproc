cmake_minimum_required(VERSION 3.9.5)

project(preproc)

find_package(ITK REQUIRED)

add_executable(preproc
  main.cxx
  )

target_link_libraries( preproc
  ${ITK_LIBRARIES}
)
