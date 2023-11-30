#include "gameconstants.h"

template<>
EnumLeaderId::Definition EnumLeaderId::entries = {
    {Leader::ID_Yueh     , "Dr. Wellington Yueh" },
    {Leader::ID_Duncan   , "Duncan Idaho"        },
    {Leader::ID_Gurney   , "Gurney Hallack"      },
    {Leader::ID_Thufir   , "Thufir Hawat"        },
    {Leader::ID_Jessica  , "Lady Jessica"        },
    {Leader::ID_Alia     , "Alia"                },
    {Leader::ID_Wanna    , "Wanna Marcus"        },
    {Leader::ID_Irulan   , "Princess Irulan"     },
    {Leader::ID_Margot   , "Margot Lady Fenring" },
    {Leader::ID_Ramallo  , "Mother Ramallo"      },
    {Leader::ID_Kudu     , "Umman Kudu"          },
    {Leader::ID_Nefud    , "Captain Iakin Nefud" },
    {Leader::ID_Piter    , "Piter De Vries"      },
    {Leader::ID_Rabban   , "Beast Rabban"        },
    {Leader::ID_Feyd     , "Feyd Rautha"         },
    {Leader::ID_Bashar   , "Bashar"              },
    {Leader::ID_Burseg   , "Burseg"              },
    {Leader::ID_Caid     , "Caid"                },
    {Leader::ID_Aramsha  , "Captain Aramsham"    },
    {Leader::ID_Fenring  , "Hasimir Fenring"     },
    {Leader::ID_Repr     , "Guild Representative"},
    {Leader::ID_Sook     , "Soo-Soo Sook"        },
    {Leader::ID_Esmar    , "Esmar Tuek"          },
    {Leader::ID_Bewt     , "Master Bewt"         },
    {Leader::ID_Staban   , "Staban Tuek"         },
    {Leader::ID_Jamis    , "Jamis"               },
    {Leader::ID_Mapes    , "Shadout Mapes"       },
    {Leader::ID_Otheym   , "Otheym"              },
    {Leader::ID_Chani    , "Chani"               },
    {Leader::ID_Stilgar  , "Stilgar"             },
    {Leader::ID_Zoal     , "Zoal"                },
    {Leader::ID_Hidar    , "Hidar Fen Ajidica"   },
    {Leader::ID_Zaaf     , "Master Zaaf"         },
    {Leader::ID_Wykk     , "Wykk"                },
    {Leader::ID_Blin     , "Blin"                },
};

template<>
EnumBiddingAction::Definition EnumBiddingAction::entries = {
	{BiddingAction::PASS      , "pass"      },
	{BiddingAction::BID       , "bid"       },
	{BiddingAction::KARAMA_BID, "karamaBid" },
	{BiddingAction::KARAMA_BUY, "karamaBuy" },
};

const Leader Leader::leaders[] = {
	Leader(ID_Yueh   , Faction::atreides()),
	Leader(ID_Duncan , Faction::atreides()),
	Leader(ID_Gurney , Faction::atreides()),
	Leader(ID_Thufir , Faction::atreides()),
	Leader(ID_Jessica, Faction::atreides()),
	Leader(ID_Alia   , Faction::beneGesserit()),
	Leader(ID_Wanna  , Faction::beneGesserit()),
	Leader(ID_Irulan , Faction::beneGesserit()),
	Leader(ID_Margot , Faction::beneGesserit()),
	Leader(ID_Ramallo, Faction::beneGesserit()),
	Leader(ID_Kudu   , Faction::harkonnen()),
	Leader(ID_Nefud  , Faction::harkonnen()),
	Leader(ID_Piter  , Faction::harkonnen()),
	Leader(ID_Rabban , Faction::harkonnen()),
	Leader(ID_Feyd   , Faction::harkonnen()),
	Leader(ID_Bashar , Faction::emperor()),
	Leader(ID_Burseg , Faction::emperor()),
	Leader(ID_Caid   , Faction::emperor()),
	Leader(ID_Aramsha, Faction::emperor()),
	Leader(ID_Fenring, Faction::emperor()),
	Leader(ID_Repr   , Faction::spacingGuild()),
	Leader(ID_Sook   , Faction::spacingGuild()),
	Leader(ID_Esmar  , Faction::spacingGuild()),
	Leader(ID_Bewt   , Faction::spacingGuild()),
	Leader(ID_Staban , Faction::spacingGuild()),
	Leader(ID_Jamis  , Faction::fremen()),
	Leader(ID_Mapes  , Faction::fremen()),
	Leader(ID_Otheym , Faction::fremen()),
	Leader(ID_Chani  , Faction::fremen()),
	Leader(ID_Stilgar, Faction::fremen()),
	Leader(ID_Zoal   , Faction::tleilaxu()),
	Leader(ID_Hidar  , Faction::tleilaxu()),
	Leader(ID_Zaaf   , Faction::tleilaxu()),
	Leader(ID_Wykk   , Faction::tleilaxu()),
	Leader(ID_Blin   , Faction::tleilaxu())};

const Leader Leader::INVALID = Leader(Leader::ID_INVALID, Faction::none());

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
GamePhaseLabels::Definition GamePhaseLabels::entries =
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
	{PHASE_BATTLE_COLLECT_BATTLES,      "battle.collectBattles"      },
	{PHASE_SPICE_HARVEST,               "spiceHarvest"               },
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

//static
Faction Faction::fromString(const std::string& label)
{
	if (label == "emperor"      )
		return Faction::emperor();
	if (label == "spacingGuild" )
		return Faction::spacingGuild();
	if (label == "fremen"       )
		return Faction::fremen();
	if (label == "atreides"     )
		return Faction::atreides();
	if (label == "harkonnen"    )
		return Faction::harkonnen();
	if (label == "beneGesserit" )
		return Faction::beneGesserit();
	if (label == "tleilaxu"     )
		return Faction::tleilaxu();
	return Faction::none();
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
