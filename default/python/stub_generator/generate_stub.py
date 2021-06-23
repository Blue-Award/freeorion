from typing import List, Set

from stub_generator.interface_inspector import (
    ClassInfo,
    EnumInfo,
    FunctionInfo,
    InstanceInfo,
)
from stub_generator.stub_generator import EnumGenerator
from stub_generator.stub_generator.class_generator import ClassGenerator
from stub_generator.stub_generator.function_generator import FunctionGenerator


def make_stub(classes: List[ClassInfo], enums: List[EnumInfo], functions: List[FunctionInfo],
              instances: List[InstanceInfo], result_path, classes_to_ignore: Set[str]):

    processors = [
        ClassGenerator(classes, instances, classes_to_ignore, enums),
        EnumGenerator(enums),
        FunctionGenerator(functions),
    ]

    header = (
        "# Autogenerated do not modify manually!\n"
        "# This is a type-hinting python stub file, used by python IDEs to provide type hints. For more information\n"
        "# about stub files, see https://www.python.org/dev/peps/pep-0484/#stub-files\n"
        "# During execution, the actual module is made available via\n"
        "# a C++ Boost-python process as part of the launch.\n"
        "from enum import IntEnum\n"
        "from typing import Dict, Iterator, Sequence, Set, overload\n"
        "\n"
        "from common.fo_typing import (\n"
        "    BuildingId,\n"
        "    EmpireId,\n"
        "    FleetId,\n"
        "    ObjectId,\n"
        "    PlanetId,\n"
        "    ShipId,\n"
        "    SpeciesName,\n"
        "    SystemId,\n"
        ")\n\n"
    )

    res = []
    for processor in processors:
        res.extend(processor.body)

    with open(result_path, 'w') as f:
        f.write(header)
        f.write('\n\n\n'.join(res))
        f.write('\n')
