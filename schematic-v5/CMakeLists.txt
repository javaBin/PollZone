kicad_generate_header(
    TARGET headers-v5 ALL
    OUTPUT ../PollZone/schematic-v5.h
    NET pollbox.net
    REF U1
    IN_SOURCE
    TEMPLATE nodemcu-arduino)

kicad_pcb_plot(
    TARGET plots-v5-pdf ALL
    PCB_FILE pollbox.kicad_pcb
    DIR ${CMAKE_CURRENT_SOURCE_DIR}/pdf)

kicad_pcb_plot(
    TARGET plots-v5-ps ALL
    PCB_FILE pollbox.kicad_pcb
    FORMAT PS
    DIR ${CMAKE_CURRENT_SOURCE_DIR}/ps)

kicad_pcb_plot(
    TARGET plots-v5-svg ALL
    PCB_FILE pollbox.kicad_pcb
    FORMAT SVG
    DIR ${CMAKE_CURRENT_SOURCE_DIR}/svg)
