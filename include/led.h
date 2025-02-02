#include "pico/stdlib.h"

#ifndef LED_H
#define LED_H

#define MIN_LED 0
#define MAX_LED 2

// Função para piscar o LED vermelho do LED RGB 5 vezes por segundo
void blink_led(int led_color) {
    int pin;

    switch (led_color) {
        case 2:
            pin = LED_RGB_BLUE_PIN;
            break;
        case 1:
            pin = LED_RGB_GREEN_PIN;
            break;
        case 0:
        default:
            pin = LED_RGB_RED_PIN;
            break;
    }

    while(true) {
        gpio_put(pin, 1);
        sleep_ms(100);
        gpio_put(pin, 0);
        sleep_ms(100);
    }
}

#endif