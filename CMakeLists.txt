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

#Define Vision enable Macro + specific includes
if (BUILD_TEST_VISION)
    enable_language(ASM)
    add_definitions(-DVISION_COMPONENT)
    add_definitions(-DENABLE_OPENCV)
    set (OpenCV_DIR /home/gregwar/opencv/build/)
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_DIR}/include)
    include_directories(${OpenCV_DIR}/include/opencv2)
    link_directories(${OpenCV_DIR}/lib ${OpenCV_DIR}/3rdparty/lib)

    # Use SSE2 and AVX if provided, otherwise, show a warning
    if ("${CMAKE_CXX_FLAGS}" MATCHES "-msse2" AND
            "${CMAKE_CXX_FLAGS}" MATCHES "-mavx")
        # Enable SSE2 optimizations
        add_definitions(-DCNN_USE_SSE)
        # Enable Advanced Vector Extensions
        add_definitions(-DCNN_USE_AVX)
    else()
        message(WARNING "missing flags for SSE2 and AVX! DNN will be really slow")
    endif()
endif ()

if (BUILD_VISION_OPENCL)
    add_definitions(-DENABLE_OPENCL)
endif ()

#Enable C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Enable real warnings
set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")

# Enable open mp support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")


#Enable Weird option linker
set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -fPIC")

#Included internal Directories
set (INCLUDED_DIRECTORIES
        # Motion
        ${catkin_INCLUDE_DIRS}
        )

if (BUILD_TEST_VISION)
    # set (INCLUDED_DIRECTORIES ${INCLUDED_DIRECTORIES} Vision)
endif()

include_directories(${INCLUDED_DIRECTORIES})

#Linked dependency external Libraries
set (LINKED_LIBRARIES
        pthread
        ${catkin_LIBRARIES}
        )

#Add Vision libraries
if (BUILD_TEST_VISION)
    set (LINKED_LIBRARIES
            ${LINKED_LIBRARIES}
            ${OpenCV_LIBS}
            #      v4l2 #v4l2 cameras are not used anymore
            # flycapture
            )
endif ()


#List Sources sub source directories
set(SOURCES_DIRECTORIES

        )

if (BUILD_TEST_VISION)
    set (SOURCES_DIRECTORIES
            ${SOURCES_DIRECTORIES}

            )
endif(BUILD_TEST_VISION)


#Include Sources sub sources
foreach (DIRECTORY ${SOURCES_DIRECTORIES})
    message(STATUS "-- Sources: Adding ${DIRECTORY}/")
    include (${DIRECTORY}/Sources.cmake)
    set (PREFIXED_SOURCES)
    foreach (SOURCE ${SOURCES})
        set (PREFIXED_SOURCES ${PREFIXED_SOURCES} ${DIRECTORY}/${SOURCE})
    endforeach (SOURCE)
    set (ALL_SOURCES ${ALL_SOURCES} ${PREFIXED_SOURCES})
endforeach (DIRECTORY)

#Build the test main library
add_library (test SHARED ${ALL_SOURCES})
target_link_libraries (test ${LINKED_LIBRARIES})

# Adding build infos in the main
exec_program(
        "git"
        ${CMAKE_CURRENT_SOURCE_DIR}
        ARGS "rev-parse HEAD"
        OUTPUT_VARIABLE VERSION_NUM )
exec_program(
        "whoami"
        ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE VERSION_USER )
exec_program(
        "hostname"
        ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE VERSION_HOSTNAME )

set_source_files_properties (
        Test/buildinfos.c
        PROPERTIES COMPILE_FLAGS
        "-DVERSION_USER=\"${VERSION_USER}\" -DVERSION_NUM=\"${VERSION_NUM}\" -DVERSION_HOSTNAME=\"${VERSION_HOSTNAME}\"")

#Force main recompilation to have up to date build info
add_custom_target(date ALL COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/Test/buildinfos.c)

#Build the main test binary
add_executable (Test Test/main.cpp)
target_link_libraries (Test ${LINKED_LIBRARIES} test)
add_dependencies(Test date)

if (BUILD_test_PROGRAM_STRATEGY)
    # add_executable(KickStrategy Motion/strategy/main.cpp)
    # target_link_libraries(KickStrategy ${LINKED_LIBRARIES} test)
endif ()

#Build Vision Programs
if (BUILD_test_PROGRAM_VISION)
    # Simple player
    add_executable(Player Vision/Examples/Player.cpp)
    target_link_libraries(Player ${LINKED_LIBRARIES} test)
endif ()

