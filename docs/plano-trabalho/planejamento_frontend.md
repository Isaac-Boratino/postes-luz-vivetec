# Planejamento de Trabalho — Front-End — Projeto VivEtec

## Responsáveis

| Integrante | Responsabilidades (conforme papéis do grupo) |
|---|---|
| Victor Eufrasio | Consumo dos dados vindos do back-end/ThingSpeak para exibição em tempo real |
| Gabriel Moura | Responsividade e usabilidade para a demonstração na Etec de Portas Abertas |

## Visão Geral

Como o projeto substituiu o painel web embutido no próprio ESP32 (servidor HTTP local) pela plataforma **ThingSpeak** como camada de armazenamento, consulta ao vivo e alerta, o Front-End do VivEtec não é responsável por hospedar um servidor nem por processar lógica de negócio — ele é uma **camada de apresentação** que consome os dados já publicados no canal do ThingSpeak (via Channel View nativo e/ou API REST pública) e os exibe de forma clara, legível e visualmente adequada ao público leigo da Feira EPA.

O Front-End cumpre três funções centrais:
1. **Exibir o status atual dos 3 postes** (aceso/apagado, dentro do padrão esperado ou em falha), atualizado em tempo quase real;
2. **Exibir o histórico gráfico de luminosidade** de cada poste (aproveitando os gráficos de série temporal já gerados automaticamente pelo ThingSpeak);
3. **Garantir que a visualização funcione bem no dispositivo usado na demonstração** (tablet, notebook ou monitor conectado à maquete), com boa legibilidade a curta e média distância para visitantes do evento.

## Decisão de Arquitetura: Origem dos Dados

O Front-End tem duas formas possíveis (não excludentes) de exibir os dados do ThingSpeak, e a equipe deve decidir qual adotar como principal:

| Opção | Como funciona | Vantagens | Limitações |
|---|---|---|---|
| **A — Embed do Channel View nativo** | Incorporar via `<iframe>` os widgets de gráfico que o próprio ThingSpeak gera para cada campo (`field1`, `field2`, `field3`) | Zero código de integração, pronto e testado, atualiza sozinho | Menos controle visual (layout, cores, idioma), depende de o canal estar público ou de link compartilhável |
| **B — Dashboard próprio via API REST** | Buscar os dados brutos via `GET https://api.thingspeak.com/channels/<CHANNEL_ID>/feeds.json?api_key=<READ_API_KEY>&results=1` (ou `/last.json` para o valor mais recente) e renderizar com HTML/CSS/JS próprio | Controle total do layout, idioma, cores por poste, ícones de alerta customizados — mais alinhado à identidade visual do projeto | Exige código de integração, tratamento de erros de rede e testes adicionais |

*Recomendação:* usar a **Opção B** como dashboard principal da demonstração (por dar controle visual total e permitir indicadores customizados de falha), com a **Opção A** como plano de contingência (link direto do Channel View aberto em outra aba, caso o dashboard próprio apresente problema no dia do evento).

## Tecnologias Previstas

| Camada | Tecnologia | Observação |
|---|---|---|
| Estrutura | HTML5 | Estrutura semântica da página do dashboard |
| Estilo | CSS3 (ou framework leve, ex.: Bootstrap/Tailwind via CDN) | Responsividade e identidade visual do VivEtec |
| Lógica de exibição | JavaScript puro (Fetch API) | Consumo do endpoint REST do ThingSpeak, sem necessidade de framework pesado dado o escopo pequeno (3 campos) |
| Gráficos (opcional, se a Opção B for adotada) | Chart.js (via CDN) | Alternativa ao gráfico nativo do ThingSpeak, caso se deseje estilização própria |
| Hospedagem da página | Estática (aberta localmente via navegador, ou hospedada em GitHub Pages) | Não exige servidor próprio, coerente com a arquitetura sem painel web embutido no ESP32 |

