# 3.6 Fonte de Alimentação

## Fonte Escolhida: USB

O projeto utiliza alimentação via **cabo USB** (Micro-USB ou USB-C, dependendo do módulo específico do ESP32 DevKit V1 utilizado), conectado a uma fonte de 5V — seja um carregador de celular comum, seja a porta USB de um computador. O próprio ESP32 possui um regulador de tensão embutido na placa (responsável por converter os 5V da entrada USB para os 3,3V exigidos pelo SoC e pelos demais componentes do circuito), então nenhum componente adicional de regulagem é necessário.

## Justificativa Técnica

A escolha do USB em vez das demais opções do enunciado se justifica pelo contexto específico de uso do sistema — uma maquete estacionária para demonstração:

**Frente a bateria:** uma fonte de bateria (poder-banco ou pack de pilhas/baterias recarregáveis) seria a escolha adequada para uma instalação real de campo, onde não há tomada disponível em cada poste de rua. Entretanto, para a maquete, ela introduziria uma variável desnecessária: a necessidade de calcular a autonomia (mAh) suficiente para cobrir todo o tempo de demonstração na Feira EPA, o risco de a bateria descarregar durante a apresentação, e o cuidado extra de recarregá-la antes do evento. Como o local de demonstração conta com acesso a tomada, essa complexidade adicional não traz benefício real.

**Frente a fonte externa 5V/12V dedicada:** uma fonte de bancada ou um adaptador de parede genérico funcionaria tecnicamente, mas exigiria a compra de um componente extra e cuidado adicional com a compatibilidade de tensão/corrente — o cabo USB já citado cumpre exatamente essa mesma função (fornecer 5V estáveis), com a vantagem de ser o mesmo cabo já usado para programar o ESP32, eliminando a necessidade de qualquer acessório extra.

**Frente a painel solar + bateria:** essa é a opção mais alinhada a um cenário de instalação real de um poste de iluminação pública, que não conta com fiação elétrica de baixa tensão disponível nas proximidades. Para a maquete, no entanto, ela adicionaria complexidade de montagem (painel, controlador de carga, bateria) sem qualquer ganho prático dentro de um ambiente fechado e com tomada disponível, e ainda dependeria da incidência de luz solar, o que é incompatível com testes e demonstrações em ambiente interno.

A alimentação via USB é, portanto, a opção que minimiza pontos de falha, dispensa cálculos de autonomia e mantém o foco do projeto na validação da lógica de sensoriamento e comunicação — que é o objetivo central da demonstração.

## Consumo Estimado do Sistema

O consumo do sistema é determinado principalmente pelo ESP32 operando com o rádio Wi-Fi ativo continuamente, já que ele precisa manter o envio periódico dos dados ao ThingSpeak. Esse consumo fica na faixa de 80 mA a 160 mA em operação normal, com picos de até 260 mA durante transmissões de dados. Os LEDs consomem poucos miliamperes cada (limitados pelos resistores de 220 Ω), e o buzzer, quando acionado, adiciona um consumo pontual e breve, apenas nos momentos de alerta. Somando todos os componentes, o consumo total do sistema fica com folga dentro da capacidade de uma porta USB padrão (5V/500mA a 900mA) ou de um carregador de celular comum (5V/1A ou mais), confirmando que nenhuma fonte externa adicional é necessária para o funcionamento da maquete.

## Considerações para uma Aplicação Real

Vale registrar, como limitação e ponto de melhoria futura (retomado na seção 4.3), que a alimentação via USB é adequada apenas ao contexto de demonstração da maquete. Em uma instalação real em um poste de iluminação pública, a fonte de alimentação mais coerente seria um conjunto de painel solar com bateria de backup, já que postes de rua normalmente contam apenas com a fiação de alta tensão que alimenta a própria lâmpada, sem um circuito de baixa tensão dedicado e estável para alimentar continuamente um microcontrolador e seus sensores.
