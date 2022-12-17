#include "../gamelogic.h"
#include "../forces.h"

#if 1
int main()
{
	GameLogic game(Faction::any(), 4004030159);
	game.post(std::make_unique<ActionPrediction>(Faction::beneGesserit(), Faction::atreides(), 5));

	game.post(std::make_unique<ActionHarkonnenRedraw> (Faction::harkonnen(),    true));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::emperor(),      Leader::ID_Irulan));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::spacingGuild(), Leader::ID_Burseg));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::fremen(),       Leader::ID_Alia));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::atreides(),     Leader::ID_Feyd));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::beneGesserit(), Leader::ID_Duncan));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::tleilaxu(),     Leader::ID_Wanna));

	// fail
//	game.post(std::make_unique<ActionFremenPlacement>(Faction::fremen(), std::vector<Placement>{Placement{Arrakis::FalseWallSouth_5,105, 3},
//			                                                                                    Placement{Arrakis::FalseWallWest_18, 1, 6}}));

	// fail
//	game.post(std::make_unique<ActionFremenPlacement>(Faction::fremen(), std::vector<Placement>{Placement{Arrakis::FalseWallSouth_5, 1, 2},
//			                                                                                    Placement{Arrakis::FalseWallWest_18, 1, 6}}));
	// fail
//	game.post(std::make_unique<ActionFremenPlacement>(Faction::fremen(), std::vector<Placement>{Placement{Arrakis::FalseWallSouth_5,105, 3},
//			                                                                                    Placement{Arrakis::TheGreatFlat, 1, 6}}));

	// success
	game.post(std::make_unique<ActionFremenPlacement>(Faction::fremen(), std::vector<Placement>{Placement{FalseWallSouth_5, 6, 2},
			                                                                                    Placement{FalseWallWest_18, 1, 1}}));

	// advisor
	//game.post(std::make_unique<ActionBeneGesseritStartingForce>(Faction::beneGesserit(), Carthag));
	// advisor
	//game.post(std::make_unique<ActionBeneGesseritStartingForce>(Faction::beneGesserit(), Arrakeen));
	// advisor
	//game.post(std::make_unique<ActionBeneGesseritStartingForce>(Faction::beneGesserit(), TueksSietch));
	// fighter
	game.post(std::make_unique<ActionBeneGesseritStartingForce>(Faction::beneGesserit(), TheGreatFlat));

	game.post(std::make_unique<ActionStormInitialDial>(Faction::spacingGuild(), 13));
	game.post(std::make_unique<ActionStormInitialDial>(Faction::tleilaxu(), 19));

	game.post(std::make_unique<ActionChoamCharity>(Faction::harkonnen(),    false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::emperor(),      false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::fremen(),       false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::atreides(),     false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::beneGesserit(), true));
	game.post(std::make_unique<ActionChoamCharity>(Faction::tleilaxu(),     false));

	// AUCTION CARD 1
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), 1));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     2));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    3));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     4));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      5));

	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));

	// AUCTION CARD 2
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     1));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    2));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     3));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), 4));

	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    5));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     6));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));

	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    7));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));

	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));

	// AUCTION CARD 3
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));

	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));

	game.tick();
	return 0;
}
#endif
