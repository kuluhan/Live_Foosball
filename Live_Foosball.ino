#include <Countdown.h>
#include <FP.h>
#include <IPStack.h>
#include <MQTTClient.h>
#include <MQTTConnect.h>
#include <MQTTFormat.h>
#include <MQTTLogging.h>
#include <MQTTPacket.h>
#include <MQTTPublish.h>
#include <MQTTSubscribe.h>
#include <MQTTUnsubscribe.h>
#include <StackTrace.h>
#include <WifiIPStack.h>

#include <hcsr04.h>
#define trigPin 13
#define echoPin 12
#define trigPin2 3
#define echoPin2 2
#define E 11
#define D 10
#define C 8
#define G 7
#define fer 6
#define A 5
#define B 4
#include <SPI.h>
#include <YunClient.h>
#include <IPStack.h>
#include <Countdown.h>
#include <MQTTClient.h>

//#include will be needed depending on your Ethernet shield type
#define MQTT_MAX_PACKET_SIZE 100
#define SIZE 100

#define MQTT_PORT 1883

#define PUBLISH_TOPIC "iot-2/evt/status/fmt/json"
#define SUBSCRIBE_TOPIC "iot-2/cmd/+/fmt/json"
#define AUTHMETHOD "use-token-auth"

#define CLIENT_ID "d:9eefm8:arduino:arduino1"
#define MS_PROXY "9eefm8.messaging.internetofthings.ibmcloud.com"
#define AUTHTOKEN "12345678"

YunClient c;
IPStack ipstack(c);

MQTT::Client<IPStack, Countdown, 100, 1> client = MQTT::Client<IPStack, Countdown, 100, 1>(ipstack);

String deviceEvent;
int score;
bool goal;
int score2;
bool goal2;
void sevenseg(int x);
void setup() {
  Bridge.begin();
  Console.begin();
  delay(1000);
  score=0;
  goal = 0;
  score2 = 0;
  goal2 = 0;
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode (A, OUTPUT);
  pinMode (B, OUTPUT);
  pinMode (C, OUTPUT);
  pinMode (D, OUTPUT);
  pinMode (E, OUTPUT);
  pinMode (fer, OUTPUT);
  pinMode (G, OUTPUT);
}

