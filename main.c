/*
 * Project: Smart Irrigation System - GPIO Register Level Driver
 * Author: Team Proposal
 * Description: Control of Solenoid Valve (Output) and Push Button (Input)
 * using direct Register Access (CMSIS).
 */

#include "stm32f4xx.h"  // STM32F4 serisi için başlık

void GPIO_Config(void) {
    // 1. Clock Aktivasyonu (RCC AHB1 Register)
    // GPIOA (Bit 0) ve GPIOC (Bit 2) clock sinyallerini aktif et.
    RCC->AHB1ENR |= (1UL << 0); // GPIOA Clock Enable
    RCC->AHB1ENR |= (1UL << 2); // GPIOC Clock Enable

    // 2. PA5'i Output (Çıkış) Olarak Ayarla (MODER Register)
    // Pin 5 için bitler [11:10]. Output modu '01'dir.
    GPIOA->MODER &= ~(3UL << (5 * 2));  // Önce temizle
    GPIOA->MODER |= (1UL << (5 * 2));   // '01' yap (Output Mode)

    // 3. PC13'ü Input (Giriş) Olarak Ayarla (MODER Register)
    // Pin 13 için bitler [27:26]. Input modu '00'dır.
    GPIOC->MODER &= ~(3UL << (13 * 2)); // Temizle -> '00' (Input Mode)
    
    // 4. Buton için Pull-up/Pull-down Ayarı (PUPDR)
    GPIOC->PUPDR &= ~(3UL << (13 * 2)); // No pull-up, no pull-down
}

int main(void) {
    // GPIO Ayarlarını Yükle
    GPIO_Config();

    while (1) {
        // IDR (Input Data Register) okuma işlemi
        // PC13 pininin durumunu kontrol et (Bit 13)
        // Nucleo butonuna basılınca LOW (0) olur.
        
        if (!(GPIOC->IDR & (1UL << 13))) { 
            // BUTONA BASILDI: Valfi (PA5) Aç (HIGH)
            GPIOA->ODR |= (1UL << 5); 
            
        } else {
            // BUTON BIRAKILDI: Valfi (PA5) Kapat (LOW)
            GPIOA->ODR &= ~(1UL << 5);
        }
        
        // Basit bir gecikme (Debounce için)
        for (volatile int i = 0; i < 50000; i++);
    }
}
