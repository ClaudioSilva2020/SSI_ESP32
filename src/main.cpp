// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>



//Semáforo de cima tensão alternada
#define SEMAC_1_R 15
#define SEMAC_1_Y 2
#define SEMAC_1_G 4
#define SEMAC_1_ER 16
//Semáforo de baixo tensão alternada
#define SEMAC_2_R 17
#define SEMAC_2_Y 5
#define SEMAC_2_G 18
#define SEMAC_2_ER 19

//Semáforo de cima tensão contínua
#define SEMDC_1_ER 13
#define SEMDC_1_R 12
#define SEMDC_1_Y 14
#define SEMDC_1_G 27
//Semáforo de baixo tensão contínua
#define SEMDC_2_ER 26
#define SEMDC_2_R 25
#define SEMDC_2_Y 33
#define SEMDC_2_G 32

uint32_t  gpios_semac[8] = {SEMAC_1_R, SEMAC_1_Y, SEMAC_1_G, SEMAC_1_ER, SEMAC_2_R, SEMAC_2_Y,SEMAC_2_G, SEMAC_2_ER};
uint32_t  gpios_semdc[8] = {SEMDC_1_R,SEMDC_1_Y,SEMDC_1_G, SEMDC_1_ER, SEMDC_2_R,SEMDC_2_Y,SEMDC_2_G, SEMDC_2_ER};

#define GPIO_OUTPUT_PIN_SELAC ((1ULL<<SEMAC_1_R)|(1ULL<<SEMAC_1_Y)|(1ULL<<SEMAC_1_G)|(1ULL<<SEMAC_1_ER)|(1ULL<<SEMAC_2_R)|(1ULL<SEMAC_2_Y)|(1ULL<<SEMAC_2_G)|(1ULL<<SEMAC_2_ER))
#define GPIO_OUTPUT_PIN_SELDC ((1ULL<<SEMDC_1_R)|(1ULL<<SEMDC_1_Y)|(1ULL<<SEMDC_1_G)|(1ULL<<SEMDC_1_ER)|(1ULL<<SEMDC_2_R)|(1ULL<<SEMDC_2_Y)|(1ULL<<SEMDC_2_G)|(1ULL<<SEMDC_2_ER))
#define GPIO_OUTPUT_PIN_SELACDC (GPIO_OUTPUT_PIN_SELAC | GPIO_OUTPUT_PIN_SELDC)

// Replace with your network credentials
const char* ssid = "mITS-SSI";
const char* password = "mits@mobit";



