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
    Soil -->|Analog (PA0)| MCU
    MCU <-->|I2C (PB8/PB9)| BME
    MCU -->|I2C (PB8/PB9)| OLED
    MCU -->|GPIO Out (PA5)| Relay
    Btn -->|GPIO In (PC13)| MCU
    MCU <-->|UART (PA9/PA10)| GSM
    
    %% Renklendirme (Opsiyonel)
    style MCU fill:#f9f,stroke:#333,stroke-width:4px
    style Relay fill:#ff9,stroke:#333
    style Soil fill:#bbf,stroke:#333
