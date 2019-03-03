cmake_minimum_required(VERSION 2.8)
project(ROServer)

find_package(catkin REQUIRED COMPONENTS
    ShellService
    TL
)

catkin_package( CATKIN_DEPENDS
	ShellService
	TL
)
### include TL package
include_directories(${catkin_INCLUDE_DIRS})

add_executable (Test Test/main.cpp)

target_link_libraries(Test ${catkin_LIBRARIES})

