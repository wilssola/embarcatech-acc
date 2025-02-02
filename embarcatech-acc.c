#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "ws2812.pio.h"

#define LED_RGB_RED_PIN 11
#define LED_RGB_GREEN_PIN 12
#define LED_RGB_BLUE_PIN 13
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define WS2812_PIN 7

#define DEBOUNCE_DELAY_MS 50

volatile uint8_t number = 0;

volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;

volatile uint32_t last_button_a_time = 0;
volatile uint32_t last_button_b_time = 0;

// Função para inicializar o controlador WS2812
void ws2812_init() {
    // Inicializa o controlador WS2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
}

#define MATRIX_WIDTH 5
#define MATRIX_HEIGHT 5
#define MATRIX_FLIPPED 1 // Defina como 1 se a matriz estiver de cabeça para baixo, 0 caso contrário

uint32_t led_matrix[MATRIX_HEIGHT][MATRIX_WIDTH];

// Função para definir a cor de um pixel na matriz de LEDs WS2812
void ws2812_set_pixel(uint8_t x, uint8_t y, uint32_t color) {
    if (x < MATRIX_WIDTH && y < MATRIX_HEIGHT) {
        led_matrix[y][x] = color;
    }
}

uint32_t adjust_brightness(uint32_t color, float brightness) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    r = (uint8_t)(r * brightness);
    g = (uint8_t)(g * brightness);
    b = (uint8_t)(b * brightness);

    return (r << 16) | (g << 8) | b;
}

// Função para atualizar a matriz de LEDs WS2812
void ws2812_draw() {
    float brightness = 0.01; // Ajuste o valor de 0.0 a 1.0 para definir o brilho
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            // Calcular a posição correta considerando a orientação da matriz
            uint8_t draw_y = MATRIX_FLIPPED ? (MATRIX_HEIGHT - 1 - y) : y;        

            uint32_t color = led_matrix[draw_y][x];
            color = adjust_brightness(color, brightness);

            // Enviar a cor ajustada para o LED correspondente
            pio_sm_put_blocking(pio0, 0, color << 8u);
        }
    }
}

// Função para exibir o número na matriz de LEDs WS2812
void display_number(uint8_t num) {
    // Cores para cada número de 0 a 9
    uint32_t colors[] = {
        0xFF0000, // Vermelho para 0
        0x00FF00, // Verde para 1
        0x0000FF, // Azul para 2
        0xFFFF00, // Amarelo para 3
        0xFF00FF, // Magenta para 4
        0x00FFFF, // Ciano para 5
        0xFFFFFF, // Branco para 6
        0xFF8000, // Laranja para 7
        0x8000FF, // Roxo para 8
        0x0080FF  // Azul claro para 9
    };

    // Padrões de LEDs para cada número de 0 a 9
    uint8_t patterns[10][MATRIX_HEIGHT][MATRIX_WIDTH] = {
        // 0
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}
        },
        // 1
        {
            {0, 0, 1, 0, 0},
            {0, 1, 1, 0, 0},
            {1, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {1, 1, 1, 1, 1}
        },
        // 2
        {
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1}
        },
        // 3
        {
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}
        },
        // 4
        {
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1}
        },
        // 5
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}
        },
        // 6
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}
        },
        // 7
        {
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1}
        },
        // 8
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}
        },
        // 9
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}
        }
    };

    uint32_t color = colors[num];

    // Limpa a matriz
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            ws2812_set_pixel(x, y, 0x000000); // Apaga o pixel
        }
    }

    // Define os LEDs para o número
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            if (patterns[num][y][x] == 1) {
                ws2812_set_pixel(x, y, color);
            }
        }
    }

    // Atualiza a matriz
    ws2812_draw();
}

// Callback de interrupção para os botões
void gpio_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio == BUTTON_A_PIN && (current_time - last_button_a_time) > DEBOUNCE_DELAY_MS) {
        button_a_pressed = true;
        last_button_a_time = current_time;
    } else if (gpio == BUTTON_B_PIN && (current_time - last_button_b_time) > DEBOUNCE_DELAY_MS) {
        button_b_pressed = true;
        last_button_b_time = current_time;
    }
}

int main() {
    stdio_init_all();

    // Inicializa os pinos do LED RGB
    gpio_init(LED_RGB_RED_PIN);
    gpio_set_dir(LED_RGB_RED_PIN, GPIO_OUT);
    gpio_init(LED_RGB_GREEN_PIN);
    gpio_set_dir(LED_RGB_GREEN_PIN, GPIO_OUT);
    gpio_init(LED_RGB_BLUE_PIN);
    gpio_set_dir(LED_RGB_BLUE_PIN, GPIO_OUT);

    // Inicializa os pinos dos botões com pull-up
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    // Configura interrupções para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Inicializa o controlador WS2812
    ws2812_init();

    while (true) {
        // Piscar o LED vermelho do LED RGB 5 vezes por segundo
        gpio_put(LED_RGB_RED_PIN, 1);
        sleep_ms(100);
        gpio_put(LED_RGB_RED_PIN, 0);
        sleep_ms(100);

        // Verifica se o botão A foi pressionado
        if (button_a_pressed) {
            button_a_pressed = false;
            if (number < 9) {
                number++;
            }
            display_number(number);
        }

        // Verifica se o botão B foi pressionado
        if (button_b_pressed) {
            button_b_pressed = false;
            if (number > 0) {
                number--;
            }
            display_number(number);
        }
    }
}