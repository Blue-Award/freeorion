from focs._industry import _BASIC_INDUSTRY, _industry
from macros.misc import DESCRIPTION_EFFECTSGROUP_MACRO
from macros.multiplier import BAD_MULTIPLIER, GOOD_MULTIPLIER, GREAT_MULTIPLIER, VERY_BAD_MULTIPLIER

NO_INDUSTRY = DESCRIPTION_EFFECTSGROUP_MACRO("NO_INDUSTRY_DESC")

VERY_BAD_INDUSTRY = _industry("VERY_BAD", VERY_BAD_MULTIPLIER)
BAD_INDUSTRY = _industry("BAD", BAD_MULTIPLIER)
AVERAGE_INDUSTRY = _BASIC_INDUSTRY
GOOD_INDUSTRY = _industry("GOOD", GOOD_MULTIPLIER)
GREAT_INDUSTRY = _industry("GREAT", GREAT_MULTIPLIER)
