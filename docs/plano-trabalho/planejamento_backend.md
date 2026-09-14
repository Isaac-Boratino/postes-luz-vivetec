# Planejamento de Trabalho — Back-End (Firmware ESP32) — Projeto VivEtec

## Responsáveis

| Integrante | Responsabilidades (conforme papéis do grupo) |
|---|---|
| Tulio Dias | Desenvolvimento da lógica embarcada no ESP32 (leitura dos sensores LDR); calibração dos sensores |
| Rafael Alves | Controle do buzzer e lógica de reforço sonoro; integração com o ThingSpeak (envio de dados de luminosidade) |

## Visão Geral

O Back-End do VivEtec é o **firmware embarcado no ESP32 DevKit V1**, responsável por toda a lógica de processamento do sistema: leitura contínua dos 3 sensores LDR, sincronização de horário via NTP, decisão sobre o estado esperado de cada poste (dia/noite), controle dos 3 LEDs, acionamento do buzzer em caso de falha, e envio periódico dos dados de luminosidade ao canal do ThingSpeak. Diferente de uma arquitetura com servidor web local ou bot de notificação externo, todo o "cérebro" do sistema roda localmente no ESP32, e a única saída de dados para fora da maquete é a requisição HTTP ao ThingSpeak.

O firmware não recebe nenhuma entrada digital das chaves gangorra: elas atuam de forma puramente elétrica sobre a alimentação dos LEDs, então o Back-End "enxerga" o efeito de qualquer falha simulada exclusivamente através da leitura do LDR correspondente — nunca por um sinal direto de qual chave foi acionada.

## Tecnologias e Bibliotecas

| Item | Escolha | Observação |
|---|---|---|
| Linguagem | C/C++ com framework Arduino | Maior suporte de bibliotecas prontas para ESP32 e ThingSpeak, ampla documentação em português |
| IDE | Arduino IDE (com pacote de placas ESP32 instalado) | Ambiente já validado e simples de configurar |
| Biblioteca de rede | `WiFi.h` (nativa do core ESP32) | Conexão Wi-Fi em modo estação (STA) |
| Biblioteca de tempo | `time.h` + `configTime()` (NTP nativo do core ESP32) | Sincronização de horário via `pool.ntp.org` |
| Biblioteca de integração IoT | `ThingSpeak.h` (MathWorks) | Encapsula a montagem da requisição HTTP e o tratamento da resposta do `api.thingspeak.com` |
| Leitura analógica | `analogRead()` (nativa) | Leitura dos 3 canais ADC1 (GPIO 34, 35, 32) |
| Saídas digitais | `digitalWrite()` / `pinMode()` (nativas) | Controle dos LEDs (GPIO 25, 26, 27) e do buzzer (GPIO 13) |

## Mapeamento de Pinos (referência da montagem física)

| Componente | Pino do ESP32 | Tipo de acesso no firmware |
|---|---|---|
| LDR — Poste 1 | GPIO 34 (ADC1_CH6) | `analogRead(34)` |
| LDR — Poste 2 | GPIO 35 (ADC1_CH7) | `analogRead(35)` |
| LDR — Poste 3 | GPIO 32 (ADC1_CH4) | `analogRead(32)` |
| LED — Poste 1 | GPIO 25 | `digitalWrite(25, ...)` |
| LED — Poste 2 | GPIO 26 | `digitalWrite(26, ...)` |
| LED — Poste 3 | GPIO 27 | `digitalWrite(27, ...)` |
| Buzzer | GPIO 13 | `digitalWrite(13, ...)` (ou PWM, se buzzer passivo) |

*As chaves gangorra de 3 posições não ocupam nenhum pino do ESP32 como entrada — o firmware continua controlando o LED pelo GPIO independente da posição física da chave, exatamente como já documentado na montagem da maquete.*

## Lógica Central do Firmware (Máquina de Estados por Poste)

Para cada um dos 3 postes, o firmware deve resolver continuamente a seguinte lógica condicional:

| Condição (hora do dia via NTP) | Leitura do LDR | Interpretação do sistema |
|---|---|---|
| Noite | Luz apagada (abaixo de `limiarLuz`) | **Falha — poste queimado/quebrado** |
| Noite | Luz acesa (acima de `limiarLuz`) | Normal (funcionamento esperado) |
| Dia | Luz apagada | Normal (funcionamento esperado) |
| Dia | Luz acesa (acima de `limiarLuz`) | **Falha — aceso em horário errado (desperdício)** |

