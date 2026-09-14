# Planejamento de Trabalho — QA (Garantia de Qualidade) — Projeto VivEtec

## Responsável

| Integrante | Responsabilidades (conforme papéis do grupo) |
|---|---|
| Cauã Galvão | Testes funcionais do sistema completo (ESP32, ThingSpeak); validação de cenários de falha; checagem de consistência entre os dados exibidos no ThingSpeak; levantamento de bugs e acompanhamento das correções |

## Visão Geral

O QA do VivEtec atua como a etapa de **validação independente** do trabalho produzido pelo Back-End (firmware) e pelo Front-End (dashboard), garantindo que o sistema completo se comporte exatamente como especificado na arquitetura do projeto antes da apresentação na Feira EPA. Como o sistema tem uma superfície de testes relativamente pequena e bem definida (3 postes, 3 estados possíveis por poste, 1 buzzer, 1 canal ThingSpeak), o QA pode e deve cobrir **todas as combinações relevantes de cenário**, não apenas uma amostra.

O trabalho de QA se divide em quatro frentes:
1. **Testes funcionais unitários** (cada componente isoladamente);
2. **Testes de cenário de falha** (as 3 posições da chave gangorra, nos 3 postes);
3. **Testes de consistência de dados** (o que a maquete faz fisicamente vs. o que o ThingSpeak/dashboard exibe);
4. **Gestão de bugs** (registro, priorização e acompanhamento de correção junto às demais frentes).

## Matriz de Cenários de Teste (Cobertura Completa)

Como cada um dos 3 postes tem uma chave gangorra independente com 3 posições possíveis, e a lógica de falha depende também do período do dia (dia/noite), a matriz completa de cenários relevantes é:

| Poste | Posição da chave | Estado simulado | Período do dia | Resultado esperado no sistema |
|---|---|---|---|---|
| 1, 2 ou 3 | Posição A (Normal) | Controle normal pelo ESP32 | Noite | LED aceso, LDR detecta luz, status "Normal" |
| 1, 2 ou 3 | Posição A (Normal) | Controle normal pelo ESP32 | Dia | LED apagado, LDR não detecta luz, status "Normal" |
| 1, 2 ou 3 | Posição OFF (central) | Queimado | Noite | LED apagado, LDR não detecta luz, status "Falha — queimado", buzzer aciona |
| 1, 2 ou 3 | Posição OFF (central) | Queimado | Dia | LED apagado, LDR não detecta luz, status "Normal" (esperado apagado de dia) |
| 1, 2 ou 3 | Posição B | Aceso em horário errado | Dia | LED aceso, LDR detecta luz, status "Falha — aceso indevido", buzzer aciona |
| 1, 2 ou 3 | Posição B | Aceso em horário errado | Noite | LED aceso, LDR detecta luz, status "Normal" (esperado aceso à noite) |

*Observação importante para o QA:* a posição **OFF durante o dia** e a posição **B durante a noite** não configuram falha do ponto de vista da lógica dia/noite (mesmo resultado que o estado normal esperado) — esses dois cenários são especialmente importantes de testar, pois validam que o sistema não gera **falso positivo** de falha nessas combinações.

Com 3 postes × 3 posições × 2 períodos do dia, a matriz completa soma **18 combinações de teste**, que devem ser executadas ao menos uma vez cada durante a fase de testes integrados.

## Etapas Detalhadas de Testes

### Etapa 1 — Preparar o ambiente de teste
- Confirmar com o Back-End que o firmware já está com a lógica de leitura, detecção de falha, buzzer e envio ao ThingSpeak implementada e funcional isoladamente
- Confirmar com o Front-End que o dashboard já está consumindo os dados reais do canal ThingSpeak (não valores fixos/mockados)
- Preparar uma forma de simular "dia" e "noite" de forma controlada durante os testes (ex.: ajustar temporariamente o horário de referência no firmware, ou cobrir/destampar fisicamente os LDRs em ambiente controlado de luz), já que nem sempre será viável testar apenas esperando a virada real de dia para noite

### Etapa 2 — Testes funcionais unitários (por componente)
- **LDRs:** verificar, via monitor serial, se os 3 sensores retornam valores coerentes com a luz recebida, sem valores travados ou ruído excessivo
- **LEDs:** verificar se cada LED responde corretamente ao comando do firmware quando a chave está na posição Normal, sem interferência entre postes vizinhos
- **Chaves gangorra:** testar manualmente as 3 posições de cada uma das 3 chaves, confirmando que a mudança de posição realmente altera o estado físico do LED daquele poste, e apenas daquele poste
- **Buzzer:** confirmar que o som dispara de forma perceptível e no momento correto, sem atraso perceptível em relação à mudança de estado do poste
- **Conexão Wi-Fi/NTP:** confirmar, via serial, que o ESP32 conecta à rede e sincroniza corretamente o horário na inicialização

### Etapa 3 — Execução da matriz completa de cenários de falha
- Executar, de forma sistemática, as 18 combinações da matriz de cenários acima, registrando para cada uma:
  - Estado esperado (segundo a especificação do projeto)
  - Estado realmente observado no LED/LDR (localmente, na maquete)
  - Estado exibido no ThingSpeak (Channel View e/ou dashboard do Front-End)
  - Se o buzzer disparou (quando aplicável)
