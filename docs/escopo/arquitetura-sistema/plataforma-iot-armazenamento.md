# 3.5 Interface Local / Dashboard

## Solução Escolhida: Servidor HTTP Embutido no ESP32 + Dashboard Local

O projeto abandonou o uso de uma plataforma de IoT em nuvem (o ThingSpeak, cogitado em uma versão anterior do escopo) e passou a usar o próprio **ESP32 como servidor**, função antes atribuída à nuvem: ele expõe, na porta 80 da rede local, um endpoint HTTP (`/api/status`) que retorna, em JSON, o estado atual dos três postes monitorados (leitura de cada LDR, se o slide switch daquele poste está simulando falha, e o estado do LED correspondente). Essa implementação (Wi-Fi + servidor local) foi conduzida por Rafael Alves, com apoio técnico de Victor Eufrásio, que já dominava essa parte.

Um **dashboard HTML/CSS/JS**, hospedado em outro dispositivo conectado à mesma rede Wi-Fi (notebook, celular ou tablet), consome esse endpoint em intervalos regulares e exibe visualmente, em tempo real, o estado de cada poste ao público da Feira EPA.

## Por que essa opção substitui a plataforma de nuvem cogitada anteriormente

- **Não depende de internet externa.** Toda a comunicação acontece dentro da rede Wi-Fi local — a mesma que será fornecida pelo professor no dia da apresentação —, sem nenhuma requisição para um servidor fora da sala/local do evento, o que reduz um ponto de falha (indisponibilidade de internet externa no dia da demonstração).
- **Mais robusto para uma rede fornecida ad hoc.** Como a rede do dia da apresentação é distribuída pelo próprio professor apenas para o grupo, uma solução que dependa somente dessa rede interna (sem precisar de acesso real à internet) é mais confiável para o cenário da feira do que depender de um serviço externo.
- **Controle total sobre o formato dos dados exibidos.** Por ser um endpoint próprio (`/api/status`), o grupo decide exatamente quais campos aparecem no JSON e como o dashboard os interpreta, sem ficar limitado à estrutura de campos de uma plataforma de terceiros.

## O que essa opção não cobre (comparado à plataforma de nuvem descartada)

- **Sem histórico de longo prazo.** O dashboard exibe o estado mais recente de cada poste, mas não mantém, nesta versão, um registro histórico de luminosidade ao longo do tempo (algo que uma plataforma de nuvem como o ThingSpeak oferecia nativamente, com gráficos de série temporal).
- **Sem alertas automáticos por push/e-mail/webhook.** A notificação de falha, nesta versão, é visual — depende de alguém observar o dashboard —, e não existe mais um recurso equivalente ao antigo "ThingSpeak Alerts" disparando notificação automática a um responsável fora da rede local.
- **Acesso restrito à rede local.** Diferente de um canal em nuvem (acessível de qualquer lugar com internet), o dashboard só é acessível a partir de dispositivos conectados à mesma rede Wi-Fi do ESP32.

Essas limitações estão registradas também na seção "Aplicação e Resultados Esperados" (item 4.3), como pontos que uma futura adição de uma camada de nuvem (opcional) poderia resolver.

## Fluxo de Dados

O ciclo de disponibilização de dados funciona da seguinte forma: o firmware do ESP32 lê continuamente os três canais ADC1 correspondentes aos LDRs e o estado dos três slide switches, decide localmente o estado de cada poste (comparando a leitura ao limiar `LIMIAR_LUZ` e invertendo a decisão quando o switch daquele poste indica falha simulada), e mantém essa informação atualizada em memória. Quando o dashboard faz uma requisição HTTP `GET` para `/api/status`, o servidor embutido no ESP32 responde imediatamente com um JSON contendo o estado dos três postes. O dashboard então atualiza a tela com esses dados, repetindo a requisição em intervalos regulares para manter a exibição em tempo (quase) real.

Esse fluxo garante a consulta ao vivo do status dos postes diretamente na rede local da Feira EPA, sem qualquer dependência de uma plataforma de IoT externa.
