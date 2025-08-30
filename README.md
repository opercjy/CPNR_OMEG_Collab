
# CPNR-OMEG-colab: ⁶⁰Co 선원 기반 액체섬광체 검출기 Geant4 시뮬레이션 (알파버전, 광학/광전증배관 모델 미구현)

## 1. 개요 (Overview)

본 프로젝트는 ⁶⁰Co (코발트-60) 감마선 교정 선원을 이용한 액체섬광체(LS) 검출 시스템의 응답 특성을 평가하기 위한 Geant4 시뮬레이션입니다. 이 프로젝트는 Geant4의 고급 기능을 활용하여 두 가지 주요 방식으로 데이터를 출력하도록 설계되었습니다.

1.  **정밀 이벤트 데이터 (Detailed Event Data):** LS 검출기 내(**액체섬광체(LS)와 PMT 윈도우**)에서 발생하는 모든 물리적 상호작용(hit)에 대한 상세 정보(입자 종류, 운동량, 에너지 손실, 생성 프로세스 등)를 TTree 형태로 저장합니다. 이는 `G4VSensitiveDetector`와 `G4VHit` 클래스를 통해 구현됩니다.
2.  **복셀화 선량 데이터 (Voxelized Dose Data):** LS 검출기 영역을 사용자가 정의한 3차원 격자(복셀, Voxel)로 나누고, 각 복셀에 흡수된 평균 선량(Dose)을 CSV 파일로 저장합니다. 이 기능은 Geant4에 내장된 `G4ScoringManager`를 통해 구현됩니다.


---
## 2. 기술 스택 및 물리 모델

* **시뮬레이션**: Geant4 (v11.3)
* **데이터 분석**: ROOT
* **언어**: C++17
* **빌드**: CMake
* **물리 리스트**: `G4VModularPhysicsList`를 상속받는 사용자 정의 물리 리스트(Custom Physics List)를 사용합니다. 이는 `Shielding`과 같은 표준 리스트를 직접 사용하는 것보다 제어가 명확하고, UI 명령어 충돌과 같은 예기치 않은 문제를 방지하는 데 유리합니다. 포함된 주요 모듈은 다음과 같습니다.
    * `G4EmStandardPhysics` (전자기 상호작용)
    * `G4RadioactiveDecayPhysics` (방사성 붕괴, UI 명령어 활성화)
    * `G4DecayPhysics` (일반 입자 붕괴)
    * `G4HadronPhysicsFTFP_BERT_HP` (고정밀 중성자 모델을 포함한 강입자 상호작용)
    * `G4IonPhysics` 및 `G4StoppingPhysics`

---
## 3. 실행 환경: Docker 기반 표준화

라이브러리 버전 충돌을 원천적으로 방지하고 완벽한 재현성을 보장하기 위해, **Docker 컨테이너 환경에서 빌드하고 실행하는 것을 표준으로 합니다.**

### 3.1. 최초 1회 환경 구축

**1) Docker 이미지 생성:** 이 저장소의 지침에 따라 Geant4와 ROOT를 소스 빌드한 컨테이너를 `my-g4-env:1.0`이라는 이미지로 저장합니다. 이 과정은 최초 한 번만 수행하면 됩니다.

**2) 호스트 머신에 Helper 함수 추가:** 편의를 위해 아래 함수를 **호스트 머신(로컬 PC)**의 `~/.bashrc` 또는 `.zshrc` 파일에 추가합니다. 이 함수는 X11 GUI 포워딩 및 관련 오류를 자동으로 처리합니다.

```bash
# Geant4 개발용 Docker 컨테이너를 실행하는 함수 (최종 수정 버전)
function rung4() {
    local image_name=${1:-"my-g4-env:1.0"}
    echo "Starting container with image: ${image_name}"

    xhost +

    local runtime_dir="/tmp/runtime-$(id -u)"
    mkdir -p "${runtime_dir}"
    chmod 0700 "${runtime_dir}"

    docker run -it --rm \
        -e DISPLAY=$DISPLAY \
        -v /tmp/.X11-unix:/tmp/.X11-unix \
        -e XDG_RUNTIME_DIR="${runtime_dir}" \
        -v "${runtime_dir}":"${runtime_dir}" \
        --name g4dev \
        -v "$(pwd)":/work \
        -w /work \
        "${image_name}" bash
}
```

추가 후 `source ~/.bashrc` 명령어로 적용하거나 새 터미널을 엽니다.

### 3.2. 컨테이너 환경 변수 영구 설정 (선택 사항)

매번 컨테이너에 접속할 때마다 `source` 명령어를 입력하는 것이 번거롭다면, **컨테이너 내부의** `~/.bashrc` 파일에 환경 변수를 미리 등록해두는 것이 편리합니다.

