cmake_minimum_required (VERSION 2.6)
project (SvarunAssembly)

set (
	Source
	main.cpp
	token.cpp
	conversion.cpp
	filesystem.cpp
	opcodes.cpp
)
## Create project
add_executable( ${PROJECT_NAME} ${Source})

## Set flags for project
set_property(
	TARGET 
	${PROJECT_NAME} 
	PROPERTY 
	CXX_STANDARD 17
)

## Set include files for project
target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	../../SvarunCommon/include
	../include
	include
)