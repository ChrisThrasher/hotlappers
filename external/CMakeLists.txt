set(CPM_DONT_UPDATE_MODULE_PATH ON)

CPMAddPackage("gh:SFML/SFML#master")

CPMAddPackage("gh:ocornut/imgui@1.87")

CPMAddPackage(GITHUB_REPOSITORY SFML/imgui-sfml
              GIT_TAG master
              OPTIONS "IMGUI_DIR ${imgui_SOURCE_DIR}" "IMGUI_SFML_FIND_SFML OFF")

CPMAddPackage("gh:catchorg/Catch2@3.1.0")
