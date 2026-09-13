# 2. Introdução — Contexto e Objetivo do Projeto

**Projeto:** Maquete com ESP32 para identificação de postes de luz quebrados, queimados ou acesos em horário errado
**Finalidade:** Feira EPA (Etec de Portas Abertas) — Etec de Boituva
**Uso deste documento:** Base de contexto e objetivo para o escopo do projeto (referência interna do grupo)

---

## 2.1 Contextualização do Problema

### Qual problema foi identificado
Postes de iluminação pública que ficam quebrados, queimados ou acesos em horário errado (piscando ou ligados de dia), sem detecção rápida do defeito.

### Onde ocorre
A manutenção da iluminação pública no Brasil é, desde 2015, responsabilidade das prefeituras municipais (as distribuidoras de energia só fornecem a energia elétrica para o sistema). Isso significa que reparos de lâmpadas queimadas, luminárias danificadas ou postes com defeito dependem quase sempre de reclamações registradas pelos próprios moradores — por telefone, aplicativos (como o Poupatempo SP ou portais como o Carioca Digital) ou atendimento presencial.

Reportagens e relatos de moradores mostram um padrão recorrente: postes ficam queimados por longos períodos, luminárias apresentam falhas de fiação que fazem a luz acender em horários incorretos (ou piscar), e postes danificados por acidentes (como colisões de veículos) às vezes nem chegam a ser repostos. Em um caso relatado em Brasília, doze postes de um mesmo bairro permaneceram apagados enquanto moradores tentavam, sem sucesso, que a concessionária e a prefeitura resolvessem o problema.

### Quem é afetado
- **Pedestres e moradores**, que precisam circular por ruas e calçadas escuras à noite;
- **Comerciantes locais**, cujo movimento noturno cai em áreas mal iluminadas;
- **Motoristas**, expostos a maior risco de acidentes em vias sem iluminação adequada;
- **Prefeituras**, que arcam com custos de manutenção reativa (mais cara e lenta que a manutenção preventiva) e recebem reclamações constantes dos cidadãos.

### Consequências do problema
- **Segurança pública:** vias mal iluminadas favorecem furtos, roubos e outros crimes, e aumentam a sensação de insegurança da população;
- **Segurança viária:** estudos apontam redução expressiva de acidentes de trânsito em vias bem iluminadas, já que a iluminação melhora a visibilidade de motoristas e pedestres;
- **Econômicas:** o poder público continua pagando pela conta de energia mesmo quando o ponto de luz está inativo, e a manutenção corretiva (após a falha já ter ocorrido) é mais cara do que a manutenção preventiva;
- **Sociais/mobilidade urbana:** ruas escuras desestimulam o uso de espaços públicos à noite (praças, calçadas, pontos de ônibus), reduzindo a circulação de pessoas e o comércio local.

---

## 2.2 Justificativa

### Por que o problema precisa ser resolvido
Porque hoje a correção depende quase sempre de alguém perceber o defeito e denunciar — muitos postes continuam quebrados por falta de reclamação, prolongando os riscos de segurança e o desperdício de energia.

### Relevância social e econômica
Diversos estudos acadêmicos brasileiros relacionam a qualidade da iluminação pública à criminalidade. Uma pesquisa baseada em dados da PNAD Contínua (2021) encontrou associação entre melhoria da iluminação pública e redução de furtos e roubos a residências, atribuída ao aumento da vigilância informal proporcionada por ruas mais visíveis. Outros trabalhos destacam que a iluminação adequada dificulta a fuga e o "esconderijo" de criminosos, e que a presença de pessoas circulando à noite — os chamados "olhos da rua" — só acontece quando o ambiente transmite segurança.

Um experimento controlado da polícia de Nova York (citado em fontes do setor) mostrou redução de 36% a 60% em crimes noturnos em ruas que receberam reforço de iluminação. No Brasil, reportagens do setor apontam reduções de até 30% em ocorrências criminais em áreas com modernização da iluminação.

Além da segurança, a falta de manutenção gera um ciclo de desperdício: a população paga pela iluminação pública na conta de luz independentemente de o serviço funcionar, e falhas não identificadas continuam consumindo energia (ou deixando de gerar o benefício esperado) até que alguém denuncie o problema manualmente. Do ponto de vista social, iluminação adequada aumenta a sensação de segurança e incentiva o uso de espaços públicos à noite; do ponto de vista econômico, a detecção mais rápida de falhas evita gasto de energia com pontos com defeito e reduz o custo de manutenção corretiva.

