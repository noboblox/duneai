#include "../gamelogic.h"
#include "../forces.h"

void round1AuctionPhase(GameLogic& game);
void round1Shipment(GameLogic& game);
void round1Battle(GameLogic& game);

int main()
{
	GameLogic game;


	game.addFaction(Faction::harkonnen(),    true);
	game.addFaction(Faction::emperor(),      false);
	game.addFaction(Faction::spacingGuild(), false);
	game.addFaction(Faction::fremen(),       false);
	game.addFaction(Faction::atreides(),     false);
	game.addFaction(Faction::beneGesserit(), false);
	game.addFaction(Faction::tleilaxu(),     false);
	game.setup(4004030159);

	game.post(std::make_unique<ActionPrediction>(Faction::beneGesserit(), Faction::atreides(), 4));
	game.post(std::make_unique<ActionHarkonnenRedraw> (Faction::harkonnen(),    true));

	game.post(std::make_unique<ActionTraitorSelection>(Faction::emperor(), Leader::ID_Irulan));
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

	game.post(std::make_unique<ActionStormDial>(Faction::spacingGuild(), 13));
	game.post(std::make_unique<ActionStormDial>(Faction::beneGesserit(), 19));

	game.post(std::make_unique<ActionChoamCharity>(Faction::harkonnen(),    false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::emperor(),      false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::fremen(),       false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::atreides(),     false));
	game.post(std::make_unique<ActionChoamCharity>(Faction::beneGesserit(), true));
	game.post(std::make_unique<ActionChoamCharity>(Faction::tleilaxu(),     false));

	round1AuctionPhase(game);
	round1Shipment(game);
	round1Battle(game);

	game.tick();
	return 0;
}

void round1AuctionPhase(GameLogic& game)
{
	// AUCTION CARD 1
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::BID,  1));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::BID,  2));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::BID,  3));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::BID,  4));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::BID,  5));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::BID,  6));

	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));

	// AUCTION CARD 2
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::BID, 1));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::BID, 2));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::BID, 3));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::BID, 4));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::BID, 5));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::PASS, 0));

	// AUCTION CARD 3
	game.post(std::make_unique<ActionBid>(Faction::fremen(),   BiddingAction::BID, 1));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(), BiddingAction::BID, 2));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),BiddingAction::BID, 3));
	game.post(std::make_unique<ActionBid>(Faction::atreides(), BiddingAction::BID, 4));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),  BiddingAction::KARAMA_BUY, 0));

	// AUCTION CARD 4
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::BID,  1));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::BID,  2));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::BID,  3));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::BID,  4));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::BID,  5));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::BID,  6));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::PASS, 0));

	// AUCTION CARD 5
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::BID, 1));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::BID, 2));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::BID, 3));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::BID,  4));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));

	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::BID,  5));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::BID,  6));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));
	
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::BID, 7));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));
	
	// AUCTION CARD 6
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::BID,  1));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::BID,  2));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::BID,  3));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::BID,  4));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::PASS, 0));
	
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::BID,  5));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::BID,  6));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::PASS, 0));
	
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::BID,  7));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::PASS, 0));

	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));

	// AUCTION CARD 7
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::BID, 1));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::BID, 2));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::BID, 3));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::BID, 4));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));
	
	game.post(std::make_unique<ActionBid>(Faction::emperor(),      BiddingAction::BID, 5));
	game.post(std::make_unique<ActionBid>(Faction::spacingGuild(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::beneGesserit(), BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::fremen(),       BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::tleilaxu(),     BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::harkonnen(),    BiddingAction::PASS, 0));
	game.post(std::make_unique<ActionBid>(Faction::atreides(),     BiddingAction::PASS, 0));
}

void round1Shipment(GameLogic& game)
{
	// 1. bene gesserit
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::beneGesserit(), Placement{AreaId::SietchTabr, 3}));
	game.post(std::make_unique<ActionMove>(Faction::beneGesserit(), AreaId::SietchTabr, Placement{AreaId::RockOutcroppings_14, 3}));

	// 2. fremen
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::fremen(), Placement{AreaId::FalseWallWest_18, 3}));
	game.post(std::make_unique<ActionMove>(Faction::fremen(), AreaId::FalseWallWest_18, Placement{AreaId::HabbanyaErg_16, 4}));

	// 3. tleilaxu
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::tleilaxu(), Placement{AreaId::Arrakeen, 2}));
	game.post(std::make_unique<ActionAccompanyDecision>(Faction::beneGesserit(), ActionAccompanyDecision::ACCOMPANY_SHIPMENT));
	game.post(std::make_unique<ActionMove>(Faction::tleilaxu(), AreaId::INVALID, Placement{}));

	// 4. harkonnen
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::harkonnen(), Placement{AreaId::INVALID, 0}));
	game.post(std::make_unique<ActionMove>(Faction::harkonnen(), AreaId::Carthag, Placement{AreaId::RockOutcroppings_14, 3}));
	game.post(std::make_unique<ActionIntrusionReaction>(Faction::beneGesserit(), false));

	// 5. areitdes
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::atreides(), Placement{AreaId::INVALID, 0}));
	game.post(std::make_unique<ActionMove>(Faction::atreides(), AreaId::INVALID, Placement{}));

	// 6. emperor
	game.post(std::make_unique<ActionGuildShipmentDecision>(Faction::spacingGuild(), false));
	game.post(std::make_unique<ActionShip>(Faction::emperor(), Placement{AreaId::TueksSietch, 2, 1}));
	game.post(std::make_unique<ActionAccompanyDecision>(Faction::beneGesserit(), ActionAccompanyDecision::ACCOMPANY_SHIPMENT));
	game.post(std::make_unique<ActionMove>(Faction::emperor(), AreaId::INVALID, Placement{}));

	// 7. spacing guild out of turn
	game.post(std::make_unique<ActionShip>(Faction::spacingGuild(), AreaId::TueksSietch, Placement{AreaId::SietchTabr, 5}));
	game.post(std::make_unique<ActionMove>(Faction::spacingGuild(), AreaId::SietchTabr, Placement{AreaId::RockOutcroppings_14, 5}));
	game.post(std::make_unique<ActionIntrusionReaction>(Faction::beneGesserit(), false));
}

void round1Battle(GameLogic& game)
{
}
