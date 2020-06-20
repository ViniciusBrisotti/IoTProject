# Irrigação Automatizada
Projeto Irrigação Automatizada, referente a matéria de Objetos Inteligentes

Objetivo do projeto:

Sistema de irrigação automática que idêntifica, com base na umidade do solo, a necessidade de liverar um fluxo de água no solo. Quando a umidade encontra-se de acordo com o um solo saudáve para o desenvolvimento da planta o fluxo de água é sessado. 
Tanto este monitoramente quanto as atividades recorrentes do mesmo (irrigar ou sessar a água) são, via conexão com Broker utilizando o protocolo MQTT, ao celular do usuário enviadas.

-Métodos utilizados e suas respectivas descrições:

void Wifi_INICIANDO() 
Realiza conxão com a rede Wifi determinada.

void WIFI_Reconectando()
Reconectar-se ao WiFi

void MQTT_INICIANDO()
inicializa parâmetros de conexão MQTT(endereço do broker, porta e seta função de callback)

void Callback_MQTT(char* topico, byte* payload, unsigned int length)
Objetivo da função: função de callback - esta função é chamada toda vez que uma informação de um dos tópicos subescritos chega

void reconnectMQTT()
Reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair) em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.

void Verificando_Conexoes(void)
Verifica o estado das conexões WiFI e ao broker MQTT.Em caso de desconexão (qualquer uma das duas), a conexão é refeita.

float LEITURA_UMIDADE_SOLO(void)
Realiza a leitura do nível de umidade

void setup()
Realiza o setup

void loop()
Loop principal do código

- Componentes de Hardware utilizados:

NodeMCU Esp 8266 
Apresenta-se como hardware base o micro controlador NodeMCU Esp 8266. Com este hardware será possível controlar a comunicação com o serviço de MQTT e Shields. É um System-On-Chip com Wi-Fi embutido. Tem conectores GPIO, barramentos I2C, SPI, UART, entrada ADC, saída PWM e sensor interno de temperatura. CPU que opera em 80MHz, com possibilidade de operar em 160MHz. Arquitetura RISC de 32 bits. 32KBytes de RAM para instruções. 96KBytes de RAM para dados. 64KBytes de ROM para boot. Possui uma memória Flash SPI Winbond W25Q40BVNIG de 512KBytes. O núcleo é baseado no IP Diamand Standard LX3 da Tensilica. Fabricado pela Espressif

Diodo de uso geral
É um componente que permite a passagem de corrente somente quando está diretamente polarizado. Assim, o seu sentido de condução é marcado por uma listra cinza em seu corpo, de forma que a corrente deve fluir do lado preto para a lista cinza. Portanto, o terminal negativo da fonte deve estar ligado a listra cinza do diodo para que ele esteja diretamente polarizado. Neste projeto é o Diodo utilizado apresenta as seguintes características: Vrrm (tensão inversa repetitiva) = 100 V (Max); Vr (tensão contínua inversa máxima) 100 V; IF (Corrente direta máxima) = 200 mA; Ptot (dissipação total máxima) = 500 mW.

Transistor BC548
O BC548 é um transistor simples e de uso geral na eletrônica. É muito utilizado em aplicações de chaveamento e amplificação. Suas principais características são: NPN, máxima tensão de coletor: 30v, máxima corrente de coletor: 100mA, ganho(hfe): 100-800.

Valvula Solenoide
A valvula solenoide é utilizada para fechar, dosar, distribuir ou misturar o fluxo de gás ou líquido. Seu funcionamento é baseado em uma bobina (solenoide), que ao ser energizada gera um campo eletromagnética responsável por movimentar o êmbolo da válvula, possuindo as seguintes características:
Tensão de operação: 120V DC, termoplástico, aço zincado, borracha, abertura de entradas: ¾ polegada, saída: ¾ polegada.

Módulo Relé
O Módulo relé  permite controlar a Solenoi, tensão de operação, possuindo as seguintes características:  	
Modelo: SRD-03VDC-SL-C(Datasheet); Tensão da bobina: 3 a 3.3VDC; Capacidade do relé (AC): 125VAC/10A / 250VAC/10A; Resistência do contato: 100mO; Tempo de resposta: 10ms; Expectativa de vida útil: 10.000.000 operações; Quantidade de pinos: 5; Dimensões: 19mm(L) X 16mm(A) X 15mm(C); Peso: 2g

Sensor de umidade de solo
Este sensor realiza a leitura da umidade do solo onde estiver instalado e informar ao microcontrolador. Quando o solo está seco a saída do sensor fica em estado alto / nível lógico 1, e quando o solo está úmido a saída do sensor fica em estado baixo / nível lógico 0. Possui as seguintes características: Tensão de Operação: 3,3-5V; Sensibilidade ajustável via potenciômetro; Saída Digital e Analógica; Led indicador para tensão (vermelho); Led indicador para saída digital (verde); Comparador LM393; Dimensões PCB: 3x1,5 cm; Dimensões Sonda: 6x2 cm; Pinagem: VCC: 3,3-5v ; GND: GND; D0: Saída Digital; A0: Saída analógica.

Resistor
Resistores são dispositivos que compõem circuitos elétricos diversos, a sua finalidade básica é a conversão de energia elétrica em energia térmica (Efeito Joule). Outra função dos resistors é a possibilidade de alterar a diferença de potencial em determinada parte do circuito, isso ocorre por conta da diminuição da corrente elétrica devido à presença do equipamento. Os resistors presents neste projeto possuem as seguintes características quanto a voltage suportada: Resistência: 10K, 2,2K e 100OMS		

Placa de Ensaio
A Placa de Ensaio é um componente feito, em sua maior parte, de material plástico. Esta ferramenta permite a montagem e manipulação de circuitos eletrônicos.
A Placa de Ensaio permite que as conexões sejam realizadas e testadas sem que, necessariamente, precise-se soldar os componentes. Logo, há uma maior praticidade quanto a alterações. Por dentro da Placa de Ensaio há uma matriz de contato e barramentos metálicos em paralelo.
Neste projeto foram utilizadas 2 Placas de Ensaio: uma de 400 furos e outra de 630 furos.

- Métodos e sua descrição:

Os sensores de temperatura estarão em contato com o solo e enviarão informações acerca de suas condições de temperatura levando o software a concluir, por condições na programação pré-definidas, se o solo encontra-se molhado ou ressecado e, com base nisto, realizará a ação de permitir a abertura da válvula Solenoide para que o fluxo de água ocorra ou manter a válvula Solenoide fechada, impedindo o fluxo de água. Mensagens informativas acerca das condições do solo e ações pelo software tomadas serão, em caráter de monitoramento, á plataforma de broker Mosquitto enviadas afim de que os dados possam ser pelo usuário analisadas e o cuidado com o solo supervisionado. Para tal comunicação a placa NodeMCU Esp8266 enviará, via protocolo Broker MQTT, as informações para a plataforma Mosquitto, possibilitando então a apresentação dos dados no aparelho celular através do aplicativo IoT MQTT Panel.




