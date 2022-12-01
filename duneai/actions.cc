#include "actions.h"

template<>
std::unordered_map<ActionType, const char*> ActionTypeLabels::labels =
{
		{ACTION_PREDICT,           "predict"},
		{ACTION_HARKONNEN_REDRAW,  "harkonnenRedraw"},
		{ACTION_TRAITOR_SELECTION, "traitorSelection"},
		{ACTION_FREMEN_PLACEMENT,  "fremenPlacement"}
};
