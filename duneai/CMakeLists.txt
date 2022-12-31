# CMakeList.txt : CMake project for duneai, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("duneai")
set_property(GLOBAL PROPERTY CXX_STANDARD 14)

# Add source to this project's executable.
add_executable ( example1
	"examples/example.cc" 
)

target_link_libraries( example1 
    PRIVATE dunecore
)

add_library ( dunecore
	"actions.h" "actions.cc"
    "arrakis.h" "arrakis.cc"
	"auction.h" "auction.cc"
	"battleplan.h"
	"enumlabels.h"
	"event.h"
	"forces.h" "forces.cc"
	"gameconstants.h" "gameconstants.cc"
	"gamedata.h" "gamedata.cc"
	"gamelogic.h" "gamelogic.cc"
	"logger.h" "logger.cc"
	"savegame.h" "savegame.cc"
	"shipormove.h" "shipormove.cc"
	"systemevents.h"
    "statefunctions.h" "statefunctions.cc")
