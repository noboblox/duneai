# CMakeList.txt : CMake project for duneai, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("duneai")
set_property(GLOBAL PROPERTY CXX_STANDARD 14)

#find_package(Boost
#    REQUIRED
#)

# Add source to this project's executable.
add_executable ( example1
	"examples/example.cc" 
)

target_link_libraries( example1 
    PRIVATE dunecore
)

add_library( dunecore STATIC
	"actions.h" "actions.cc"
	"arrakis.h" "arrakis.cc"
	"auction.h" "auction.cc"
	"battleplan.h"
	"deletedefaults.h"
	"enumlabels.h"
	"event.h"
	"forces.h" "forces.cc"
	"gameconstants.h" "gameconstants.cc"
	"gamedata.h" "gamedata.cc"
	"gamelogic.h" "gamelogic.cc"
	"game.h" "game.cc"
	"json.h" "json.cc"
	"logger.h" "logger.cc"
	"message.h"
	"messagethread.h" "messagethread.cc"
	"savegame.h" "savegame.cc"
	"shipormove.h" "shipormove.cc"
	"systemevents.h"
	"statefunctions.h" "statefunctions.cc"
	"game.h"
)

 target_include_directories( dunecore
	PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty
)

#add_executable( duneserver
#   "server/main.cc"
#   "server/server.h" "server/server.cc"
#   "server/connection.h")

#target_include_directories( duneserver
#	PRIVATE ${Boost_INCLUDE_DIRS}
#)

#target_link_libraries( duneserver
#	PRIVATE ${Boost_system_LIBRARY}
#	PRIVATE dunecore
#)