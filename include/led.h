#include "pico/stdlib.h"

#ifndef LED_H
#define LED_H

// Função para piscar o LED vermelho do LED RGB 5 vezes por segundo
void blink_led() {
    gpio_put(LED_RGB_RED_PIN, 1);
    sleep_ms(100);
    gpio_put(LED_RGB_RED_PIN, 0);
    sleep_ms(100);
}

#endif