#!/bin/bash

# Cores para o output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

BUILD_DIR="build"
THREADS=$(nproc)

echo -e "${BLUE}=== BodySlide & Outfit Studio - Linux Build Script ===${NC}"

# Verifica argumentos
if [[ "$1" == "clean" ]]; then
    echo -e "${RED}Limpando diretório de build...${NC}"
    rm -rf $BUILD_DIR
    exit 0
fi

# Cria diretório de build se não existir
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${BLUE}Criando diretório de build...${NC}"
    mkdir $BUILD_DIR
fi

cd $BUILD_DIR

# Configuração do CMake
echo -e "${BLUE}Configurando projeto com CMake...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo -e "${RED}Erro na configuração do CMake!${NC}"
    exit 1
fi

# Compilação
echo -e "${BLUE}Iniciando compilação com $THREADS threads...${NC}"
cmake --build . --config Release -j $THREADS

if [ $? -eq 0 ]; then
    echo -e "${GREEN}=== Compilação concluída com sucesso! ===${NC}"
    echo -e "${BLUE}Os executáveis estão em: ${NC}"
    echo -e "${GREEN}$(pwd)/bin/CalienteTools/BodySlide/${NC}"
else
    echo -e "${RED}Erro durante a compilação!${NC}"
    exit 1
fi
