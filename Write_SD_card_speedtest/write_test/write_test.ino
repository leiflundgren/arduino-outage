/*
  Listfiles

 This example shows how print out the files in a
 directory on a SD card

 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 2 Feb 2014
 by Scott Fitzgerald

 This example code is in the public domain.

 */
#include <SPI.h>
#include <SD.h>

File root;

File logfile;

const int data_len = 1024;
int data[data_len];

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   for (int b = 0; b < 5; ++b)
        blink(50, 100);
        
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  for (int b = 0; b < 5; ++b)
       blink(50, 100);

  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  Serial.println("done!");

  
}

void createNewFile()
{
     // create a new file
    char filename[] = "LOGGER00.CSV";
    for (uint8_t i = 0; i < 100; i++) {
        filename[6] = i / 10 + '0';
        filename[7] = i % 10 + '0';
        if (!SD.exists(filename)) {
            // only open a new file if it doesn't exist
            logfile = SD.open(filename, FILE_WRITE);
            Serial.print("Opened ");
            Serial.print(logfile);
            Serial.println();
            break;  // leave the loop!
        }
    }
    if (!logfile) {
        error("couldnt create file");
    }
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}


void error(const char *str)
{
    Serial.print("error: ");
    Serial.println(str);

    while (1)
        blink(150, 100);
}
void blink(int on_ms, int off_ms)
{
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(on_ms);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(off_ms);                       // wait for a second

}

void loop() {
  // nothing happens after setup finishes.


  createNewFile();

    delay(1000);
    Serial.println("Ready to start!");
 
   // error("avoiding start");

    int millis_start = millis();
    

    for (int i = 0; i < data_len; ++i)
    {
        logfile.print(i);
        logfile.print(", ");
        logfile.print(data[i]);
        logfile.println();
    }

   
    // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
    // which uses a bunch of power and takes time
    logfile.flush();
    logfile.close();
 int millis_end = millis();

    Serial.print("Writing ");
    Serial.print(data_len);
    Serial.print("took ");
    Serial.print(millis_end - millis_start);
    Serial.print("ms.");
    Serial.println();

    logfile.print("Writing took ");
    logfile.print(millis_end - millis_start);
    logfile.print("ms.");
    logfile.println();
        

    
    while (1)
        blink(200, 1200);
}
