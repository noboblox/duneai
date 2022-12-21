#include "gameconstants.h"

const Leader Leader::leaders[] = {
	Leader(ID_Yueh   ,  "Dr. Wellington Yueh",  Faction::atreides()),
	Leader(ID_Duncan ,  "Duncan Idaho",         Faction::atreides()),
	Leader(ID_Gurney ,  "Gurney Hallack" ,      Faction::atreides()),
	Leader(ID_Thufir ,  "Thufir Hawat",         Faction::atreides()),
	Leader(ID_Jessica,  "Lady Jessica",         Faction::atreides()),
	Leader(ID_Alia   ,  "Alia",                 Faction::beneGesserit()),
	Leader(ID_Wanna  ,  "Wanna Marcus",         Faction::beneGesserit()),
	Leader(ID_Irulan ,  "Princess Irulan",      Faction::beneGesserit()),
	Leader(ID_Margot ,  "Margot Lady Fenring",  Faction::beneGesserit()),
	Leader(ID_Ramallo,  "Mother Ramallo",       Faction::beneGesserit()),
	Leader(ID_Kudu   ,  "Umman Kudu",           Faction::harkonnen()),
	Leader(ID_Nefud  ,  "Captain Iakin Nefud",  Faction::harkonnen()),
	Leader(ID_Piter  ,  "Piter De Vries",       Faction::harkonnen()),
	Leader(ID_Rabban ,  "Beast Rabban",         Faction::harkonnen()),
	Leader(ID_Feyd   ,  "Feyd Rautha",          Faction::harkonnen()),
	Leader(ID_Bashar ,  "Bashar",               Faction::emperor()),
	Leader(ID_Burseg ,  "Burseg",               Faction::emperor()),
	Leader(ID_Caid   ,  "Caid",                 Faction::emperor()),
	Leader(ID_Aramsha,  "Captain Aramsham",     Faction::emperor()),
	Leader(ID_Fenring,  "Hasimir Fenring",      Faction::emperor()),
	Leader(ID_Repr   ,  "Guild Representative", Faction::spacingGuild()),
	Leader(ID_Sook   ,  "Soo-Soo Sook",         Faction::spacingGuild()),
	Leader(ID_Esmar  ,  "Esmar Tuek",           Faction::spacingGuild()),
	Leader(ID_Bewt   ,  "Master Bewt",          Faction::spacingGuild()),
	Leader(ID_Staban ,  "Staban Tuek",          Faction::spacingGuild()),
	Leader(ID_Jamis  ,  "Jamis",                Faction::fremen()),
	Leader(ID_Mapes  ,  "Shadout Mapes",        Faction::fremen()),
	Leader(ID_Otheym ,  "Otheym",               Faction::fremen()),
	Leader(ID_Chani  ,  "Chani",                Faction::fremen()),
	Leader(ID_Stilgar,  "Stilgar",              Faction::fremen()),
	Leader(ID_Zoal   ,  "Zoal",                 Faction::tleilaxu()),
	Leader(ID_Hidar  ,  "Hidar Fen Ajidica",    Faction::tleilaxu()),
	Leader(ID_Zaaf   ,  "Master Zaaf",          Faction::tleilaxu()),
	Leader(ID_Wykk   ,  "Wykk",                 Faction::tleilaxu()),
	Leader(ID_Blin   ,  "Blin",                 Faction::tleilaxu())};

const Leader Leader::INVALID = Leader(Leader::LEADERS_end, "INVALID", Faction::none());

const std::vector<int> SeatConfig::INVALID      = {};
const std::vector<int> SeatConfig::twoPlayers   = {2, 11};
const std::vector<int> SeatConfig::threePlayers = {2, 8, 14};
const std::vector<int> SeatConfig::fourPlayers  = {2, 6, 11, 15};
const std::vector<int> SeatConfig::fivePlayers  = {2, 5, 9, 12, 16};
const std::vector<int> SeatConfig::sixPlayers   = {2, 5, 8, 11, 14, 17};
const std::vector<int> SeatConfig::sevenPlayers = {2, 4, 7, 9, 12, 14, 17};
const std::vector<int> SeatConfig::eightPlayers = {2, 4, 6, 8, 11, 13, 15, 17};
const std::vector<int> SeatConfig::ninePlayers  = {2, 4, 6, 8, 10, 12, 14, 16, 18};
const std::vector<int> SeatConfig::tenPlayers   = {2, 3, 5, 7, 9, 11, 12, 14, 16, 18};

const std::vector<int>* const SeatConfig::configs[] = {
		&INVALID,
		&INVALID,
		&twoPlayers,
		&threePlayers,
		&fourPlayers ,
		&fivePlayers ,
		&sixPlayers  ,
		&sevenPlayers,
		&eightPlayers,
		&ninePlayers ,
		&tenPlayers};

template<>
std::unordered_map<GamePhase, const char*> GamePhaseLabels::labels =
{
	{PHASE_INIT_PREDICTION,             "init.prediction"            },
	{PHASE_INIT_HARKONNEN_REDRAW,       "init.harkonnenRedraw"       },
	{PHASE_INIT_TRAITOR_SELECTION,      "init.traitorSelection"      },
	{PHASE_INIT_FREMEN_PLACEMENT,       "init.fremenPlacement"       },
	{PHASE_INIT_BG_PLACEMENT,           "init.beneGesseritPlacement" },
	{PHASE_STORM_INITAL_DIAL,           "storm.initialDial"          },
	{PHASE_SPICE_SPICE_BLOW,            "spice.spiceBlow"            },
	{PHASE_CHOAM_CHARITY,               "choam.charity"              },
	{PHASE_AUCTION_BIDDING,             "auction.bidding"            },
	{PHASE_SHIPMENT_GUILD_DECISION,     "shipment.guildDecision"     },
	{PHASE_SHIPMENT_SHIP,               "shipment.ship"              },
	{PHASE_SHIPMENT_INTRUSION_REACTION, "shipment.intrusionReaction" },
	{PHASE_SHIPMENT_ACCOMPANY_DECISION, "shipment.accompanyDecision" },
	{PHASE_SHIPMENT_MOVE,               "shipment.move"              },
	{PHASE_BATTLE,                      "battle"                     },
};

std::vector<Faction> Faction::expand(Faction mask)
{
	std::vector<Faction> result;
	result.reserve(10);

	int done = 0;
	int value = 1;

	while (done < Faction::CODE_ANY)
	{
		if (mask.mValue & value)
			result.push_back(Faction(value));

		done |= value;
		value <<= 1;
	}

	return result;
}

std::vector<Leader::Id> Leader::of(Faction faction)
{
	std::vector<Leader::Id> result;
	result.reserve(5);

	for (const auto& leader : leaders)
	{
		if (leader.faction().contains(faction))
			result.push_back(leader.id());
	}

	return result;
}