```bash
# rung4 명령으로 컨테이너에 접속한 뒤, 아래 명령어들을 실행
echo '' >> ~/.bashrc
echo '# Auto-load Geant4 and ROOT environments' >> ~/.bashrc
echo 'source /usr/local/geant4/bin/geant4.sh' >> ~/.bashrc
echo 'source /usr/local/root/bin/thisroot.sh' >> ~/.bashrc
```

이렇게 설정하면, 다음부터 컨테이너에 접속할 때 Geant4와 ROOT 환경이 자동으로 활성화됩니다.

-----

## 4\. 빌드 및 실행 방법

### 4.1. 빌드 (최초 또는 코드 수정 시)

** 중요:** C++ 헤더(`*.hh`) 파일을 수정하거나 `CMakeLists.txt`를 변경하는 등 프로젝트 구조에 중요한 변경이 있을 경우, 기존 `build` 디렉토리를 완전히 삭제하고 새로 빌드하는 \*\*클린 빌드(Clean Build)\*\*를 강력히 권장합니다. 이는 오래된 컴파일 결과물(`.o` 파일)과의 예측 불가능한 충돌을 방지하는 가장 확실한 방법입니다.

```bash
# 1. 프로젝트 최상위 디렉토리로 이동
cd /path/to/CPNR-OMEG-colab

# 2. 기존 빌드 디렉토리 삭제 (클린 빌드를 위해)
rm -rf build

# 3. 새 빌드 디렉토리 생성 및 이동
mkdir build && cd build

# 4. rung4 함수로 컨테이너 시작
rung4

# 5. 컨테이너 내부에서 빌드
# (만약 3.2 단계를 수행했다면 아래 source 명령어들은 생략 가능)
source /usr/local/geant4/bin/geant4.sh
source /usr/local/root/bin/thisroot.sh
cmake ..
make -j$(nproc)
```

### 4.2. 실행

#### GUI 모드 (시각화 및 디버깅)

`build` 디렉토리에서 인자 없이 실행합니다. `CPNR_OMEG_colab.cc`에 설정된 `init_vis_voxel.mac`이 자동으로 실행되어, 입자 트랙과 복셀화된 선량 분포를 시각적으로 확인할 수 있습니다.

```bash
./CPNR_OMEG_colab
```

#### 배치 모드 (대량 계산)

`build` 디렉토리에서 `run_voxel.mac` 파일을 인자로 실행합니다. GUI 없이 터미널에서만 계산이 진행됩니다.

```bash
./CPNR_OMEG_colab ../run_voxel.mac
```

-----

## 5\. 데이터 출력 및 분석

실행 후 `build` 디렉토리에 다음 결과 파일들이 생성됩니다.

1.  **`CPNR_OMEG_colab_voxel.root`**:
      * **`Hits` (TTree):** 액체섬광체 내의 모든 에너지 손실(step)에 대한 상세 정보가 저장됩니다.
      * **`EventSummary` (TTree):** 각 이벤트별로 액체섬광체에 들어온 1차/2차 입자의 총 개수가 저장됩니다.
2.  **`LSDose_1M_events.csv`**:
      * 액체섬광체 영역에 설정된 1mm³ 복셀 격자 각 셀의 **흡수 선량(Dose)** 값이 Gy 단위로 저장됩니다.
3.  **분석 스크립트**:
      * 선호하는 방법으로 분석
-----

## 6\. Geant4 v11 주요 명령어 및 학습 내용

본 프로젝트를 진행하며 확인된 Geant4 최신 버전(v11.x)의 주요 명령어 및 특징은 다음과 같습니다.

  * **방사성 붕괴 임계값**: Geant4 v11.2부터 반감기가 **1년 이상**인 핵종은 계산 효율을 위해 기본적으로 붕괴하지 않도록 변경되었습니다. Co-60(반감기 5.27년)과 같은 장수명 핵종을 시뮬레이션하려면, 매크로에서 반드시 다음 명령어를 `/run/initialize` **이후에** 실행해야 합니다.

      * `/process/had/rdm/thresholdForVeryLongDecayTime 1.0e+60 year`

  * **방사성 붕괴 명령어 경로**: Geant4 v11.0부터 기존의 `/grdm/` 명령어는 **완전히 제거**되고, `/process/had/rdm/` 경로가 유일한 표준이 되었습니다.

  * **스코어링 메쉬 명령어**: 복셀화(`Scoring Mesh`) 관련 명령어는 버전에 따라 문법이 다를 수 있습니다. v11.3 기준, 필터 적용 방식은 다음과 같습니다.

    1.  필터 생성: `/score/filter/volumeFilter <필터이름> <논리볼륨이름>`
    2.  필터 적용: `/score/quantity/doseDeposit <데이터이름> <필터이름>`

<!-- end list -->
