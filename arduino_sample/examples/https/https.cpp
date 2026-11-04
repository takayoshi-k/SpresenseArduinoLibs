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
static const char GTS_ROOT_CA[] = 
"-----BEGIN CERTIFICATE-----\n"
"MIIHkjCCBzegAwIBAgIRAKNba7BW76hrEPyYtBP27+kwCgYIKoZIzj0EAwIwOzEL\n"
"MAkGA1UEBhMCVVMxHjAcBgNVBAoTFUdvb2dsZSBUcnVzdCBTZXJ2aWNlczEMMAoG\n"
"A1UEAxMDV0UyMB4XDTI2MDkyODE4MzgzM1oXDTI2MTIyMTE4MzgzMlowFzEVMBMG\n"
"A1UEAwwMKi5nb29nbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE3ixM\n"
"ZA6N6hjSpb96OXGuocqcZ9nx3S63pa6iOHdeK+HhOHQMNpJd0PM71dMG9WQjclke\n"
"genKgoxh1AGYsPO9eaOCBj4wggY6MA4GA1UdDwEB/wQEAwIHgDATBgNVHSUEDDAK\n"
"BggrBgEFBQcDATAMBgNVHRMBAf8EAjAAMB0GA1UdDgQWBBT6UhYabGE180j4/HuU\n"
"QOpd53ZR/zAfBgNVHSMEGDAWgBR1vsR3ron2RDd9z7FoHx0a69w0WTA1BggrBgEF\n"
"BQcBAQQpMCcwJQYIKwYBBQUHMAKGGWh0dHA6Ly9pLnBraS5nb29nL3dlMi5jcnQw\n"
"ggQ4BgNVHREEggQvMIIEK4IMKi5nb29nbGUuY29tghYqLmFwcGVuZ2luZS5nb29n\n"
"bGUuY29tggkqLmJkbi5kZXaCFSoub3JpZ2luLXRlc3QuYmRuLmRldoISKi5jbG91\n"
"ZC5nb29nbGUuY29tghgqLmNyb3dkc291cmNlLmdvb2dsZS5jb22CGCouZGF0YWNv\n"
"bXB1dGUuZ29vZ2xlLmNvbYILKi5nb29nbGUuY2GCCyouZ29vZ2xlLmNsgg4qLmdv\n"
"b2dsZS5jby5pboIOKi5nb29nbGUuY28uanCCDiouZ29vZ2xlLmNvLnVrgg8qLmdv\n"
"b2dsZS5jb20uYXKCDyouZ29vZ2xlLmNvbS5hdYIPKi5nb29nbGUuY29tLmJygg8q\n"
"Lmdvb2dsZS5jb20uY2+CDyouZ29vZ2xlLmNvbS5teIIPKi5nb29nbGUuY29tLnRy\n"
"gg8qLmdvb2dsZS5jb20udm6CCyouZ29vZ2xlLmRlggsqLmdvb2dsZS5lc4ILKi5n\n"
"b29nbGUuZnKCCyouZ29vZ2xlLmh1ggsqLmdvb2dsZS5pdIILKi5nb29nbGUubmyC\n"
"CyouZ29vZ2xlLnBsggsqLmdvb2dsZS5wdIIZKi5nZW1pbmkuY2xvdWQuZ29vZ2xl\n"
"LmNvbYINKi5nc3RhdGljLmNvbYIUKi5tZXRyaWMuZ3N0YXRpYy5jb22CCiouZ3Z0\n"
"MS5jb22CESouZ2NwY2RuLmd2dDEuY29tggoqLmd2dDIuY29tgg4qLmdjcC5ndnQy\n"
"LmNvbYIQKi51cmwuZ29vZ2xlLmNvbYIWKi55b3V0dWJlLW5vY29va2llLmNvbYIL\n"
"Ki55dGltZy5jb22CCmFpLmFuZHJvaWSCC2FuZHJvaWQuY29tgg0qLmFuZHJvaWQu\n"
"Y29tghMqLmZsYXNoLmFuZHJvaWQuY29tggRnLmNvggYqLmcuY2+CBmdvby5nbIIK\n"
"d3d3Lmdvby5nbIIUZ29vZ2xlLWFuYWx5dGljcy5jb22CFiouZ29vZ2xlLWFuYWx5\n"
"dGljcy5jb22CCmdvb2dsZS5jb22CEmdvb2dsZWNvbW1lcmNlLmNvbYIUKi5nb29n\n"
"bGVjb21tZXJjZS5jb22CCnVyY2hpbi5jb22CDCoudXJjaGluLmNvbYIIeW91dHUu\n"
"YmWCC3lvdXR1YmUuY29tgg0qLnlvdXR1YmUuY29tghFtdXNpYy55b3V0dWJlLmNv\n"
"bYITKi5tdXNpYy55b3V0dWJlLmNvbYIUeW91dHViZWVkdWNhdGlvbi5jb22CFiou\n"
"eW91dHViZWVkdWNhdGlvbi5jb22CD3lvdXR1YmVraWRzLmNvbYIRKi55b3V0dWJl\n"
"a2lkcy5jb22CBXl0LmJlggcqLnl0LmJlghphbmRyb2lkLmNsaWVudHMuZ29vZ2xl\n"
"LmNvbYIVKi5haXN0dWRpby5nb29nbGUuY29tMBMGA1UdIAQMMAowCAYGZ4EMAQIB\n"
"MDYGA1UdHwQvMC0wK6ApoCeGJWh0dHA6Ly9jLnBraS5nb29nL3dlMi94dXp0M1BV\n"
"OUZfdy5jcmwwggEDBgorBgEEAdZ5AgQCBIH0BIHxAO8AdQDXbX0Q0af1d8LH6V/X\n"
"AL/5gskzWmXh0LMBcxfAyMVpdwAAAaDphrvGAAAEAwBGMEQCIHWulV3A063f1lUL\n"
"A6WMmyO7hu/4Mp9/Rf7GRhogUR2AAiBOaNncpzXoyP9i4WLUjc5x9vnzUMYqrV7q\n"
"GRUtDexNYwB2AMIxfldFGaNF7n843rKQQevHwiFaIr9/1bWtdprZDlLNAAABoOmG\n"
"u+QAAAQDAEcwRQIhAKlIst6mO0N8aRye7xaa/VWtgPCm+iPYSq02Zf78e6N5AiAc\n"
"aaqj+rUED36zJP1sTogMJ5obl95ytc+cd7jpSsmB5DAKBggqhkjOPQQDAgNJADBG\n"
"AiEA0LRDq0cBL+4aoZsTHC1r6NYjGaXwpDHpvUczt7nt8QQCIQDwlgtgp/yEjCxI\n"
"apVCq+4hoTJvNVu+QkNjwAGz6INDsg==\n"
"-----END CERTIFICATE-----\n"
"-----BEGIN CERTIFICATE-----\n"
"MIICnjCCAiWgAwIBAgIQf/Mta0CdFdWWWwWHOnxy4DAKBggqhkjOPQQDAzBHMQsw\n"
"CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n"
"MBIGA1UEAxMLR1RTIFJvb3QgUjQwHhcNMjMxMjEzMDkwMDAwWhcNMjkwMjIwMTQw\n"
"MDAwWjA7MQswCQYDVQQGEwJVUzEeMBwGA1UEChMVR29vZ2xlIFRydXN0IFNlcnZp\n"
"Y2VzMQwwCgYDVQQDEwNXRTIwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQ1fh/y\n"
"FO2QfeGeKjRDhsHVlugncN+eBMupyoZ5CwhNRorCdKS72b/u/SPXOPNL71QX4b7n\n"
"ylUlqAwwrC1dTqFRo4H+MIH7MA4GA1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggr\n"
"BgEFBQcDAQYIKwYBBQUHAwIwEgYDVR0TAQH/BAgwBgEB/wIBADAdBgNVHQ4EFgQU\n"
"db7Ed66J9kQ3fc+xaB8dGuvcNFkwHwYDVR0jBBgwFoAUgEzW63T/STaj1dj8tT7F\n"
"avCUHYwwNAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzAChhhodHRwOi8vaS5wa2ku\n"
"Z29vZy9yNC5jcnQwKwYDVR0fBCQwIjAgoB6gHIYaaHR0cDovL2MucGtpLmdvb2cv\n"
"ci9yNC5jcmwwEwYDVR0gBAwwCjAIBgZngQwBAgEwCgYIKoZIzj0EAwMDZwAwZAIw\n"
"C724NlXINaPS2X05c9P394K4CdGBb+VkRdveqsAORRKPrJPoH2DsLn5ELCKUkeys\n"
"AjAv3wyQdkwtaWHVT/2YmBiE2zTqmOybzYhi/9Jl5TNqmgztI0k4L1G/kdASosk4\n"
"ONo=\n"
"-----END CERTIFICATE-----\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIDejCCAmKgAwIBAgIQf+UwvzMTQ77dghYQST2KGzANBgkqhkiG9w0BAQsFADBX\n"
"MQswCQYDVQQGEwJCRTEZMBcGA1UEChMQR2xvYmFsU2lnbiBudi1zYTEQMA4GA1UE\n"
"CxMHUm9vdCBDQTEbMBkGA1UEAxMSR2xvYmFsU2lnbiBSb290IENBMB4XDTIzMTEx\n"
"NTAzNDMyMVoXDTI4MDEyODAwMDA0MlowRzELMAkGA1UEBhMCVVMxIjAgBgNVBAoT\n"
"GUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBMTEMxFDASBgNVBAMTC0dUUyBSb290IFI0\n"
"MHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE83Rzp2iLYK5DuDXFgTB7S0md+8Fhzube\n"
"Rr1r1WEYNa5A3XP3iZEwWus87oV8okB2O6nGuEfYKueSkWpz6bFyOZ8pn6KY019e\n"
"WIZlD6GEZQbR3IvJx3PIjGov5cSr0R2Ko4H/MIH8MA4GA1UdDwEB/wQEAwIBhjAd\n"
"BgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDwYDVR0TAQH/BAUwAwEB/zAd\n"
"BgNVHQ4EFgQUgEzW63T/STaj1dj8tT7FavCUHYwwHwYDVR0jBBgwFoAUYHtmGkUN\n"
"l8qJUC99BM00qP/8/UswNgYIKwYBBQUHAQEEKjAoMCYGCCsGAQUFBzAChhpodHRw\n"
"Oi8vaS5wa2kuZ29vZy9nc3IxLmNydDAtBgNVHR8EJjAkMCKgIKAehhxodHRwOi8v\n"
"Yy5wa2kuZ29vZy9yL2dzcjEuY3JsMBMGA1UdIAQMMAowCAYGZ4EMAQIBMA0GCSqG\n"
"SIb3DQEBCwUAA4IBAQAYQrsPBtYDh5bjP2OBDwmkoWhIDDkic574y04tfzHpn+cJ\n"
"odI2D4SseesQ6bDrarZ7C30ddLibZatoKiws3UL9xnELz4ct92vID24FfVbiI1hY\n"
"+SW6FoVHkNeWIP0GCbaM4C6uVdF5dTUsMVs/ZbzNnIdCp5Gxmx5ejvEau8otR/Cs\n"
"kGN+hr/W5GvT1tMBjgWKZ1i4//emhA1JG1BbPzoLJQvyEotc03lXjTaCzv8mEbep\n"
"8RqZ7a2CPsgRbuvTPBwcOMBBmuFeU88+FSBX6+7iP0il8b4Z0QFqIwwMHfs/L6K1\n"
"vepuoxtGzi4CZ68zJpiq1UvSqTbFJjtbD4seiMHl\n"
"-----END CERTIFICATE-----\n"
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

#if 0
  printf("making GET request : %s\n", pathbuf);
  client.get(pathbuf);
#else
  printf("@@@ making POST request : %s\n", pathbuf);
  client.post(pathbuf, type, len, dat);
#endif

  // read the status code and body of the response
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

#if 0
  String hdr, val;
  while (client.headerAvailable())
    {
      hdr = client.readHeaderName();
      val = client.readHeaderValue();
      if (hdr == "Location")
        {
          ParsedUrl url(val.c_str());
          printf("port: %d\n", url.port());
          printf("schema: %s\n", url.schema());
          printf("host: %s\n", url.host());
          printf("path: %s\n", url.path());
          printf("query: %s\n", url.query());
          printf("userinfo: %s\n", url.userinfo());
        }
    }

  String response = client.responseBody();

  printf("Response: \n");
  printf("%s\n", response.c_str());
#endif
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
