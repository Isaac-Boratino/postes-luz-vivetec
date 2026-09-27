# 3.3 Sensores e Atuadores

## Tabela de Componentes

| Componente | Tipo | Função | Protocolo/Interface |
|---|---|---|---|
| LDR (fotorresistor) | Sensor | Medir a luminosidade de cada poste (aceso/apagado) | Analógico (ADC1) |
| Resistor 10 kΩ | Componente passivo | Formar o divisor de tensão com o LDR | — |
| LED (5 mm, branco/amarelo) | Atuador | Simular a lâmpada de cada poste na maquete | Digital (GPIO) |
| Resistor 220 Ω | Componente passivo | Proteger o LED contra sobrecorrente | — |
| Slide switch (ON-OFF) | Sensor/Entrada | Sinalizar, por poste, que uma falha está sendo simulada manualmente | Digital (GPIO com `INPUT_PULLUP`) |

## Descrição Detalhada dos Componentes

### LDR (Light Dependent Resistor) — Sensor de Luminosidade

O LDR é um resistor cuja resistência elétrica varia conforme a quantidade de luz que incide sobre ele: quanto mais luz, menor a resistência; no escuro, a resistência sobe bastante. No projeto, um LDR é posicionado em cada poste, "olhando" diretamente para o LED que representa a lâmpada. Na demonstração, o cenário de "noite" é simulado cobrindo o LDR de 5 mm com uma pequena caixinha, reduzindo a luz percebida por ele.

Como o ESP32 só consegue ler tensão (não resistência) em seus pinos analógicos, o LDR é ligado em um **divisor de tensão** junto com um resistor fixo de 10 kΩ: a tensão no ponto entre os dois varia proporcionalmente à luz captada, e é esse valor de tensão que o pino ADC1 do ESP32 lê. Esse valor lido é então comparado a um limiar (`LIMIAR_LUZ`) para decidir se o sistema entende o poste como "aceso" ou "apagado" — limiar que precisa ser calibrado no ambiente real da maquete antes da apresentação, já que a luminosidade ambiente do local pode alterar as leituras.

A escolha do LDR (em vez de, por exemplo, um sensor de corrente no próprio LED) continua sendo proposital mesmo com o slide switch informando ao firmware que uma falha está sendo simulada: é o LDR quem confirma o estado real da luz emitida, mantendo a lógica de detecção fiel a um cenário real, onde esse switch não existiria — numa instalação de campo, seria exatamente essa leitura de luminosidade, e não um sinal de switch, que indicaria a falha.

### Resistor 10 kΩ — Divisor de Tensão

Cada LDR é acompanhado por um resistor fixo de 10 kΩ, formando o divisor de tensão citado acima. O valor de 10 kΩ foi escolhido por ficar numa faixa intermediária entre a resistência do LDR no claro (algumas centenas de ohms) e no escuro (dezenas a centenas de kΩ), o que produz uma variação de tensão mais sensível e linear ao longo da faixa de luminosidade que o projeto precisa distinguir (dia/noite/poste apagado/poste aceso indevidamente).

### LED — Atuador que Simula a Lâmpada do Poste

Cada um dos três postes da maquete é representado por um LED (branco ou amarelo, para remeter à cor de uma luminária de rua). O LED é o elemento que efetivamente "acende" ou "apaga" o poste, e é justamente essa luz que o LDR correspondente irá captar. Ele é **sempre** controlado por um pino de saída digital do ESP32: em condição normal, o firmware decide aceso/apagado pela comparação da leitura do LDR com `LIMIAR_LUZ`; quando o slide switch daquele poste sinaliza falha simulada, o firmware inverte por software a decisão que normalmente tomaria, fazendo o LED se comportar de forma consistente com o tipo de falha (por exemplo: apagado à noite, simulando lâmpada queimada; ou aceso durante o dia, simulando acionamento em horário errado).

### Resistor 220 Ω — Proteção do LED

Ligado em série com cada LED, o resistor de 220 Ω limita a corrente que passa pelo componente, evitando que ele seja danificado pela tensão de 3,3 V fornecida pelo pino GPIO do ESP32. Sem esse resistor, a corrente poderia ultrapassar a capacidade do LED (e também sobrecarregar o próprio pino do microcontrolador).

### Slide Switch (ON-OFF) — Simulação Digital da Falha

Em uma revisão anterior do projeto, cada poste usava uma chave gangorra de 3 posições que comutava fisicamente a alimentação do LED, de forma totalmente invisível ao firmware. Nesta versão, cada poste conta com um **slide switch simples, do tipo ON-OFF**, ligado entre um pino GPIO do ESP32 e o GND, com o resistor de pull-up interno habilitado via `INPUT_PULLUP` (dispensando resistor externo).

Diferente da chave gangorra, o slide switch **não** está no caminho elétrico do LED — ele é uma entrada digital independente, lida pelo firmware com `digitalRead()`. Isso muda um ponto conceitual importante em relação à versão anterior: agora o ESP32 **sabe** que uma falha está sendo simulada manualmente naquele poste (o switch deixa de ser "invisível" ao firmware, como era a chave gangorra). Quando o switch está acionado, o firmware inverte por software o comando que daria normalmente ao LED, gerando o comportamento de falha (queimado ou aceso em horário errado); o LDR daquele poste continua sendo o responsável por confirmar, pela luz de fato emitida, que o estado do poste mudou.

**Configuração para os 3 postes:** o mesmo esquema é repetido de forma idêntica e independente em cada um dos 3 postes — um slide switch por poste, ligado a um pino GPIO dedicado. Isso permite ao grupo demonstrar, durante a Feira EPA, uma falha simulada em qualquer um dos postes, a qualquer momento, sem depender de reprogramar o ESP32 ou trocar fiação durante a apresentação — basta acionar o switch do poste desejado (e, para simular "noite", cobrir o respectivo LDR com a caixinha).

## Integração entre os Componentes

Na prática, sensores e atuadores trabalham em conjunto formando o ciclo de simulação e detecção do sistema: o slide switch de cada poste (entrada digital) informa ao firmware que uma falha está sendo simulada manualmente naquele poste; o LDR (sensor) mede a luminosidade real captada, de forma independente; e o ESP32 combina essas duas informações para decidir o estado final do LED daquele poste (invertendo o comando normal quando o switch indica falha) e para montar o estado de cada poste exposto no endpoint `/api/status` do servidor local, consumido pelo dashboard. Esse desenho garante que a maquete replique, na sua essência, o comportamento real de um sistema de monitoramento por sensoriamento de luz, mesmo usando um mecanismo simplificado (switch + caixinha sobre o LDR) para acionar as falhas durante a demonstração.
