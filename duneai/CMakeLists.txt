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

add_executable ( exampleDevClient
	"examples/exampledevclient.cc" 
)

target_link_libraries( exampleDevClient 
    PRIVATE dunecore
)

add_library( dunecore STATIC
	"actor.h" "actor.cpp"
	"actions.h" "actions.cc"
	"areaid.h"
	"arrakis.h" "arrakis.cc"
	"auction.h" "auction.cc"
	"battle.h" "battle.cpp"
	"broker.h" "broker.cpp"
	"battleplan.h"
	"conflict.h"
	"conflicts.h" "conflicts.cpp"
	"deletedefaults.h"
	"IDevActions.h"
	"enumlabels.h"
	"factionposition.h"
	"forces.h" "forces.cc"
	"game.h"
	"gameconstants.h" "gameconstants.cc"
	"gamedata.h" "gamedata.cc"
	"gamelogic.h" "gamelogic.cc"
	"GameMasterClient.h" "GameMasterClient.cpp"
	"game.h" "game.cc"
	"IPlayerActions.h"
	"json.h" "json.cc"
	"logger.h" "logger.cc"
	"message.h"
	"messagethread.h" "messagethread.cc"
	"partialterritory.h" "partialterritory.cpp"
	"resultcode.h"
	"shipormove.h" "shipormove.cc"
	"standardplayerclient.h" "standardplayerclient.cpp"
	"statefunctions.h" "statefunctions.cc"
	"stormposition.h"
	"territories.h" "territories.cpp"
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