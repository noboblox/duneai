#include "gamelogic.h"

#include <algorithm>
#include <map>
#include "arrakis.h"
#include "gamedata.h"

std::vector<GameLogic::AllowedAction> GameLogic::msAllowedActions =
{
	{PHASE_INIT_JOIN,                   false,Faction::any(),                           ACTION_CONNECT},
	{PHASE_INIT_JOIN,                   false,Faction::any(),                           ACTION_DISCONNECT},
	{PHASE_INIT_PREDICTION,             true, Faction::beneGesserit(),                  ACTION_PREDICT},
	{PHASE_INIT_HARKONNEN_REDRAW,       true, Faction::harkonnen(),                     ACTION_HARKONNEN_REDRAW},
	{PHASE_INIT_TRAITOR_SELECTION,      true, Faction::anyExcept(Faction::harkonnen()), ACTION_TRAITOR_SELECTION},
	{PHASE_INIT_FREMEN_PLACEMENT,       true, Faction::fremen(),                        ACTION_FREMEN_PLACEMENT},
	{PHASE_INIT_BG_PLACEMENT,           true, Faction::beneGesserit(),                  ACTION_BENE_GESSERIT_START_FORCE},
	{PHASE_STORM_INITAL_DIAL,           true, Faction::any(),                           ACTION_STORM_DIAL},
	{PHASE_CHOAM_CHARITY,               true, Faction::any(),                           ACTION_CHOAM_CHARITY},
	{PHASE_AUCTION_BIDDING,             true, Faction::any(),                           ACTION_BID},
	{PHASE_SHIPMENT_GUILD_DECISION,     true, Faction::spacingGuild(),                  ACTION_GUILD_SHIPMENT_DECISION},
	{PHASE_SHIPMENT_SHIP,               true, Faction::any(),                           ACTION_SHIP},
	{PHASE_SHIPMENT_INTRUSION_REACTION, true, Faction::beneGesserit(),                  ACTION_INTRUSION_RESPONSE},
	{PHASE_SHIPMENT_ACCOMPANY_DECISION, true, Faction::beneGesserit(),                  ACTION_ACCOMPANY_SHIPMENT},
	{PHASE_SHIPMENT_MOVE,               true, Faction::any(),                           ACTION_MOVE},
};

std::vector<GameLogic::PhaseExecutionFunction> GameLogic::initPhaseFunctions(GameLogic& self)
{
	return
	{
		{PHASE_SPICE_SPICE_BLOW,       [&](GameState& g){ self.phaseSpiceSpiceBlow(g); }},
		{PHASE_BATTLE_COLLECT_BATTLES, [&](GameState& g){ self.prepareBattlePhase(g); }},
	};
};

GameLogic::GameLogic(bool developerActions)
: mUseDevActions(developerActions),
  mSetupWithoutDraw(false),
  mPhaseFunc(initPhaseFunctions(*this)),
  log(new StdoutLogger())
{
}

bool GameLogic::idle() noexcept 
{
	return mPending.empty();
}

bool GameLogic::addFaction(Faction faction, bool gameMaster) noexcept
{
	if (initialized)
		return false;

	if (!faction.exactlyOne() || inGame.contains(faction))
		return false;

	inGame |= faction;

	if (gameMaster)
		gameMasters |= faction;

	return true;
}

void GameLogic::removeFaction(Faction faction) noexcept
{
	if (initialized || !faction.exactlyOne())
		return;

	inGame.clear(faction);
	gameMasters.clear(faction);
}
void GameLogic::setup()
{
	std::random_device rd;
	setup(rd());
}

void GameLogic::setup(unsigned aSeed)
{
	if (!initialized)
	{
		init(mGame, inGame, aSeed, mSetupWithoutDraw);
		initialized = true;
	}
}


void GameLogic::tick()
{
	while (!mPending.empty())
		executeOne();
}

bool GameLogic::executeOne() 
{
	if (mPending.empty())
	{
		log->warn("called executeOne with empty queue");
		return false;
	}

	auto& ac = *mPending.front();

	bool result = executeAction(mGame, ac);
	if (result)
		record(std::unique_ptr<Action>(static_cast<Action*> (mPending.front().release())));
	else
		log->info("discard event %s from %s", ac.label(), ac.from().label().c_str());

	mPending.pop();
	return result;
}

