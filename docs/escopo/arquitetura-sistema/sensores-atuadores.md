# 3.3 Sensores e Atuadores

## Tabela de Componentes

| Componente | Tipo | Função | Protocolo/Interface |
|---|---|---|---|
| LDR (fotorresistor) | Sensor | Medir a luminosidade de cada poste (aceso/apagado) | Analógico (ADC1) |
| Resistor 10 kΩ | Componente passivo | Formar o divisor de tensão com o LDR | — |
| LED (5 mm, branco/amarelo) | Atuador | Simular a lâmpada de cada poste na maquete | Digital (GPIO) |
| Resistor 220 Ω | Componente passivo | Proteger o LED contra sobrecorrente | — |
| Chave gangorra 3 posições (ON-OFF-ON) | Seletor mecânico | Simular, por poste, os 3 estados possíveis da lâmpada: normal, queimada ou acesa em horário errado | Mecânica (comuta a alimentação do LED, sem entrada digital no ESP32) |
| Buzzer | Atuador | Emitir alerta sonoro local quando uma falha é detectada | Digital (GPIO) |

## Descrição Detalhada dos Componentes

### LDR (Light Dependent Resistor) — Sensor de Luminosidade

O LDR é um resistor cuja resistência elétrica varia conforme a quantidade de luz que incide sobre ele: quanto mais luz, menor a resistência; no escuro, a resistência sobe bastante. No projeto, um LDR é posicionado em cada poste, "olhando" diretamente para o LED que representa a lâmpada.

Como o ESP32 só consegue ler tensão (não resistência) em seus pinos analógicos, o LDR é ligado em um **divisor de tensão** junto com um resistor fixo de 10 kΩ: a tensão no ponto entre os dois varia proporcionalmente à luz captada, e é esse valor de tensão que o pino ADC1 do ESP32 lê. Esse valor lido é então comparado a um limiar (`limiarLuz`) para decidir se o sistema entende o poste como "aceso" ou "apagado" — limiar que precisa ser calibrado no ambiente real da maquete antes da apresentação, já que a luminosidade ambiente do local pode alterar as leituras.

A escolha do LDR (em vez de, por exemplo, um sensor de corrente no próprio LED) foi proposital: o sistema enxerga o estado real da luz emitida, e não apenas se o circuito está energizado — isso significa que o método de detecção funciona independentemente de qual das três posições da chave gangorra provocou a mudança, já que o LDR reage à luz de fato emitida, e não à causa elétrica por trás dela.

### Resistor 10 kΩ — Divisor de Tensão

Cada LDR é acompanhado por um resistor fixo de 10 kΩ, formando o divisor de tensão citado acima. O valor de 10 kΩ foi escolhido por ficar numa faixa intermediária entre a resistência do LDR no claro (algumas centenas de ohms) e no escuro (dezenas a centenas de kΩ), o que produz uma variação de tensão mais sensível e linear ao longo da faixa de luminosidade que o projeto precisa distinguir (dia/noite/poste apagado/poste aceso indevidamente).

### LED — Atuador que Simula a Lâmpada do Poste

Cada um dos três postes da maquete é representado por um LED (branco ou amarelo, para remeter à cor de uma luminária de rua). O LED é o elemento que efetivamente "acende" ou "apaga" o poste, e é justamente essa luz que o LDR correspondente irá captar. Em condição normal, ele é controlado por um pino de saída digital do ESP32 (que decide aceso/apagado pela lógica dia/noite); nas condições de falha simulada, seu comportamento passa a depender da chave gangorra associada, conforme descrito abaixo.

### Resistor 220 Ω — Proteção do LED

Ligado em série com cada LED, o resistor de 220 Ω limita a corrente que passa pelo componente, evitando que ele seja danificado pela tensão de alimentação (3,3 V vindo do GPIO do ESP32, ou a tensão de alimentação direta, conforme a posição da chave gangorra). Sem esse resistor, a corrente poderia ultrapassar a capacidade do LED (e também sobrecarregar o próprio pino do microcontrolador, quando a alimentação vem do GPIO).

