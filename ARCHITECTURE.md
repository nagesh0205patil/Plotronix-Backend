# PlotTronix VI Lab - Modular Architecture Documentation

## Overview

The PlotTronix VI Lab has been refactored into a **modular, MVC-based architecture** with clear separation of concerns. This structure makes the codebase more maintainable, testable, and scalable.

## Directory Structure

```
Plotronix_VI_Lab/
├── Plotronix_VI_Lab.ino          # Main Arduino sketch (entry point)
├── src/
   ├── core/                     # Application orchestration (Controller)
   │   ├── app_manager.h         # Main application manager
   │   └── app_manager.cpp       # Implementation
   │
   ├── hardware/                 # Hardware abstraction drivers
   │   ├── adc_driver.h          # ADC input abstraction
   │   ├── adc_driver.cpp
   │   ├── display_driver.h      # TFT display abstraction
   │   └── display_driver.cpp
   │
   ├── data/                     # Data models & configuration
   │   ├── config.h              # All configuration constants
   │   ├── sensor_data.h         # SensorData struct
   │   └── component_info.h      # ComponentInfo & enum
   │
   ├── analysis/                 # Business logic
   │   ├── component_detector.h  # Component detection logic
   │   └── component_detector.cpp
   │
   ├── communication/            # Serial protocol & management
   │   ├── serial_protocol.h     # Protocol documentation
   │   ├── serial_manager.h      # Serial communication handler
   │   └── serial_manager.cpp
   │
   └── ui/                       # Presentation layer (View)
       ├── display_manager.h     # High-level display control
       └── display_manager.cpp

```

## Architecture Pattern: Model-View-Controller (MVC)

### Model Layer
- **SensorData** (`src/data/sensor_data.h`) - Sensor measurements
- **ComponentInfo** (`src/data/component_info.h`) - Component identification
- Configuration constants (`src/data/config.h`)

### View Layer  
- **DisplayManager** (`src/ui/display_manager.h`) - Presentation logic
- **DisplayDriver** (`src/hardware/display_driver.h`) - Hardware control abstraction

### Controller Layer
- **AppManager** (`src/core/app_manager.h`) - Application orchestration
- Coordinates data flow between subsystems
- Manages timing and state

## Module Descriptions

### 1. Core (`src/core/`)
**Responsibility**: Application orchestration

- **AppManager**: Singleton that manages the entire application lifecycle
  - Initializes all subsystems
  - Executes main measurement cycle
  - Coordinates component interactions
  - Uses the Singleton pattern for global access

### 2. Hardware (`src/hardware/`)
**Responsibility**: Hardware abstraction layer (drivers)

- **ADCDriver**: Abstracts analog-to-digital conversion
  - Provides `readAllSensors()` interface
  - Handles averaging and voltage calibration
  - Isolates raw ADC operations

- **DisplayDriver**: Abstracts ST7735 TFT display
  - Low-level display operations
  - Wraps Adafruit_ST7735 library
  - Easy to swap display hardware

### 3. Analysis (`src/analysis/`)
**Responsibility**: Business logic and data analysis

- **ComponentDetector**: Identifies component type
  - Maps selector voltage to component type
  - Encapsulates detection thresholds
  - Returns structured ComponentInfo

### 4. Communication (`src/communication/`)
**Responsibility**: Serial protocol and state management

- **SerialManager**: Bidirectional serial communication
  - Processes HOST → FIRMWARE commands (START/STOP)
  - Sends FIRMWARE → HOST data frames
  - Maintains streaming state
  - Rate-limited transmission

- **SerialProtocol**: Protocol definitions
  - Documents command contract
  - Defines frame format
  - Baud rate and format specifications

### 5. UI (`src/ui/`)
**Responsibility**: Display management and presentation

- **DisplayManager**: High-level display control
  - Shows startup screen
  - Updates measurement display
  - Rate-limited rendering
  - Abstracts display operations

### 6. Data (`src/data/`)
**Responsibility**: Data structures and configuration

- **sensor_data.h**: SensorData struct definition
- **component_info.h**: ComponentInfo struct and ComponentType enum
- **config.h**: Centralized configuration constants
  - Pin definitions
  - Timing parameters
  - ADC settings
  - Baud rate

## Data Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                      Arduino Loop                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  AppManager::update()                                          │
│      ↓                                                          │
│  SerialManager::processIncomingCommands()  ← Host (START/STOP) │
│      ↓                                                          │
│  [if not ready, exit]                                          │
│      ↓                                                          │
│  ADCDriver::readAllSensors()  ← Hardware (Voltage/Current)    │
│      ↓                                                          │
│  ComponentDetector::detect()  ← Analysis (Type Detection)      │
│      ↓                                                          │
│  DisplayManager::updateMeasurementDisplay()  → Display (UI)    │
│      ↓                                                          │
│  SerialManager::sendDataFrame()  → Host (DATA,TAG,V,I)        │
│      ↓                                                          │
│  [loop completes, return to Arduino]                          │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

## Key Design Principles

### 1. Separation of Concerns
- Each module has a single, well-defined responsibility
- Modules are loosely coupled
- Easy to modify or replace individual components

### 2. Hardware Abstraction
- ADCDriver and DisplayDriver isolate hardware details
- Easy to swap drivers for different hardware
- Testable without physical hardware

### 3. Configuration Centralization
- All constants in `config.h`
- Single source of truth for timing, pins, thresholds
- Easy to adjust without touching logic code

### 4. Clear Data Models
- Explicit struct definitions (SensorData, ComponentInfo)
- Type-safe component enumeration
- Self-documenting code

### 5. Singleton Pattern
- AppManager is a singleton for convenient global access
- Ensures single instance during application lifetime
- Clean integration with Arduino's setup/loop pattern

## Serial Protocol

### HOST → FIRMWARE (Commands)
- `START` - Begin streaming measurements
- `STOP` - Stop streaming measurements
- Commands are newline-terminated

### FIRMWARE → HOST (Data Frames)
```
DATA,<TAG>,<VOLTAGE>,<CURRENT>
```
- TAG: Component identifier (DIODE, ZENER, BJT_CB, etc.)
- VOLTAGE: Float, 3 decimal places
- CURRENT: Float, 5 decimal places
- Baud Rate: 9600

## Adding New Features

### To add a new sensor:
1. Update `config.h` with new pin definition
2. Extend `ADCDriver::readAllSensors()` to read new pin
3. Add field to `SensorData` struct if needed

### To add a new component type:
1. Add enum value to `ComponentType` in `component_info.h`
2. Add threshold in `ComponentDetector::detect()`
3. Define tag and display name

### To modify display layout:
1. Edit `DisplayManager::updateMeasurementDisplay()`
2. Modify setCursor positions and print statements
3. No need to touch hardware or core logic

### To change serial protocol:
1. Update `serial_protocol.h` documentation
2. Modify `SerialManager::sendDataFrame()` format
3. Update host GUI expectations

## Compilation Notes

- Ensure `#include` paths point to new `src/` structure
- All hardware dependencies remain the same:
  - Adafruit_GFX
  - Adafruit_ST7735
  - SPI (for display)

---

**Architecture Refactored**: May 2026
**Pattern**: MVC with Hardware Abstraction Layer (HAL)