void GameLogic::post(std::unique_ptr<Action>&& action)
{
	if (action != nullptr)
		mPending.push(std::move(action));
}

void GameLogic::setLogger(std::unique_ptr<const Logger>&& aLogger)
{
	log = std::move(aLogger);
}

//
//-- PHASES
//

bool GameLogic::executeAction(GameState& game, const Action& action)
{
	if (action.isUserAction())
		return userAction(game, action);
	else if (action.isGMAction() && gameMasters.contains(action.from()))
		return gameMasterAction(game, action);
	else if (mUseDevActions && action.isDevAction())
		return devAction(game, action);

	return false;
}

bool GameLogic::gameMasterAction(GameState& game, const Action& action)
{
	switch (action.type())
	{
	case GM_ACTION_START_GAME:
		setup();
		return true;
	default:
		return false;
	}
}

bool GameLogic::devAction(GameState& game, const Action& action)
{
	switch (action.type())
	{
	case DEV_ACTION_START_WITHOUT_DRAW:
		return devActionSetupWithoutDraw(game, action);
	case DEV_ACTION_DIAL_TREACHERY_CARD:
		return true; // TODO
	case DEV_ACTION_PLACE_TROOPS:
		return true; // TODO
	case DEV_ACTION_SET_STORM:
		return true; // TODO
	case DEV_ACTION_SET_GAME_PHASE:
		return devActionSetPhase(game, action);
	default:
		return false;
	}
}

bool GameLogic::userAction(GameState& game, const Action& action)
{
	if (!isAllowedAction(game, action))
		return false;

	switch (game.phase)
	{
	case PHASE_INIT_JOIN:
		return phaseInitJoin(action);
	case PHASE_INIT_PREDICTION:
		return phaseInitPrediction(game, action);
	case PHASE_INIT_HARKONNEN_REDRAW:
		return phaseInitHarkonnenRedraw(game, action);
	case PHASE_INIT_TRAITOR_SELECTION:
		return phaseInitTraitorSelect(game, action);
	case PHASE_INIT_FREMEN_PLACEMENT:
		return phaseInitFremenPlacement(game, action);
	case PHASE_INIT_BG_PLACEMENT:
		return phaseInitBeneGesseritPlacement(game, action);
	case PHASE_STORM_INITAL_DIAL:
		return phaseStormInitialStormDial(game, action);
	case PHASE_CHOAM_CHARITY:
		return phaseChoamCharity(game, action);
	case PHASE_AUCTION_BIDDING:
		return phaseBidding(game, action);
	case PHASE_SHIPMENT_GUILD_DECISION:
		return phaseShipmentGuildDecision(game, action);
	case PHASE_SHIPMENT_SHIP:
		return phaseShipmentShip(game, action);
	case PHASE_SHIPMENT_INTRUSION_REACTION:
		return phaseShipmentIntrusionReaction(game, action);
	case PHASE_SHIPMENT_ACCOMPANY_DECISION:
		return phaseShipmentAccompanyDecision(game, action);
	case PHASE_SHIPMENT_MOVE:
		return phaseShipmentMove(game, action);
	default:
		return false;
	}
}

bool GameLogic::phaseInitJoin(const Action& action)
{
	if (action.type() == ACTION_CONNECT)
	{
		auto ac = static_cast<const ActionConnect*> (&action);
		log->info("%s connected as %s", ac->from().label().c_str(), ac->asGameMaster ? "game master" : "standard player");

		return addFaction(ac->from(), ac->asGameMaster);
	}
	else if (action.type() == ACTION_DISCONNECT)
	{
		log->info("%s disconnected", action.from().label().c_str());
		removeFaction(action.from());
		return true;
	}

	return false;
}

bool GameLogic::phaseInitPrediction(GameState& game, const Action& action)
{
	auto prediction = expectedAction<ActionPrediction>(game, action, ACTION_PREDICT);
	if (!prediction) return false;

	if (!factionAvailable(game, prediction->winner()))
		return false;

	if (prediction->round() < 1 || prediction->round() > game.maxRound)
		return false;

	game.predictedFaction = prediction->winner();
	game.predictedTurn = prediction->round();
	log->info("set prediction to %s in turn %d", game.predictedFaction.label().c_str(), game.predictedTurn);

	if (harkonnenMayRedraw(game))
		advance(game, PHASE_INIT_HARKONNEN_REDRAW);
	else
		advance(game, PHASE_INIT_TRAITOR_SELECTION);
	return true;
}

