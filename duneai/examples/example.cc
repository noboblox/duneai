#include "../gamelogic.h"
#include "../forces.h"

void round1AuctionPhase(GameLogic& game);
void round1Shipment(GameLogic& game);

int main()
{
	GameLogic game(Faction::any(), 4004030159);
	game.post(std::make_unique<ActionPrediction>(Faction::beneGesserit(), Faction::atreides(), 5));

	game.post(std::make_unique<ActionHarkonnenRedraw> (Faction::harkonnen(),    true));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::emperor(),      Leader::ID_Irulan));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::spacingGuild(), Leader::ID_Otheym));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::fremen(),       Leader::ID_Gurney));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::atreides(),     Leader::ID_Margot));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::beneGesserit(), Leader::ID_Jessica));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::tleilaxu(),     Leader::ID_Aramsha));

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
	game.post(std::make_unique<ActionBeneGesseritStartingForce>(Faction::beneGesserit(), Carthag));

	game.post(std::make_unique<ActionStormInitialDial>(Faction::spacingGuild(), 13));
	game.post(std::make_unique<ActionStormInitialDial>(Faction::beneGesserit(), 19));

	game.post(std::make_unique<ActionChoamCharity>(Faction::harkonnen(),    false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::emperor(),      false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::fremen(),       false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::atreides(),     false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::beneGesserit(), true));
	game.post(std::make_unique<ActionChoamCharity>(Faction::tleilaxu(),     false));

	round1AuctionPhase(game);
	round1Shipment(game);

	game.tick();
	return 0;
}

void round1AuctionPhase(GameLogic& game)
{
	// AUCTION CARD 1
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), 1));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), 2));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       3));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     4));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    5));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      6));

	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));

	// AUCTION CARD 2
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), 1));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       2));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     3));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    4));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), 5));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      ActionBid::PASS));

	// AUCTION CARD 3
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       1));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     2));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    3));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     4));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      ActionBid::KARAMA_BUY));

	// AUCTION CARD 4
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     1));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    2));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     3));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      4));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), 5));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     6));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));

	// AUCTION CARD 5
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    1));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     2));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      3));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), 4));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));

	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    5));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      6));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    7));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	
	// AUCTION CARD 6
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     1));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      2));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), 3));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     4));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));
	
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      5));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), 6));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));
	
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      7));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));

	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));

	// AUCTION CARD 7
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      1));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), 2));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       3));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     4));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
	
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      5));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    ActionBid::PASS));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     ActionBid::PASS));
}

void round1Shipment(GameLogic& game)
{
	// 1. bene gesserit
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::beneGesserit(), Placement{AreaId::SietchTabr, 3, 0}));
	game.post(std::make_unique<ActionMove>(Faction::beneGesserit(), AreaId::SietchTabr, Placement{AreaId::RockOutcroppings_14, 3, 0}));

	// 2. fremen
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::fremen(), Placement{AreaId::FalseWallWest_18, 3, 0}));
	game.post(std::make_unique<ActionMove>(Faction::fremen(), AreaId::FalseWallWest_18, Placement{AreaId::HabbanyaErg_16, 4, 0}));

	// 3. tleilaxu
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::tleilaxu(), Placement{AreaId::Arrakeen, 2, 0}));
	game.post(std::make_unique<ActionAccompanyDecision>(Faction::beneGesserit(), ActionAccompanyDecision::ACCOMPANY_SHIPMENT));
	game.post(std::make_unique<ActionMove>(Faction::tleilaxu(), AreaId::INVALID, Placement{}));

	// 4. harkonnen
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::harkonnen(), Placement{AreaId::INVALID, 0, 0}));
	game.post(std::make_unique<ActionMove>(Faction::harkonnen(), AreaId::Carthag, Placement{AreaId::RockOutcroppings_14, 3, 0}));
	game.post(std::make_unique<ActionIntrusionReaction>(Faction::beneGesserit(), false));

	// 5. areitdes
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::atreides(), Placement{AreaId::INVALID, 0, 0}));
	game.post(std::make_unique<ActionMove>(Faction::atreides(), AreaId::INVALID, Placement{}));

	// 6. emperor
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::emperor(), Placement{AreaId::TueksSietch, 2, 1}));
	game.post(std::make_unique<ActionAccompanyDecision>(Faction::beneGesserit(), ActionAccompanyDecision::ACCOMPANY_SHIPMENT));
	game.post(std::make_unique<ActionMove>(Faction::emperor(), AreaId::INVALID, Placement{}));

	// 7. spacing guild out of turn
	game.post(std::make_unique<ActionShip>(Faction::spacingGuild(), AreaId::TueksSietch, Placement{AreaId::SietchTabr, 5, 0}));
	game.post(std::make_unique<ActionMove>(Faction::spacingGuild(), AreaId::SietchTabr, Placement{AreaId::RockOutcroppings_14, 5, 0}));
	game.post(std::make_unique<ActionIntrusionReaction>(Faction::beneGesserit(), false));
}
