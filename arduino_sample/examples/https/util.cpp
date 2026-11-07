#include "util.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <b64.h>

unsigned char *readFile(const char *filename, int *size)
{
  if (size == NULL)
    {
      return NULL;
    }

  *size = 0;
  if (filename == NULL)
    {
      return NULL;
    }

  FILE *file = fopen(filename, "rb");
  if (file == NULL)
    {
      return NULL;
    }

  if (fseek(file, 0, SEEK_END) != 0)
    {
      fclose(file);
      return NULL;
    }

  int fileSize = ftell(file);
  if (fileSize <= 0 || fseek(file, 0, SEEK_SET) != 0)
    {
      fclose(file);
      return NULL;
    }

  unsigned char *buffer = (unsigned char *)malloc(fileSize);
  if (buffer == NULL)
    {
      fclose(file);
      return NULL;
    }

  int bytesRead = fread(buffer, 1, fileSize, file);
  int readError = ferror(file);
  int closeResult = fclose(file);
  if (bytesRead != fileSize || readError || closeResult != 0)
    {
      free(buffer);
      return NULL;
    }

  int encodedSize = ((fileSize + 2) / 3) * 4;
  unsigned char *encodedBuffer = (unsigned char *)malloc(encodedSize);
  if (encodedBuffer == NULL)
    {
      free(buffer);
      return NULL;
    }

  int result = b64_encode(buffer, (int)fileSize, encodedBuffer, (int)encodedSize);
  free(buffer);
  if (result != (int)encodedSize)
    {
      free(encodedBuffer);
      return NULL;
    }

  *size = result;
  return encodedBuffer;
}