bool GameLogic::phaseInitHarkonnenRedraw(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionHarkonnenRedraw>(game, action, ACTION_HARKONNEN_REDRAW);
	if (!ac) return false;

	if (ac->redraw == true)
	{
		log->info("harkonnen mulligan. discard and redraw new traitors");
		discardTraitors(game);
		game.traitors.reshuffle();
		drawTraitors(game);
	}

	advance(game, PHASE_INIT_TRAITOR_SELECTION);
	return true;
}

bool GameLogic::phaseInitTraitorSelect(GameState& game, const Action& action)
{
	auto selection = expectedAction<ActionTraitorSelection> (game, action, ACTION_TRAITOR_SELECTION);
	if (!selection) return false;

	auto* player = getPlayerState(game, action.from());
	if (!player) return false;

	bool found = false;
	for (std::size_t i = 0; i < player->selectedTraitors.size(); ++i)
	{
		if (player->selectedTraitors[i] == selection->selection())
		{
			found = true;
			break;
		}
	}

	if (!found)
		return false;

	log->info("%s selected traitor %s", selection->from().label().c_str(), EnumLeaderId::label(selection->selection()));
	player->selectedTraitors.clear();
	player->selectedTraitors.push_back(selection->selection());

	game.expectingInputFrom.clear(action.from());

	if (game.expectingInputFrom == Faction::none())
	{
		if (factionAvailable(game, Faction::fremen()))
		{
			advance(game, PHASE_INIT_FREMEN_PLACEMENT);
			return true;
		}

		placeStaticStartForces(game);

		if (factionAvailable(game, Faction::beneGesserit()))
			advance(game, PHASE_INIT_BG_PLACEMENT);
		else
			advance(game, PHASE_STORM_INITAL_DIAL, initialStormDialFactions(game));
	}

	return true;
}

bool GameLogic::phaseInitFremenPlacement(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionFremenPlacement>(game, action, ACTION_FREMEN_PLACEMENT);
	if (!ac) return false;

	auto player = getPlayerState(game, action.from());
	if (!player) return false;

	int sum_normals = 0;
	int sum_specials = 0;

	for (const auto& p : ac->placements)
	{
		if (!Arrakis::fremenInitArea(p.where))
		{
			log->warn("area %s is not a fremen init area", Arrakis::areaName(p.where));
			return false;
		}

		sum_normals += p.normal;
		sum_specials += p.special;
	}

	if (sum_normals + sum_specials != 10)
	{
		log->warn("sum of tokens to place %u != 10", sum_normals + sum_specials);
		return false;
	}

	if (sum_specials > player->specialForcesReserve)
	{
		log->warn("sum of fedaykin tokens to place %u > %u", sum_specials, player->specialForcesReserve);
		return false;
	}

	for (const auto& p : ac->placements)
	{
		game.board.placeHostile(ac->from(), p);
		player->reserve -= p.normal;
		player->specialForcesReserve -= p.special;
		log->info("add %u normals and %u fedaykin to area %s", p.normal, p.special, Arrakis::areaName(p.where));
		log->info("fremen now has %u normals and %u fedaykin in reserve", player->reserve, player->specialForcesReserve);
	}

	placeStaticStartForces(game);

	if (factionAvailable(game, Faction::beneGesserit()))
		advance(game, PHASE_INIT_BG_PLACEMENT);
	else
		advance(game, PHASE_STORM_INITAL_DIAL, initialStormDialFactions(game));

	return true;
}

bool GameLogic::phaseInitBeneGesseritPlacement(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionBeneGesseritStartingForce>(game, action, ACTION_BENE_GESSERIT_START_FORCE);
	if (!ac) return false;

	auto player = getPlayerState(game, action.from());
	if (!player) return false;

	if (game.board.hostileFactionsInTerritory(ac->where) > 0)
	{
		game.board.placeNeutral(player->faction, Placement{ac->where, 1, 0});
		player->reserve -= 1;
		log->info("place beneGesserit force in %s as advisor", Arrakis::areaName(ac->where));
	}
	else
	{
		game.board.placeHostile(player->faction, Placement{ac->where, 1, 0});
		player->reserve -= 1;
		log->info("place beneGesserit force in %s as fighter", Arrakis::areaName(ac->where));
	}

	advance(game, PHASE_STORM_INITAL_DIAL, initialStormDialFactions(game));
	return true;
}

