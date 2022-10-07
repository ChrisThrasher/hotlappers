set(CPM_DONT_UPDATE_MODULE_PATH ON)

CPMAddPackage("gh:SFML/SFML#753644a5bd58ba2e7fca8faab8cfe38906bf14f0")

CPMAddPackage("gh:ocornut/imgui@1.87")

CPMAddPackage(GITHUB_REPOSITORY eliasdaler/imgui-sfml
              GIT_TAG 9fbd3adc4da4d65cddccb658f504a0184149abe6
              OPTIONS "IMGUI_DIR ${imgui_SOURCE_DIR}" "IMGUI_SFML_FIND_SFML OFF")

CPMAddPackage("gh:catchorg/Catch2@3.1.0")
