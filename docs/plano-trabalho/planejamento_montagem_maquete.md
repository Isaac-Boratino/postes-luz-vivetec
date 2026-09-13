# Planejamento da Montagem da Maquete — Projeto VivEtec

> **Status:** proposta de montagem para avaliação do grupo. As etapas, a ordem e o mapeamento de pinos abaixo são uma sugestão de ponto de partida — a equipe decide se serão seguidas conforme apresentado ou ajustadas conforme os componentes recebidos por patrocínio da escola.

## Como Montar a Maquete — Visão Geral

A maquete representa 3 postes de iluminação pública, cada um com:
- Um **LED** simulando a lâmpada do poste;
- Um **LDR** posicionado para captar a luz emitida por esse LED;
- Resistores de apoio (10 kΩ para o LDR, 220 Ω para o LED);
- Uma **chave gangorra de 3 posições (ON-OFF-ON)**, que simula os 3 estados possíveis daquele poste: normal, queimado ou aceso em horário errado.

Além disso, a maquete conta com um **buzzer** (reforço sonoro), ligado a um único **ESP32 DevKit V1**, que centraliza a leitura dos sensores e o envio dos dados ao ThingSpeak.

A montagem segue uma lógica de "de dentro para fora": primeiro o cérebro do sistema (ESP32) fixo na base, depois cada poste individualmente (LED + LDR + chave gangorra), depois o componente de alerta (buzzer), e por fim a fiação de conexão entre tudo.

## Lista de Componentes e Função na Montagem

| Componente | Quantidade | Função na Maquete |
|---|---|---|
| ESP32 DevKit V1 | 1 | Microcontrolador central — leitura dos sensores e envio ao ThingSpeak |
| LED (5 mm, branco ou amarelo) | 3 | Simula a lâmpada de cada poste |
| LDR (fotorresistor) | 3 | Mede a luminosidade emitida pelo LED do poste correspondente |
| Resistor 220 Ω | 3 | Protege cada LED contra sobrecorrente |
| Resistor 10 kΩ | 3 | Forma o divisor de tensão de cada LDR |
| Chave gangorra 3 posições (ON-OFF-ON) | 3 (uma por poste) | Simula, por poste, o estado normal, queimado ou aceso em horário errado |
| Buzzer | 1 | Emite alerta sonoro local quando uma falha é detectada |
| Protoboard | 1 (ou 3 pequenas) | Base de montagem dos circuitos, sem solda |
| Jumpers macho-macho | conforme necessidade | Interligação entre ESP32, protoboard e componentes |
| Cabo USB (Micro-USB ou USB-C) | 1 | Alimentação do ESP32 (5V) |
| Estrutura física dos postes (haste + base) | 3 | Suporte físico onde o LED, o LDR e a chave gangorra ficam fixados |

## Mapeamento Sugerido de Pinos (ESP32)

| Componente | Pino do ESP32 | Observação |
|---|---|---|
| LDR — Poste 1 | GPIO 34 (ADC1_CH6) | Entrada analógica, canal ADC1 (não interfere com o Wi-Fi) |
| LDR — Poste 2 | GPIO 35 (ADC1_CH7) | Entrada analógica, canal ADC1 |
| LDR — Poste 3 | GPIO 32 (ADC1_CH4) | Entrada analógica, canal ADC1 |
| LED — Poste 1 | GPIO 25 | Saída digital — ligada ao terminal comum da chave do Poste 1 |
| LED — Poste 2 | GPIO 26 | Saída digital — ligada ao terminal comum da chave do Poste 2 |
| LED — Poste 3 | GPIO 27 | Saída digital — ligada ao terminal comum da chave do Poste 3 |
| Chave gangorra — Poste 1, 2 e 3 | Não usa pino de entrada do ESP32 | Comuta apenas a alimentação do LED (ver tabela de ligação abaixo) |
| Buzzer | GPIO 13 | Saída digital (ou PWM, se for buzzer passivo) |

*Observação: os 3 LDRs usam obrigatoriamente canais ADC1 (GPIO 32 a 39), já que os canais ADC2 apresentam falhas de leitura quando o Wi-Fi está ativo. Os demais números de pino podem ser trocados por outros GPIOs livres, desde que mantida essa restrição para os LDRs. As chaves gangorra não ocupam nenhum pino do ESP32 como entrada — elas atuam de forma puramente elétrica sobre a alimentação do LED.*

## Como Ligar Cada Chave Gangorra de 3 Posições

Cada poste tem sua própria chave, com 3 terminais (comum + 2 contatos):

| Terminal da chave | Onde ligar | Posição correspondente | Estado do poste |
|---|---|---|---|
| Comum | LED daquele poste (em série com o resistor de 220 Ω) | — | — |
| Contato A | Pino GPIO do ESP32 daquele poste (25, 26 ou 27) | Posição A | Normal (ESP32 decide aceso/apagado) |
| — | (chave na posição central) | Posição OFF | Queimado (LED sem alimentação) |
| Contato B | Alimentação direta (3,3 V ou 5 V) | Posição B | Aceso em horário errado (LED sempre ligado) |

## Etapas Detalhadas de Montagem

