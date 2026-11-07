#include <iostream>
#include <ArduinoHttpClient.h>
#include <LTETLSClient.h>
#include <WString.h>
#include <URLParser.h>

#include "util.h"

#define DEPLOYMENT_ID "AKfycbwx_KauMHMQkYMPXBvsemoTzsN8eQ9j7kwwVsQuf8qiyjaFx7QLXKSU9gtDllEATK8"

#define SERVER_HOST "script.google.com"
#define SERVER_PATH "/macros/s/" DEPLOYMENT_ID "/exec?filename="

static const char GTS_ROOT_CA[] = 
#include "caroot.inc"
;

LTETLSClient tlsconnect;
unsigned char *filedat;
int filedat_size;

String postToGDrive(const char *fname, const char *type, const uint8_t *dat, int len)
{
  HttpClient client(tlsconnect, SERVER_HOST, 443);

  String access_path = String(SERVER_PATH) + String(fname);
  std::cout << "Access path: " << access_path.c_str() << std::endl;
  client.post(access_path.c_str(), type, len, dat);

  std::cout << "Waiting for response..." << std::endl;
  int statusCode = client.responseStatusCode();
  std::cout << "Status code: " << statusCode << std::endl;

  String ret;
  if (statusCode == 302)
    {
      while (client.headerAvailable())
        {
          if (client.readHeaderName() == "Location")
            {
              ret = client.readHeaderValue();
              break;
            }
        }
    }

  client.responseBody();

  return ret;
}

bool getResult(const char *host, int port, const char *path, const char *query)
{
  HttpClient client(tlsconnect, host, port);

  String access_path = String(path) + "?" + String(query);
  
  std::cout << "Making GET request: " << access_path.c_str() << std::endl;
  client.get(access_path.c_str());

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  std::cout << "Status code: " << statusCode << std::endl;
  std::cout << "Response: " << std::endl;
  std::cout << response.c_str() << std::endl;

  return true;
}

void setup()
{
  tlsconnect.setCACert(GTS_ROOT_CA);
  filedat = readFile("HenoHeno.jpg", &filedat_size);
  std::cout << "File data size: " << filedat_size << std::endl;
}

void loop()
{
  if (filedat == NULL)
    {
      std::cout << "Failed to read file." << std::endl;
      return;
    }

  String resultURL = postToGDrive("HenoHeno.jpg", "image/jpeg", filedat, filedat_size);

  if (resultURL.length() > 0)
    {
      ParsedUrl url(resultURL.c_str());
      std::cout << "port: " << url.port() << std::endl;
      std::cout << "host: " << url.host() << std::endl;
      std::cout << "path: " << url.path() << std::endl;
      std::cout << "query: " << url.query() << std::endl;

      getResult(url.host(), url.port(), url.path(), url.query());
    }
}