### Chave Gangorra de 3 Posições (ON-OFF-ON) — Simulação Física dos 3 Tipos de Falha

Diferente de um botão push-button simples (que só consegue simular um poste "queimado", já que apenas interrompe momentaneamente o circuito), cada poste da maquete recebe uma **chave gangorra de 3 posições, tipo ON-OFF-ON** (também chamada de chave SPDT com posição central desligada). Essa chave tem 3 terminais — um comum e dois de contato — e permite alternar fisicamente a origem da alimentação do LED daquele poste entre três estados, sem qualquer intervenção do firmware:

| Posição da chave | Ligação elétrica do LED | Estado do poste simulado |
|---|---|---|
| Posição A | LED ligado ao pino GPIO do ESP32 | **Normal** — o próprio firmware decide aceso/apagado pela lógica dia/noite |
| Posição central (OFF) | LED sem nenhuma alimentação | **Queimado** — luz apagada permanentemente até a chave ser mudada |
| Posição B | LED ligado direto à alimentação (3,3 V ou 5 V) | **Aceso em horário errado** — luz sempre acesa, independente da decisão do firmware |

O terminal comum da chave é ligado ao LED (em série com o resistor de 220 Ω, como já descrito); um dos terminais de contato vai ao GPIO do ESP32 correspondente àquele poste, e o outro vai direto à alimentação (3,3 V ou 5 V, dependendo da tensão desejada no LED nessa posição).

Essa é uma solução puramente mecânica/elétrica: o ESP32 não recebe nenhum sinal digital informando qual posição foi escolhida — ele apenas continua tentando controlar o LED pelo GPIO como sempre faz, e só percebe o efeito indiretamente, através da leitura do LDR correspondente. Isso torna a demonstração mais fiel à realidade: assim como uma prefeitura não sabe de antemão por que um poste apresenta problema, o sistema aqui também detecta a falha (queimado ou aceso indevido) pelo comportamento real da luz, e não por um aviso explícito de qual chave foi acionada.

**Configuração para os 3 postes:** o mesmo esquema é repetido de forma idêntica e independente em cada um dos 3 postes — uma chave gangorra de 3 posições por poste, ligada ao respectivo LED. Isso permite ao grupo demonstrar, durante a Feira EPA, qualquer combinação dos três estados (normal, queimado, aceso errado) em qualquer um dos postes, sem depender de reprogramar o ESP32 ou trocar fiação durante a apresentação — basta acionar a chave do poste desejado.

### Buzzer — Alerta Sonoro Local

Como reforço para a demonstração da maquete, o sistema também conta com um buzzer conectado a uma saída digital do ESP32, que emite um som quando uma falha é detectada em qualquer um dos postes. Diferente do envio de dados ao ThingSpeak — que é o canal oficial de registro do status —, o buzzer funciona apenas como um indicativo sonoro local e imediato, útil especialmente para a Feira EPA: ele permite que quem estiver observando a maquete perceba instantaneamente o momento exato em que o sistema identifica o problema.

## Integração entre os Componentes

Na prática, sensores e atuadores trabalham em conjunto formando o ciclo de simulação e detecção do sistema: a chave gangorra de cada poste seleciona, de forma puramente elétrica, o estado da alimentação do LED daquele poste (normal, queimado ou aceso indevidamente); o LDR (sensor) capta essa mudança de luminosidade de forma independente, sem qualquer aviso prévio de qual chave foi acionada; e o ESP32 processa essa leitura para acionar os atuadores de resposta — o próprio LED (nas posições em que ainda está sob seu controle), o buzzer (indicativo sonoro imediato) e, por fim, o encaminhamento dessa leitura para a camada de conectividade, que registra o histórico no ThingSpeak. Esse desenho garante que a maquete não seja uma simulação artificial de alarme, mas replique o comportamento real de um sistema de monitoramento por sensoriamento de luz.
