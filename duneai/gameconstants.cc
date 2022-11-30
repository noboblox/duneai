#include "gameconstants.h"

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
