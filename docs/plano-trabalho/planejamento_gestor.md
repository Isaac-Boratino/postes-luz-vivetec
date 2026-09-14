# Planejamento de Trabalho — Gestor do Projeto — Projeto VivEtec

## Responsável

| Integrante | Responsabilidades (conforme papéis do grupo) |
|---|---|
| Isaac Boratino | Coordenação geral do cronograma e das entregas do grupo; organização e manutenção do repositório no GitHub; elaboração/consolidação da documentação de escopo; ponto de contato entre a equipe e os professores/avaliadores |

## Visão Geral

O papel de Gestor do Projeto no VivEtec não é técnico no sentido de escrever firmware ou dashboard, mas é responsável por garantir que o **trabalho técnico das outras três frentes (Back-End, Front-End e QA) seja rastreável, organizado e entregue dentro do prazo**, além de ser o elo de comunicação entre o grupo e a escola — tanto para a obtenção dos materiais patrocinados quanto para a relação com professores/avaliadores. Isso é feito principalmente através de três instrumentos: o **repositório GitHub**, o **GitHub Project (board Kanban)**, e a **documentação de escopo consolidada** (os arquivos de contexto, arquitetura, hardware, sensores, conectividade, fonte de alimentação e plataforma IoT, além dos planejamentos de trabalho de cada frente — montagem da maquete, Back-End, Front-End, Gestor e QA — já produzidos pelo grupo).

## Estrutura do Repositório GitHub

Conforme o Guia de Configuração do GitHub Project fornecido pela Etec, o Gestor deve garantir:

| Item | Definição |
|---|---|
| Nome do repositório | Sugestão no padrão `nome-do-tema-vivetec` (ex.: `postes-luz-vivetec`) |
| Visibilidade | Privado ou público, conforme preferência do grupo |
| Colaboradores | Todos os 6 integrantes adicionados como colaboradores do repositório, cada um com sua própria conta do GitHub |
| GitHub Project vinculado | Criado a partir da aba "Projects" > "New project" > modelo "Board" |
| Estrutura de pastas sugerida | `/firmware` (código do ESP32), `/frontend` (dashboard), `/docs` (documentação de escopo e pesquisa), `/diario-de-bordo` (registros individuais) |

## Estrutura do Board Kanban

| Coluna | Significado |
|---|---|
| Backlog | Ideias e tarefas ainda não priorizadas |
| A Fazer | Tarefas priorizadas para a etapa atual (Entrega 1 ou Entrega 2) |
| Em Andamento | Tarefa sendo executada no momento — idealmente 1 por pessoa por vez |
| Em Revisão | Pronta para o grupo revisar/testar antes de considerar concluída |
| Concluído | Tarefa finalizada e validada pelo grupo |

## Labels por Frente de Trabalho

| Label | Uso |
|---|---|
| `firmware/esp32` | Código embarcado, sensores, atuadores (frente Back-End) |
| `backend/api` | Integração com o ThingSpeak, lógica de envio de dados |
| `frontend/web` | Interface web, dashboard de consumo do ThingSpeak |
| `integração/testes` | Comunicação entre partes, testes end-to-end (frente QA) |
| `documentação` | README, relatório, diário de bordo, material de apresentação |

*As cores de cada label ficam a critério do grupo — o importante, segundo o guia da escola, é manter consistência ao longo de todo o projeto.*

## Regras de Uso das Issues

- Toda tarefa relevante do projeto (técnica ou de documentação) deve virar uma issue — nada de trabalho "invisível" fora do board
- Toda issue precisa de um responsável (assignee); issues sem responsável não contam como evidência individual perante a avaliação
- Título objetivo, com a label de frente correspondente (ex.: "[firmware/esp32] Calibrar limiarLuz dos 3 LDRs")
- Ao concluir, a issue deve ser movida para "Concluído" e, se aplicável, referenciada no commit/pull request (ex.: "Closes #12")

### Modelo de Issue (padrão a ser replicado pelo Gestor)

```
Título: [Frente] Descrição curta da tarefa
Assignee: @usuario-do-aluno
Label: firmware/esp32 (ou outra)
Descrição:
- O que precisa ser feito
- Critério de pronto (o que define a tarefa como concluída)
- Dependências (se houver)
```

## Cronograma Macro do Projeto (Marcos)

| Fase | Entregável principal | Frentes envolvidas |
|---|---|---|
| 1. Pesquisa e escopo | Documentos de contexto, objetivo, justificativa e arquitetura do sistema (já consolidados) | Todo o grupo (mantido pelo Gestor) |
| 2. Obtenção dos materiais | Solicitação e recebimento dos componentes patrocinados pela escola | Gestor (solicitação/acompanhamento) |
| 3. Montagem física da maquete | Postes, LEDs, LDRs, chaves gangorra e buzzer montados e testados fisicamente | Back-End (apoio de todo o grupo na montagem física) |
| 4. Desenvolvimento do firmware | Leitura de sensores, lógica de falha, buzzer, integração ThingSpeak | Back-End |
| 5. Desenvolvimento do dashboard | Consumo dos dados do ThingSpeak, responsividade | Front-End |
| 6. Testes integrados | Validação de cenários de falha, consistência dos dados exibidos | QA |
| 7. Preparação da apresentação | Roteiro de demonstração, material visual, ensaio | Todo o grupo (coordenado pelo Gestor) |
| 8. Feira EPA | Apresentação final na Etec de Boituva | Todo o grupo |

## Etapas Detalhadas de Gestão

