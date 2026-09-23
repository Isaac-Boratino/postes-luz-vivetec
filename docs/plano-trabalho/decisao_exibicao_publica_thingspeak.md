# Decisão de Exibição Pública dos Dados do ThingSpeak

## Objetivo

Definir como os dados enviados pelo ESP32 ao ThingSpeak serão exibidos ao público durante a apresentação do projeto na VivEtec / Etec de Portas Abertas.

## Decisão adotada

A equipe adotará uma **tela própria de dashboard** como forma principal de exibição dos dados ao público.

Essa tela será feita com HTML, CSS e JavaScript, consumindo os dados do ThingSpeak pela API de leitura. O Channel View nativo do ThingSpeak ficará como opção de apoio ou contingência, caso a tela própria apresente algum problema no dia da demonstração.

## Motivo da escolha

A tela própria foi escolhida porque permite uma apresentação mais clara para visitantes que não conhecem o projeto. Em vez de mostrar apenas gráficos técnicos do ThingSpeak, o dashboard poderá traduzir os dados em informações simples, como:

- qual poste está normal;
- qual poste está com falha;
- qual foi o último valor lido pelo sensor;
- quando ocorreu a última atualização;
- uma mensagem clara explicando o estado geral do sistema.

Essa abordagem também facilita ajustes visuais para a feira, como tamanho de fonte, cores, contraste e organização da tela.

## Dados exibidos ao público

A tela pública exibirá:

- status do Poste 1;
- status do Poste 2;
- status do Poste 3;
- valor bruto de luminosidade de cada poste;
- horário da última atualização recebida do ThingSpeak;
- mensagem geral do sistema, indicando se todos os postes estão normais ou se existe alguma falha;
- legenda simples explicando os estados exibidos.

## Organização dos dados

O canal do ThingSpeak será usado com três campos principais:

| Campo do ThingSpeak | Informação exibida |
|---|---|
| `field1` | Dados do Poste 1 |
| `field2` | Dados do Poste 2 |
| `field3` | Dados do Poste 3 |

A tela consultará o último registro do canal usando a API de leitura do ThingSpeak:

```txt
https://api.thingspeak.com/channels/<CHANNEL_ID>/feeds/last.json
```

Se o canal for privado, será necessário informar também a Read API Key:

```txt
https://api.thingspeak.com/channels/<CHANNEL_ID>/feeds/last.json?api_key=<READ_API_KEY>
```

## Atualização em tempo real

O dashboard fará uma nova leitura em intervalo compatível com o ThingSpeak, respeitando o tempo mínimo de atualização usado pelo projeto. A referência adotada será uma atualização a cada **15 segundos ou mais**, evitando excesso de requisições.

## Layout previsto

A tela será organizada em:

- cabeçalho com o nome do projeto;
- três cartões principais, um para cada poste;
- indicação visual de estado, usando cores;
- área de última atualização;
- mensagem de alerta ou funcionamento normal;
- legenda para visitantes.

## Estados visuais

Os estados previstos são:

| Estado | Significado visual |
|---|---|
| Normal | Poste funcionando conforme esperado |
| Falha | Poste apagado à noite ou aceso em horário indevido |
| Sem dados | O ThingSpeak ainda não retornou leitura válida |
| Desatualizado | A última leitura é antiga ou houve falha de conexão |

## Responsabilidades

- Victor: consumir, organizar e exibir os dados vindos do ThingSpeak.
- Gabriel: apoiar nos ajustes de responsividade, legibilidade e usabilidade da tela para a demonstração.

## Próximos passos

1. Confirmar com o responsável pelo back-end o Channel ID do ThingSpeak.
2. Confirmar se o canal será público ou privado.
3. Confirmar se haverá Read API Key.
4. Criar a primeira versão da tela em HTML, CSS e JavaScript.
5. Testar a atualização dos três postes usando dados reais ou simulados.
