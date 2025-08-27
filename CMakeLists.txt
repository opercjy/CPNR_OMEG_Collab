# CMake 최소 요구 버전을 3.16으로 설정합니다.
cmake_minimum_required(VERSION 3.16)

# 프로젝트 이름, 버전, 언어를 정의합니다.
# Geant4 11+ 버전은 C++17 표준이 필요합니다.
project(CPNR_OMEG_colab VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Geant4 패키지를 찾습니다.
# Qt 사용 여부는 빌드 시 cmake 명령어의 옵션(-DGEANT4_USE_QT=ON)으로
# 결정되므로, 스크립트 자체에서는 특별한 분기 처리가 필요 없습니다.
find_package(Geant4 REQUIRED)

# ROOT 패키지를 찾습니다. TTree 출력을 위해 필요한 컴포넌트를 명시합니다.
find_package(ROOT REQUIRED COMPONENTS Core Graf Tree)

# Geant4와 ROOT의 설정을 현재 프로젝트에 적용합니다.
# 이 한 줄로 필요한 헤더 경로와 컴파일러 정의가 대부분 자동으로 설정됩니다.
include(${Geant4_USE_FILE})

# 프로젝트의 헤더 파일이 위치한 디렉토리를 지정합니다.
include_directories(include)

# 컴파일할 소스 파일들의 목록을 변수에 저장합니다.
# file(GLOB ...)을 사용하면 src 디렉토리의 모든 .cc 파일을 자동으로 찾아줍니다.
file(GLOB SOURCES "src/*.cc")

# 실행 파일을 생성하고, main 소스 파일과 위에서 찾은 소스 파일들을 포함시킵니다.
add_executable(CPNR_OMEG_colab CPNR_OMEG_colab.cc ${SOURCES})

# 생성된 실행 파일에 Geant4 라이브러리를 링크합니다.
# Geant4::Geant4 타겟은 필요한 모든 코어 라이브러리를 포함합니다.
target_link_libraries(CPNR_OMEG_colab PRIVATE ${Geant4_LIBRARIES})

# 생성된 실행 파일에 ROOT 라이브러리를 링크합니다.
target_link_libraries(CPNR_OMEG_colab PRIVATE ${ROOT_LIBRARIES})

# (선택 사항) make install 명령을 위한 설치 경로를 지정합니다.
install(TARGETS CPNR_OMEG_colab
  RUNTIME DESTINATION bin
)
