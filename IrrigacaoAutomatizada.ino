#include <ESP8266WiFi.h>  
#include <PubSubClient.h> 
 
//defines - mapeamento de pinos do NodeMCU
#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1
  
//defines
#define SSID_REDE     "PALMEIRAS"  
#define SENHA_REDE    "Texas2020"  
#define INTERVALO_ENVIO_MQTT        3000  //intervalo entre envios de dados via MQTT (em ms)
#define LIMITE_UMIDADE_PARA_REGAR   42     //limite percentual inferior para a planta se auto regar
#define TEMPO_PARA_REGAR            2000   //tempo (em ms) ao qual a válvula de vazão solenóide de água será acionada quando for necessário regar a planta 
#define SAIDA_COMANDO_VALVULA       D0     //saída do NodeMCU que acionará a válvula de vazão solenóide de água
   
//defines de id mqtt e tópicos para publicação e subscribe
#define TOPICO_SUBSCRIBE_NAO_REGAR "SmartPlantaEnvia18101995NaoRegar" //tópico MQTT de escuta
#define TOPICO_PUBLISH   "SmartPlantaRecebe18101995"    //tópico MQTT de envio de informações para Broker
                                                 
#define ID_MQTT  "SmartPlanta18101995"     //id mqtt para identificar a sessão)
                                       
//PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
   
//constantes e variáveis globais
const char* BROKER_MQTT = "test.mosquitto.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883;                      // Porta do Broker MQTT
long lastConnectionTime; 
long lastMQTTSendTime;
WiFiClient client;
WiFiClient clientMQTT;
PubSubClient MQTT(clientMQTT); // Instancia o Cliente MQTT passando o objeto clientMQTT
   
//prototypes
float FazLeituraUmidade(void);
void initWiFi(void);
void initMQTT(void);
void reconectWiFi(void); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void); 

//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID_REDE);
    Serial.println("Aguarde");
       
    reconectWiFi();
}
    
//Função: inicializa parâmetros de conexão MQTT(endereço do 
//        broker, porta e seta função de callback)
//Parâmetros: nenhum
//Retorno: nenhum
void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
    
//Função: função de callback 
//        esta função é chamada toda vez que uma informação de 
//        um dos tópicos subescritos chega)
//Parâmetros: nenhum
//Retorno: nenhum
void callback(char* topic, byte* payload, unsigned int length) {
  char messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp = messageTemp + (char)payload[i];
  }
  Serial.println(messageTemp);

  if(messageTemp == char(1)) {
     digitalWrite(SAIDA_COMANDO_VALVULA,HIGH);  //abre a válvula de vazão solenóide de água
     delay(TEMPO_PARA_REGAR);
     digitalWrite(SAIDA_COMANDO_VALVULA,LOW);
  }
 
}  
  
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            //MQTT.subscribe(TOPICO_SUBSCRIBE_REGAR); 
            MQTT.subscribe("SmartPlantaEnvia18101995NaoRegar");
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
    
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
           
    WiFi.begin(SSID_REDE, SENHA_REDE); // Conecta na rede WI-FI
       
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
     
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID_REDE);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
   
//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
       
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
   
//Função: faz a leitura do nível de umidade
//Parâmetros: nenhum
//Retorno: umidade percentual (0-100)
float FazLeituraUmidade(void)
{
    int ValorADC;
    float UmidadePercentual;
   
     ValorADC = analogRead(0);   //418 -> 1.0V
     Serial.print("[Leitura ADC] ");
     Serial.println(ValorADC);
   
     //Quanto maior o numero lido do ADC, menor a umidade.
        
     UmidadePercentual = 100 * ((418-(float)ValorADC) / 418);
     Serial.print("[Umidade Percentual] ");
     Serial.print(UmidadePercentual);
     Serial.println("%");

     if(UmidadePercentual < 40){
      Serial.print("Solo Seco! Sendo molhado....");
     }
     else{
      Serial.print("Solo úmido! Não será mais molhado.");
     }
   
     return UmidadePercentual;
}
void setup()
{  
    Serial.begin(9600);
    lastConnectionTime = 0; 
    lastMQTTSendTime = 0;
    initWiFi();
    initMQTT();
    pinMode(SAIDA_COMANDO_VALVULA,OUTPUT);
    digitalWrite(SAIDA_COMANDO_VALVULA,LOW);  //por default, a válvula de vazão solenóide de água começa fechada    
    Serial.println("Planta IoT com ESP8266 NodeMCU");
   
}
   
//loop principal
void loop()
{
    
    float UmidadePercentualLida;
    int UmidadePercentualTruncada;
    char FieldUmidade[11];
    char MsgUmidadeMQTT[50];
    
    pinMode(SAIDA_COMANDO_VALVULA,OUTPUT);
    digitalWrite(SAIDA_COMANDO_VALVULA,LOW);  //Válvula inicia fechada
    
    VerificaConexoesWiFIEMQTT(); 
    
    UmidadePercentualLida = FazLeituraUmidade();
    UmidadePercentualTruncada = (int)UmidadePercentualLida; //umidade e número inteiro são trnvados
    
    //Decide se é a hora de enviar as informações
    if ((millis() - lastMQTTSendTime) > INTERVALO_ENVIO_MQTT)
        
    {
        if(UmidadePercentualTruncada < 40){
          sprintf(MsgUmidadeMQTT,"Umidade do solo: %d por cento. - Solo SECO! Sendo irrigado...",UmidadePercentualTruncada);
        }
        else{
          sprintf(MsgUmidadeMQTT,"Umidade do solo: %d por cento. - Solo ÚMIDO! Não será irrigado.",UmidadePercentualTruncada);
     }
        MQTT.publish(TOPICO_PUBLISH, MsgUmidadeMQTT);
        lastMQTTSendTime = millis();
    }
    
    
//    verifica se a planta deve ser regada
    if (UmidadePercentualTruncada < LIMITE_UMIDADE_PARA_REGAR)
    {
        //em caso positivo, a planta é regada no tempo contido no define TEMPO_PARA_REGAR
        digitalWrite(SAIDA_COMANDO_VALVULA,HIGH);  //abre a válvula de vazão solenóide de água
        delay(TEMPO_PARA_REGAR);
        digitalWrite(SAIDA_COMANDO_VALVULA,LOW);  //fecha a válvula de vazão solenóide de água
        
    }
    delay(1000);
}
