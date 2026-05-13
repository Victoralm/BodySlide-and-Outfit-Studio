cmake_minimum_required(VERSION 3.11)

project(BSOS)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Coloca os executáveis e recursos na estrutura padrão de mods do Skyrim
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/CalienteTools/BodySlide)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

find_package(wxWidgets REQUIRED gl core base net xrc adv qa html propgrid)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
set(fbxsdk_dir "" CACHE PATH "Directory of FBX SDK")
if(fbxsdk_dir)
	find_library(fbxsdk fbxsdk PATHS ${fbxsdk_dir}/lib/gcc/x64/release ${fbxsdk_dir}/lib NO_DEFAULT_PATH)
else()
	find_library(fbxsdk fbxsdk)
endif()

set(commonsources
	lib/FSEngine/FSBSA.cpp
	lib/FSEngine/FSEngine.cpp
	lib/FSEngine/FSManager.cpp
	lib/gli/glm/detail/glm.cpp
	lib/LZ4F/lz4.c
	lib/LZ4F/lz4frame.c
	lib/LZ4F/lz4hc.c
	lib/LZ4F/xxhash.c
	lib/nifly/src/Animation.cpp
	lib/nifly/src/BasicTypes.cpp
	lib/nifly/src/bhk.cpp
	lib/nifly/src/ExtraData.cpp
	lib/nifly/src/Factory.cpp
	lib/nifly/src/Geometry.cpp
	lib/nifly/src/NifFile.cpp
	lib/nifly/src/NifUtil.cpp
	lib/nifly/src/Nodes.cpp
	lib/nifly/src/Object3d.cpp
	lib/nifly/src/Objects.cpp
	lib/nifly/src/Particles.cpp
	lib/nifly/src/Shaders.cpp
	lib/nifly/src/Skin.cpp
	lib/SOIL2/wfETC.c
	lib/SOIL2/image_DXT.c
	lib/SOIL2/image_helper.c
	lib/SOIL2/SOIL2.c
	lib/TinyXML-2/tinyxml2.cpp
	src/components/Anim.cpp
	src/components/Automorph.cpp
	src/components/ClippingFixer.cpp
	src/components/DiffData.cpp
	src/components/Mesh.cpp
	src/components/NormalGenLayers.cpp
	src/components/SliderCategories.cpp
	src/components/SliderData.cpp
	src/components/SliderGroup.cpp
	src/components/SliderManager.cpp
	src/components/SliderPresets.cpp
	src/components/SliderSet.cpp
	src/files/MaskFile.cpp
	src/files/MaterialFile.cpp
	src/files/ResourceLoader.cpp
	src/files/TriFile.cpp
	src/program/GroupManager.cpp
	src/program/PresetSaveDialog.cpp
	src/render/GLExtensions.cpp
	src/render/GLMaterial.cpp
	src/render/GLOffscreenBuffer.cpp
	src/render/GLShader.cpp
	src/render/GLSurface.cpp
	src/render/GLCanvas.cpp
	src/render/GLDialog.cpp
	src/ui/wxStateButton.cpp
	src/ui/wxSliderPanel.cpp
	src/utils/AABBTree.cpp
	src/utils/ConfigurationManager.cpp
	src/utils/Log.cpp
	src/utils/PlatformUtil.cpp
	src/utils/StringStuff.cpp
	)
set(OSsources
	${commonsources}
	src/components/PoseData.cpp
	src/components/PoseDataYaml.cpp
	src/components/PoseDataJson.cpp
	src/components/PoseDataHkx.cpp
	src/components/RefTemplates.cpp
	src/components/Automation.cpp
	src/components/TweakBrush.cpp
	src/components/UndoHistory.cpp
	src/components/WeightNorm.cpp
	src/files/FBXWrangler.cpp
	src/files/HkxFile.cpp
	src/files/ObjFile.cpp
	src/files/SFMorphFile.cpp
	src/ui/wxBrushSettingsPopup.cpp
	src/ui/WeightCopyDialog.cpp
	src/program/EditUV.cpp
	src/program/FBXImportDialog.cpp
	src/program/ObjImportDialog.cpp
	src/program/OutfitProject.cpp
	src/program/OutfitStudio.cpp
	src/program/ShapeProperties.cpp
	src/program/ConvertBodyReferenceDialog.cpp
	src/program/AutomationDialog.cpp
	src/program/SliderDataImportDialog.cpp
	)
set(BSsources
	${commonsources}
	src/files/wxDDSImage.cpp
	src/program/BodySlideApp.cpp
	src/program/NormalsGenDialog.cpp
	src/program/PreviewWindow.cpp
	src/ui/PreviewPanel.cpp
	src/ui/wxNormalsGenDlg.cpp
	src/components/BuildSelection.cpp
	)

add_executable(OutfitStudio ${OSsources})
add_executable(BodySlide ${BSsources})

target_compile_definitions(OutfitStudio PRIVATE USE_FBXSDK)

include(${wxWidgets_USE_FILE})
target_include_directories(OutfitStudio SYSTEM PRIVATE
	${fbxsdk_dir}/include
	/usr/include
	/usr/include/wine/windows
	)
target_include_directories(BodySlide SYSTEM PRIVATE
	/usr/include/wine/windows
	)
target_include_directories(OutfitStudio PUBLIC
	lib/gli
	lib/nifly/include
	lib/nifly/external
	lib/TinyXML-2
	lib/fkYAML/include
	lib/nlohmannjson/include
	)
target_include_directories(BodySlide PUBLIC
	lib/gli
	lib/nifly/include
	lib/nifly/external
	lib/TinyXML-2
	)

target_link_libraries(OutfitStudio
	${wxWidgets_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${fbxsdk}
	xml2)

target_link_libraries(BodySlide
	${wxWidgets_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	xml2)

# --- Automação para Build Local (Linux) ---
# Copia a pasta de recursos e arquivos de configuração para o diretório de saída limpo
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/res" DESTINATION "${OUTPUT_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Config.xml" DESTINATION "${OUTPUT_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/BodySlide.xml" DESTINATION "${OUTPUT_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/OutfitStudio.xml" DESTINATION "${OUTPUT_DIR}")

# Cria as pastas obrigatórias para o funcionamento do programa
file(MAKE_DIRECTORY "${OUTPUT_DIR}/SliderPresets")
file(MAKE_DIRECTORY "${OUTPUT_DIR}/SliderGroups")
file(MAKE_DIRECTORY "${OUTPUT_DIR}/SliderSets")
file(MAKE_DIRECTORY "${OUTPUT_DIR}/ShapeData")
file(MAKE_DIRECTORY "${OUTPUT_DIR}/PoseData")
file(MAKE_DIRECTORY "${OUTPUT_DIR}/RefTemplates")

# --- Automação de Empacotamento (Snapshot) ---
string(TIMESTAMP TODAY "%m-%d-%Y")
set(ARCHIVE_NAME "Bodyslide_snapshot-${TODAY}.7z")

# Tenta encontrar o 7-Zip no sistema (seja Windows ou Linux)
find_program(7Z_EXECUTABLE NAMES 7z 7za 7z.exe)
if(7Z_EXECUTABLE)
    add_custom_command(TARGET OutfitStudio POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Packaging snapshot: ${ARCHIVE_NAME}..."
        COMMAND ${7Z_EXECUTABLE} a -t7z "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}" "."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        COMMENT "Creating 7zip snapshot with standard mod structure"
    )
endif()
