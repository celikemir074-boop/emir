/*
 * Project: Smart Irrigation System - GPIO Register Level Driver
 * Hardware: STM32F407 Discovery Board
 * Description: Control of Solenoid Valve (PD12) and Push Button (PA0)
 * using direct Register Access.
 */

#include "stm32f4xx.h"

void GPIO_Config(void) {
    // 1. Clock Aktivasyonu (RCC AHB1 Register)
    // GPIOA (Buton için Bit 0) ve GPIOD (Valf/LED için Bit 3) clock aç.
    RCC->AHB1ENR |= (1UL << 0); // GPIOA Clock Enable
    RCC->AHB1ENR |= (1UL << 3); // GPIOD Clock Enable

    // 2. PD12'yi Output (Çıkış) Olarak Ayarla (MODER Register)
    // Pin 12 için bitler [25:24]. Output modu '01'dir.
    GPIOD->MODER &= ~(3UL << (12 * 2));  // Temizle
    GPIOD->MODER |= (1UL << (12 * 2));   // '01' yap (Output Mode)

    // 3. PA0'ı Input (Giriş) Olarak Ayarla (MODER Register)
    // Pin 0 için bitler [1:0]. Input modu '00'dır.
    GPIOA->MODER &= ~(3UL << (0 * 2));   // Temizle -> '00' (Input Mode)
    
    // 4. PA0 Butonu için Pull-down (Discovery'de gerekebilir ama genelde haricidir)
    // "10" Pull-down mode yapar.
    GPIOA->PUPDR &= ~(3UL << (0 * 2)); // Temizle
    GPIOA->PUPDR |= (2UL << (0 * 2));  // Pull-down (Garanti olsun)
}

int main(void) {
    GPIO_Config();

    while (1) {
        // IDR (Input Data Register) okuma
        // STM32F407 Discovery üzerindeki Mavi buton basılınca "1" (HIGH) verir.
        
        if (GPIOA->IDR & (1UL << 0)) { 
            // BUTONA BASILDI: Valfi (PD12) Aç
            GPIOD->ODR |= (1UL << 12); 
            
        } else {
            // BUTON BIRAKILDI: Valfi (PD12) Kapat
            GPIOD->ODR &= ~(1UL << 12);
        }
        
        // Debounce gecikmesi
        for (volatile int i = 0; i < 50000; i++);
    }
}
