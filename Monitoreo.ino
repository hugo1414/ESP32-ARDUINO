#include <WiFi.h>

const char *ssid = "WirelessDS";
const char *password = "052927799@ha06";
String trama = "";
String cabecera = "0";
long V_panel;
long I_panel;
long V_bateria;
long I_bateria;

int Zero1 = 3010;
int Zero2 = 3010; 
long Sensor1 = 0;
long Sensor2 = 0;
long Sensor3 = 0;
long Sensor4 = 0;

const int sensorIn1 = 34;      // pin where the OUT pin from sensor is connected on Arduino
const int sensorIn2 = 35;      // pin where the OUT pin from sensor is connected on Arduino
const int sensorVn1 = 33;
const int sensorVn2 = 32;


const IPAddress serverIP(192,168,0,140); // La dirección que desea visitar
uint16_t serverPort = 4444;         // Número de puerto del servidor

WiFiClient client; // Declarar un objeto cliente para conectarse al servidor

void setup()
{
    Serial.begin(115200);
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); // Desactiva la suspensión de wifi en modo STA para mejorar la velocidad de respuesta
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());
}

void loop()
{
    //Lectura sensores
      /////////////////////// LECTURA DE SENSOR DE CORRIENTE 1 /////////////////////////
    for(int i = 0; i < 1000; i++){ 
    Sensor1 += analogRead(sensorIn1);
    }
    Sensor1 /= 1000; // promedio.
    float Intensidad1 = (5*(float)(Sensor1-Zero1)/1024.0)*1000/66; // la condición es
    // que el valor de Zero sea el que lee el sensor.
    Serial.print("Intensidad: "); 
    Serial.print(Intensidad1); 
    Serial.println(" A");

    /////////////////////// LECTURA DE SENSOR DE CORRIENTE 2 /////////////////////////
    for(int i = 0; i < 1000; i++){ 
    Sensor2 += analogRead(sensorIn2);
    }
    Sensor2 /= 1000; // promedio
    float Intensidad2 = (0.9*(float)(Sensor2-Zero2)/1024.0)*1000/66; // la condición es
    // que el valor de Zero sea el que lee el sensor.
    Serial.print("Intensidad: "); 
    Serial.print(Intensidad2); 
    Serial.println(" A");
    
    /////////////////////// LECTURA DE SENSOR DE VOLTAJE 1 /////////////////////////
    for(int i = 0; i < 1000; i++){ 
    Sensor3 += analogRead(sensorVn1);
    }
    Sensor3 /= 1000; // promedio
    float voltaje1 =  (float)4.4*Sensor3/1023;     
    Serial.print("Voltaje: "); 
    Serial.print(voltaje1); 
    Serial.println(" V");    

    /////////////////////// LECTURA DE SENSOR DE VOLTAJE 2 /////////////////////////
    for(int i = 0; i < 1000; i++){ 
    Sensor4 += analogRead(sensorVn2);
    }
    Sensor4 /= 1000; // promedio
    float voltaje2 =  (float)4.35*Sensor4/1023;     
    Serial.print("Voltaje: "); 
    Serial.print(voltaje2); 
    Serial.println(" V");   
    
    Serial.println("Intenta acceder al servidor");
    if (client.connect(serverIP, serverPort)) // Intenta acceder a la dirección de destino
    {
        Serial.println("Visita exitosa");
        V_panel = random(1,14);   
        I_panel = random(0,5);               
        V_bateria = random(1,14);  
        I_bateria = random(0,5);      
        String SV_panel = (String) voltaje1;//voltaje1;  V_panel
        String SI_panel = (String) Intensidad1;//Intensidad1;   I_panel             
        String SV_bateria = (String) voltaje2;//voltaje2; V_bateria;
        String SI_bateria = (String) Intensidad2;//Intensidad2; I_bateria
  
        // TRAMA = |Voltaje panel|Corriente panel|Voltaje bateria|Corriente bateria|
        //trama = cabecera+","+SV_panel+","+SI_panel+","+SV_bateria+","+SI_bateria;
        trama = cabecera+","+SV_panel+","+SI_panel+","+SV_bateria+","+SI_bateria;
        client.print(trama);                    // Enviar datos al servidor
        while (client.connected() || client.available()) // Si está conectado o se han recibido datos no leídos
        {
            if (client.available()) // Si hay datos para leer
            {
                String line = client.readStringUntil('\n'); // Leer datos a nueva línea
                Serial.print("Leer datos:");
                Serial.println(line);
                client.write(line.c_str()); // Devuelve los datos recibidos
            }
        }
        Serial.println("Cerrar la conexión actual");
        client.stop(); // Cerrar el cliente
    }
    else
    {
        Serial.println("Acceso fallido");
        client.stop(); // Cerrar el cliente
    }
    delay(1000);
}