bool GameLogic::phaseStormInitialStormDial(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionStormDial>(game, action, ACTION_STORM_DIAL);
	if (!ac) return false;

	if (ac->dial < 0 || ac->dial > 20)
		return false;

	if (game.initialStormDial[0].first == Faction::none())
		game.initialStormDial[0] = std::make_pair(action.from(), ac->dial);
	else
		game.initialStormDial[1] = std::make_pair(action.from(), ac->dial);

	game.expectingInputFrom.clear(action.from());

	if (game.expectingInputFrom != Faction::none())
		return true;

	const auto& dial = game.initialStormDial;
	log->info("initial storm dial: %d (%s) + %d (%s) = %d", dial[0].second, dial[0].first.label().c_str(),
			  dial[1].second, dial[1].first.label().c_str(), dial[0].second + dial[1].second);

	const int toAdvance = (dial[0].second + dial[1].second) % 18;
	log->info("initial storm advances %d sectors", toAdvance);
	const int storm = game.board.advanceStorm(toAdvance);
	log->info("storm at sector %d", storm);


	advance(game, PHASE_SPICE_SPICE_BLOW);
	return true;
}

void GameLogic::phaseSpiceSpiceBlow(GameState& game)
{
	// TODO basic rule 1 blow
	// TODO round > 1

	int territoriesDrawn = 0;
	int shaiHuluds = 0;

	while (territoriesDrawn < 2)
	{
		const auto& card = game.spiceDeck.draw();
		if (card.id() == SpiceCard::SHAI_HULUD)
		{
			++shaiHuluds;
			game.spiceDeck.placeDrawBottom(card);
			log->info("shai hulud is ignored in round 1");
		}
		else
		{
			++territoriesDrawn;

			if (game.board.insideStorm(card.area()))
			{
				log->info("ignore spice area %s inside storm", Arrakis::areaName(card.area()));
			}
			else
			{
				game.board.addSpice(card.area(), card.base());
				log->info("add %d spice to %s", card.base(), Arrakis::areaName(card.area()));
			}


			if (territoriesDrawn == 1)
				game.spiceDeck.discardA(card);
			else
				game.spiceDeck.discardB(card);
		}
	}

	if (shaiHuluds > 0)
	{
		game.spiceDeck.reshuffle();
		log->info("drawn %d shai huluds -> reshuffle deck", shaiHuluds);
	}

	advance(game, PHASE_CHOAM_CHARITY);
}

bool GameLogic::phaseChoamCharity(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionChoamCharity>(game, action, ACTION_CHOAM_CHARITY);
	if (!ac) return false;

	auto player = getPlayerState(game, action.from());

	if (ac->need)
	{
		int amount = 2;

		if (action.from() != Faction::beneGesserit())
			amount -= player->spice;

		if (amount <= 0)
			return false;

		player->spice += amount;
		log->info("%s receives %d spice", action.from().label().c_str(), amount);
	}

	game.expectingInputFrom.clear(action.from());

	if (game.expectingInputFrom == Faction::none())
	{
		if (prepareAuction(game) > 0)
			advance(game, PHASE_AUCTION_BIDDING, game.auction.currentBidder());
		else
			advanceToShipmentPhase(game);
	}

	return true;
}

