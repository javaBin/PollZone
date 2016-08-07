cmake_minimum_required(VERSION 3.5)

project(PollZone)

find_package(KicadUtils)

kicad_generate_header(
    OUTPUT PollZone/schematic-v4.h
    NET schematic-v4/pollbox.net
    REF U1
    IN_SOURCE
    TEMPLATE nodemcu-arduino)

add_custom_target(headers ALL DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/PollZone/schematic-v4.h)
