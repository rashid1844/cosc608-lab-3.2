#include <SPI.h>
#include <Ethernet.h>
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };
//byte mac[] =   {0xf0, 0x1f, 0xaf, 0x33, 0x62, 0x2f };
IPAddress ip(192,168,1,20);
// initialize the library instance:
EthernetClient client;
char server[] = "rashid.systemdev.org";
double dummyValue = 7.5;


const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor






void setup()
{
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
}













void loop()
{
  
  
  
   long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
cm = microsecondsToCentimeters(duration);
  Serial.print("sensor:");
  Serial.println(cm);
  
  
  
  
    // connect to the server
    if (client.connect(server, 80))
    {
        // print to serial monitor
        Serial.println("connected...");
        Serial.println("ARDUINO: forming HTTP request message");
        // send data the server through GET request
        client.print("GET /php/sensor_upload.php?type=Ultrasonic&reading=");
        client.print(cm);
        client.println(" HTTP/1.0");
        client.print("HOST: ");
        client.println(server);
        client.println();
        Serial.println("ARDUINO: HTTP message sent");
        // give server some time to receive and store data
        // before asking for response from it
        delay(5000);
        // get the response from the page and print it to serial port
        // to ascertain that data was received properly
        if(client.available())
        {
            Serial.println("ARDUINO: HTTP message received");
            Serial.println("ARDUINO: printing received headers and script response...\n");
            while(client.available())
            {
                char c = client.read();
                Serial.print(c);
            }
        }
        else
        {
            Serial.println("ARDUINO: no response received / no response received in time");
        }
        client.stop();
    }
    // do nothing forever after:
    while(true);
}




long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
