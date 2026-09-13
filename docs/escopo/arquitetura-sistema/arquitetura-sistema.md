# Arquitetura do Sistema - Iluminação Pública Inteligente (Projeto IoT)

Nesta seção, é apresentada a especificação técnica detalhada da **Arquitetura do Sistema** (diagrama de blocos) para o projeto de monitoramento automatizado e detecção de falhas em postes de iluminação pública. A solução foi concebida para operar como um ecossistema IoT ponta a ponta, integrando desde o sensoriamento físico até a notificação em tempo real e o registro histórico dos dados para os órgãos de gestão municipal.

---

### Descrição Detalhada dos Blocos do Sistema IoT

* **Sensores (Camada de Percepção e Aquisição):** Vão atuar como a entrada do sistema IoT. A camada é composta por fotoresistores (sensores LDR - *Light Dependent Resistor*), individualmente associados a cada poste de iluminação da maquete/circuito. Esses sensores funcionam baseados na variação de resistência elétrica em resposta à intensidade de luz incidente. Ligados em uma configuração de divisor de tensão com resistores fixos de 10 kΩ, os LDRs convertem os níveis de luminosidade (aceso/apagado/luz natural) em variações contínuas de tensão analógica, enviando os dados em tempo real para os pinos ADC1 do Microcontrolador. Isso possibilita ao sistema medir com precisão o estado físico da lâmpada de cada poste.

* **Microcontrolador (Camada de Processamento e Controle):** Vai atuar como o processamento do sistema IoT. O núcleo do projeto utiliza a placa **ESP32 DevKit V1**, equipada com microprocessador de dois núcleos Xtensa® 32-bit LX6 e programada na linguagem **C/C++** (utilizando o ecossistema/framework Arduino). O ESP32 realiza periodicamente a leitura dos canais analógicos dos sensores LDR e sincroniza o horário exato com servidores globais via protocolo **NTP** (*Network Time Protocol*). Com essas informações, o firmware executa a lógica condicional de verificação de falhas:
  1. *Noite + Poste Apagado:* Sinaliza falha/queima de lâmpada ou interrupção elétrica.
  2. *Dia + Poste Aceso:* Sinaliza desperdício de energia elétrica por acionamento indevido.
  3. *Injeção Física de Falhas:* Monitora a entrada do botão push-button (GPIO com `INPUT_PULLUP`) utilizado para simular a interrupção proposital do circuito e validar o diagnóstico automático do sistema em tempo real.
  4. *Sinalização Local (Demonstração):* Controla atuadores locais, incluindo LEDs indicadores dos postes e um Buzzer, que emite um alerta sonoro imediato quando uma falha é detectada. Essa sinalização é um reforço visual/sonoro para a demonstração na Feira EPA, e não substitui os canais oficiais de notificação e registro descritos na camada de Nuvem/App.

* **Conectividade (Camada de Rede e Transmissão):** Será o que conecta o sistema IoT à Nuvem/App. A conectividade do projeto baseia-se na interface **Wi-Fi 802.11 b/g/n (2.4 GHz)** integrada nativamente ao SoC ESP32. Ela interliga o microcontrolador à rede local com acesso à Internet. A transmissão de dados emprega criptografia/protocolos seguros de camada de aplicação (**HTTP/HTTPS**), com requisições à API do Telegram (rotas REST de envio de mensagens) para o alerta de falhas e à API do ThingSpeak para o registro periódico dos dados de luminosidade, além de suportar soquetes TCP/IP para servir conteúdo web em tempo real.

* **Nuvem/App (Camada de Aplicação, Armazenamento e Interface do Usuário):** Vai atuar tanto como a saída de dados quanto como a plataforma de armazenamento e interface de gerenciamento do sistema IoT. O sistema opera em três frentes complementares e independentes entre si:
  1. **Bot do Telegram (Notificação):** Assim que uma anomalia é confirmada pela lógica do microcontrolador, o bot encaminha instantaneamente uma notificação push para a equipe de manutenção da prefeitura, contendo o identificador do poste, a localização simulada e o horário exato da ocorrência (*ex: "⚠️ Alerta: Poste 02 - Rua X - Lâmpada queimada detectada às 22h14"*).
  2. **Servidor Web Embutido (Painel Prefeitura Digital):** O ESP32 atua simultaneamente como um servidor HTTP local. Ele renderiza uma interface dashboard em HTML/CSS para ser acessada via navegador (smartphone ou computador), permitindo a visualização ao vivo do status de todos os postes da rede, sendo ideal para demonstrações locais no evento EPA (Etec de Portas Abertas).
  3. **ThingSpeak (Registro Histórico):** Em intervalos regulares, o ESP32 envia os valores de luminosidade de cada um dos três postes para um canal do ThingSpeak, que armazena o histórico e gera automaticamente gráficos de série temporal. Essa frente complementa as duas anteriores ao evidenciar visualmente, ao longo do tempo, o padrão de acender/apagar dos postes que sustenta a lógica de detecção do sistema.

---

*Trabalho desenvolvido para apresentação no EPA (Etec de Portas Abertas) — Etec Vereador Valdivino Antônio Marcusso.*
