#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "ws2812.pio.h"

#include "include/pin.h"
#include "include/led.h"
#include "include/button.h"
#include "include/ws2812.h"

volatile uint8_t led_color = MIN_LED;

volatile uint8_t number = MIN_NUMBER;

// Função para inicializar o LED RGB
void led_init() {    
    gpio_init(LED_RGB_RED_PIN);
    gpio_set_dir(LED_RGB_RED_PIN, GPIO_OUT);
    gpio_init(LED_RGB_GREEN_PIN);
    gpio_set_dir(LED_RGB_GREEN_PIN, GPIO_OUT);
    gpio_init(LED_RGB_BLUE_PIN);
    gpio_set_dir(LED_RGB_BLUE_PIN, GPIO_OUT);
}

// Função para inicializar os botões
void button_init() {
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    // Configura interrupções para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_button_callback);
}

// Função para inicializar o controlador WS2812
void ws2812_init() {
    // Inicializa o controlador WS2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
}

void core1_entry() {
    blink_led();
}

void switch_led() {
    if (led_color < MAX_LED) {
        led_color++;
    } else {
        led_color = MIN_LED;
    }
    
    // Reiniciar o núcleo 1 para atualizar a cor do LED
    multicore_reset_core1();
    multicore_launch_core1(core1_entry);
}

int main() {
    stdio_init_all();

    // Inicializa os pinos do LED RGB
    led_init();

    // Inicializa os pinos dos botões com pull-up
    button_init();

    // Inicializa o controlador WS2812
    ws2812_init();

    // Iniciar o núcleo 1 para piscar o LED
    multicore_launch_core1(core1_entry);

    while (true) {
        if (button_a_pressed && button_b_pressed) {
            button_a_pressed = false;
            button_b_pressed = false;
            
            number = 0;

            switch_led();

            ws2812_clear();
            ws2812_draw();
        }

        sleep_ms(50);

        // Verifica se o botão A foi pressionado
        if (button_a_pressed && !button_b_pressed) {
            button_a_pressed = false;
            if (number > MIN_NUMBER) {
                number--;
            }
            
            display_number(number);
        }

        sleep_ms(50);

        // Verifica se o botão B foi pressionado
        if (button_b_pressed && !button_a_pressed) {
            button_b_pressed = false;
            if (number < MAX_NUMBER) {
                number++;
            }

            display_number(number);
        }
    }
}