### Etapa 1 — Configuração inicial do repositório e do board
- Criar o repositório GitHub com o nome definido pelo grupo e adicionar os 6 integrantes como colaboradores
- Criar o GitHub Project vinculado, configurando as 5 colunas do Kanban e as labels por frente de trabalho
- Cadastrar as primeiras issues de cada frente com base nos planejamentos já elaborados (montagem da maquete, Back-End, Front-End, QA)

### Etapa 2 — Solicitação e acompanhamento dos materiais patrocinados
- Formalizar junto à escola a solicitação dos componentes previstos (ESP32, LDRs, LEDs, chaves gangorra, buzzer, resistores, protoboard, jumpers, fonte de alimentação), conforme a lista já definida no escopo técnico do projeto
- Registrar a data de entrega dos componentes e conferir a lista recebida contra a lista solicitada, sinalizando ao grupo qualquer item faltante ou substituído
- Atualizar a documentação de obtenção de materiais caso haja itens adicionais confirmados pela escola

### Etapa 3 — Coordenação do cronograma entre as frentes
- Acompanhar semanalmente o andamento das colunas do board, identificando tarefas paradas sem justificativa
- Garantir que nenhuma dependência entre frentes vire gargalo sem aviso prévio (ex.: o Front-End depende do Channel ID/API Key que só o Back-End pode gerar ao criar o canal no ThingSpeak — o Gestor deve garantir que essa informação circule a tempo)
- Redistribuir prioridades no board conforme a proximidade da Feira EPA, sempre registrando a mudança como atualização de issue, não como decisão informal fora do board

### Etapa 4 — Manutenção da documentação de escopo já consolidada
- Manter organizada, na pasta `/docs` do repositório, a documentação de escopo já consolidada pelo grupo (contexto e objetivo, justificativa, arquitetura do sistema, hardware, sensores/atuadores, conectividade, fonte de alimentação, plataforma IoT, aplicação e resultados esperados), incluindo os planejamentos de trabalho de cada frente (montagem da maquete, Back-End, Front-End, Gestor e QA)
- Incorporar à documentação eventuais ajustes pontuais que surjam ao longo do projeto (ex.: troca de um componente, novo valor de calibração, mudança de um pino), mantendo-a sempre coerente com o que é de fato implementado
- Manter o `README.md` do repositório atualizado com uma visão geral do projeto, a lista de integrantes e papéis, e instruções básicas de uso do firmware e do dashboard

### Etapa 5 — Ponto de contato com professores e avaliadores
- Centralizar as dúvidas e comunicações formais entre o grupo e os professores responsáveis pela avaliação do projeto
- Repassar ao grupo, de forma organizada, qualquer critério de avaliação, prazo ou ajuste de escopo comunicado pela escola
- Preparar, junto com o grupo, os materiais de acompanhamento individual exigidos pela atividade (diário de bordo, autoavaliação/avaliação por pares), reforçando que o GitHub Project é o principal instrumento de rastreabilidade técnica individual, mas deve ser lido em conjunto com esses outros instrumentos

### Etapa 6 — Acompanhamento das boas práticas de commit e issues
- Reforçar com o grupo que cada aluno deve usar sua própria conta do GitHub, nunca um único integrante subindo commits em nome de todos
- Orientar mensagens de commit objetivas (ex.: "Adiciona leitura de temperatura via DHT22" como padrão de referência) e a preferência por commits pequenos e frequentes em vez de um único commit grande no fim
- Verificar periodicamente, pela aba "Insights > Contributors" do repositório, se a distribuição de commits e issues concluídas está equilibrada entre os integrantes, sinalizando ao grupo qualquer integrante com pouca ou nenhuma issue atribuída

### Etapa 7 — Preparação final para a Feira EPA
- Organizar, junto ao grupo, o roteiro de apresentação da maquete (o que cada integrante vai explicar, incluindo a demonstração ao vivo das 3 chaves gangorra e a leitura do dashboard)
- Confirmar com a escola os detalhes logísticos do dia do evento (tomada disponível para alimentação via USB, espaço físico da maquete, acesso à rede Wi-Fi)
- Realizar, com o grupo, ao menos um ensaio completo da demonstração antes da data oficial, incluindo a checagem cruzada com os checklists de montagem, Back-End, Front-End e QA

## Checklist Final de Gestão

| Item verificado | Esperado |
|---|---|
| Repositório GitHub criado, com todos os 6 integrantes como colaboradores | Sim |
| GitHub Project configurado com as 5 colunas e as labels por frente | Sim |
| Todas as tarefas relevantes registradas como issues, com responsável definido | Sim |
| Materiais patrocinados solicitados e recebidos, conferidos contra a lista do escopo | Sim |
| Documentação de escopo consolidada mantida organizada e atualizada na pasta `/docs` | Sim |
| Fontes de pesquisa citadas conforme normas ABNT no material final | Sim |
| Distribuição de commits/issues equilibrada entre os integrantes | Sim |
| Ponto de contato com professores mantido atualizado ao longo do projeto | Sim |
| Roteiro de apresentação e logística da Feira EPA definidos com antecedência | Sim |
| Ao menos um ensaio completo realizado antes do evento | Sim |

## Observações

- Este planejamento segue diretamente o Guia de Configuração do GitHub Project fornecido pela Etec para os projetos VivEtec, adaptando-o à realidade específica das 4 frentes de trabalho do grupo (Back-End, Front-End, Gestor e QA).
- O Gestor não substitui a responsabilidade técnica de cada frente — sua função é garantir visibilidade, rastreabilidade e cumprimento de prazos, não executar as tarefas técnicas em si.
- Este planejamento pode ser adaptado conforme o andamento real do projeto e eventuais orientações adicionais dos professores responsáveis pela avaliação.
