#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough.: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "JOSEPH";
const char* password = "00000000";

// Initialize Telegram BOT
#define BOTtoken "2035773795:AAGLoBUrKI0_vIk1f63RWhjaDt7mXd6peLM"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "1135395154"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int fan = 12;
const int bulb = 14;
const int AC = 26;
const int door = 27;
bool ledState = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands is to control your SMART HOME.\n\n";
      welcome += "/fan_on to turn FAN ON \n";
      welcome += "/fan_off to turn FAN OFF \n";
      welcome += "/AC_on to turn AC ON \n";
      welcome += "/AC_off to turn AC OFF \n";
      welcome += "/bulb_on to turn BULB ON \n";
      welcome += "/bulb_off to turn BULB OFF \n";
      welcome += "/door_open to  OPEN DOOR \n";
      welcome += "/door_close to CLOSE \n";
      welcome += "/state to the state of the House \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/fan_on") {
      bot.sendMessage(chat_id, "The fan is now ON", "");
      ledState = HIGH;
      digitalWrite(fan, ledState);
    }
    
    if (text == "/fan_off") {
      bot.sendMessage(chat_id, " Fan is OFF", "");
      ledState = LOW;
      digitalWrite(fan, ledState);
    }

    if (text == "/AC_on") {
      bot.sendMessage(chat_id, "AC is ON", "");
      ledState = HIGH;
      digitalWrite(AC, ledState);
    }
    
    if (text == "/AC_off") {
      bot.sendMessage(chat_id, "AC is OFF", "");
      ledState = LOW;
      digitalWrite(AC, ledState);
    }

    if (text == "/bulb_on") {
      bot.sendMessage(chat_id, "Bulb is ON", "");
      ledState = HIGH;
      digitalWrite(bulb, ledState);
    }
    
    if (text == "/bulb_off") {
      bot.sendMessage(chat_id, "Bulb OFF", "");
      ledState = LOW;
      digitalWrite(bulb, ledState);
    }

    if (text == "/door_open") {
      bot.sendMessage(chat_id, "Door is OPEN", "");
      ledState = HIGH;
      digitalWrite(door, ledState);
    }
    
    if (text == "/door_close") {
      bot.sendMessage(chat_id, "Door is CLOSED", "");
      ledState = LOW;
      digitalWrite(door, ledState);
    }
    
    if (text == "/state") {
      if (digitalRead(fan)){
        bot.sendMessage(chat_id, "FAN is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "FAN is OFF", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(fan, OUTPUT);
  digitalWrite(fan, ledState);
  pinMode(AC, OUTPUT);
  digitalWrite(AC, ledState);
  pinMode(bulb, OUTPUT);
  digitalWrite(bulb, ledState);
  pinMode(door, OUTPUT);
  digitalWrite(door, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
