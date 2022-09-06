set(CPM_DONT_UPDATE_MODULE_PATH ON)

CPMAddPackage("gh:SFML/SFML#45986c6400d11c29c47511e5962465ed8a12c3a8")

CPMAddPackage("gh:ocornut/imgui@1.87")

CPMAddPackage(GITHUB_REPOSITORY eliasdaler/imgui-sfml
              GIT_TAG 29dbc25b5a4f3e7bde4a07472395071457a7d2ee
              OPTIONS "IMGUI_DIR ${imgui_SOURCE_DIR}" "IMGUI_SFML_FIND_SFML OFF")

CPMAddPackage("gh:catchorg/Catch2@3.1.0")
