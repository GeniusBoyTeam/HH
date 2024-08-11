#include "main.h"
#include <WiFi.h>

#include <SPI.h>
#include <SD.h>
#include <HTTPClient.h>

File myFile;

void sendFile(char *serverUrl, File file)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(serverUrl);

    http.addHeader("Content-Type", "text/plain");

    String fileContent = file.readString(); // Replace with your file content
    log_i("server: %s", serverUrl);

    int httpResponseCode = http.POST(fileContent);

    if (httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    else
    {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
  }
  else
  {
    Serial.println("WiFi not connected");
  }
}

void setup()
{
  Serial.begin(115200);
  Serial1.setPins(RX_PIN, TX_PIN);
  Serial1.begin(115200);

  // WiFi.mode(WIFI_STA);
  // WiFi.begin("GRBL_devak", "12345678");

  // log_i("Connecting to WiFi ..");
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   log_i(".");
  //   delay(1000);
  // }
  // log_i("local ip: %s", (WiFi.localIP().toString()).c_str());

  // // SPIClass spiSD = SPIClass(HSPI);
  // // spiSD.begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
  // // spiSD.begin(6, 5, 7, 15);
  // // SPI.begin()
  // // SPIClass sd_spi;
  // // spi.begin(6, 5, 7, 15);
  // // uint8_t cardType;

  // // // first init and check SD card
  // // if (!SD.begin(38, spi, 40000000))
  // // {
  // //   log_i("Card Mount Failed");
  // // }

  // SPIClass sd_spi;
  // sd_spi.begin(6, 5, 7, 15);
  // if (!SD.begin(15, sd_spi))
  // {
  //   log_i("Card Mount Failed");
  //   return;
  // }

  // myFile = SD.open("/file.TAP", FILE_READ);

  // if (myFile)
  // {
  //   log_i("file exist");
  // }
  // else
  // {
  //   log_i("error opening file.TAP");
  // }

  // char *serverUrl = "http://192.168.4.1/upload";
  // sendFile(serverUrl, myFile);

  serialSetup();
  displaySetup();
  keypadSetup();
  initBuzzer();
}

void loop()
{
  vTaskDelay(1);
}