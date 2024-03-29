#include "shipormove.h"
#include "gamedata.h"
#include "statefunctions.h"

#include "territories.h"
ShipOrMove::ShipOrMove()
{
	Territories::cielagoSouth.contains(AreaId::INVALID);
}

ShipOrMove::ShipOrMove(GameState& aGame, const Logger& aLog)
: game(&aGame), log(&aLog)
{
	const auto seats = game->board.stormOrder();
	turnOrder.reserve(seats.size());

	Faction available = Faction::none();
	std::for_each(seats.cbegin(), seats.cend(),
		[this, &available] (const auto& o)
		{
			available |= o.faction;
			turnOrder.emplace_back(o.faction);
		});

	if (available.contains(Faction::spacingGuild()))
	{
		priorityShipping = AVAILABLE;
		phase = SP_GUILD_DECISION;
	}

	if (turnOrder[0] == Faction::spacingGuild())
		++currentIndex;

	if(available.contains(Faction::beneGesserit()))
		beneGesseritAvailable = true;
}

bool ShipOrMove::pendingGuildDecision() const noexcept
{
	return phase == SP_GUILD_DECISION;
}

void ShipOrMove::forceGuildShipment() noexcept
{
	if (phase != SP_GUILD_DECISION)
		return;

	log->info("guild forces priority shipment");
	priorityShipping = ACTIVE;
	phase = SP_SHIP;
}

void ShipOrMove::delayGuildShipment() noexcept
{
	if (phase != SP_GUILD_DECISION)
		return;

	log->info("guild delays shipment");
	phase = SP_SHIP;
}

bool ShipOrMove::pendingIntrusionReaction() const noexcept
{
	return phase == SP_INTRUSION;
}

void ShipOrMove::disengageIntrusion() noexcept
{
	if (phase != SP_INTRUSION)
		return;

	log->info("beneGesserit swaps to advisor in %s", EnumAreaId::label(last.where));
	game->board.setTerritoryHostility(Faction::beneGesserit(), Territories::of(last.where), false);

	if (beneGesseritCanAccompany())
		phase = SP_ACCOMPANY;
	else if (last.origin != LastAction::MOVE_PHASE)
		phase = SP_MOVE;
	else
		advanceToNextPlayer();
}

void ShipOrMove::engageIntrusion() noexcept
{
	if (phase != SP_INTRUSION)
		return;

	log->info("beneGesserit stays fighter in %s", EnumAreaId::label(last.where));

	if (last.origin != LastAction::MOVE_PHASE)
		phase = SP_MOVE;
	else
		advanceToNextPlayer();
}

bool ShipOrMove::pendingAccompanyDecision() const noexcept
{
	return phase == SP_ACCOMPANY;
}

void ShipOrMove::sendAdvisor(bool toPolarSink)
{
	if (phase != SP_ACCOMPANY)
		return;

	log->info("beneGesserit sends advisor -> %s",
			  toPolarSink ? EnumAreaId::label(AreaId::PolarSink) : EnumAreaId::label(last.where));
	auto player = getPlayerState(*game, Faction::beneGesserit());
	--(player->reserve);

	if (toPolarSink)
		game->board.placeHostile(Faction::beneGesserit(), Placement{AreaId::PolarSink, 1, 0});
	else
		game->board.placeNeutral(Faction::beneGesserit(), Placement{last.where, 1, 0});

	phase = SP_MOVE;
}

void ShipOrMove::keepAdvisor()
{
	if (phase == SP_ACCOMPANY)
		phase = SP_MOVE;
}

bool ShipOrMove::pendingMovement() const noexcept
{
	return phase == SP_MOVE;
}

static bool consumeHajr(GameState& game, PlayerState& player)
{
	return StateFunctions::discardCard(player, TreacheryCard::HAJR, game.treacheryDeck);
}

static bool isHostileInTargetArea(GameState& game, PlayerState& player, AreaId where, bool hostileRequested = true)
{
	auto forces = game.board.forcesInArea(where);

	if (forces.hasHostileForces(player.faction))
		return true;
	else if (forces.hasNonHostileForces(player.faction))
		return false;
	else if (game.board.hostileEnemies(player.faction, Territories::of(where)) == 0)
		return true;
	else
		return hostileRequested;
}

static bool swapLoneAdvisors(GameState& game, AreaId where)
{
	return !game.board.hostileEnemies(Faction::beneGesserit(), Territories::of(where)) &&
		    game.board.setTerritoryHostility(Faction::beneGesserit(), Territories::of(where), true);
}

