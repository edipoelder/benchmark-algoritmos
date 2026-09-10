cmake_minimum_required(VERSION 3.12)
project(MultiTargetBench CXX)

# Força o padrão C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Garante que a pasta 'bin' seja criada na raiz do projeto (onde está o CMakeLists.txt)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# 1. Busca todos os arquivos fontes em src/
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.h")

# Lista para armazenar o caminho de todos os executáveis gerados
set(BENCH_EXECUTABLES "")

# Loop para criar um alvo/binário para cada arquivo .cpp
foreach(SOURCE_FILE ${SOURCES})
    # Extrai o nome do arquivo sem a extensão
    get_filename_component(TARGET_NAME ${SOURCE_FILE} NAME_WE)

    # Cria o executável com o mesmo nome do arquivo fonte
    add_executable(${TARGET_NAME} ${SOURCE_FILE} ${HEADERS})

    # Adiciona o caminho do binário à lista de benchmarks
    list(APPEND BENCH_EXECUTABLES "$<TARGET_FILE:${TARGET_NAME}>")
endforeach()

# 2. Alvo: build (Compila todos os executáveis encontrados)
add_custom_target(build ALL DEPENDS ${SOURCES})

# 3. Alvo: run_bench
# Define o arquivo de log diretamente na raiz do projeto
set(LOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/bench_log.txt")

# Comando para limpar o log antigo usando o motor do CMake (Cross-platform)
add_custom_target(run_bench
        COMMAND ${CMAKE_COMMAND} -E rm -f "${LOG_FILE}"
        COMMENT "Limpando log antigo..."
)

# Adiciona a execução cumulativa de cada binário ao alvo run_bench
foreach(EXE_PATH ${BENCH_EXECUTABLES})
    add_custom_command(TARGET run_bench POST_BUILD
            COMMAND ${EXE_PATH} >> "${LOG_FILE}" 2>&1
            COMMENT "Executando ${EXE_PATH}..."
            VERBATIM
    )
endforeach()

# Garante que todos os binários sejam compilados antes do bench rodar
add_dependencies(run_bench build)

# 4. Alvo: clean
# Adiciona o bench_log.txt e a pasta bin inteira para serem limpos no 'make clean'
set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES "${LOG_FILE};${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
