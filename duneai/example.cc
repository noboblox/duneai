#include "gamelogic.h"
#include "forces.h"

//int main()
//{
//	GameLogic game(Faction::any(), 4004030159);
//	game.post(std::make_unique<ActionPrediction>(Faction::beneGesserit(), Faction::atreides(), 5));
//
//	game.post(std::make_unique<ActionHarkonnenRedraw> (Faction::harkonnen(),    true));
//	game.post(std::make_unique<ActionTraitorSelection>(Faction::emperor(),      Leader::ID_Irulan));
//	game.post(std::make_unique<ActionTraitorSelection>(Faction::spacingGuild(), Leader::ID_Burseg));
//	game.post(std::make_unique<ActionTraitorSelection>(Faction::fremen(),       Leader::ID_Alia));
//	game.post(std::make_unique<ActionTraitorSelection>(Faction::atreides(),     Leader::ID_Feyd));
//	game.post(std::make_unique<ActionTraitorSelection>(Faction::beneGesserit(), Leader::ID_Duncan));
//	game.post(std::make_unique<ActionTraitorSelection>(Faction::tleilaxu(),     Leader::ID_Wanna));
//
//	// fail
////	game.post(std::make_unique<ActionFremenPlacement>(Faction::fremen(), std::vector<Placement>{Placement{Arrakis::FalseWallSouth_5,105, 3},
////			                                                                                    Placement{Arrakis::FalseWallWest_18, 1, 6}}));
//
//	// fail
////	game.post(std::make_unique<ActionFremenPlacement>(Faction::fremen(), std::vector<Placement>{Placement{Arrakis::FalseWallSouth_5, 1, 2},
////			                                                                                    Placement{Arrakis::FalseWallWest_18, 1, 6}}));
//	// fail
////	game.post(std::make_unique<ActionFremenPlacement>(Faction::fremen(), std::vector<Placement>{Placement{Arrakis::FalseWallSouth_5,105, 3},
////			                                                                                    Placement{Arrakis::TheGreatFlat, 1, 6}}));
//
//	// success
//	game.post(std::make_unique<ActionFremenPlacement>(Faction::fremen(), std::vector<Placement>{Placement{FalseWallSouth_5, 5, 3},
//			                                                                                    Placement{FalseWallWest_18, 1, 1}}));
//
//	// advisor
//	//game.post(std::make_unique<ActionBeneGesseritStartingForce>(Faction::beneGesserit(), Carthag));
//	// advisor
//	//game.post(std::make_unique<ActionBeneGesseritStartingForce>(Faction::beneGesserit(), Arrakeen));
//	// advisor
//	//game.post(std::make_unique<ActionBeneGesseritStartingForce>(Faction::beneGesserit(), TueksSietch));
//	// fighter
//	game.post(std::make_unique<ActionBeneGesseritStartingForce>(Faction::beneGesserit(), TheGreatFlat));
//
//
//	game.tick();
//	return 0;
//}
