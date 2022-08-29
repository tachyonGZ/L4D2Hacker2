include_directories(Head)

aux_source_directory(Src HACKER2DLL_SRC)
aux_source_directory(../imgui/Src IMGUI_SRC)

add_library(Hacker2DLL SHARED ${HACKER2DLL_SRC} ${IMGUI_SRC})