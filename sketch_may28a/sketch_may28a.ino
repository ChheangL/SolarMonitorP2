#include <SPIFFS.h>

String readFile( const char * path){
  File file = SPIFFS.open(path, FILE_READ);
  Serial.println("- read from file:");
  String fileContent;
  for(uint8_t i = 0; i<6 && file.available(); i++){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(const char * path, const char * message){
  File file = SPIFFS.open(path, FILE_WRITE);
  file.print(message);
  file.flush();
  file.close();
}

void writeFile2()
{
Serial.println("writing");
File f = SPIFFS.open("/myFile.txt", "w");
if (!f) Serial.println("File creation failed");
else
{
f.print("networkConfig");
f.print("#");
f.flush();
f.close();
}
}
void readFile2()
{
Serial.println("reading");
File f = SPIFFS.open("/a.txt", "r");
if (!f) Serial.println("file not available");
else if (f.available()<=0) Serial.println("file exists but available <0");
else
{
String ssidString = f.readStringUntil('#');
Serial.print("read from file: ");
Serial.println(ssidString);
}
f.close();
}
void setup() {
  Serial.begin(115200);
    if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }
  writeFile("/a.txt","MTech_Solution");
  writeFile("/b.txt","No88138Jun2011");
  Serial.println(readFile("/a.txt").c_str());
  Serial.println(readFile("/b.txt").c_str());
  writeFile2();
  readFile2();
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