### Soluções atuais e suas limitações

**Denúncia manual:** a forma mais comum de solução hoje é reativa — o cidadão identifica o poste com defeito e registra uma solicitação via aplicativo da prefeitura, central telefônica (0800) ou atendimento presencial. É simples, mas lenta, e depende do cidadão notar e reportar o problema; muitos postes continuam apagados por falta de reclamação, e a burocracia entre concessionária e prefeitura (comum desde a transferência dos ativos de iluminação em 2015) costuma gerar o chamado "jogo de empurra" relatado por moradores.

**Telegestão e iluminação pública inteligente (IoT comercial):** cidades maiores e mais estruturadas vêm adotando sistemas de telegestão — luminárias conectadas por redes como LoRaWAN, NB-IoT, Zigbee ou redes celulares, monitoradas por uma plataforma central, permitindo detecção automática de falhas, ajuste remoto de intensidade (dimerização, com economia de energia estimada entre 40% e 45%) e medição de consumo, corrente e tensão de cada ponto de luz. Exemplos citados incluem cidades como Barcelona, Copenhague e projetos-piloto em São Paulo. Apesar de eficiente, essa solução tem alto custo de implantação, exige licitações complexas e contratos longos (mínimo legal de 5 anos), além de exigir homologação pelo Inmetro para sistemas de medição de consumo — o que a torna inviável para pequena escala ou municípios com orçamento limitado. Além disso, muitas soluções de mercado priorizam a dimerização e a economia de energia, sem focar especificamente na detecção simples e de baixo custo de falhas físicas (poste quebrado, queimado ou piscando).

Essas duas soluções deixam uma lacuna entre "barato e lento" (denúncia manual) e "eficiente e caro" (telegestão comercial), que justifica uma alternativa de detecção automática e baixo custo — a proposta central deste projeto.

Um sistema com ESP32 conectado à internet via Wi-Fi é tecnicamente adequado para preencher essa lacuna, pois tem custo baixo comparado às soluções comerciais de telegestão, já possui conectividade Wi-Fi integrada (dispensando módulos extras de rede), permite identificar automaticamente as três situações de falha do escopo do projeto (poste apagado/quebrado, queimado e aceso em horário errado) e é simples o suficiente para ser compreendido, montado e apresentado por estudantes do Ensino Médio.

---

## 2.3 Objetivo do Projeto

### Objetivo geral
Desenvolver, em uma maquete com ESP32, um sistema de baixo custo que identifique automaticamente postes de iluminação pública com defeito (apagados, queimados ou acesos em horário errado) e notifique o responsável pela manutenção, sem depender de denúncia manual.

### Objetivos específicos
1. Desenvolver um sistema de monitoramento de postes de iluminação pública utilizando ESP32 e sensores LDR, capaz de identificar automaticamente falhas (poste apagado, queimado ou aceso fora do horário correto) sem intervenção humana;
2. Reduzir o tempo de detecção de falhas de dias/semanas (tempo médio de um modelo reativo, dependente de denúncia do morador) para poucos segundos, através da comparação contínua entre a leitura do LDR e o horário sincronizado via protocolo NTP;
3. Notificar automaticamente o responsável pela manutenção em tempo real, via bot do Telegram, eliminando a necessidade de o cidadão registrar uma reclamação manual para que o reparo seja iniciado;
4. Viabilizar uma alternativa de baixo custo (em torno de R$70-90 em componentes) à telegestão comercial de iluminação pública, que exige investimentos altos e contratos de longo prazo, tornando a detecção automática de falhas acessível também a municípios ou contextos com orçamento limitado;
5. Disponibilizar um histórico de dados de luminosidade de cada poste, via ThingSpeak, permitindo à equipe de manutenção visualizar padrões de funcionamento ao longo do tempo e apoiar decisões de manutenção preventiva, além da simples notificação pontual de falha;
6. Simular, na maquete, três postes de luz monitorados individualmente por sensores conectados ao ESP32;
7. Detectar automaticamente quando um poste tem falha, simulada fisicamente por um botão push-button que interrompe o circuito do LED correspondente;
8. Apresentar o funcionamento do sistema de forma clara e acessível ao público da Feira EPA.

---

## Fontes consultadas

