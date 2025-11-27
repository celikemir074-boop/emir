/*
 * Project: Smart Irrigation System - GPIO Register Level Driver (INTERRUPT VERSION)
 * Hardware: STM32F407 Discovery Board
 * Description: Control of Solenoid Valve (PD12) via Push Button (PA0) using EXTI Interrupt.
 *
 * Logic:
 * - Button Press (Rising Edge) -> Valve ON
 * - Button Release (Falling Edge) -> Valve OFF
 */

#include "stm32f4xx.h"

void GPIO_Config(void) {
    // 1. Clock Aktivasyonu (AHB1)
    RCC->AHB1ENR |= (1UL << 0); // GPIOA Clock Enable (Buton)
    RCC->AHB1ENR |= (1UL << 3); // GPIOD Clock Enable (Valf)

    // 2. SYSCFG Clock Aktivasyonu (Interrupt için gerekli - APB2)
    RCC->APB2ENR |= (1UL << 14); // SYSCFG Clock Enable

    // 3. PD12 Output Ayarı (Valf)
    GPIOD->MODER &= ~(3UL << (12 * 2)); // Temizle
    GPIOD->MODER |= (1UL << (12 * 2));  // '01' Output Mode

    // 4. PA0 Input Ayarı (Buton)
    GPIOA->MODER &= ~(3UL << (0 * 2));  // '00' Input Mode
    GPIOA->PUPDR &= ~(3UL << (0 * 2));  // Temizle
    GPIOA->PUPDR |= (2UL << (0 * 2));   // Pull-down (varsayılan '0')

    // 5. EXTI (Harici Kesme) Ayarları
    // PA0'ı EXTI0 hattına bağla (SYSCFG_EXTICR1)
    // EXTI0, [3:0] bitleridir. PA için '0000' yazılır.
    SYSCFG->EXTICR[0] &= ~(0xF << 0); 

    // Maskeyi kaldır (Interrupt'a izin ver) - IMR Register
    EXTI->IMR |= (1UL << 0); // Line 0 aktif

    // Hem Yükselen (Basınca) Hem Düşen (Bırakınca) Kenarı Algıla
    EXTI->RTSR |= (1UL << 0); // Rising Trigger (Basma)
    EXTI->FTSR |= (1UL << 0); // Falling Trigger (Bırakma)

    // 6. NVIC (Çekirdek Kesme Kontrolcüsü) Ayarı
    // EXTI0 kesmesine işlemci seviyesinde izin ver
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_SetPriority(EXTI0_IRQn, 0); // En yüksek öncelik
}

// Bu fonksiyon SADECE butona basıldığında veya bırakıldığında çalışır
void EXTI0_IRQHandler(void) {
    // Kesme bayrağını kontrol et (Line 0 mı tetiklendi?)
    if (EXTI->PR & (1UL << 0)) {
        
        // Buton durumunu kontrol et (PA0 High mı?)
        if (GPIOA->IDR & (1UL << 0)) {
            // Butona BASILIYOR -> Valfi Aç
            GPIOD->ODR |= (1UL << 12);
        } else {
            // Buton BIRAKILDI -> Valfi Kapat
            GPIOD->ODR &= ~(1UL << 12);
        }

        // ÇOK ÖNEMLİ: Kesme bayrağını temizle (Yoksa sonsuz döngüye girer)
        EXTI->PR |= (1UL << 0); 
    }
}

int main(void) {
    GPIO_Config();

    // Sonsuz döngü (Burada işlemci başka işler yapabilir veya uyuyabilir)
    while (1) {
        // Ana döngü boş. İşlemci sadece interrupt bekliyor.
        // Buraya "System Idle" veya sensör okuma kodları gelebilir.
        __NOP(); 
    }
}
