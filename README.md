
-----

# CPNR-OMEG-colab: ⁶⁰Co 선원 기반 액체섬광체 검출기 Geant4 시뮬레이션 (광전증배관 및 광학 모형 제외된 고에너지 물리 모델) 

## 1. 개요 (Overview)

본 프로젝트는 ⁶⁰Co (코발트-60) 감마선 교정 선원을 이용한 액체섬광체 검출 시스템의 응답 특성을 평가하기 위한 Geant4 시뮬레이션입니다. 에폭시에 봉인된 디스크 형태의 선원, 100ml 듀란(Duran) 병에 담긴 LAB 기반 액체섬광체, 그리고 Hamamatsu PMT로 구성된 전체 검출기 시스템에서 방사선의 수송 및 에너지 전달을 전산모사합니다.

## 2. 시뮬레이션 명세 (Simulation Specification)

* **방사선원**: 에폭시(Ø30mm, t=5mm) 중앙에 봉인된 ⁶⁰Co 활성 디스크(Ø10mm, t=1mm)
* **물리 프로세스**: `G4RadioactiveDecayPhysics`를 포함한 `FTFP_BERT` or `Sheild` 기반 물리 리스트
* **데이터 출력**: ROOT TTree (`.root` 파일), 액체섬광체 내 에너지 축적량 기록

---
## 3. 실행 환경: Docker 기반의 표준화된 개발 환경 (선택)

이 프로젝트는 다양한 리눅스 배포판에서 발생할 수 있는 시스템 라이브러리(`libc`) 버전 충돌 문제를 원천적으로 방지하고, 어떤 컴퓨터에서든 100% 동일한 결과를 보장하기 위해 **Docker 컨테이너 환경에서 빌드하고 실행하는 것을 표준으로 합니다.**
**물론 libc 버전 충돌이 없다면 컨테이너 기술을 적용안해도 무방 합니다.** 

### 3.1. 왜 Docker를 사용해야 하는가?

최신 Geant4와 ROOT를 구형 OS나 특정 라이브러리 버전을 사용하는 시스템에서 직접 빌드할 경우, 예측하기 어려운 `Segmentation Fault`와 같은 충돌이 발생할 수 있습니다. 저희는 이 문제를 해결하기 위해, 모든 개발 도구와 라이브러리가 완벽하게 갖춰진 격리된 OS 환경을 Docker 컨테이너 기술을 통해 구축했습니다. 이 방법을 통해 학문후속세대의 소중한 시간을 디버깅이 아닌 연구 자체에 집중할 수 있습니다.

### 3.2. 요구사항 (Prerequisites)

* **Docker**: 시스템에 설치되어 있어야 합니다.
* **X11 서버**: GUI 모드로 시뮬레이션을 시각화하려면 필요합니다. (Windows에서는 Xming, VcXsrv 등, macOS에서는 XQuartz 설치)

---
### 3.3. Docker 개발 환경 구축

물리 연구 커뮤니티에서는 RHEL 계열(Rocky/Alma Linux)과 데비안 계열(Ubuntu)이 널리 사용됩니다. 두 가지 방식 모두를 안내합니다.

#### **방법 A: AlmaLinux 9 기반으로 직접 환경 구축하기 (가장 확실하고 추천하는 방법)**

이 방법은 Rocky Linux 9와 거의 동일한 AlmaLinux 9 환경에서 Geant4와 ROOT를 직접 소스 코드로 빌드하여, 우리 프로젝트만을 위한 완벽한 맞춤형 환경을 만드는 과정입니다.

***3.3.1. AlmaLinux 9 컨테이너 시작**
```bash
# 프로젝트 폴더로 이동 후, 아래 명령어로 컨테이너 시작
# --name 옵션으로 컨테이너에 g4dev 라는 이름을 부여합니다.
docker run -it --name g4dev -v "$(pwd)":/work -w /work almalinux:9 bash
````

**3.3.2. 컨테이너 내부에서 필수 패키지 설치**
컨테이너 터미널(`[root@... work]#`) 안에서 아래 명령어를 실행하여 모든 개발 도구와 라이브러리를 설치합니다.

```bash
# 시스템 업데이트 및 기본 개발 도구 설치
dnf update -y
dnf groupinstall "Development Tools" -y
dnf install -y epel-release
dnf config-manager --set-enabled crb

# Geant4와 ROOT 빌드에 필요한 모든 라이브러리 설치
dnf install -y cmake expat-devel xerces-c-devel libX11-devel libXext-devel libXmu-devel \
               libXpm-devel libXft-devel mesa-libGL-devel mesa-libGLU-devel \
               qt6-qtbase-devel 'qt6-*-devel' python3-devel openssl-devel
```

**3.3.3. ROOT 소스 코드 빌드 및 설치** 

```bash
# 소스 코드를 받을 디렉토리로 이동
cd /usr/local/src

# 최신 ROOT 소스 코드 다운로드 및 압축 해제 (버전은 필요에 따라 변경)
wget [https://root.cern/download/root_v6.36.04.source.tar.gz](https://root.cern/download/root_v6.36.04.source.tar.gz)
tar -xzvf root_v6.36.04.source.tar.gz

# 빌드 디렉토리 생성 및 빌드
mkdir root_build && cd root_build
cmake ../root-6.36.04 -DCMAKE_INSTALL_PREFIX=/usr/local/root -Dall=ON
make -j$(nproc)
make install
```

**3.3.4. Geant4 소스 코드 빌드 및 설치**