*Observação: como o ThingSpeak é mantido pela MathWorks e não expõe CORS liberado para todos os cenários por padrão, a equipe deve validar durante os testes se o `fetch()` direto do navegador funciona sem bloqueio; caso haja restrição de CORS, a alternativa é usar o parâmetro JSONP suportado pela API do ThingSpeak ou recorrer à Opção A (embed).*

## Estrutura das Telas do Dashboard

| Elemento da tela | Fonte do dado | Função |
|---|---|---|
| Cabeçalho com nome do projeto (VivEtec) | Estático | Identificação visual na Feira EPA |
| Indicador de status — Poste 1, 2 e 3 (cartão colorido: verde = normal, vermelho = falha) | `field1`, `field2`, `field3` (última leitura) comparados ao limiar de referência | Leitura instantânea do estado de cada poste, sem precisar interpretar números |
| Valor numérico bruto do LDR de cada poste | `field1`, `field2`, `field3` | Apoio técnico/didático para explicar o funcionamento aos visitantes |
| Horário da última atualização recebida | Campo `created_at` do feed | Transparência sobre a atualidade do dado exibido |
| Gráfico de histórico (linha do tempo) por poste | Feeds históricos do canal (`feeds.json?results=N`) ou embed nativo | Evidenciar visualmente o padrão de acender/apagar ao longo do tempo |
| Rodapé com legenda dos estados | Estático | Ajuda o visitante a interpretar rapidamente o painel |

## Etapas Detalhadas de Desenvolvimento

### Etapa 1 — Levantar os dados de acesso ao canal ThingSpeak
- Confirmar com o Back-End (responsável pela integração ThingSpeak) o **Channel ID** e a **Read API Key** do canal já criado
- Verificar se o canal está configurado como público ou privado, já que isso define se a Read API Key é obrigatória em toda requisição
- Validar manualmente, via navegador ou ferramenta como Postman, que o endpoint `https://api.thingspeak.com/channels/<CHANNEL_ID>/feeds/last.json` retorna os três campos (`field1`, `field2`, `field3`) preenchidos

### Etapa 2 — Estruturar o HTML base do dashboard
- Criar a estrutura de seções descrita na tabela acima (cabeçalho, 3 cartões de poste, área de gráfico, rodapé)
- Definir a grade responsiva (३ colunas em telas largas, empilhado em telas estreitas) usando CSS Grid ou Flexbox
- Aplicar a identidade visual do projeto (cores, tipografia) de forma consistente com o restante da apresentação/documentação do VivEtec

### Etapa 3 — Implementar o consumo de dados (Fetch)
- Escrever a função de requisição periódica ao endpoint `/feeds/last.json`, respeitando um intervalo de atualização compatível com o intervalo mínimo de gravação do ThingSpeak (mínimo 15 segundos; sugestão: poll a cada 20–30 segundos para não sobrecarregar sem necessidade)
- Tratar erros de rede (timeout, resposta vazia) exibindo um estado visual de "sem conexão" em vez de travar o dashboard
- Armazenar em variáveis de estado no JavaScript os últimos valores recebidos de cada campo, para computar comparações (ex.: valor mudou desde a última leitura)

### Etapa 4 — Implementar a lógica de exibição do status (normal/falha)
- Definir, em conjunto com o Back-End, o mesmo `limiarLuz` (ou uma lógica equivalente) usado no firmware, para que o Front-End "traduza" o valor bruto do LDR em um rótulo (Aceso/Apagado) coerente com o que o ESP32 já decidiu
- Implementar a comparação simples: se o valor indicar "apagado à noite" ou "aceso de dia" (a mesma lógica dia/noite documentada na arquitetura do sistema), marcar o cartão daquele poste como **Falha**; caso contrário, marcar como **Normal**
- Atualizar dinamicamente a cor/ícone do cartão a cada novo ciclo de requisição, sem precisar recarregar a página inteira

