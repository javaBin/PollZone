cmake_minimum_required(VERSION 3.5)

project(PollZone)

find_package(KicadUtils)

function(kicad_plot_pcb PCB_FILE OUT_DIR FILES_VAR)
  get_filename_component(basename "${PCB_FILE}" NAME_WE)

  set(outputs
      "${basename}-Assembly.pdf"
      "${basename}-AssyOutlinesTop.pdf"
      "${basename}-AssyTop.pdf"
      "${basename}-CuBottom.pdf"
      "${basename}-CuTop.pdf"
      "${basename}-EdgeCuts.pdf"
      "${basename}-Layout.pdf"
      "${basename}-MaskBottom.pdf"
      "${basename}-MaskTop.pdf"
      "${basename}-PasteBottom.pdf"
      "${basename}-PasteTop.pdf"
      "${basename}-SilkBottom.pdf"
      "${basename}-Silk.pdf"
      "${basename}-SilkTop.pdf")

  message(STATUS "PCB_FILE: ${PCB_FILE}")
  message(STATUS "OUT_DIR: ${OUT_DIR}")
  add_custom_command(
      OUTPUT ${outputs}
      COMMAND cmake -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/${OUT_DIR}"
      COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/plot_board.py"
              "${CMAKE_CURRENT_SOURCE_DIR}/${PCB_FILE}"
              "${CMAKE_CURRENT_SOURCE_DIR}/${OUT_DIR}"
      )

  if (FILES_VAR)
    set(${FILES_VAR} ${outputs} PARENT_SCOPE)
  endif()
endfunction()

kicad_generate_header(
    OUTPUT PollZone/schematic-v4.h
    NET schematic-v4/pollbox.net
    REF U1
    IN_SOURCE
    TEMPLATE nodemcu-arduino)

kicad_plot_pcb(schematic-v4/pollbox.kicad_pcb schematic-v4/pdf PDFS)

add_custom_target(headers ALL DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/PollZone/schematic-v4.h)

add_custom_target(docs ALL DEPENDS
	${PDFS})