```bash
# 소스 코드 디렉토리로 다시 이동
cd /usr/local/src

# Geant4 소스 코드 다운로드 및 압축 해제
wget [https://geant4-data.web.cern.ch/geant4-data/releases/geant4-v11.2.1.tar.gz](https://geant4-data.web.cern.ch/geant4-data/releases/geant4-v11.2.1.tar.gz)
tar -xzvf geant4-v11.3.2.tar.gz

# 빌드 디렉토리 생성 및 빌드
mkdir geant4_build && cd geant4_build
source /usr/local/root/bin/thisroot.sh # ROOT 환경을 먼저 활성화
cmake ../geant4-v11.3.2 \
      -DCMAKE_INSTALL_PREFIX=/usr/local/geant4 \
      -DGEANT4_BUILD_MULTITHREADED=ON \
      -DGEANT4_USE_QT6=ON \
      -DGEANT4_USE_ROOT=ON \
      -DGEANT4_INSTALL_DATA=ON
make -j$(nproc)
make install
```

**3.3.5. 완성된 환경을 새 이미지로 저장 (매우 중요)**
모든 설치가 완료되면, `exit`으로 컨테이너를 종료합니다. 그 다음, **호스트 머신의 새 터미널 창**에서 아래 명령어를 실행하여 지금까지의 모든 작업 내용을 `my-g4-env:1.0`이라는 새로운 이미지로 저장합니다.

```bash
docker commit g4dev my-g4-env:1.0
```

이제부터는 위반복적 설치 과정을 반복할 필요 없이, `my-g4-env:1.0` 이미지 하나로 언제든지 동일한 환경을 불러올 수 있습니다.

#### **방법 B: 우분투 기반의 미리 빌드된 이미지 사용하기 (가장 빠른 방법)**

과학 커뮤니티에서 널리 사용되는 우분투 기반의 Geant4 이미지를 사용하면 설치 과정을 생략하고 바로 시뮬레이션을 실행할 수 있습니다.

```bash
# 1. 이미지 내려받기
docker pull geant4/geant4:latest

# 2. 컨테이너 실행 (이미지 이름만 다름)
# 프로젝트 폴더에서 아래 명령어 실행
docker run -it --rm -v "$(pwd)":/work -w /work geant4/geant4:latest bash

# 3. 컨테이너 내부에서 환경 설정 후 바로 빌드
source /usr/local/geant4/bin/geant4.sh
source /usr/local/root/bin/thisroot.sh
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 4\. 빌드 및 실행 방법

### 4.1. `.bashrc`에 Helper 함수 추가 (최초 1회)

편의를 위해 아래 함수를 호스트 머신의 `~/.bashrc` 파일 맨 아래에 추가합니다.

```bash
# Geant4 개발용 Docker 컨테이너를 실행하는 함수
function rung4() {
    local image_name=${1:-"my-g4-env:1.0"}
    echo "Starting container with image: ${image_name}"
    
    # GUI 앱 실행을 위한 X11 포워딩 설정
    xhost +
    
    docker run -it --rm \
        -e DISPLAY=$DISPLAY \
        -v /tmp/.X11-unix:/tmp/.X11-unix \
        --name g4dev \
        -v "$(pwd)":/work \
        -w /work \
        "${image_name}" bash
}
```

추가 후 `source ~/.bashrc` 명령어로 적용합니다.

### 4.2. 빌드 (최초 1회)

1.  프로젝트 최상위 디렉토리에서 컨테이너를 시작합니다.
    ```bash
    rung4
    ```
2.  컨테이너 내부에서 환경 변수를 설정하고 빌드를 진행합니다.
    ```bash
    source /usr/local/geant4/bin/geant4.sh
    source /usr/local/root/bin/thisroot.sh
    mkdir build
    cd build
    cmake ..
    make -j$(nproc)
    ```

### 4.3. 실행

  * **GUI 모드 (인터랙티브):**
    `build` 디렉토리에서 인자 없이 실행합니다.

    ```bash
    ./CPNR_OMEG_colab
    ```
<img width="1320" height="1020" alt="image" src="https://github.com/user-attachments/assets/068f96eb-5873-4648-9f70-90e4fcdb9d84" />

  * **배치 모드 (계산):**
    `build` 디렉토리에서 `run.mac` 파일을 인자로 실행합니다.

    ```bash
    ./CPNR_OMEG_colab ../run.mac
    ```

-----

## 5\. 디버깅 기록 (Troubleshooting)

이 프로젝트를 빌드하는 과정에서 몇 가지 중요한 문제들을 해결했습니다.

  * **시스템 `libc` 충돌**: 초기 Rocky Linux 9 환경에서 Geant4 최신 버전과 라이브러리 충돌로 인한 `segmentation fault`가 지속적으로 발생하여 Docker 환경으로 전환했습니다.
  * **CMake 설정**: Geant4 라이브러리가 Qt 지원을 포함하여 올바르게 빌드되었음에도 불구하고, 사용자 프로젝트에서 GUI가 활성화되지 않는 문제가 있었습니다. 이는 `CMakeLists.txt`가 Geant4의 컴파일 옵션(`-DG4UI_USE`)을 제대로 상속받지 못했기 때문이며, `include(${Geant4_USE_FILE})`과 `${Geant4_LIBRARIES}`를 사용하는 호환성 높은 스크립트로 최종 수정하여 해결했습니다.
  * **Docker X11 포워딩**: 컨테이너의 GUI를 호스트 화면에 표시하기 위해 `xhost +` 설정과 `docker run` 명령어에 `-e DISPLAY` 및 `-v /tmp/.X11-unix` 옵션을 추가했습니다.

<!-- end list -->



