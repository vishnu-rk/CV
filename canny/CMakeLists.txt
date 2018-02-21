cmake_minimum_required(VERSION 2.8)
project( Canny)
find_package( OpenCV REQUIRED )
include_directories(include)

add_library(implementation src/canny.cpp)
# include_directories("${PROJECT_SOURCE_DIR}/implementation")
# add_subdirectory (implementation) 



add_executable( Canny src/main.cpp)
target_link_libraries( Canny implementation ${OpenCV_LIBS} )