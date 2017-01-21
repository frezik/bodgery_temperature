/*
Copyright 2017 Timm Murray

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, 
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <Wire.h>
#include <ESP8266WiFi.h>

#define TMP_102_ADDR 0x48

#define WIFI_SSID "the_bodgery"
#define WIFI_PASS "0987654321"
#define HOST "10.0.0.74"
#define PORT 7399


void setup()
{
    Serial.begin( 9600 );
    Wire.begin( 0, 2 );

    WiFi.mode( WIFI_STA );
    WiFi.begin( WIFI_SSID, WIFI_PASS );

    while (WiFi.status() != WL_CONNECTED) {
        delay( 500 );
    }
}

void loop()
{
    float celsius = getTemperature();
    sendTemperature( celsius );
    delay( 200 );
}


void sendTemperature( float celsius )
{
    WiFiClient client;
    client.connect( HOST, PORT );
    client.print( celsius );
    client.print( "\n" );
    client.stop();
}

float getTemperature ()
{
    Wire.requestFrom( TMP_102_ADDR, 2 );

    byte msb = Wire.read();
    byte lsb = Wire.read();

    int temp_sum = ((msb << 8) | lsb) >> 4; 

    float celsius = temp_sum * 0.0625;
    return celsius;
}
