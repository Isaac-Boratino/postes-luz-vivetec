# 3.3 Sensores e Atuadores

## Tabela de Componentes

| Componente | Tipo | Função | Protocolo/Interface |
|---|---|---|---|
| LDR (fotorresistor) | Sensor | Medir a luminosidade de cada poste (aceso/apagado) | Analógico (ADC1) |
| Resistor 10 kΩ | Componente passivo | Formar o divisor de tensão com o LDR | — |
| LED (5 mm, branco/amarelo) | Atuador | Simular a lâmpada de cada poste na maquete | Digital (GPIO) |
| Resistor 220 Ω | Componente passivo | Proteger o LED contra sobrecorrente | — |
| Botão push-button | Sensor/Entrada | Simular a falha física de um poste (interrupção do circuito) | Digital (GPIO com INPUT_PULLUP) |
| Buzzer | Atuador | Emitir alerta sonoro local quando uma falha é detectada | Digital (GPIO) |

## Descrição Detalhada dos Componentes

### LDR (Light Dependent Resistor) — Sensor de Luminosidade

O LDR é um resistor cuja resistência elétrica varia conforme a quantidade de luz que incide sobre ele: quanto mais luz, menor a resistência; no escuro, a resistência sobe bastante. No projeto, um LDR é posicionado em cada poste, "olhando" diretamente para o LED que representa a lâmpada.

Como o ESP32 só consegue ler tensão (não resistência) em seus pinos analógicos, o LDR é ligado em um **divisor de tensão** junto com um resistor fixo de 10 kΩ: a tensão no ponto entre os dois varia proporcionalmente à luz captada, e é esse valor de tensão que o pino ADC1 do ESP32 lê. Esse valor lido é então comparado a um limiar (`limiarLuz`) para decidir se o sistema entende o poste como "aceso" ou "apagado" — limiar que precisa ser calibrado no ambiente real da maquete antes da apresentação, já que a luminosidade ambiente do local pode alterar as leituras.

A escolha do LDR (em vez de, por exemplo, um sensor de corrente no próprio LED) foi proposital: o sistema enxerga o estado real da luz emitida, e não apenas se o circuito está energizado — isso significa que o método de detecção funciona mesmo que a causa da falha não seja o botão de simulação, mas qualquer outro motivo que apague fisicamente a luz do poste.

### Resistor 10 kΩ — Divisor de Tensão

Cada LDR é acompanhado por um resistor fixo de 10 kΩ, formando o divisor de tensão citado acima. O valor de 10 kΩ foi escolhido por ficar numa faixa intermediária entre a resistência do LDR no claro (algumas centenas de ohms) e no escuro (dezenas a centenas de kΩ), o que produz uma variação de tensão mais sensível e linear ao longo da faixa de luminosidade que o projeto precisa distinguir (dia/noite/poste apagado).

### LED — Atuador que Simula a Lâmpada do Poste

Cada um dos três postes da maquete é representado por um LED (branco ou amarelo, para remeter à cor de uma luminária de rua). O LED é o elemento que efetivamente "acende" ou "apaga" o poste, e é justamente essa luz que o LDR correspondente irá captar. Ele é controlado por um pino de saída digital do ESP32, que pode ligá-lo ou desligá-lo, e também é afetado pelo botão de simulação de falha, conforme descrito abaixo.

### Resistor 220 Ω — Proteção do LED

Ligado em série com cada LED, o resistor de 220 Ω limita a corrente que passa pelo componente, evitando que ele seja danificado pela tensão de 3,3 V fornecida pelos pinos GPIO do ESP32. Sem esse resistor, a corrente poderia ultrapassar a capacidade do LED (e também sobrecarregar o próprio pino do microcontrolador).

### Botão Push-Button — Simulação Física da Falha

O botão está ligado diretamente ao circuito de alimentação de um dos LEDs, e não apenas conectado como uma entrada "de software": ao ser pressionado, ele efetivamente interrompe a corrente que chega ao LED daquele poste, apagando-o fisicamente. Essa escolha de projeto torna a demonstração mais fiel à realidade — o ESP32 não "sabe" que o botão foi apertado por um comando direto; ele percebe a falha da mesma forma que perceberia uma lâmpada queimada de verdade: através da leitura do LDR correspondente, que capta a ausência de luz.

O botão utiliza a configuração `INPUT_PULLUP` do próprio ESP32, que ativa um resistor de pull-up interno no pino. Isso dispensa o uso de um resistor externo adicional: basta ligar o botão entre o pino digital e o GND, e o firmware interpreta o nível lógico invertido (pino em nível alto quando solto, baixo quando pressionado).

### Buzzer — Alerta Sonoro Local

Como reforço para a demonstração da maquete, o sistema também conta com um buzzer conectado a uma saída digital do ESP32, que emite um som quando uma falha é detectada em qualquer um dos postes. Diferente do alerta via Telegram, do painel web e do envio de dados ao ThingSpeak — que são os canais oficiais de notificação, consulta e registro do status —, o buzzer funciona apenas como um indicativo sonoro local e imediato, útil especialmente para a Feira EPA: ele permite que quem estiver observando a maquete perceba instantaneamente, mesmo sem olhar para um celular ou painel, o momento exato em que o sistema identifica o problema.

## Integração entre os Componentes

Na prática, sensores e atuadores trabalham em conjunto formando o ciclo de simulação e detecção do sistema: o botão (entrada) interfere fisicamente no LED (atuador), o LDR (sensor) capta essa mudança de luminosidade de forma independente, e o ESP32 processa essa leitura para acionar os atuadores de resposta — o próprio LED (mantendo o estado real do poste), o buzzer (indicativo sonoro imediato) e, por fim, o encaminhamento dessa leitura para a camada de conectividade, que dispara o alerta via Telegram, disponibiliza o status no painel web e registra o histórico no ThingSpeak. Esse desenho garante que a maquete não seja uma simulação artificial de alarme, mas replique o comportamento real de um sistema de monitoramento por sensoriamento de luz.
