# 3.2 Hardware - Placa Microcontroladora

## Modelo Escolhido

**ESP32 DevKit V1** (placa de desenvolvimento baseada no SoC ESP32-WROOM-32, versão de 30 ou 38 pinos).

## Especificações Técnicas

| Característica | Especificação |
|---|---|
| Processador | Xtensa® 32-bit LX6, dual-core, até 240 MHz |
| Memória RAM (SRAM) | 520 KB |
| Memória Flash | 4 MB |
| Conectividade | Wi-Fi 802.11 b/g/n (2,4 GHz) + Bluetooth Classic/BLE |
| Conversores ADC | 2 controladores (ADC1: 8 canais / ADC2: 10 canais), resolução de até 12 bits |
| GPIOs disponíveis | Cerca de 25 pinos de uso geral (variando conforme a versão de 30 ou 38 pinos) |
| Tensão de operação | 3,3 V |
| Alimentação | 5 V via USB (Micro-USB ou USB-C, dependendo do módulo), regulada internamente para 3,3 V |
| Interfaces de comunicação | UART, SPI, I²C, PWM |
| Consumo de corrente | ~80-260 mA em operação ativa com Wi-Fi transmitindo; ~20 mA em modo modem-sleep; ~10 µA em deep sleep |

## Justificativa Técnica

A escolha do ESP32 DevKit V1 para este projeto se justifica por três fatores diretamente ligados aos requisitos do sistema de monitoramento de postes:

**1. Necessidade de conectividade Wi-Fi nativa.** O sistema depende de duas funções que exigem acesso à internet: o envio periódico dos dados de luminosidade ao ThingSpeak (protocolo HTTP) e a sincronização do horário via NTP, usada na lógica de comparação dia/noite. Placas sem conectividade nativa, como Arduino Uno ou Mega, exigiriam um módulo Wi-Fi externo (como o ESP8266 em modo shield), aumentando custo, complexidade de montagem e pontos de falha — o ESP32 resolve isso com uma única placa.

**2. Compatibilidade dos canais ADC com o uso simultâneo de Wi-Fi.** O projeto utiliza três sensores LDR, cada um exigindo leitura analógica contínua enquanto o rádio Wi-Fi permanece ativo para o envio de alertas. Os canais ADC2 do ESP32 compartilham hardware com o rádio Wi-Fi e apresentam falhas de leitura quando ele está em uso; por isso, o sistema foi projetado para usar exclusivamente os canais ADC1 (GPIO 34, 35 e 32), que não sofrem essa interferência. Essa característica específica do ESP32 é decisiva para a confiabilidade do monitoramento em tempo real.

**3. Processamento dual-core para tarefas concorrentes.** O firmware precisa executar, de forma praticamente simultânea, a leitura periódica dos três sensores, a lógica condicional de detecção de falhas e as chamadas à API do ThingSpeak. O núcleo dual-core do ESP32 (240 MHz) permite distribuir essas tarefas sem gargalos perceptíveis, algo que um microcontrolador single-core de 16 MHz, como o do Arduino Uno, teria dificuldade em sustentar com a mesma responsividade.

Além dos fatores técnicos, o ESP32 tem baixo custo de aquisição (poucas dezenas de reais), ampla documentação e forte suporte da comunidade para bibliotecas de Wi-Fi e integração com plataformas IoT como o ThingSpeak — o que reduz o tempo de desenvolvimento em um projeto acadêmico com prazo definido.

## Consumo de Energia

Na configuração do projeto, o ESP32 permanece com o rádio Wi-Fi ativo continuamente (para manter a conexão com a internet), operando predominantemente no modo ativo. Isso resulta em um consumo médio estimado entre 80 mA e 160 mA, com picos de até 260 mA durante a transmissão de pacotes Wi-Fi (por exemplo, ao enviar os dados ao ThingSpeak). Como a alimentação é feita via cabo USB conectado a uma fonte de 5V, esse consumo é plenamente suportado sem necessidade de bateria ou fonte externa dedicada, o que é adequado para o cenário de demonstração da maquete, mas seria um ponto a reconsiderar (ex.: uso de deep sleep entre leituras) em uma instalação real de campo alimentada por bateria ou painel solar.
