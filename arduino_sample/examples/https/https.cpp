/*
 *  LteHttpSecureClient.ino - Example for secure HTTP client using LTE
 *  Copyright 2019, 2021, 2022 Sony Semiconductor Solutions Corporation
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  This sketch connects to a website via LTE.
 *  In this example, an HTTP GET request is sent to https://httpin.org/get,
 *  and an HTTP POST request is sent to https://httpin.org/post.
 */

// libraries
#include <ArduinoHttpClient.h>
#include <LTETLSClient.h>
#include <WString.h>
#include <URLParser.h>

#define DEPLOYMENT_ID "AKfycbwx_KauMHMQkYMPXBvsemoTzsN8eQ9j7kwwVsQuf8qiyjaFx7QLXKSU9gtDllEATK8"

#define SERVER_HOST "script.google.com"
#define SERVER_PATH "/macros/s/" DEPLOYMENT_ID "/exec?filename="
#define SERVER_URL "https://" SERVER_HOST SERVER_PATH

// Googleのルート証明書 (GTS Root R1 などのPEM)
// Execute below command
// openssl s_client -connect script.google.com:443 -servername script.google.com -showcerts </dev/null > caroot.inc
// And delete some lines
static const char GTS_ROOT_CA[] = 
#include "caroot.inc"
;

static char pathbuf[1024 * 2];
LTETLSClient tlsconnect;

void setup()
{
}

String postToGDrive(const char *fname, const char *type, const uint8_t *dat, int len)
{
  HttpClient client(tlsconnect, SERVER_HOST, 443);

  tlsconnect.setCACert(GTS_ROOT_CA);

  snprintf(pathbuf, sizeof(pathbuf), SERVER_PATH "%s", fname);

  printf("@@@ making POST request : %s\n", pathbuf);
  client.post(pathbuf, type, len, dat);

  int statusCode = client.responseStatusCode();
  printf("Status code: %d\n", statusCode);

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

  snprintf(pathbuf, sizeof(pathbuf), "%s?%s", path, query);
  printf("\n@@@ making GET request : %s\n", pathbuf);
  client.get(pathbuf);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  printf("Status code: %d\n", statusCode);
  printf("Response: \n");
  printf("%s\n", response.c_str());

  return true;
}

void loop()
{
  String resultURL = postToGDrive("test.txt", "text/plain", (const uint8_t *)"Test Data 2\n", 12);

  if (resultURL.length() >= 2)
    {
      ParsedUrl url(resultURL.c_str());
      printf("port: %d\n", url.port());
      printf("schema: %s\n", url.schema());
      printf("host: %s\n", url.host());
      printf("path: %s\n", url.path());
      printf("query: %s\n", url.query());
      printf("userinfo: %s\n", url.userinfo());

      getResult(url.host(), url.port(), url.path(), url.query());
    }
}
