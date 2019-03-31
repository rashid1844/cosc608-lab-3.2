#include <SPI.h>
#include <Ethernet.h>
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };
//byte mac[] =   {0xf0, 0x1f, 0xaf, 0x33, 0x62, 0x2f };
IPAddress ip(168,0,1,100);
//IPAddress server(3,18,60,91); // numeric ip of your local machine
char server[]="rashid.systemdev.org";

//String server ="rashid.systemdev.org";
// initialize the library instance:
EthernetClient client;
//char server[] = "localhost";
//variables to help in parsing XML received
String currentLine = "";
String unread = "";
boolean foundIt = false;
int led = 13;






void setup() {
  pinMode(led, OUTPUT);
  // reserve space for the strings:
  currentLine.reserve(256);
  unread.reserve(50);
  //Open connection to serial port for writing info to Serial Monitor
  Serial.begin(9600);
  // attempt a DHCP connection:
  Serial.println("Attempting to get an IP address using DHCP:");
  if (!Ethernet.begin(mac)) {
    // if DHCP fails, start with a hard-coded address:
    Serial.println("failed to get an IP address using DHCP, trying manually");
    Ethernet.begin(mac, ip);
  }
  Serial.print("My address:");
  Serial.println(Ethernet.localIP());
  // connect to ec2:
  connectToServer();
}






void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    //Serial.write(c);
    currentLine += c;
    if (c == '\n') {
      currentLine = "";
    }
    if ( currentLine.endsWith("<fullcount>")) {
        foundIt = true;
        // break out of the loop so this character is not added to unread;:
        return;
    }
    // if you are currently reading the bytes of the number indicating unread msgs,
    // add them to the unread String:
    if (foundIt) {
      if (c != '<') {
        unread += c;
      }
      else {
        // if you got a '<' character, you have read the number representing unread msgs:
        foundIt = false;
        Serial.println(unread+" unread messages.");
        // 'unread' variable is a string, convert it to integer
        char t[unread.length()+1];
        unread.toCharArray(t, (sizeof(t))); //string => char array
        int msgs = atoi(t); //char array => integer
        // blink the led 'unread' number times
        if (msgs > 0)
        {
          // turn the led on:
          digitalWrite(led, HIGH);
          Serial.println("LED High");
          delay(3000);
          // turn the led off:
          digitalWrite(led, LOW);
          Serial.println("LED Low");
          delay(1000);
        }
      }
    }
  }
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    // do nothing forever after:
    while(true);
  }
}
void connectToServer() {
  // attempt to connect, and wait a millisecond:
  Serial.println("connecting to server...");
  if (client.connect(server, 80)) {
    Serial.println("making HTTP request...");
    // make HTTP GET request to twitter:
    //    client.println("GET /1/statuses/user_timeline.xml?screen_name=arduino&count=1 HTTP/1.1");
    client.println("GET /php/get_gmail.php HTTP/1.0");
    client.print("HOST: ");
    client.println(server);
     client.println("Connection: close");
    client.println();
  }
}