typedef struct {

    int id;
    int temp_g;
    int temp_r;
    int temp_y;
    int time;
}Semaphores;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


  
static void rumSemafore(void *pvParameter)
{
  Semaphores *semaphore_t;
  semaphore_t = (Semaphores *) pvParameter;
  int a, b, c;

  a = semaphore_t->temp_g;
  b = semaphore_t->temp_y;
  c = semaphore_t->temp_r;
 
  if (semaphore_t->id==1)
  {
    Serial.println(semaphore_t->time);
    for (;;)
    {
      digitalWrite(SEMAC_1_G, 0);
      digitalWrite(SEMAC_1_R, 1);
      digitalWrite(SEMAC_1_Y, 1);
      Serial.println("VERDE 1");
      vTaskDelay(a *1000/portTICK_PERIOD_MS);
      
      Serial.println(a);

      digitalWrite(SEMAC_1_G, 1);
      digitalWrite(SEMAC_1_R, 1);
      digitalWrite(SEMAC_1_Y, 0);
      Serial.println("AMARELO 1");
      vTaskDelay(b *1000/portTICK_PERIOD_MS);
      
      Serial.println(b);

      digitalWrite(SEMAC_1_G, 1);
      digitalWrite(SEMAC_1_R, 1);
      digitalWrite(SEMAC_1_Y, 0);
      Serial.println("VERMELHO 1");
      vTaskDelay(c *1000/portTICK_PERIOD_MS);
      Serial.println(c);
      
    }
    
  }

  if (semaphore_t->id==2)
  {
    for (;;)
    {
      digitalWrite(SEMAC_2_G, 0);
      digitalWrite(SEMAC_2_R, 1);
      digitalWrite(SEMAC_2_Y, 1);
      Serial.println("VERDE 2");
      vTaskDelay(a *1000/portTICK_PERIOD_MS);
      Serial.println(a);

      digitalWrite(SEMAC_2_G, 1);
      digitalWrite(SEMAC_2_R, 1);
      digitalWrite(SEMAC_2_Y, 0);
      Serial.println("AMARELO 2");
      vTaskDelay(b *1000/portTICK_PERIOD_MS);
      Serial.println(b);

      digitalWrite(SEMAC_2_G, 1);
      digitalWrite(SEMAC_2_R, 1);
      digitalWrite(SEMAC_2_Y, 0);
      Serial.println("VERMELHO 2");
      vTaskDelay(c *1000/portTICK_PERIOD_MS);
      Serial.println(c);
    }
    
    
  }

  if (semaphore_t->id==3)
  {
    digitalWrite(SEMDC_1_G, 0);
    digitalWrite(SEMDC_1_R, 1);
    digitalWrite(SEMDC_1_Y, 1);
    delay(semaphore_t->temp_g*1000);
    Serial.println("VERDE 3");

    digitalWrite(SEMDC_1_G, 1);
    digitalWrite(SEMDC_1_R, 1);
    digitalWrite(SEMDC_1_Y, 0);
    delay(semaphore_t->temp_g*1000);
    Serial.println("AMARELO 3");

    digitalWrite(SEMDC_1_G, 1);
    digitalWrite(SEMDC_1_R, 1);
    digitalWrite(SEMDC_1_Y, 0);
    delay(semaphore_t->temp_g*1000);
    Serial.println("VERMELHO 3");

  }

  if (semaphore_t->id==4)
  {
    digitalWrite(SEMDC_2_G, 0);
    digitalWrite(SEMDC_2_R, 1);
    digitalWrite(SEMDC_2_Y, 1);
    vTaskDelay(semaphore_t->temp_g *1000/portTICK_PERIOD_MS);
    Serial.println("VERDE 4");

    digitalWrite(SEMDC_2_G, 1);
    digitalWrite(SEMDC_2_R, 1);
    digitalWrite(SEMDC_2_Y, 0);
     vTaskDelay(semaphore_t->temp_y *1000/portTICK_PERIOD_MS);
    Serial.println("AMARELO 4");

    digitalWrite(SEMDC_2_G, 1);
    digitalWrite(SEMDC_2_R, 1);
    digitalWrite(SEMDC_2_Y, 0);
    vTaskDelay(semaphore_t->temp_r *1000/portTICK_PERIOD_MS);
    Serial.println("VERMELHO 4");

  }
  vTaskDelay(portTICK_PERIOD_MS);
}


 
void setup(){
  // Serial port for debugging purposes
  
  
  Serial.begin(115200);
  for (size_t i = 0; i<8 ;i++)
  {
    pinMode(gpios_semdc[i], OUTPUT);
    pinMode(gpios_semac[i], OUTPUT);
  }
      // set all pin in logic level high
  for (size_t i = 0; i < 8; i++)
  {
    digitalWrite(gpios_semac[i],1);
    digitalWrite(gpios_semdc[i],1);
  }


  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  /*WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
   {
      delay(1000);
      Serial.println("Connecting to WiFi..");
   }*/
  WiFi.softAP(ssid, password);
  
  //Serial.println(WiFi.localIP());
  // Print ESP32 Local IP Address
  Serial.println("IP ADREES");
  Serial.println(WiFi.softAPIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request){
     Semaphores sen[4];

      String vd = request->getParam("tgreen1")->value();
      String vm = request->getParam("tred1")->value();
      String am = request->getParam("tyellow1")->value();
    
       
      String vd1 = request->getParam("tgreen2")->value();
      String vm1 = request->getParam("tred2")->value();
      String am1 = request->getParam("tyellow2")->value();
      


      

      
        sen[0].temp_g = vd.toInt();
        sen[0].temp_r = vm.toInt();
        sen[0].temp_y = am.toInt();
        sen[0].id = 1;
        
        xTaskCreate(rumSemafore, "rumSemafore", 4024*5, (void *) &sen[0],  12, NULL);           
      

      
        sen[1].temp_g = vd.toInt();
        sen[1].temp_r = vm.toInt();
        sen[1].temp_y = am.toInt();
        sen[1].id = 2;
        xTaskCreate(rumSemafore, "rumSemafore", 4024*5, (void *) &sen[1],  12, NULL);           
      
     

   request->send(SPIFFS, "/index.html", "text/html");
   request->send(SPIFFS, "/style.css", "text/css");
  
  });
  // Start server
  server.begin();
}
 
void loop()
{
  
    
}