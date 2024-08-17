/*
 *   Q0000
 *   AUTOR:   BrincandoComIdeias
 *   LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/
 *   COMPRE:  https://www.arducore.com.br/
 *   SKETCH:  Módulo para Arduino HX711
 *   DATA:    04/07/2019
*/

// INCLUSÃO DE BIBLIOTECAS
#include <HX711.h>
#include <PushButton.h>

#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite funcoes = TFT_eSprite(&tft);
TFT_eSprite stack = TFT_eSprite(&tft);


// DEFINIÇÕES DE PINOS
#define pinDT  13
#define pinSCK  27

#define pinBotaoRESET 25

#define pinBotaoSTART 33

// DEFINIÇÕES
#define pesoMin 0.010
#define pesoMax 30.0

#define escala 460000

// INSTANCIANDO OBJETOS
HX711 scale;
PushButton botao_start(pinBotaoSTART);
PushButton botao_reset(pinBotaoRESET);

// DECLARAÇÃO DE VARIÁVEIS  
float medida=0;

void setup() {
  Serial.begin(9600);

  scale.begin(pinDT, pinSCK); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.set_scale(escala); // ENVIANDO O VALOR DA ESCALA CALIBRADO

  delay(2000);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA
  Serial.println("Setup Finalizado!");

   tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true);


  stack.setColorDepth(8);
  stack.createSprite(100, 240);  
 
}

void loop() {
  botao_start.button_loop(); // LEITURA DO BOTAO

  botao_reset.button_loop();

  tft.fillScreen(TFT_WHITE);
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN);
  tft.setTextDatum(TC_DATUM);
  tft.drawCentreString("PRESS GREEN", 160, 80, 4);
  tft.drawCentreString("2", 160, 120, 4);
  tft.drawCentreString("START", 160, 160, 4);

  if(botao_start.pressed()){ // QUANDO PRESSIONAR O BOTÃO


  while (true)
  {
    if(botao_reset.pressed()){
      break;
    }
    /* code */
    scale.power_up(); // LIGANDO O SENSOR
    
    medida = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
    
    if (medida <= pesoMin ){ // CONFERE SE A MASSA ESTÁ NA FAIXA VÁLIDA
      scale.tare(); // ZERA A BALANÇA CASO A MASSA SEJA MENOR QUE O VALOR MIN
      medida = 0;
      Serial.println("Tara Configurada! mim ");
    } else if ( medida >= pesoMax ){
      scale.tare(); // ZERA A BALANÇA CASO A MASSA SEJA MAIOR QUE O VALOR MÁX
      medida = 0;
      Serial.println("Tara Configurada! max");
    } 
        tft.fillScreen(TFT_WHITE);
        tft.setTextSize(4);
        tft.setTextColor(TFT_RED);
        tft.setTextDatum(TC_DATUM);
        String numberString = String(medida);
        tft.drawCentreString(numberString, 160, 80, 4);
        delay(50);
      Serial.println(medida,3);
    

    scale.power_down(); // DESLIGANDO O SENSOR
  }
  
    
  }
  
}

// IMPLEMENTO DE FUNÇÕES