### Etapa 5 — Implementar a visualização de histórico
- Se optar pela Opção B (dashboard próprio): buscar os últimos N registros via `feeds.json?results=N` e renderizar um gráfico de linha por poste com Chart.js, com o eixo X representando o horário (`created_at`) e o eixo Y o valor do LDR
- Se optar pela Opção A (embed nativo): inserir os `<iframe>` fornecidos pelo próprio ThingSpeak na tela de Channel View do canal, um por campo
- Validar visualmente que o gráfico realmente evidencia o padrão de acender/apagar ao alternar as chaves gangorra durante um teste manual

### Etapa 6 — Responsividade e usabilidade para a demonstração (foco: Gabriel Moura)
- Testar o dashboard em pelo menos dois tamanhos de tela (o dispositivo real usado no estande e um celular, para verificar comportamento fora do previsto)
- Ajustar tamanhos de fonte e espaçamento para leitura a uma distância de 1–2 metros (situação típica de uma feira, com visitantes observando em pé)
- Garantir contraste adequado entre as cores de status (verde/vermelho) para acessibilidade e boa visibilidade sob iluminação de estande
- Adicionar um estado de carregamento (loading) visível na primeira requisição, evitando uma tela em branco enquanto os dados chegam
- Revisar a navegação/fluxo para que um visitante consiga entender o status dos postes sem precisar de explicação verbal extensa da equipe

### Etapa 7 — Testes de integração com o Back-End
- Realizar testes junto com o Back-End alternando manualmente as chaves gangorra dos 3 postes (normal, queimado, aceso em horário errado) e conferir se o dashboard reflete corretamente cada mudança dentro do intervalo de atualização esperado
- Validar o comportamento do dashboard em caso de atraso ou falha temporária de conexão Wi-Fi do ESP32 (o dashboard deve indicar dado desatualizado, não mostrar um estado incorreto)
- Repassar ao QA (Cauã Galvão) os cenários já validados para checagem cruzada de consistência

### Etapa 8 — Preparação para a apresentação
- Testar o dashboard na rede Wi-Fi real do local de demonstração (ou a mais próxima possível), já que a conectividade da Etec pode diferir do ambiente de desenvolvimento
- Preparar uma versão de contingência offline/estática (prints de tela ou vídeo curto do dashboard funcionando) para o caso de indisponibilidade de internet no dia do evento
- Definir o dispositivo físico (notebook/tablet) que exibirá o dashboard ao lado da maquete, considerando ângulo de visão do público

## Checklist Final de Front-End

| Item verificado | Esperado |
|---|---|
| O dashboard busca os dados corretos do canal ThingSpeak (Channel ID/API Key configurados) | Sim |
| Os 3 cartões de poste refletem corretamente o status Normal/Falha | Sim |
| O horário da última atualização é exibido e condizente com a hora real | Sim |
| O gráfico de histórico exibe corretamente as mudanças de luminosidade | Sim |
| O layout se adapta ao dispositivo usado na demonstração (responsividade) | Sim |
| A leitura do status é possível a uma distância de 1–2 metros | Sim |
| O dashboard trata corretamente falhas temporárias de rede sem travar | Sim |
| Existe um plano de contingência para indisponibilidade de internet no evento | Sim |

## Observações

- Este planejamento assume a arquitetura final do projeto (sem servidor web embutido no ESP32 e sem bot de notificação via Telegram), já que essas alternativas foram substituídas pelo uso do ThingSpeak como camada única de nuvem/aplicação, conforme documentado na arquitetura do sistema.
- Qualquer mudança na estrutura de campos do canal ThingSpeak feita pelo Back-End (ex.: adição de um quarto campo) deve ser comunicada ao Front-End para ajuste do dashboard.
- Este planejamento pode ser adaptado conforme os testes práticos realizados pelo grupo e eventuais limitações de tempo até a Feira EPA.