### Etapa 1 — Preparar a base física dos postes
- Montar a estrutura física dos 3 postes (haste + base), separados o suficiente para que a luz de um LED não incida no LDR do poste vizinho
- Fixar, no topo de cada haste, o ponto onde o LED ficará posicionado como "lâmpada"
- Posicionar o suporte do LDR de cada poste virado diretamente para o respectivo LED, a uma distância curta e fixa (para manter a calibração estável)
- Reservar, na base de cada poste, um ponto de fácil acesso para fixar a chave gangorra daquele poste, visível durante a demonstração

### Etapa 2 — Montar o circuito de cada LED (atuador)
- Em uma protoboard, ligar o **resistor de 220 Ω** em série com o **LED**
- Ligar a outra ponta do LED ao GND
- Repetir esse processo para os 3 LEDs, um por poste
- Nesta etapa, ainda não conectar o LED diretamente ao GPIO — essa conexão passará pela chave gangorra, montada na Etapa 4

### Etapa 3 — Montar o circuito de cada LDR (sensor)
- Em uma protoboard, montar o **divisor de tensão**: LDR em série com o **resistor de 10 kΩ**, entre 3,3V e GND
- Conectar o ponto central do divisor (entre o LDR e o resistor) ao pino ADC1 correspondente do ESP32 (GPIO 34, 35 ou 32)
- Repetir esse processo para os 3 LDRs, um por poste, alinhando fisicamente cada um com o LED do seu próprio poste (Etapa 1)
- Testar cada LDR isoladamente (lendo o valor bruto via `analogRead()` e imprimindo na porta serial), verificando se o valor muda claramente ao acender/apagar o LED correspondente com uma alimentação provisória

### Etapa 4 — Montar a chave gangorra de 3 posições de cada poste
- Fixar a chave gangorra na base de cada poste, em local acessível para acionamento manual durante a demonstração
- Ligar o **terminal comum** da chave ao LED daquele poste (montado na Etapa 2)
- Ligar o **contato A** ao pino GPIO do ESP32 correspondente ao poste (25, 26 ou 27)
- Ligar o **contato B** direto à alimentação (3,3 V ou 5 V, conforme a tensão desejada no LED nessa posição)
- Repetir esse processo para as 3 chaves, uma por poste, sem compartilhar contatos entre postes diferentes
- Testar cada chave isoladamente nas 3 posições, verificando: Posição A → LED responde ao `digitalWrite()` do ESP32; Posição OFF → LED apagado, independente do firmware; Posição B → LED aceso, independente do firmware

### Etapa 5 — Montar o buzzer
- Conectar o buzzer entre o pino GPIO 13 e o GND
- Testar o disparo do som isoladamente (som curto de 1–2 segundos), antes de integrar com a lógica de detecção de falha

### Etapa 6 — Organizar a fiação e fixar os componentes
- Agrupar os jumpers por poste (LED + LDR + chave gangorra de cada um), evitando cruzamento excessivo de fios entre postes diferentes
- Fixar a protoboard (ou protoboards) numa base comum, junto ao ESP32, de forma que a maquete possa ser transportada sem que os fios se soltem
- Deixar as 3 chaves gangorra e o buzzer em posições de fácil acesso/visualização para a demonstração na Feira EPA, identificando cada chave com o número do poste correspondente

### Etapa 7 — Conectar a alimentação e validar o conjunto
- Conectar o ESP32 à alimentação via cabo USB (5V)
- Com o firmware básico carregado (leitura dos 3 LDRs, controle dos 3 LEDs, buzzer), verificar visualmente, com todas as chaves na posição Normal, que:
  - Os 3 postes acendem corretamente de forma independente, conforme a lógica do firmware
- Em seguida, testar cada chave isoladamente nas posições Queimado e Aceso Errado, verificando que:
  - O LDR do poste correspondente detecta a mudança de luminosidade em cada posição
  - O buzzer soa no momento em que a falha é detectada
- Só depois dessa validação física, seguir para a integração com a lógica de envio ao ThingSpeak (etapa tratada no planejamento do Back-End)

## Checklist Final de Montagem

| Item verificado | Esperado |
|---|---|
| Os 3 LEDs acendem/apagam de forma independente na posição Normal | Sim |
| Os 3 LDRs respondem à luz do próprio LED, sem interferência dos postes vizinhos | Sim |
| Cada chave gangorra alterna corretamente entre Normal, Queimado e Aceso Errado | Sim |
| O LDR do poste com falha simulada detecta a mudança de luminosidade em cada posição | Sim |
| O buzzer soa ao ser detectada a falha (Queimado ou Aceso Errado) | Sim |
| As 3 chaves estão identificadas e de fácil acesso para a demonstração | Sim |
| A fiação está organizada e fixada, sem fios soltos | Sim |
| A maquete se mantém estável para transporte até a Feira EPA | Sim |

## Observações

- A distância entre cada LED e o LDR correspondente deve ser mantida fixa após a calibração (etapa de calibração do Back-End), já que qualquer mudança física de posição altera os valores de referência.
- Como a chave gangorra é puramente elétrica, o firmware não precisa de nenhuma lógica de leitura de botão — ele continua apenas controlando o LED pelo GPIO e lendo o LDR normalmente; é o próprio circuito que decide se esse controle "vale" ou é sobreposto pela posição da chave.
- Este planejamento pode ser adaptado conforme os componentes efetivamente recebidos por patrocínio da escola e os testes práticos realizados pelo grupo.
