# 3.4 Conectividade e Comunicação

## Tecnologia de Conectividade Escolhida: Wi-Fi

O projeto utiliza a interface Wi-Fi 802.11 b/g/n (2,4 GHz) nativa do ESP32, operando em **modo estação (Station Mode / STA)** — ou seja, o ESP32 se conecta como cliente a um roteador já existente, em vez de criar sua própria rede (modo Access Point). A associação à rede é feita via protocolo de segurança **WPA2-PSK**, o padrão suportado por praticamente qualquer roteador doméstico ou de escola, e o endereço IP do dispositivo na rede local é obtido automaticamente por **DHCP**, dispensando qualquer configuração manual de rede no dia da demonstração.

Essa camada é responsável por manter a conexão lógica sobre o rádio físico usada quando o ESP32 atua como cliente para contatar servidores externos (API do ThingSpeak, servidor NTP). O SoC ESP32 gerencia essa função através da pilha TCP/IP (**lwIP**) embutida no seu SDK, que permite abrir sockets sem necessidade de hardware adicional.

A escolha do Wi-Fi em detrimento das demais opções se justifica tecnicamente:

- **Frente a Bluetooth (HC-05/06):** o Bluetooth clássico não possui pilha IP própria — ele exigiria um dispositivo intermediário (como um smartphone) atuando como ponte entre o ESP32 e a internet, o que adicionaria uma camada de complexidade e um ponto de falha extra, inviabilizando o envio direto de dados para serviços externos;
- **Frente a LoRa (SX1278):** o LoRa opera com taxas de transmissão muito baixas (tipicamente entre 0,3 e 50 kbps) e não é orientado a IP — ele é ideal para telemetria de longuíssimo alcance com poucos bytes por transmissão, o que não corresponde à necessidade do projeto de enviar requisições HTTP/HTTPS estruturadas;
- **Frente a GSM/GPRS (SIM800L):** apesar de resolver o problema de alcance via rede celular, o módulo exigiria um chip SIM ativo com plano de dados, custo recorrente mensal e uma antena externa — despesas incompatíveis com a proposta de solução de baixo custo do projeto, para um cenário (maquete estacionária) em que já existe Wi-Fi disponível.

Em termos de desempenho, a interface Wi-Fi do ESP32 alcança taxas de transmissão de até 150 Mbps (802.11n), muito acima do necessário para o volume de dados do projeto — cada requisição carrega poucos kilobytes (os valores numéricos enviados ao ThingSpeak), de modo que a latência de rede não é um fator crítico de projeto.

## Protocolo de Comunicação Escolhido: HTTP/HTTPS

O sistema utiliza o protocolo HTTP para o envio periódico dos dados de luminosidade ao ThingSpeak (respeitando o intervalo mínimo de 15 segundos entre gravações do plano gratuito): o ESP32 monta uma requisição `GET` para o endpoint `https://api.thingspeak.com/update`, informando a **Write API Key** do canal e os valores lidos de cada um dos três LDRs como campos (`field1`, `field2`, `field3`). Essa comunicação pode ser feita com a biblioteca oficial `ThingSpeak.h` (da MathWorks), que encapsula a montagem da requisição e o tratamento da resposta. A partir do momento em que os dados chegam ao canal, o próprio ThingSpeak assume, do lado da plataforma (sem exigir requisições adicionais do ESP32), tanto a consulta ao vivo do status (Channel View, acessível por navegador) quanto o disparo do alerta de falha (ThingSpeak Alerts, configurado sobre os valores dos campos).

Comparando com as alternativas do enunciado: o **MQTT** é otimizado para o padrão publish/subscribe, no qual múltiplos dispositivos publicam continuamente pequenas mensagens de telemetria para um broker central que redistribui aos interessados — um modelo vantajoso em escala (dezenas/centenas de sensores), mas que, para os três postes de um único nó ESP32 sem necessidade de broker dedicado, adicionaria uma peça de infraestrutura sem ganho real de desempenho (o próprio ThingSpeak, inclusive, aceita publicação via MQTT como alternativa ao HTTP, mas a via HTTP é mais simples de implementar e depurar num projeto acadêmico). O **WebSocket**, por sua vez, é voltado para manter uma conexão persistente e bidirecional entre dois pontos, cenário que não se aplica ao envio periódico e unidirecional de dados ao ThingSpeak. Já o protocolo **Serial** não se aplica ao escopo, por não existir, no projeto, comunicação direta por fio entre dois dispositivos processadores distintos.

## Sincronização de Horário via NTP

Uma segunda comunicação, de menor volume mas igualmente essencial, ocorre sobre a mesma conexão Wi-Fi: a sincronização de horário via **NTP (Network Time Protocol)**, que roda sobre a porta UDP 123 e é usada pelo ESP32 para consultar servidores públicos de tempo (como os do `pool.ntp.org`). Diferente do HTTP, o NTP é um protocolo leve, de requisição única e resposta compacta, adequado para uma consulta esporádica de horário em vez de uma troca contínua de dados. É esse horário sincronizado que alimenta a lógica condicional central do firmware — comparar se é dia ou noite para decidir se o estado de um poste (aceso/apagado) representa uma falha ou um funcionamento normal — e por isso a exatidão dessa sincronização é um requisito técnico tão importante quanto o próprio registro do histórico.

## Integração das Camadas

A conectividade (Wi-Fi) e a comunicação (HTTP) atuam de forma conjunta e dependente: o Wi-Fi fornece o meio físico de acesso à rede, enquanto o HTTP define a forma como as mensagens são estruturadas e trafegam sobre esse meio. Na prática, o ESP32 envia periodicamente os valores de luminosidade ao ThingSpeak através da conexão Wi-Fi já estabelecida, construindo o histórico em forma de gráfico, ao mesmo tempo em que utiliza a mesma conexão para sincronizar o horário via NTP, alimentando a lógica de comparação dia/noite que sustenta a detecção de falhas.
