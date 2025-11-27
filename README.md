# Smart Irrigation System Project

**Team Members:**
* **Emir Çelik** (Department of Electrical & Electronics Engineering)
* **Harun Berk Avşar** (Department of Electrical & Electronics Engineering)
* **Selhan Coşkun** (Department of Electrical & Electronics Engineering)

## 1. Project Overview
This project focuses on designing an embedded smart irrigation system to address water waste and inefficient manual irrigation practices. The system utilizes an STM32 microcontroller to monitor real-time environmental data—specifically soil moisture, ambient temperature, and humidity—and automatically controls water pumps based on these readings.

## 2. System Objectives
The primary goals of the project are to:
* **Monitor Environment:** Accurately measure soil moisture via ADC and ambient temperature/humidity via I2C sensors.
* **Automated Control:** Implement a closed-loop system that activates a solenoid valve/pump when soil moisture drops below a pre-set threshold.
* **User Interface:** Display live data and system status on an SSD1306 OLED screen.
* **Manual Override:** Include a push-button mechanism to manually activate the pump using GPIO interrupts.
* **Connectivity:** Transmit data to a PC or cloud via UART/GSM for remote monitoring and debugging.

## 3. Hardware Architecture
The system is built around the **STM32F407 / Nucleo-F446RE** microcontroller acting as the master unit.

| Component | Function | Interface |
| :--- | :--- | :--- |
| **STM32 MCU** | Main control logic and protocol management | Master |
| **Soil Moisture Sensor** | Measures volumetric water content | Analog (ADC) |
| **BME280 Sensor** | Measures temperature & humidity | I2C |
| **SSD1306 OLED** | Displays real-time status | I2C |
| **Solenoid Valve** | Controls water flow | Digital GPIO (Relay/MOSFET) |
| **Push Button** | Manual irrigation trigger | Digital Input (Interrupt) |
| **GSM Module** | IoT Data Transmission | UART |

## 4. System Line Diagram
Below is the line diagram showing the external devices and communication interfaces:

```mermaid
graph TD
    %% Ana İşlemci
    MCU[STM32 Nucleo-F446RE]

    %% Bileşenler
    Soil[Soil Moisture Sensor]
    BME[BME280 Temp/Hum Sensor]
    OLED[OLED Display SSD1306]
    Relay[Relay / Water Valve]
    Btn[Push Button]
    GSM[GSM Module]

    %% Bağlantılar
    Soil -->|Analog PA0| MCU
    MCU <-->|I2C PB8/PB9| BME
    MCU -->|I2C PB8/PB9| OLED
    MCU -->|GPIO Out PA5| Relay
    Btn -->|GPIO In PC13| MCU
    MCU <-->|UART PA9/PA10| GSM

    %% Renklendirme
    style MCU fill:#f9f,stroke:#333,stroke-width:4px
    style Relay fill:#ff9,stroke:#333
    style Soil fill:#bbf,stroke:#333
