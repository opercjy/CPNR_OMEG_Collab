
# CPNR-OMEG-colab: ⁶⁰Co 선원 기반 액체섬광체 검출기 Geant4 시뮬레이션

## 1\. 개요 (Overview)

본 프로젝트는 CPNR-OMEG-colab의 일환으로, ⁶⁰Co (코발트-60) 감마선 교정 선원을 이용한 액체섬광체 검출 시스템의 응답 특성을 평가하기 위한 Geant4 시뮬레이션입니다. 에폭시에 봉인된 디스크 형태의 선원, 100ml 듀란(Duran) 병에 담긴 LAB 기반 액체섬광체, 그리고 Hamamatsu PMT로 구성된 전체 검출기 시스템에서 방사선의 수송 및 에너지 전달을 전산모사합니다.

주요 목표는 각 검출기 구성 요소에서의 에너지 축적량을 정량적으로 분석하고, 추후 광학적 모델링을 추가하기 전 방사선 상호작용 단계의 기반을 다지는 것입니다.

-----

## 2\. 시뮬레이션 명세 (Simulation Specification)

본 시뮬레이션은 다음과 같은 물리적, 기하학적 모델을 기반으로 합니다.

### 2.1. 기하학적 구조 (Geometry)

모든 구성 요소는 Z축을 중심으로 동축(co-axial) 배치됩니다.

  * **방사선원**: 에폭시(Epoxy) 재질의 원통(Ø30mm, 두께 5mm) 중앙에 ⁶⁰Co 활성 영역(Ø10mm, 두께 1mm)이 위치합니다.
  * **검출기 용기**: DURAN® GL 45 100ml 규격의 붕규산 유리(Borosilicate Glass) 병.
  * **섬광체**: LAB(Linear Alkyl Benzene) 용매에 PPO(3g/L)와 bis-MSB(30mg/L)가 용해된 액체섬광체.
  * **PMT**: Hamamatsu R7195 모델을 가정한 2인치 직경의 길쭉한 실린더 형태.
  * **배치**: 선원과 듀란 병 상단 사이 거리는 10cm 입니다.

### 2.2. 방사선원 (Primary Generator)

  * **핵종**: ⁶⁰Co
  * **물리 프로세스**: `G4RadioactiveDecayPhysics`를 통해 ⁶⁰Co의 실제 붕괴 사슬(β⁻ 붕괴 후 1.173 MeV, 1.332 MeV 감마선 순차 방출)을 시뮬레이션합니다.
  * **생성 방식**: `G4GeneralParticleSource (GPS)`를 사용하여 활성 영역 내에서 등방성(isotropic)으로 ⁶⁰Co 이온을 생성합니다.

### 2.3. 물리 모델 (Physics List)

  * **기본 물리 리스트**: `FTFP_BERT` (범용 물리 모델)
  * **전자기 물리**: `G4EmStandardPhysics_option4` (정밀 저에너지 전자기 상호작용 모델)
  * **특수 물리**: `G4RadioactiveDecayPhysics` (방사성 붕괴)
  * **특수 물리**: `G4OpticalPhysics` (전자기파 물리)

### 2.4. 데이터 출력 (Data Output)

  * **포맷**: ROOT TTree (`.root` 파일)
  * **저장 정보**:
      * **이벤트 단위**: 각 이벤트 ID, 검출기 주요 볼륨(액체섬광체, 유리병, PMT 광창)별 총 에너지 축적량.
      * **스텝/트랙 단위**: 각 상호작용 단계(step)의 위치, 전달된 에너지, 입자 종류, 생성 프로세스 등의 상세 정보.

-----

## 3\. 요구사항 (Prerequisites)

본 시뮬레이션을 빌드하고 실행하기 위해 다음 환경이 필요합니다.

  * **Geant4**: v11.0 이상
  * **ROOT**: v6.24 이상
  * **CMake**: v3.16 이상
  * **C++ 컴파일러**: C++17 표준을 지원하는 컴파일러 (e.g., GCC 9.0 이상)

-----

## 4\. 빌드 방법 (Build Instructions) ⚙️

1.  **소스 코드 복제 (Clone)**

    ```bash
    git clone https://github.com/opercjy/CPNR_OMEG_Collab
    cd CPNR_OMEG_Collab
    ```

2.  **빌드 디렉토리 생성**

    ```bash
    mkdir build
    cd build
    ```

3.  **환경 설정**
    터미널에서 Geant4와 ROOT 환경을 설정합니다. (경로는 실제 설치 위치에 맞게 수정)

    ```bash
    source /path/to/geant4-install/bin/geant4.sh
    source /path/to/root-install/bin/thisroot.sh
    ```

4.  **CMake 실행 (모드 선택)**

      * **A) 인터랙티브 모드 (Qt GUI 포함) 빌드:**
        ```bash
        cmake -DGEANT4_USE_QT=ON ..
        ```
      * **B) 배치 모드 (GUI 없음) 빌드:**
        ```bash
        cmake ..
        ```

5.  **컴파일**

    ```bash
    make -jN  # N은 빌드에 사용할 CPU 코어 수
    ```

-----

## 5\. 실행 방법 (How to Run) 🚀

빌드가 완료되면 `build/` 디렉토리에 `CPNR_OMEG_colab` 실행 파일이 생성됩니다.

### 5.1. 인터랙티브 모드 (Interactive Mode)

Qt GUI를 통해 검출기 형상을 확인하고, 개별 이벤트를 시각화하며 실행합니다.

```bash
./CPNR_OMEG_colab
```

실행 후 나타나는 `Idle>` 프롬프트에서 Geant4 UI 명령어를 입력할 수 있습니다. (예: `/run/beamOn 1`)
초기 시각화 설정은 `init_vis.mac` 매크로 파일을 통해 자동화할 수 있습니다.

### 5.2. 배치 모드 (Batch Mode)

매크로 파일을 인자로 전달하여 GUI 없이 대량의 이벤트를 실행합니다.

```bash
./CPNR_OMEG_colab run.mac
```

**`run.mac` 파일 예시:**

```
# run.mac
# 시뮬레이션 실행을 위한 매크로 파일

# 출력 파일 이름 설정 (선택 사항)
/analysis/setFileName output.root

# 10,000개의 이벤트 실행
/run/beamOn 10000
```

-----

## 6\. 프로젝트 구조 (Project Structure)

```
CPNR_OMEG_Collab/
├── CMakeLists.txt
├── README.md
├── CPNR_OMEG_colab.cc
├── run.mac
├── init_vis.mac
├── include/
│   ├── DetectorConstruction.hh
│   ├── PhysicsList.hh
│   ├── ActionInitialization.hh
│   ├── PrimaryGeneratorAction.hh
│   ├── RunAction.hh
│   ├── EventAction.hh
│   ├── SteppingAction.hh
│   └── TrackingAction.hh
└── src/
    ├── DetectorConstruction.cc
    ├── PhysicsList.cc
    ├── ActionInitialization.cc
    ├── PrimaryGeneratorAction.cc
    ├── RunAction.cc
    ├── EventAction.cc
    ├── SteppingAction.cc
    └── TrackingAction.cc
```

-----

## 7\. 작성자 (Author)

  * **이름**: 최지영
  * **소속**: 전남대학교 물리학과/중성미자 정밀 연구센터
  * **이메일**: opercjy@chonnam.ac.kr/opercjy@gmail.com