Essa lógica deve ser avaliada de forma independente para cada um dos 3 postes a cada ciclo de leitura, já que cada poste tem seu próprio LDR, LED e chave gangorra, sem qualquer dependência entre eles.

## Etapas Detalhadas de Desenvolvimento

### Etapa 1 — Configuração do ambiente de desenvolvimento
- Instalar a Arduino IDE e adicionar o pacote de placas ESP32 via Gerenciador de Placas
- Selecionar a placa correta ("ESP32 Dev Module") e a porta serial correspondente ao cabo USB utilizado
- Instalar a biblioteca `ThingSpeak.h` via Gerenciador de Bibliotecas
- Validar a comunicação básica com um sketch de teste (`Blink` em um LED) antes de iniciar a lógica do projeto

### Etapa 2 — Conexão Wi-Fi e sincronização NTP
- Implementar a conexão Wi-Fi em modo estação (STA), usando as credenciais (SSID/senha) da rede da escola ou do local de demonstração, com tratamento de reconexão automática em caso de queda momentânea
- Implementar a sincronização de horário via `configTime()`, apontando para servidores públicos NTP (ex.: `pool.ntp.org`), e validar via porta serial se o horário retornado está correto
- Definir a função utilitária que retorna se o momento atual é "dia" ou "noite" (com base em um horário de referência configurável, ex.: 6h–18h, ajustável conforme o teste real de luminosidade ambiente)

### Etapa 3 — Leitura dos sensores LDR (responsável: Tulio Dias)
- Implementar a leitura bruta dos 3 canais ADC1 (`analogRead(34)`, `analogRead(35)`, `analogRead(32)`), imprimindo os valores na porta serial para inspeção inicial
- Implementar uma rotina simples de suavização (média móvel de poucas amostras) para reduzir ruído de leitura do ADC, caso os valores oscilem de forma perceptível
- Documentar, para cada poste, a faixa de valores observada com o LED aceso e com o LED apagado, servindo de base para a calibração da Etapa 4

### Etapa 4 — Calibração do limiar de luminosidade (`limiarLuz`) (responsável: Tulio Dias)
- Definir uma variável `limiarLuz` (podendo ser um valor único ou um valor por poste, caso a resposta dos LDRs varie entre eles) que separa a leitura "aceso" da leitura "apagado"
- Testar a calibração no ambiente físico real da maquete (mesma distância e mesma luz ambiente previstas para o dia da Feira EPA), já que a luminosidade do local altera as leituras
- Registrar o valor final calibrado em um comentário no código e/ou na documentação do projeto, para facilitar eventual recalibração no dia da apresentação

### Etapa 5 — Controle dos LEDs pela lógica dia/noite
- Implementar a função que, em condição normal, decide se cada LED deve estar aceso ou apagado com base exclusivamente no horário sincronizado via NTP (sem depender da posição da chave gangorra, já que essa decisão é sobreposta fisicamente pela chave quando ela não está na posição "Normal")
- Validar que, com as 3 chaves na posição Normal, os LEDs respondem corretamente à transição dia/noite (pode ser simulado ajustando manualmente o horário ou cobrindo/destampando o LDR para testes rápidos)

### Etapa 6 — Lógica de detecção de falha
- Implementar, para cada poste, a comparação entre o estado esperado (dia/noite) e o estado lido (LDR acima/abaixo do `limiarLuz`), conforme a tabela de máquina de estados acima
- Consolidar o resultado em uma variável de estado por poste (ex.: `falhaPoste1`, `falhaPoste2`, `falhaPoste3`), atualizada a cada ciclo do `loop()`
- Testar manualmente os 3 cenários de falha em cada poste (alternando a chave gangorra física entre Normal, Queimado e Aceso Errado) e validar que a variável de estado responde corretamente em cada caso