bool GameLogic::phaseBidding(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionBid>(game, action, ACTION_BID);
	if (!ac) return false;

	auto player = getPlayerState(game, action.from());
	if (!player) return false;

	auto& auction = game.auction;

	if (auction.currentBidder() != ac->from())
	{
		log->info("ignore unexpected bid from %s. Expected: %s", ac->from().label().c_str(),
				auction.currentBidder().label().c_str());
		return false;
	}

	if (ac->type == BiddingAction::BID)
	{
		if (ac->amount > player->spice || ac->amount <= auction.currentBid())
		{
			log->info("bid %d is not allowed. Player spice: %d, Current bid: %d", ac->amount, player->spice, auction.currentBid());
			return false;
		}

		log->info("%s bids %d", ac->from().label().c_str(), ac->amount);
		auction.bid(ac->amount);
	}
	else if (ac->type == BiddingAction::KARAMA_BUY)
	{
		if (!hasKarama(game, ac->from()))
		{
			log->info("cannot karama buy without karama card");
			return false;
		}

		log->info("%s uses karama buy", ac->from().label().c_str());
		auction.karamaWin();
	}
	else if (ac->type == BiddingAction::KARAMA_BID)
	{
		if (!hasKarama(game, ac->from()))
		{
			log->info("no karama available for bid");
			return false;
		}
		if (ac->amount <= auction.currentBid())
		{
			log->info("bid %d is too small. Current bid: %d", ac->amount, auction.currentBid());
			return false;
		}

		log->info("%s bids %d using karama", ac->from().label().c_str(), ac->amount);
		auction.bid(ac->amount, true);
	}
	else if (ac->type == BiddingAction::PASS)
	{
		log->info("%s passes", ac->from().label().c_str());
		auction.pass();
	}
	else
	{
		return false;
	}

	if (!auction.roundFinished())
	{
		game.expectingInputFrom = auction.currentBidder();
		log->info("-> next bid by %s", auction.currentBidder().label().c_str());
		return true;
	}

	if (auction.winner() != Faction::none())
	{
		log->info("%s wins round %d", auction.winner().label().c_str(), auction.currentRound());
		auctionWinTransaction(game, auction.winner(), auction.winningCost(), auction.wasKaramaWin());
	}

	if (auction.nextRound())
	{
		log->info("auction round %d, start with %s", auction.currentRound(), auction.currentBidder().label().c_str());
		game.expectingInputFrom = auction.currentBidder();
	}
	else
	{
		log->info("auction phase finished");
		cleanupAuctionPool(game);
		advanceToShipmentPhase(game);
	}

	return true;
}

bool GameLogic::phaseShipmentGuildDecision(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionGuildShipmentDecision>(game, action, ACTION_GUILD_SHIPMENT_DECISION);
	if (!ac) return false;

	auto& shipper = game.shipper;

	if (ac->shipNow)
		shipper.forceGuildShipment();
	else
		shipper.delayGuildShipment();

	advanceInShipmentPhase(game);
	return true;
}

bool GameLogic::phaseShipmentShip(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionShip>(game, action, ACTION_SHIP);
	if (!ac) return false;

	auto& shipper = game.shipper;

	if (ac->from() != shipper.currentlyShipping())
		return false;

	bool success = false;

	if (ac->to.where == AreaId::INVALID)
		success = shipper.passShipment();
	else if (ac->fromReserve && !ac->inverted)
		success = shipper.shipFromReserve(ac->to.where, ac->to.normal, ac->to.special);
	else if (ac->fromArea && !ac->inverted)
		success = shipper.shipCrossPlanet(ac->fromArea, ac->to.where, ac->to.normal, ac->to.special);
	else if (ac->fromReserve)
		success = shipper.shipToReserve(ac->to.where, ac->to.normal, ac->to.special);
	else if (ac->fromArea)
		success = shipper.shipCrossPlanet(ac->to.where, ac->fromArea, ac->to.normal, ac->to.special);

	if (!success)
		return false;

	advanceInShipmentPhase(game);
	return true;
}

bool GameLogic::phaseShipmentIntrusionReaction(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionIntrusionReaction>(game, action, ACTION_INTRUSION_RESPONSE);
	if (!ac) return false;

	if (ac->disengage)
		game.shipper.disengageIntrusion();
	else
		game.shipper.engageIntrusion();

	advanceInShipmentPhase(game);
	return true;
}

bool GameLogic::phaseShipmentAccompanyDecision(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionAccompanyDecision>(game, action, ACTION_ACCOMPANY_SHIPMENT);
	if (!ac) return false;

	switch (ac->decision)
	{
	case ActionAccompanyDecision::PASS:
		game.shipper.keepAdvisor();
		break;
	case ActionAccompanyDecision::ACCOMPANY_SHIPMENT:
		game.shipper.sendAdvisor(false);
		break;
	case ActionAccompanyDecision::SEND_TO_POLAR_SINK:
		game.shipper.sendAdvisor(true);
		break;
	default:
		return false;
	}

	advanceInShipmentPhase(game);
	return true;
}

