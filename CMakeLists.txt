cmake_minimum_required(VERSION 2.8)
project(test)

find_package(catkin REQUIRED COMPONENTS
        )

catkin_package(
        # INCLUDE_DIRS Motion/
        # LIBRARIES #TODO: which libraries are published?
        # CATKIN_DEPENDS tclap RhAL model robocup_referee rhoban_graphs rhoban_team_play rhoban_random rhoban_unsorted rhoban_model_learning
)

#Build option
option (BUILD_TEST_VISION "Build the Vision" ON)
option (BUILD_VISION_OPENCL "Build vision with OpenCL support" OFF)
option (BUILD_TEST_PROGRAM_VISION "Enable the build of Vision Programs" OFF)
option (BUILD_TEST_PROGRAM_STRATEGY "Enable the build of Strategy programs" OFF)

#Build the main test binary
add_executable (Test Test/main.cpp)