bool ShipOrMove::move(AreaId from, AreaId to, int normalAmount, int specialAmount, bool useHajr, bool advisor)
{
	const Faction who     = currentlyShipping();
	PlayerState*  player  = getPlayerState(*game, who);

	if (!verifyMovement(player, from, to, normalAmount, specialAmount, advisor))
		return false;

	if (useHajr && !consumeHajr(*game, *player))
	{
		log->error("%s has no hajr card", who.label().c_str());
		return false;
	}

	const bool hostile = isHostileInTargetArea(*game, *player, to, !advisor);
	game->board.removeForces(who, Placement{from, normalAmount, specialAmount});
	game->board.place(who, Placement{ to, normalAmount, specialAmount }, hostile);

	log->info("%s moves troops {%d, %d} from %s -> %s", who.label().c_str(),
              normalAmount, specialAmount, EnumAreaId::label(from), EnumAreaId::label(to));

	if (swapLoneAdvisors(*game, from))
	{
		log->info("swap bene gesserit troops in territory %s to fighter", EnumAreaId::label(from));
	}

	advanceAfterMove(LastAction{who, to, useHajr ? LastAction::MOVE_PHASE_HAJR : LastAction::MOVE_PHASE});
	return true;
}

bool ShipOrMove::finished() const noexcept
{
	return phase == SP_FINISHED;
}

Faction ShipOrMove::currentlyShipping() const noexcept
{
	if (priorityShipping == ACTIVE)
		return Faction::spacingGuild();
	else
		return turnOrder[currentIndex];
}

int ShipOrMove::shipmentCost(Faction who, AreaId where, int amount) const
{
	if (who == Faction::fremen())
		return 0;

	const int base10 = Arrakis::isStronghold(where) ? 10 : 20;
	int cost10 = base10 * amount;

	if (who == Faction::spacingGuild())
	{
		cost10 /= 2;
		cost10 += 5; // ceil to next full int or discard the 5 at integer division
	}

	return cost10 / 10;
}

bool ShipOrMove::beneGesseritCanReact() const noexcept
{
	if (!beneGesseritAvailable)
		return false;
	if (last.who == Faction::beneGesserit())
		return false;

	return game->board.forcesInArea(last.where).hasHostileForces(Faction::beneGesserit());
}

bool ShipOrMove::beneGesseritCanAccompany() const noexcept
{
	if (!beneGesseritAvailable)
		return false;
	if (last.origin != LastAction::SHIP_PHASE_TO_PLANET)
		return false;
	if (last.who == Faction::beneGesserit() || last.who == Faction::fremen())
		return false;
	if (getPlayerState(*game, Faction::beneGesserit())->reserve == 0)
		return false;
	if (game->board.forcesInArea(last.where).hasHostileForces(Faction::beneGesserit()))
		return false;

	return true;
}

bool ShipOrMove::passShipment()
{
	if (phase != SP_SHIP)
		return false;

	log->info("%s passes shipment", currentlyShipping().label().c_str());
	advanceAfterShip(LastAction{currentlyShipping(), AreaId::INVALID, LastAction::SHIP_PHASE_SPECIAL});
	return true;
}

bool ShipOrMove::passMovement()
{
	if (phase != SP_MOVE)
		return false;

	log->info("%s passes movement", currentlyShipping().label().c_str());
	advanceAfterMove(LastAction{currentlyShipping(), AreaId::INVALID, LastAction::MOVE_PHASE});
	return true;
}

bool ShipOrMove::shipFromReserve(AreaId to, int normalAmount, int specialAmount)
{
	const Faction who     = currentlyShipping();
	PlayerState*  player  = getPlayerState(*game, who);
	const int     cost    = shipmentCost(who, to, normalAmount + specialAmount);

	if (!verifyShipment(player, cost, nullptr, &to, normalAmount, specialAmount))
		return false;

	shipmentTransaction(player, cost, nullptr, &to, normalAmount, specialAmount);
	advanceAfterShip(LastAction{who, to, LastAction::SHIP_PHASE_TO_PLANET});
	return true;
}

bool ShipOrMove::shipToReserve(AreaId from, int normalAmount, int specialAmount)
{
	const Faction who     = currentlyShipping();
	PlayerState*  player  = getPlayerState(*game, who);

	if (who != Faction::spacingGuild())
		return false;

	if (!verifyShipment(player, 0, &from, nullptr, normalAmount, specialAmount))
		return false;

	shipmentTransaction(player, 0, &from, nullptr, normalAmount, specialAmount);
	advanceAfterShip(LastAction{who, AreaId::INVALID, LastAction::SHIP_PHASE_SPECIAL});
	return true;
}

bool ShipOrMove::shipCrossPlanet(AreaId from, AreaId to, int normalAmount, int specialAmount)
{
	const Faction who     = currentlyShipping();
	PlayerState*  player  = getPlayerState(*game, who);
	const int     cost    = shipmentCost(who, to, normalAmount + specialAmount);

	if (who != Faction::spacingGuild())
		return false;

	if (!verifyShipment(player, cost, &from, &to, normalAmount, specialAmount))
		return false;

	shipmentTransaction(player, cost, &from, &to, normalAmount, specialAmount);
	advanceAfterShip(LastAction{who, to, LastAction::SHIP_PHASE_SPECIAL});
	return true;
}

