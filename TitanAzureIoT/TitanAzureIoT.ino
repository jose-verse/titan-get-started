/*
  EXAMPLE_SEND_DATA_AZUREIOT_WITH_TITAN.
 
  modified 18 February 2019.
  by José Alfredo Rey Méndez.
  VERSE Technology.
*/

//POST //https://{your-iothub-name}.azure-devices.net/devices/{your-device}/messages/events?api-version={api-version}

String host = "";                                                             //Hostname, {your-iothub-name}.azure-devices.net
String deviceid = "";                                                         //Your device
String api = "2018-06-30";                                                    //API version
String SASToken = "";                                                         //SAS Token

String endpoint = "/devices/"+deviceid+"/messages/events?api-version="+api;

void setup() { 
  SerialUSB.begin(115200);      // (SERIAL - SIM),  (SERIAL1 -D0-D1)
  Serial.begin(115200);           // Initialize hardware serial port, pins 0/1
  pinMode(5, OUTPUT);           // PIN ON SIM5320
  SIM5320_ON();                 // ON SIM5320
  delay(10000);
  SETUP_SIM5320();              // START SIM5320
}
  

void loop() {
  WEB();                        // SEND JSON UBIDOTS
  delay(10000);                  // SAMPLE TIME
  }
  
void IMP(){

  if (Serial.available()) {   
      String toSend = ""; 
      while (Serial.available())  {   
        toSend += (char)Serial.read();
      }
      SerialUSB.print(toSend);
  } 
}

void WEB(){

  String temp1 = String(random(20, 41));
  String temp2 = String(random(101));
  String energy = String(random(50, 100));

  //sensors.requestTemperatures(); 
  //float temp = sensors.getTempCByIndex(0); 

  //JSON
  String telemetry = "{";
         telemetry +=      "\"iddevice\": \"TEST0001\",";
         telemetry +=      "\"temp1\": "+temp1+",";
         telemetry +=      "\"temp2\": "+temp2+",";
         telemetry +=      "\"energy\": "+energy;
         telemetry += "}";

  //REQUEST
  String request = "POST " + endpoint + " HTTP/1.1\r\n";
         request +="Host: " + host + "\r\n";
         request +="Authorization: " + SASToken + "\r\n";
         request +="Content-Type: application/json;type=entry;charset=utf-8\r\n";
         request +="Content-Length: " + String(telemetry.length()) + "\r\n\r\n" + telemetry;
         
  delay(500);
  Serial.println("AT+CHTTPSSTART");                         //START
  delay(500);
  IMP();
  Serial.println("AT+CHTTPSOPSE=\""+ host +"\", 443, 2");   //OPEN HTTP CONNECTION
  delay(4000);
  IMP();
  Serial.print("AT+CHTTPSSEND=");                           //PREPARE DATA TO SEND
  delay(100);
  IMP();
  Serial.println(request.length());                         //LENGTH OF REQUEST
  delay(500);
  IMP();
  //SEND REQUEST
  Serial.println(request);                                  //SEND REQUEST
  delay(2000);
  IMP();
  Serial.println("AT+CHTTPSSEND?");
  delay(2000);
  IMP();
  Serial.println("AT+CHTTPSRECV=150");                      //RECEIVE RESPONSE
  delay(1000);
  IMP();
  Serial.println("AT+CHTTPSRECV?");
  delay(1000);
  IMP();
  Serial.println("AT+CHTTPSCLSE");                          //CLOSE CONNECTION
  delay(1000);
  IMP();
  Serial.println("AT+CHTTPSSTOP");                          //STOP
  delay(1000);
  IMP();
    

}

void SIM5320_ON()          
{
  digitalWrite(5, HIGH);
  delay(2000);                            
  digitalWrite(5, LOW);
  delay(2000);
}

void SETUP_SIM5320()
{
  Serial.println("AT+CFUN=1");                      // SIM5320 WORKING WITH ALL THE FEATURES
  delay(250);
  IMP();
  Serial.println("AT+CNMP=2");                      // AUTOMATIC MODE
  delay(250);
  IMP();

  
  Serial.println("AT+CSQ");                         // SIGNAL LEVEL
  delay(250);
  IMP();
  Serial.println("AT+CREG?");          
  delay(250);
  IMP();
  Serial.println("AT+COPS?");
  delay(250);
  IMP();
  
  Serial.println("AT+CNSMOD?");                     // RED
  delay(250);
  IMP();
  Serial.println("AT+CSOCKSETPN=1");
  delay(250);
  IMP();
  Serial.println("AT+CIPMODE=0");
  delay(250);
  IMP();
  Serial.println("AT+CNBP?");
  delay(250);
  IMP();
  Serial.println("AT+NETOPEN");
  delay(8000);
  IMP();
  Serial.println("AT+IPADDR");                       // IP
  delay(500);
  IMP();
  Serial.println("AT+CGSN");                         // IMEI
  delay(500);
  IMP();
}
