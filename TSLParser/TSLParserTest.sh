#!/usr/bin/bash
../build/TSLParserTest/TSLParserTest ./light.json | clang-format -style=Webkit > light.hpp
../build/TSLParserTest/TSLParserTest ./standard.oasis.300ul.tsl | clang-format -style=Webkit > AGV.hpp