### Etapa 7 — Controle do buzzer (responsável: Rafael Alves)
- Implementar a lógica de acionamento do buzzer no pino GPIO 13, disparado sempre que qualquer uma das variáveis de falha (`falhaPoste1`, `falhaPoste2` ou `falhaPoste3`) for verdadeira
- Definir a duração e o padrão do som de alerta (contínuo enquanto a falha persistir, ou um pulso curto no momento em que a falha é detectada — a equipe decide qual reforça melhor a demonstração)
- Implementar uma lógica simples de "debounce" de alerta, evitando que o buzzer dispare de forma intermitente por pequenas oscilações de leitura próximas ao limiar

### Etapa 8 — Integração com o ThingSpeak (responsável: Rafael Alves)
- Criar o canal no ThingSpeak com 3 campos (`field1`, `field2`, `field3`), um por poste, e obter a **Write API Key**
- Implementar o envio periódico dos valores lidos dos 3 LDRs via biblioteca `ThingSpeak.h`, respeitando o intervalo mínimo de 15 segundos entre gravações do plano gratuito (sugestão: usar `millis()` para controlar o intervalo, evitando o uso de `delay()` bloqueante, que atrapalharia a leitura contínua dos sensores e o acionamento do buzzer)
- Tratar o código de retorno da requisição (`ThingSpeak.writeFields()`), registrando via serial eventuais falhas de envio (ex.: código diferente de 200), para facilitar o diagnóstico em campo
- Validar no Channel View do ThingSpeak que os 3 campos estão sendo atualizados corretamente e no intervalo esperado

### Etapa 9 — Configuração do ThingSpeak Alerts
- Configurar, na interface web do ThingSpeak, as condições de alerta sobre os campos (`field1`, `field2` ou `field3` abaixo/acima do limiar que caracteriza falha), replicando a mesma lógica de negócio já validada no firmware
- Definir o canal de notificação do Alerts (push no app, e-mail, ou webhook via ThingHTTP) em conjunto com o Gestor do Projeto, já que essa etapa não exige alteração de código no ESP32
- Validar que o alerta realmente dispara ao simular uma falha física na maquete

### Etapa 10 — Testes integrados do firmware completo
- Rodar o firmware completo (leitura dos 3 LDRs + lógica dia/noite + controle dos LEDs + buzzer + envio ao ThingSpeak) de forma contínua por um período prolongado (ex.: 30–60 minutos), observando estabilidade da conexão Wi-Fi e ausência de travamentos
- Validar o comportamento do sistema em caso de perda temporária de Wi-Fi (o firmware deve tentar reconectar automaticamente, sem travar a leitura local dos sensores e do buzzer)
- Repassar o firmware validado para os testes formais do QA (Cauã Galvão), incluindo os cenários já cobertos internamente pelo Back-End

## Checklist Final de Back-End

| Item verificado | Esperado |
|---|---|
| O ESP32 conecta à rede Wi-Fi automaticamente, com reconexão em caso de queda | Sim |
| O horário via NTP está sincronizado corretamente | Sim |
| Os 3 LDRs retornam leituras estáveis e coerentes com a luz recebida | Sim |
| O `limiarLuz` está calibrado no ambiente real da maquete | Sim |
| A lógica dia/noite controla corretamente os LEDs na posição Normal das chaves | Sim |
| A detecção de falha funciona corretamente nas 3 posições de cada chave gangorra | Sim |
| O buzzer dispara corretamente ao ser detectada qualquer falha | Sim |
| Os dados de luminosidade chegam corretamente aos 3 campos do canal ThingSpeak | Sim |
| O intervalo de envio respeita o limite mínimo de 15 segundos do plano gratuito | Sim |
| O ThingSpeak Alerts dispara corretamente ao simular uma falha | Sim |
| O firmware roda de forma estável por um período prolongado sem travar | Sim |

## Observações

- Este planejamento reflete a arquitetura final do projeto (ThingSpeak como única camada de nuvem, sem servidor web local no ESP32 e sem bot de Telegram), conforme documentado na arquitetura do sistema, na conectividade/comunicação e na plataforma IoT do projeto.
- A divisão de responsabilidades entre Tulio Dias (sensores/calibração) e Rafael Alves (buzzer/ThingSpeak) não impede colaboração cruzada — a lógica de detecção de falha (Etapa 6) depende diretamente do trabalho de ambos e deve ser desenvolvida em conjunto.
- Este planejamento pode ser adaptado conforme os componentes efetivamente recebidos por patrocínio da escola e os testes práticos realizados pelo grupo.