bool GameLogic::phaseShipmentMove(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionMove>(game, action, ACTION_MOVE);
	if (!ac) return false;

	if (ac->to.where == AreaId::INVALID)
		game.shipper.passMovement();
	else if (!game.shipper.move(ac->fromArea, ac->to.where, ac->to.normal, ac->to.special, ac->useHajr, ac->asAdvisor))
		return false;

	advanceInShipmentPhase(game);
	return true;
}

bool GameLogic::phaseBattle(GameState& game, const Action& action)
{
	return false;
}

//
//-- AUXILIARY
//

bool GameLogic::expected(GameState& game, Faction faction)
{
	return game.expectingInputFrom.contains(faction);
}

void GameLogic::init(GameState& game, Faction factionsInGame, unsigned aSeed, bool aNoDraw)
{
	const auto factions = Faction::expand(factionsInGame);

	if (factions.size() < 2)
	{
		log->crit("cannot instantiate games with less than 2 players");
		return;
	}
	if (factions.size() > 10)
	{
		log->crit("cannot instantiate games with more than 10 players");
		return;
	}

	mGame.seed = aSeed;
	mGame.random = std::mt19937(mGame.seed);
	log->info("set up game with seed %u", mGame.seed);

	std::vector<int> seats = SeatConfig::getConfig((int) factions.size());
	std::shuffle(seats.begin(), seats.end(), mGame.random);

	for (size_t i = 0; i < factions.size(); ++i)
	{
		mGame.players.push_back(PlayerState::create(seats[i], factions[i]));
		log->info("player %s at position %u", mGame.players.back().faction.label().c_str(), mGame.players.back().seat);
	}

	mGame.board = Arrakis(seats, factions);
	mGame.traitors = TraitorDeck(factionsInGame, mGame.random);
	mGame.spiceDeck = SpiceDeck(mGame.random);
	mGame.treacheryDeck = TreacheryDeck(mGame.random);

	if (!aNoDraw)
		initCards(mGame);

	if (factionAvailable(mGame, Faction::beneGesserit()))
		advance(mGame, PHASE_INIT_PREDICTION);
	else if (harkonnenMayRedraw(mGame))
		advance(mGame, PHASE_INIT_HARKONNEN_REDRAW);
	else
		advance(mGame, PHASE_INIT_TRAITOR_SELECTION);
}


void GameLogic::initCards(GameState& game)
{
	drawTraitors(mGame);

	for (auto& p : mGame.players)
	{
		p.hand.push_back(mGame.treacheryDeck.draw());
		log->info("starting card %s for %s", TreacheryCardLabels::label(p.hand[0].id()), p.faction.label().c_str());

		if (p.faction == Faction::harkonnen())
		{
			p.hand.push_back(mGame.treacheryDeck.draw());
			log->info("starting card %s for %s", TreacheryCardLabels::label(p.hand[1].id()), p.faction.label().c_str());
		}
	}
}



void GameLogic::record(std::unique_ptr<const Action>&& action)
{
	mRecorded.push_back(std::move(action));
}

template <typename A>
const A* GameLogic::expectedAction(GameState& game, const Action& action, ActionType type)
{
	if (action.type() != type)
		return nullptr;
	if (!expected(game, action.from()))
		return nullptr;
	return static_cast<const A*> (&action);
}

int GameLogic::prepareAuction(GameState& game)
{
	game.auction = Auction(game);
	const int eligible = game.auction.eligible();

	for (int i = 0; i < eligible; ++i)
	{
		game.biddingPool.insert(game.biddingPool.begin(), game.treacheryDeck.draw());
	}

	log->info("prepare auction with %d cards", game.auction.eligible());
	return eligible;
}

void GameLogic::prepareBattlePhase(GameState& game)
{
	game.conflicts = game.board.collectConflicts();

	for (const auto& c : game.conflicts)
		log->info("conflict in %s between %s",
				Arrakis::areaName(c.forces().front().where), c.parties().label().c_str());
}

