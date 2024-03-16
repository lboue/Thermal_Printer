//
// Thermal Printer List
// written by Larry Bank
// Copyright (c) 2023 BitBank Software, Inc.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <Thermal_Printer.h>

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 39;  // the number of the pushbutton pin
const int ledPin = 21;    // the number of the LED pin

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

static uint8_t ucBuf[48 * 384];
#define WIDTH 384
#define HEIGHT 240

char *myStrings[] = {"This is string 1", "This is string 2", "This is string 3",
                     "This is string 4", "This is string 5", "This is string 6",
                     "This is string 7", "This is string 8", "This is string 9"
                    };

const size_t nb_strings = sizeof(myStrings) / sizeof(myStrings[0]);

void setup() {
  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  int i;

  Serial.begin(115200);
  while (!Serial);

  //tpSetBackBuffer(ucBuf, WIDTH, HEIGHT);
  tpSetBackBuffer(ucBuf, WIDTH, 50);
  tpFill(0);

  Serial.println((char *)"Scanning for BLE printer");
  if (tpScan())
  {
    Serial.println((char *)"Found a printer!, connecting...");
    if (tpConnect())
    {
      int iWidth;
      Serial.println((char *)"Connected!, printing graphics");
      
      iWidth = tpGetWidth();
      Serial.print("Printer pixel width = ");
      Serial.println(iWidth, DEC);
            
      tpDrawText(0,0,(char *)"Article 1", FONT_LARGE, 0);
      tpPrintBuffer();
      tpDrawText(0,0,(char *)"Article 2", FONT_LARGE, 0);
      tpPrintBuffer();

      Serial.print("Array length :");Serial.println(sizeof(myStrings));

      for (int i = 0; i < nb_strings; i++) {
        Serial.println(myStrings[i]);
        tpDrawText(0,0,(char *)myStrings[i], FONT_LARGE, 0);
        tpPrintBuffer();
      }

      tpSetBackBuffer(ucBuf, WIDTH, 80);

      tpFeed(30); // Feed the paper forward 1-255 scan lines
      tpPrintBuffer();

      Serial.println((char *)"Disconnecting");
      tpDisconnect();

      Serial.println((char *)"Done!");
      //while (1) {};      
    }
  }
  else
  {
    Serial.println((char *)"Didn't find a printer :( ");
  }
}

void loop() {
  //Serial.println("Found a printer!, connecting...");

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    Serial.println("Button pressed");
    sleep(100);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
  sleep(100);
}
