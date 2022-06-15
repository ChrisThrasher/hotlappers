set(CPM_DONT_UPDATE_MODULE_PATH ON)

CPMAddPackage("gh:SFML/SFML#27a77747868b0097b63f77fa224ea047ca11ecbb")

CPMAddPackage("gh:ocornut/imgui@1.87")

CPMAddPackage(GITHUB_REPOSITORY eliasdaler/imgui-sfml
              GIT_TAG 29dbc25b5a4f3e7bde4a07472395071457a7d2ee
              OPTIONS "IMGUI_DIR ${imgui_SOURCE_DIR}" "IMGUI_SFML_FIND_SFML OFF")

CPMAddPackage("gh:catchorg/Catch2#372b7575f67ae82cd925452f6b8dcbd4ad8ea071")