- Sinalizar qualquer divergência entre o esperado e o observado como um bug, seguindo o processo descrito na Etapa 6

### Etapa 4 — Testes de consistência entre a maquete física e o ThingSpeak
- Para cada cenário testado, cronometrar o tempo entre a mudança física da chave gangorra e a atualização correspondente no canal do ThingSpeak, verificando se esse tempo é compatível com o intervalo de envio configurado no firmware (mínimo de 15 segundos)
- Verificar se os 3 campos (`field1`, `field2`, `field3`) do canal correspondem corretamente aos postes 1, 2 e 3, sem troca de campos entre postes
- Verificar se o dashboard do Front-End exibe o mesmo status de falha/normal que o ThingSpeak Alerts dispara, evitando que o dashboard mostre "Normal" enquanto o Alerts já disparou uma notificação de falha (ou vice-versa)
- Verificar o comportamento do sistema quando a conexão Wi-Fi do ESP32 é interrompida propositalmente durante o teste: o LED/buzzer devem continuar funcionando localmente (já que essa lógica não depende da rede), enquanto o ThingSpeak/dashboard devem indicar dado desatualizado, e não um estado incorreto

### Etapa 5 — Testes de robustez e condições de borda
- Testar o comportamento do sistema alternando rapidamente a posição de uma mesma chave entre os 3 estados, verificando se o firmware e o dashboard não travam nem exibem estados inconsistentes durante a transição
- Testar o sistema com mais de um poste em falha simultaneamente (ex.: Poste 1 queimado e Poste 3 aceso em horário errado ao mesmo tempo), verificando se o buzzer e os indicadores tratam corretamente múltiplas falhas concorrentes
- Testar o sistema próximo ao horário de transição dia/noite definido no firmware, verificando se não há oscilação indevida de status por conta de uma leitura de luminosidade momentaneamente próxima ao `limiarLuz` calibrado pelo Back-End
- Testar a operação contínua do sistema por um período prolongado (ao menos o tempo estimado de duração da apresentação na Feira EPA), observando estabilidade geral

### Etapa 6 — Registro, priorização e acompanhamento de bugs
- Registrar cada divergência encontrada como uma issue no GitHub Project (label `integração/testes`), com título objetivo, passos para reproduzir, resultado esperado e resultado observado
- Classificar cada bug por severidade (ex.: **Crítico** — quebra a demonstração, ex.: buzzer não dispara em nenhuma falha; **Moderado** — afeta um cenário específico, mas não compromete a demonstração geral; **Leve** — detalhe visual ou de precisão, sem impacto funcional)
- Atribuir cada bug ao responsável da frente correspondente (Back-End ou Front-End) e acompanhar a movimentação da issue até a coluna "Concluído"
- Reexecutar (teste de regressão) o cenário específico que apresentou o bug após a correção, confirmando que ele foi resolvido sem introduzir um novo problema em outro cenário já validado anteriormente

### Etapa 7 — Validação final pré-apresentação
- Executar novamente a matriz completa de 18 cenários como teste de aceitação final, próximo à data da Feira EPA, já com o sistema montado na configuração final de apresentação (mesmo local, mesma rede Wi-Fi, mesmo dispositivo exibindo o dashboard)
- Validar, junto ao Gestor do Projeto, se todos os bugs críticos e moderados foram corrigidos antes da apresentação, documentando quaisquer limitações conhecidas e não corrigidas a tempo (para que a equipe saiba como contorná-las durante a demonstração ao público)
- Preparar um roteiro curto de "o que testar ao vivo" para a demonstração, priorizando os cenários mais didáticos (ex.: alternar uma chave para "queimado" durante a noite simulada e mostrar o buzzer + a mudança no dashboard em tempo real)

## Checklist Final de QA

| Item verificado | Esperado |
|---|---|
| As 18 combinações da matriz de cenários foram executadas e documentadas | Sim |
| Os 3 LDRs, 3 LEDs e o buzzer passaram nos testes funcionais unitários | Sim |
| Não há falso positivo de falha nas combinações OFF/dia e B/noite | Sim |
| O tempo entre a mudança física e a atualização no ThingSpeak é compatível com o esperado | Sim |
| Os 3 campos do canal ThingSpeak correspondem corretamente aos 3 postes | Sim |
| O dashboard do Front-End é consistente com o status do ThingSpeak Alerts | Sim |
| O sistema local (LED/buzzer) continua funcionando durante queda de Wi-Fi | Sim |
| Múltiplas falhas simultâneas são tratadas corretamente | Sim |
| Todos os bugs críticos e moderados foram corrigidos e reexecutados (regressão) | Sim |
| O teste de aceitação final foi executado na configuração real de apresentação | Sim |

## Observações

- Esta matriz de testes assume a arquitetura final do projeto (chave gangorra de 3 posições por poste, sem botão push-button de simulação única e sem canais de notificação via Telegram/painel web local), conforme documentado na arquitetura do sistema e nos sensores/atuadores do projeto.
- O QA deve trabalhar em paralelo com o Back-End e o Front-End desde as primeiras versões funcionais de cada frente, em vez de concentrar todos os testes apenas na reta final — isso reduz o risco de bugs críticos serem descobertos tarde demais para correção antes da Feira EPA.
- Este planejamento pode ser adaptado conforme os componentes efetivamente recebidos por patrocínio da escola e o cronograma real de entrega de cada frente.