void GameLogic::cleanupAuctionPool(GameState& game)
{
	if (game.biddingPool.empty())
		return;

	log->info("%u cards were not auctioned and will be placed back onto the deck", game.biddingPool.size());

	std::for_each(game.biddingPool.cbegin(), game.biddingPool.cend(),
			[&game](const auto& c) { game.treacheryDeck.placeOnTop(c); });

	game.biddingPool.clear();
}

void GameLogic::auctionWinTransaction(GameState& game, Faction won, int spice, bool karama)
{
	if (game.biddingPool.empty())
		return;

	auto player = getPlayerState(game, won);

	if (karama)
	{
		auto it = findKarama(game, won);
		if (it == player->hand.end())
			return;

		log->info("%s discards card %s", player->faction.label().c_str(), TreacheryCardLabels::label(it->id()));
		game.treacheryDeck.discard(*it);
		player->hand.erase(it);
		game.auction.cardDiscarded();
	}
	else
	{
		player->spice -= spice;
		log->info("%s pays %d spice", player->faction.label().c_str(), spice);

		if (factionAvailable(game, Faction::emperor()) && player->faction != Faction::emperor())
		{
			getPlayerState(game, Faction::emperor())->spice += spice;
			log->info("emperor receives %d spice", spice);
		}
	}

	log->info("%s receives card %s", player->faction.label().c_str(), TreacheryCardLabels::label(game.biddingPool.back().id()));
	player->hand.push_back(game.biddingPool.back());
	game.biddingPool.pop_back();
	game.auction.cardReceived();

	if (player->faction == Faction::harkonnen() && ((int) player->hand.size() < player->maxHand))
	{
		auto card = game.treacheryDeck.draw();
		log->info("%s receives additional card %s", player->faction.label().c_str(), TreacheryCardLabels::label(card.id()));
		player->hand.push_back(card);
		game.auction.cardReceived();
	}

}

void GameLogic::advance(GameState& game, GamePhase next, Faction customFactions)
{

	auto it = std::find_if(msAllowedActions.cbegin(), msAllowedActions.cend(),
			[next](const AllowedAction& a){ return a.mainAction && (a.when == next); });

	if (customFactions != Faction::none())
		mGame.expectingInputFrom = customFactions;
	else if (it != msAllowedActions.end())
		mGame.expectingInputFrom = it->who;
	else
		mGame.expectingInputFrom = Faction::none();

	mGame.phase = next;
	log->info("advance game -> %s | input required from %s", GamePhaseLabels::label(game.phase), mGame.expectingInputFrom.label().c_str());
	executeAutomaticTasks(game, mGame.phase);

}

void GameLogic::executeAutomaticTasks(GameState& game, GamePhase phase)
{
	auto it = std::find_if(mPhaseFunc.cbegin(), mPhaseFunc.cend(),
				[phase](const PhaseExecutionFunction& el){ return el.phase ==  phase;});

	if (it != mPhaseFunc.cend())
	{
		log->info("execute automatic tasks for phase %s...", GamePhaseLabels::label(phase));
		it->function(game);
		log->info("... done ");
	}
}

void GameLogic::advanceToShipmentPhase(GameState& game)
{
	game.shipper = ShipOrMove(game, *log);
	if (game.shipper.pendingGuildDecision())
		advance(game, PHASE_SHIPMENT_GUILD_DECISION);
	else
		advance(game, PHASE_SHIPMENT_SHIP, game.shipper.currentlyShipping());
}

void GameLogic::advanceInShipmentPhase(GameState& game)
{
	if (game.shipper.pendingGuildDecision())
		advance(game, PHASE_SHIPMENT_GUILD_DECISION);
	else if (game.shipper.pendingIntrusionReaction())
		advance(game, PHASE_SHIPMENT_INTRUSION_REACTION);
	else if (game.shipper.pendingAccompanyDecision())
		advance(game, PHASE_SHIPMENT_ACCOMPANY_DECISION);
	else if (game.shipper.pendingMovement())
		advance(game, PHASE_SHIPMENT_MOVE, game.shipper.currentlyShipping());
	else if (game.shipper.finished())
		advance(game, PHASE_BATTLE_COLLECT_BATTLES);
	else
		advance(game, PHASE_SHIPMENT_SHIP, game.shipper.currentlyShipping());
}