- Jornal de Brasília — Escuridão na Asa Sul: moradores denunciam postes queimados. Disponível em: https://jornaldebrasilia.com.br/brasilia/escuridao-na-asa-sul-moradores-denunciam-postes-de-iluminacao-publica-queimados/
- Portal Carioca Digital — Notificar problemas com iluminação pública. Disponível em: https://carioca.rio/sistema/solicitacoes-on-line/?atividades=notificar-problemas-com-iluminacao-publica
- CPFL — Reparos de Iluminação Pública. Disponível em: https://cpfl.com.br/reparos-de-iluminacao-publica
- Portal de Serviços do Governo de SP — Solicitar reparo na iluminação pública. Disponível em: https://servicos.sp.gov.br/fcarta/3036ABC8-2616-45E3-A02C-4C2CAA79B3D1
- Revista USP (PAAM) — A Influência da Iluminação Pública na Segurança Urbana Noturna. Disponível em: https://revistas.usp.br/paam/en/article/download/174975/176102/517467
- SciELO Colombia — Relação entre a iluminação pública e os crimes contra o patrimônio no Brasil. Disponível em: http://www.scielo.org.co/scielo.php?pid=S0120-63462025000104984&script=sci_abstract&tlng=pt
- CONCCEPAR — A importância da iluminação na vitalidade e mantimento do espaço público urbano. Disponível em: https://conccepar.grupointegrado.br/resumo/
- ENCAC/ANTAC — Análise da relação entre poluição luminosa e furtos/roubos em Vitória-ES. Disponível em: https://eventos.antac.org.br/encac/article/view/6760
- QLuz Palhoça — Entenda a relação entre iluminação pública e segurança. Disponível em: https://www.qluzpalhoca.com.br/entenda-a-relacao-entre-iluminacao-publica-e-seguranca/
- Exame — Iluminação pública: o vetor de eficiência fiscal e segurança. Disponível em: https://exame.com/bussola/iluminacao-publica-o-vetor-de-eficiencia-fiscal-e-seguranca/
- Ubidots — Infraestrutura IoT para a Cidade Inteligente Moderna. Disponível em: https://pt.ubidots.com/solutions/iot-infrastructure-for-the-modern-smart-city
- Futurecom Digital — Iluminação pública inteligente: IoT nas cidades inteligentes. Disponível em: https://digital.futurecom.com.br/transformaodigital/iluminacao-publica-inteligente-o-uso-de-iot-nas-cidades-inteligentes/
- SBA (CBA 2024) — Desenvolvimento de um Sistema IoT para o Monitoramento de Sistemas de Iluminação. Disponível em: https://www.sba.org.br/cba2024/papers/paper_632.pdf
- Alma IoT — IoT e Iluminação Urbana: Uma Perspectiva Sustentável e Tecnológica. Disponível em: https://almaiot.com.br/iluminacao-inteligente/
- Nexum IoT — Iluminação pública inteligente: como a tecnologia IoT pode transformar a iluminação pública. Disponível em: https://nexumiot.com.br/iluminacao-publica-inteligente-como-a-tecnologia-iot-pode-transformar-a-iluminacao-publica-como-conhecemos-hoje/
- Demape — Telegestão de iluminação pública: transforme sua cidade. Disponível em: https://demape.com.br/telegestao-de-iluminacao-publica-transforme-sua-cidade/
- Conjur — Telegestão na iluminação pública: o engodo da dimerização e a omissão da aprovação do Inmetro. Disponível em: https://www.conjur.com.br/2025-fev-21/telegestao-na-iluminacao-publica-o-engodo-da-dimerizacao-e-a-omissao-da-aprovacao-do-inmetro/
- AALOK — Principais desafios dos municípios com a iluminação pública. Disponível em: https://aalok.com.br/blog/iluminacao-publica/principais-desafios-dos-municipios-com-iluminacao-publica/
- M3E — Como o inventário cadastral de iluminação pública reduz custos e otimiza PPPs municipais. Disponível em: https://m3e.com.br/artigos/geotecnologias-e-inovacao/inventario-iluminacao-publica-ppps/

*Observação: este documento é material de apoio para orientar o escopo do projeto (contexto e objetivo). Ele reúne e organiza informações de fontes públicas — na redação do artigo/trabalho final, os textos devem ser reescritos com as próprias palavras do grupo e as fontes citadas conforme as normas ABNT exigidas pela atividade.*
