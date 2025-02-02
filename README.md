# Projeto de Controle de LEDs e Botões com RP2040 🚀

## Visão Geral
Este projeto utiliza o **Raspberry Pi Pico W** com o microcontrolador **RP2040** e a placa de desenvolvimento **BitDogLab** para criar um sistema interativo com LEDs e botões. O objetivo é explorar conceitos avançados de programação de microcontroladores, como interrupções, debouncing de botões e controle de LEDs, tanto comuns quanto endereçáveis (WS2812). O projeto é a solução de uma tarefa para aprender sobre hardware e software embarcado de forma prática e que foi solicitada durante a residência **EmbarcaTech** do **CEPEDI**

## Objetivos 🎯
- **Interrupções**: Entender como funcionam as interrupções em microcontroladores e como utilizá-las para melhorar a eficiência do código.
- **Debouncing**: Implementar debouncing de botões via software para garantir leituras precisas.
- **Controle de LEDs**: Controlar LEDs comuns e LEDs WS2812 (endereçáveis) com efeitos visuais personalizados.
- **Resistores de Pull-up**: Utilizar resistores de pull-up internos do RP2040 para simplificar o circuito dos botões.
- **Integração Hardware-Software**: Desenvolver um projeto que combine hardware e software de forma eficiente.

## Componentes Utilizados 🛠️
- **Matriz 5x5 de LEDs WS2812**: Conectada à GPIO 7.
- **LED RGB**: Pinos conectados às GPIOs 11 (vermelho), 12 (verde) e 13 (azul).
- **Botão A**: Conectado à GPIO 5.
- **Botão B**: Conectado à GPIO 6.
- **Placa BitDogLab**: Baseada no RP2040, utilizada como plataforma de desenvolvimento.

## Funcionalidades ✨
1. **LED RGB**: O LED vermelho pisca 5 vezes por segundo, enquanto os outros LEDs podem ser ativados ao apertar o **Botão A** e o **Botão B** ao mesmo tempo.
2. **Botão A**: Incrementa o número exibido na matriz de LEDs WS2812.
3. **Botão B**: Decrementa o número exibido na matriz de LEDs WS2812.
4. **Matriz de LEDs WS2812**: Exibe números de 0 a 9 com efeitos visuais personalizados, como transições suaves e cores variadas. Além de um suporte a um brilho reduzido para não machucar as vistas do usuário.

## Requisitos 📋
- **Interrupções**: Uso de rotinas de interrupção (IRQ) para detectar pressionamentos dos botões.
- **Debouncing**: Implementação de debouncing via software para evitar leituras falsas dos botões.
- **Controle de LEDs**: Uso de LEDs comuns (RGB) e LEDs WS2812 (endereçáveis) com controle preciso.
- **Código Estruturado**: Código bem organizado, modular e comentado para facilitar a manutenção e avaliação.

## Estrutura do Código 📂
O projeto está organizado em vários arquivos para modularidade e clareza:
- **`embarcatech-acc.c`**: Arquivo principal com a lógica do projeto.
- **`led.h`**: Funções para controle do LED RGB.
- **`button.h`**: Funções para controle dos botões e interrupções.
- **`ws2812.h`**: Funções para controle da matriz de LEDs WS2812.
- **`ws2812.pio`**: Programa PIO para controle dos LEDs WS2812.
- **`CMakeLists.txt`**: Configuração do CMake para compilação.

## Como Compilar e Executar 🏃‍♂️

### Passo a Passo para Compilação

1. **Clone o Repositório**:
   ```bash
   git clone https://github.com/wilssola/embarcatech-acc.git
   cd embarcatech-acc
   ```

2. **Configure o Ambiente de Desenvolvimento**:
   - Instale as ferramentas necessárias para o desenvolvimento com RP2040:
     - **GCC Arm Toolchain**: Compilador para arquitetura ARM.
     - **CMake**: Ferramenta de build.
     - **Pico SDK**: SDK oficial para o RP2040.
   - Configure o ambiente para usar o Pico SDK. Exemplo:
     ```bash
     export PICO_SDK_PATH=/caminho/para/pico-sdk
     ```

3. **Compile o Projeto**:
   - Crie um diretório de build e compile o projeto:
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```
   - Após a compilação, o arquivo `.uf2` será gerado na pasta `build`.

4. **Carregue o Binário na Placa BitDogLab**:
   - Conecte a placa BitDogLab ao computador via USB.
   - Pressione o botão de bootloader da placa e solte.
   - Copie o arquivo `.uf2` gerado para a unidade de armazenamento que aparecerá no computador.

5. **Conecte os Componentes**:
   - Conecte a matriz de LEDs WS2812 à GPIO 7.
   - Conecte o LED RGB às GPIOs 11, 12 e 13.
   - Conecte os botões A e B às GPIOs 5 e 6, respectivamente.

6. **Execute o Projeto**:
   - A placa será reiniciada automaticamente após o carregamento do binário.
   - Observe o funcionamento do projeto: o LED RGB piscará, e os botões controlarão a exibição dos números na matriz de LEDs.

## Ideias para Melhorias Futuras
- Adicionar mais efeitos visuais na matriz de LEDs WS2812.
- Implementar um modo de economia de energia para os LEDs.
- Adicionar suporte para mais botões ou sensores.

## Licença
Este projeto está licenciado sob a [MIT License](https://opensource.org/license/mit). Sinta-se à vontade para utilizá-lo e modificá-lo conforme necessário.