void GameLogic::discardTraitors(GameState& game)
{
	for (auto& player : mGame.players)
	{
		auto& deck = game.traitors;

		for (const auto& traitor : player.selectedTraitors)
		{
			deck.discard(traitor);
		}

		player.selectedTraitors.clear();
	}

}

void GameLogic::drawTraitors(GameState& game)
{
	for (auto& player : mGame.players)
	{
		auto& deck = game.traitors;
		Leader::Id drawn[4] = {
			deck.draw(),
			deck.draw(),
			deck.draw(),
			deck.draw()
		};

		player.selectedTraitors.push_back(drawn[0]);
		player.selectedTraitors.push_back(drawn[1]);
		player.selectedTraitors.push_back(drawn[2]);
		player.selectedTraitors.push_back(drawn[3]);
		log->info("traitors drawn for %-12s: { %-20s, %-20s, %-20s, %-20s }", player.faction.label().c_str(),
														  EnumLeaderId::label(drawn[0]),
														  EnumLeaderId::label(drawn[1]),
														  EnumLeaderId::label(drawn[2]),
														  EnumLeaderId::label(drawn[3]));
	}
}

bool GameLogic::factionAvailable(GameState& game, Faction faction)
{
	return getPlayerState(game, faction) != nullptr;
}

bool GameLogic::harkonnenMayRedraw(GameState& game)
{
	auto* state = getPlayerState(game, Faction::harkonnen());

	if (state == nullptr)
		return false;

	int own = 0;
	for (const auto id : state->selectedTraitors)
	{
		if (Leader::leader(id).faction() == Faction::harkonnen())
			++own;
	}

	return own > 1;
}

void GameLogic::placeStaticStartForces(GameState& game)
{
	auto& board = game.board;
	PlayerState* player = nullptr;

	if ((player = getPlayerState(game, Faction::harkonnen())) != nullptr)
	{
		board.placeHostile(player->faction, Placement{Carthag, 10, 0});
		player->reserve -= 10;
	}
	if ((player = getPlayerState(game, Faction::atreides())) != nullptr)
	{
		board.placeHostile(player->faction, Placement{Arrakeen, 10, 0});
		player->reserve -= 10;
	}
	if ((player = getPlayerState(game, Faction::spacingGuild())) != nullptr)
	{
		board.placeHostile(player->faction, Placement{TueksSietch, 5, 0});
		player->reserve -= 5;
	}
}

bool GameLogic::hasKarama(GameState& game, Faction faction)
{
	auto player = getPlayerState(game, faction);
	return findKarama(game, faction) != player->hand.end();
}

std::vector<TreacheryCard>::iterator
GameLogic::findKarama(GameState& game, Faction faction)
{
	auto player = getPlayerState(game, faction);

	const bool includeWorthless = (faction == Faction::beneGesserit());
	auto it = std::find_if(player->hand.begin(), player->hand.end(),
			[includeWorthless](const TreacheryCard& c) -> bool
			{
				if (includeWorthless)
					return c.isKarama() || c.isWorthless();
				else
					return c.isKarama();
			});

	return it;
}


bool GameLogic::isAllowedAction(GameState& game, const Action& action)
{
	for (const auto& entry : msAllowedActions)
	{
		if (entry.when != game.phase)
			continue;
		if (!entry.who.contains(action.from()))
			continue;
		if (entry.what != action.type())
			continue;
		return true;
	}
	return false;
}

Faction GameLogic::initialStormDialFactions(GameState& game)
{
	Faction result = Faction::none();

	auto stormOrder = game.board.stormOrder();
	result |= stormOrder.front().faction;

	auto second = stormOrder.size() - 1;

	if (stormOrder[second].seat == game.board.getStorm())
		--second;

	result |= stormOrder[second].faction;
	return result;
}

bool GameLogic::devActionSetupWithoutDraw(GameState& game, const Action& action)
{
	log->info("received dev action setup");
	mSetupWithoutDraw = true;
	setup();
	return true;
}

bool GameLogic::devActionSetPhase(GameState& game, const Action& action)
{
	auto ac = *static_cast<const DevActionSetPhase*> (&action);
	log->info("received dev action set phase = %s", GamePhaseLabels::label(ac.phase));
	advance(game, ac.phase);
	return true;

}

