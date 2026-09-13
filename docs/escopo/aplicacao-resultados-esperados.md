# 4. Aplicação e Resultados Esperados

## 4.1 Cenário de Uso

Em uma aplicação real (fora do contexto da maquete de demonstração), o sistema seria instalado em postes de iluminação pública de ruas e bairros residenciais, sob responsabilidade da prefeitura local — que, desde 2015, é a entidade encarregada pela manutenção da iluminação pública no Brasil. Cada poste receberia um módulo com um sensor LDR posicionado próximo à luminária, monitorando continuamente se ela está acesa ou apagada.

A equipe de manutenção da prefeitura seria a principal usuária do sistema, interagindo com ele por meio de um canal do ThingSpeak, que reúne tanto a consulta ao vivo do status de cada poste (Channel View) quanto o alerta automático em caso de falha (ThingSpeak Alerts), além do histórico de comportamento dos postes ao longo do tempo — útil, por exemplo, para identificar padrões de degradação de uma lâmpada antes mesmo da falha completa, ou para embasar decisões de manutenção preventiva, sem depender de uma denúncia manual dos moradores.

Na prática, o fluxo seria: o sistema verifica continuamente o estado de cada poste comparando a leitura do LDR com o horário do dia (via sincronização NTP); ao identificar uma inconsistência (poste apagado à noite ou aceso de dia), ele envia o dado ao ThingSpeak, que registra a ocorrência no histórico e dispara automaticamente o alerta ao responsável, sem qualquer intervenção humana até esse ponto — substituindo o modelo atual, que depende de um morador perceber o problema e abrir uma reclamação.

## 4.2 Benefícios Esperados

- **Redução do tempo de detecção de falhas:** de um modelo reativo (dependente de denúncia do morador, que pode levar dias ou semanas) para um modelo de detecção em tempo real, reduzindo drasticamente o tempo entre a ocorrência da falha e o início do reparo;
- **Redução do desperdício de energia elétrica:** ao identificar automaticamente postes acesos durante o dia (funcionamento indevido), o sistema permite correção rápida desse desperdício, que hoje é pago integralmente pela prefeitura mesmo sem necessidade;
- **Redução do custo de manutenção corretiva:** a manutenção preventiva/rápida tende a ser mais barata do que a manutenção corretiva feita após a falha já ter causado impacto prolongado (rua às escuras por dias);
- **Contribuição para a segurança pública:** conforme apontado na pesquisa de contextualização do projeto, ruas mal iluminadas estão associadas a maior incidência de crimes contra o patrimônio, e reforços de iluminação já registraram reduções expressivas em crimes noturnos em estudos internacionais — a manutenção mais rápida dos postes sustenta esse benefício de forma contínua, em vez de depender de reformas pontuais de iluminação.

Como o projeto atual é uma maquete de validação de conceito, esses benefícios são projetados com base na lógica do sistema e nas referências de contextualização levantadas, e não em métricas medidas de uma instalação real — a quantificação exata desses ganhos exigiria um piloto em escala municipal, apontado como direção natural de continuidade do projeto.

## 4.3 Limitações e Melhorias Futuras

### Limitações

- **Dependência de conexão Wi-Fi:** o sistema depende de uma rede Wi-Fi com acesso à internet para o registro no ThingSpeak; em uma instalação real espalhada pela cidade, isso exigiria cobertura Wi-Fi em cada poste — algo que dificilmente estaria disponível na prática, tornando essa arquitetura de conectividade adequada à demonstração, mas não diretamente replicável em campo sem adaptação;
- **Fonte de alimentação da maquete não é a de uma instalação real:** a alimentação via USB, usada na demonstração, não se aplica a um poste de rua; uma instalação real exigiria uma fonte de alimentação própria (como painel solar com bateria de backup), o que não foi validado neste protótipo;
- **Necessidade de calibração dos sensores:** o limiar de luminosidade (`limiarLuz`) que define se um poste está "aceso" ou "apagado" precisa ser calibrado manualmente para as condições de luz do ambiente onde o sistema é testado, podendo exigir ajuste caso o projeto seja replicado em outro local;
- **Escala limitada da validação:** o protótipo monitora apenas três postes simulados; o comportamento do sistema com um número muito maior de pontos monitorados simultaneamente (centenas ou milhares de postes, como seria o caso de uma cidade real) não foi testado nesta fase.

### Melhorias Futuras

- **Substituir o Wi-Fi por uma tecnologia de longo alcance e baixo consumo (como LoRa) para uma instalação real**, permitindo que os postes se comuniquem com um gateway central sem depender de uma rede Wi-Fi ponto a ponto em cada localização;
- **Adicionar um sensor de corrente elétrica** junto ao LDR, permitindo distinguir com mais precisão entre uma lâmpada queimada e uma falha na fiação/alimentação do poste;
- **Aplicar técnicas simples de análise de tendência sobre o histórico do ThingSpeak**, para identificar sinais de degradação gradual de uma lâmpada (como oscilações de luminosidade) antes que ela chegue a queimar por completo, antecipando a manutenção;
- **Migrar a fonte de alimentação para painel solar + bateria** em uma futura instalação de campo, tornando o sistema energeticamente independente da rede elétrica do próprio poste.
