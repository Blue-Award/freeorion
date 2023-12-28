from focs._effects import (
    AsteroidsType,
    Barren,
    Contains,
    Destroy,
    EffectsGroup,
    GasGiantType,
    GenerateSitRepMessage,
    IsBuilding,
    IsSource,
    Object,
    OwnedBy,
    Planet,
    Population,
    SetPlanetType,
    Source,
    Target,
)
from macros.base_prod import BUILDING_COST_MULTIPLIER
from macros.enqueue import ENQUEUE_ARTIFICIAL_PLANET_EXCLUSION

try:
    from focs._buildings import *
except ModuleNotFoundError:
    pass

BuildingType(  # type: ignore[reportUnboundVariable]
    name="BLD_ART_PLANET",
    description="BLD_ART_PLANET_DESC",
    buildcost=200 * Target.HabitableSize * BUILDING_COST_MULTIPLIER,
    buildtime=8,
    location=(
        Planet()
        & ~Contains(IsBuilding(name=["BLD_ART_PLANET"]))
        & ~Contains(IsBuilding(name=["BLD_ART_FACTORY_PLANET"]))
        & ~Contains(IsBuilding(name=["BLD_ART_PARADISE_PLANET"]))
        & OwnedBy(empire=Source.Owner)
        & Planet(type=[AsteroidsType, GasGiantType])
        & Population(high=0)
    ),
    enqueuelocation=ENQUEUE_ARTIFICIAL_PLANET_EXCLUSION,
    effectsgroups=[
        EffectsGroup(
            scope=Object(id=Source.PlanetID) & Planet(),
            effects=[
                SetPlanetType(type=Barren),
                GenerateSitRepMessage(
                    message="EFFECT_ART_PLANET",
                    label="EFFECT_ART_PLANET_LABEL",
                    icon="icons/specials_large/tidal_lock.png",
                    parameters={"planet": Target.ID},
                    empire=Source.Owner,
                ),
            ],
        ),
        EffectsGroup(scope=IsSource, effects=Destroy),
    ],
    icon="icons/specials_huge/tidal_lock.png",
)