bool ShipOrMove::verifyShipment(const PlayerState* player, int cost, AreaId* from, AreaId* to, int normalAmount, int specialAmount)
{
	if (phase != SP_SHIP)
		return false;

	if (!player)
		return false;

	if (cost > player->spice)
		return false;

	if (to && !game->board.canShip(player->faction, *to))
		return false;

	return verifyCommon(player, from, to, normalAmount, specialAmount);
}

bool ShipOrMove::verifyMovement(const PlayerState* player, AreaId from, AreaId to, int normalAmount, int specialAmount, bool advisor)
{
	if (phase != SP_MOVE)
		return false;

	if (!player)
		return false;

	if (!game->board.canMove(player->faction, from, to, advisor))
		return false;

	if (advisor && player->faction != Faction::beneGesserit())
		return false;

	return verifyCommon(player, &from, &to, normalAmount, specialAmount);
}

bool ShipOrMove::verifyCommon(const PlayerState* player, AreaId* from, AreaId* to, int normalAmount, int specialAmount)
{
	int normalsFrom  = player->reserve;
	int specialsFrom = player->specialForcesReserve;

	if (from)
	{
		auto forces = game->board.forcesInArea(*from);

		if (!forces.hasForces(player->faction))
			return false;

		const auto playerForces = forces.getForces(player->faction);
		normalsFrom  = playerForces.normal;
		specialsFrom = playerForces.special;
	}

	if (normalsFrom < normalAmount || specialsFrom < specialAmount)
		return false;

	return true;
}

void ShipOrMove::shipmentTransaction(PlayerState* player, int cost, AreaId* from, AreaId* to, int normalAmount, int specialAmount)
{
	const Faction who = player->faction;
	payShipment(player, cost);

	const char* logNameFrom = from ? EnumAreaId::label(*from) : "reserve";
	const char* logNameTo   = to   ? EnumAreaId::label(*to)   : "reserve";
	log->info("%s deploys {%d, %d} from %s -> %s", player->faction.label().c_str(),
			  normalAmount, specialAmount, logNameFrom, logNameTo);

	if (from)
	{
		game->board.removeForces(who, Placement{*from, normalAmount, specialAmount});
	}
	else
	{
		player->reserve              -= normalAmount;
		player->specialForcesReserve -= specialAmount;
	}

	if (to)
	{
		const bool hostile = isHostileInTargetArea(*game, *player, *to);
		game->board.place(who, Placement{ *to, normalAmount, specialAmount }, hostile);
	}
	else
	{
		player->reserve              += normalAmount;
		player->specialForcesReserve += specialAmount;
	}
}

void ShipOrMove::payShipment(PlayerState* player, int cost)
{
	log->info("shipment cost %d", cost);

	if (cost == 0)
		return;

	auto guild = getPlayerState(*game, Faction::spacingGuild());

	if (guild && player != guild)
	{
		log->info("guild receives %d spice", cost);
		guild->spice += cost;
	}

	player->spice -= cost;
}

void ShipOrMove::advanceAfterShip(LastAction&& aLast)
{
	last = std::move(aLast);

	if (beneGesseritCanReact())
		phase = SP_INTRUSION;
	else if (beneGesseritCanAccompany())
		phase = SP_ACCOMPANY;
	else
		phase = SP_MOVE;
}

void ShipOrMove::advanceAfterMove(LastAction&& aLast)
{
	last = std::move(aLast);

	if (beneGesseritCanReact())
		phase = SP_INTRUSION;
	else if (last.origin == LastAction::MOVE_PHASE_HAJR)
		phase = SP_MOVE;
	else
		advanceToNextPlayer();
}

void ShipOrMove::advanceToNextPlayer()
{
	// if the guild shipped out of turn, the index already points to the next in-turn player
	if (priorityShipping == GuildStatus::ACTIVE)
		priorityShipping = GuildStatus::NOT_AVAILABLE;
	else
		++currentIndex;

	// when the guild can move out of turn, its usual turn needs to be ignored
	if (currentIndex < (int) turnOrder.size() && turnOrder[currentIndex] == Faction::spacingGuild())
		++currentIndex;

	// now we dealt with the guild out-of turn index. the index points either to the next non-guild player or to the end

	// all in turn players + guild are finished -> shipping phase is finally over
	if (currentIndex == (int) turnOrder.size() && priorityShipping != GuildStatus::AVAILABLE)
	{
		log->info("shipping phase finished");
		phase = SP_FINISHED;
	}
	// all in turn players have finished, but the guild delayed until the last turn. we need to force them to ship now
	else if (currentIndex == (int) turnOrder.size())
	{
		priorityShipping = GuildStatus::ACTIVE;
		phase = SP_SHIP;
	}
	// next is a normal player turn, but the guild still can decide to prioritize its own shipment
	else if (priorityShipping == GuildStatus::AVAILABLE)
	{
		phase = SP_GUILD_DECISION;
	}
	// all that remains is the normal in-turn shipment for the next player
	else
	{
		phase = SP_SHIP;
	}

}

