# 3.5 Plataforma IoT / Armazenamento

## Plataforma Escolhida: ThingSpeak

O projeto utiliza o **ThingSpeak** (plataforma de IoT mantida pela MathWorks, mesma empresa do MATLAB) como camada de armazenamento e visualização histórica dos dados de luminosidade captados pelos três sensores LDR. A escolha se justifica por três motivos alinhados diretamente às necessidades do projeto:

**1. Estrutura de canais e campos compatível com o cenário do projeto.** O ThingSpeak organiza os dados em "canais", cada um com até 8 "campos" (fields) numéricos independentes. Isso se encaixa perfeitamente na necessidade de monitorar três postes simultaneamente: cada LDR é mapeado para um campo distinto do mesmo canal (`field1`, `field2`, `field3`), permitindo acompanhar o comportamento de cada poste de forma individual, mas dentro de uma única estrutura de dados.

**2. Geração automática de gráficos.** Diferente de uma solução de armazenamento "cru" (como um banco de dados genérico), o ThingSpeak gera automaticamente gráficos de linha do tempo para cada campo assim que os dados são recebidos, sem necessidade de programação adicional no lado do servidor. Isso é especialmente relevante para o objetivo de "apresentar o funcionamento do sistema de forma clara na Feira EPA": o projeto ganha uma evidência visual do padrão de acender/apagar dos postes ao longo do tempo, reforçando a lógica de comparação dia/noite que sustenta todo o sistema.

**3. Integração simples e de baixo custo.** O plano gratuito do ThingSpeak permite até 3 milhões de mensagens por ano (mais do que suficiente para uma demonstração pontual) e é acessado por uma API REST simples via HTTP, sem necessidade de bibliotecas complexas ou configuração de servidor próprio. Existe ainda uma biblioteca oficial (`ThingSpeak.h`) mantida pela própria MathWorks e compatível com o ambiente Arduino/ESP32, o que reduz o tempo de desenvolvimento.

## Comparação com as Demais Opções

- **Adafruit IO:** também oferece dashboards e feeds gratuitos, e seria uma alternativa viável; porém, o ThingSpeak tem uma curva de aprendizado mais direta para o caso específico de poucos campos numéricos com foco em gráficos de série temporal, que é exatamente o que o projeto precisa.
- **Blynk:** é voltado principalmente para controle e visualização via aplicativo mobile personalizável, com foco maior em interação/controle do que em histórico gráfico — o diferencial do Blynk não agrega tanto quanto o histórico gráfico do ThingSpeak para o objetivo deste projeto.
- **Firebase:** é um banco de dados em tempo real completo, poderoso, mas exige mais configuração (criação de projeto no console do Google, regras de segurança, SDKs) para um ganho que, na escala deste projeto (três sensores, um evento de demonstração), não compensa a complexidade adicional.
- **MQTT Broker próprio (Mosquitto):** exigiria um segundo dispositivo (como um Raspberry Pi) hospedando o broker, o que aumenta custo, complexidade de montagem e pontos de falha — incompatível com a proposta de solução simples e de baixo custo do projeto.
- **Ubidots:** tem um plano educacional gratuito interessante e também seria funcional, mas com menos documentação em português e exemplos prontos para ESP32 do que o ThingSpeak, o que pesa em um projeto com prazo definido.

## Consulta ao Vivo (função antes atribuída ao painel web)

Além do histórico, o próprio canal do ThingSpeak cumpre o papel de consulta imediata do status dos postes, function que antes era do painel web local. A visualização "Channel View" do ThingSpeak é acessível via navegador (computador ou celular) por qualquer pessoa com o link do canal (público) ou com login autorizado (privado), exibindo os gráficos de cada campo (`field1`, `field2`, `field3`) já atualizados com a última leitura enviada. Para a demonstração na Feira EPA, isso dispensa a necessidade de hospedar um servidor web no próprio ESP32: basta abrir o link do canal em qualquer dispositivo conectado à internet para acompanhar, em tempo quase real, o estado de luminosidade de cada poste.

## ThingSpeak Alerts (função antes atribuída ao bot do Telegram)

Para o alerta pontual de falha, o projeto passa a usar o recurso nativo **ThingSpeak Alerts** (parte do app oficial ThingSpeak, também mantido pela MathWorks), em vez de um bot próprio. O Alerts permite configurar condições sobre os campos do canal (por exemplo, disparar quando o valor de `field1`, `field2` ou `field3` cair abaixo do limiar que caracteriza "poste apagado") e, quando a condição é atendida, envia automaticamente uma notificação — por push no aplicativo ThingSpeak, por e-mail, ou por um webhook via **ThingHTTP**, que pode inclusive ser configurado para chamar outro serviço de mensageria, se o grupo optar por isso no futuro. Como essa lógica de disparo roda no lado da plataforma (e não no firmware do ESP32), o Back-End só precisa garantir que os dados corretos cheguem ao canal; a configuração da condição de alerta e do canal de notificação é feita diretamente na interface web do ThingSpeak, sem necessidade de token de bot, `chat_id` ou bibliotecas adicionais no ESP32.

## Fluxo de Dados na Plataforma

O ciclo de armazenamento funciona da seguinte forma: em intervalos regulares (respeitando o limite mínimo de 15 segundos entre gravações do plano gratuito), o firmware do ESP32 lê os três canais ADC1 correspondentes aos LDRs, converte essas leituras nos valores que serão enviados, e realiza uma requisição HTTP `GET` para o endpoint `https://api.thingspeak.com/update`, informando a **Write API Key** do canal (chave que identifica e autoriza a escrita naquele canal específico) e os três valores como parâmetros (`field1=`, `field2=`, `field3=`). O ThingSpeak recebe essa requisição, armazena o registro com um timestamp automático, e atualiza os gráficos do canal em tempo real — que podem então ser visualizados publicamente ou de forma privada, conforme a configuração escolhida para o canal, em qualquer navegador com acesso à internet, inclusive durante a apresentação na Feira EPA.

Esse fluxo garante o histórico de longo prazo dos dados de luminosidade dos três postes, ao mesmo tempo em que sustenta a consulta ao vivo (Channel View) e o disparo de alertas (ThingSpeak Alerts) descritos acima, complementando a lógica de detecção de falhas executada localmente no ESP32.