void loop() {
int rc = -1;
  if (!client.isConnected()) {
    Serial.print("Connecting using Registered mode with clientid : ");
    Serial.print(CLIENT_ID);
    Serial.print("\tto MQTT Broker : ");
    Serial.print(MS_PROXY);
    Serial.print("\ton topic : ");
    Serial.println(PUBLISH_TOPIC);
    
    ipstack.connect(MS_PROXY, MQTT_PORT);
    
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = CLIENT_ID;
    options.username.cstring = AUTHMETHOD;
    options.password.cstring = AUTHTOKEN;
    options.keepAliveInterval = 10;
    rc = -1;
    while ((rc = client.connect(options)) != 0)
      ;
    //unsubscribe the topic, if it had subscribed it before.
 
    client.unsubscribe(SUBSCRIBE_TOPIC);
    //Try to subscribe for commands
    Serial.println("Subscription tried......");
    Serial.println("Connected successfully\n");
    Serial.println("Temperature(in C)\tDevice Event (JSON)");
    Serial.println("____________________________________________________________________________");
  }

  MQTT::Message message;
  message.qos = MQTT::QOS0; 
  message.retained = false;

  char json[56] = "{\"d\":{\"myName\":\"Arduino Yun\",\"s1\":";
  float tempValue = score ;
  dtostrf(tempValue,1,2, &json[34]);
  json[38] = ',';
  json[39] = '\"';
  json[40] = 's';
  json[41] = '\"';
  json[42] = ':';
  float tempValue2 = score2 ;
  dtostrf(tempValue2,1,2, &json[43]);

  json[47] = '}';
  json[48] = '}';
  json[49] = '\0';
  Serial.print("\t");
  Serial.print(tempValue);
  Serial.print("\t\t");
  Serial.println(json);
  message.payload = json; 
  message.payloadlen = strlen(json);

  rc = client.publish(PUBLISH_TOPIC, message);
  if (rc != 0) {
    Serial.print("Message publish failed with return code : ");
    Serial.println(rc);
  }
  
  client.yield(1000); 
  long duration, distance;  
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if(distance>18){
    goal=0;
  }

  if(distance<=18 && goal==0 ){
    Serial.print(distance);
    Serial.println(" cm");
    score=score+1;
    goal=1;
    Serial.println("Score is: ");
    Serial.println(score);
    if(score==10) score=0;
    //sevenseg(score);
  }
  //delay(500);

long duration2, distance2;  
  digitalWrite(trigPin2, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
  if(distance2>18){
    goal2=0;
  }

  if(distance2<=18 && goal2==0 ){
    Serial.print(distance2);
    Serial.println(" cm");
    score2=score2+1;
    goal2=1;
    Serial.println("Score is: ");
    Serial.println(score2);
    if(score2==10) score2=0;
    sevenseg(score2);
  }
  delay(500);
}
void sevenseg(int x){
  Serial.println("x is");
  Serial.println(x);
  if (x ==0){
  digitalWrite (A, LOW);
  digitalWrite (B, LOW);
  digitalWrite (C, LOW);
  digitalWrite (D, LOW);
  digitalWrite (E, LOW);
  digitalWrite (fer, LOW);
  digitalWrite (G, HIGH);
  }
  if (x ==1){
  digitalWrite (A, HIGH);
  digitalWrite (B, LOW);
  digitalWrite (C, LOW);
  digitalWrite (D, HIGH);
  digitalWrite (E, HIGH);
  digitalWrite (fer, HIGH);
  digitalWrite (G, HIGH);
  }
  if (x ==2){
  digitalWrite (A, LOW);
  digitalWrite (B, LOW);
  digitalWrite (C, HIGH);
  digitalWrite (D, LOW);
  digitalWrite (E, LOW);
  digitalWrite (fer, HIGH);
  digitalWrite (G, LOW);
  }
  if (x ==3){
  digitalWrite (A, LOW);
  digitalWrite (B, LOW);
  digitalWrite (C, LOW);
  digitalWrite (D, LOW);
  digitalWrite (E, HIGH);
  digitalWrite (fer, HIGH);
  digitalWrite (G, LOW);
  }
  if (x ==4){
  digitalWrite (A, HIGH);
  digitalWrite (B, LOW);
  digitalWrite (C, LOW);
  digitalWrite (D, HIGH);
  digitalWrite (E, HIGH);
  digitalWrite (fer, LOW);
  digitalWrite (G, LOW);
  }
  if (x ==5){
  digitalWrite (A, LOW);
  digitalWrite (B, HIGH);
  digitalWrite (C, LOW);
  digitalWrite (D, LOW);
  digitalWrite (E, HIGH);
  digitalWrite (fer, LOW);
  digitalWrite (G, LOW);
  }
  if (x ==6){
  digitalWrite (A, LOW);
  digitalWrite (B, HIGH);
  digitalWrite (C, LOW);
  digitalWrite (D, LOW);
  digitalWrite (E, LOW);
  digitalWrite (fer, LOW);
  digitalWrite (G, LOW);
  }
  if (x ==7){
  digitalWrite (A, LOW);
  digitalWrite (B, LOW);
  digitalWrite (C, LOW);
  digitalWrite (D, HIGH);
  digitalWrite (E, HIGH);
  digitalWrite (fer, HIGH);
  digitalWrite (G, HIGH);
  }
  if (x ==8){
  digitalWrite (A, LOW);
  digitalWrite (B, LOW);
  digitalWrite (C, LOW);
  digitalWrite (D, LOW);
  digitalWrite (E, LOW);
  digitalWrite (fer, LOW);
  digitalWrite (G, LOW);
  }
  if (x ==9){
  digitalWrite (A, LOW);
  digitalWrite (B, LOW);
  digitalWrite (C, LOW);
  digitalWrite (D, LOW);
  digitalWrite (E, HIGH);
  digitalWrite (fer, LOW);
  digitalWrite (G, LOW);
  }
  
}

