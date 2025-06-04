#include "globals.h"

extern WiFiClient debugClient;

void dpln(const String &message)
{
  if (enableSerialLogs)
    Serial.println(message);
  if (enableTelnetDebugging)
  {
    if (debugClient)
    {
      debugClient.println(message);
    }
  }
}

void dp(const String &message)
{
  if (enableSerialLogs)
    Serial.print(message);
  if (enableTelnetDebugging)
  {
    if (debugClient)
    {
      debugClient.print(message);
    }
  }
}

int dpf(const char *format, ...)
{
  char loc_buf[128];
  char *temp = loc_buf;

  va_list arg;
  va_list copy;

  // Use a local buffer for small messages to avoid dynamic memory allocation
  va_start(arg, format);

  // Copy the variable arguments to a new list for vsnprintf
  va_copy(copy, arg);
  int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
  va_end(copy);

  // If vsnprintf returns a negative value, it indicates an error
  if (len < 0)
  {
    va_end(arg);
    return 0;
  };

  // If the formatted string is larger than the local buffer, allocate a new buffer
  if (len >= sizeof(loc_buf))
  {
    temp = (char *)malloc(len + 1);
    if (temp == NULL)
    {
      va_end(arg);
      return 0;
    }
    len = vsnprintf(temp, len + 1, format, arg);
  }

  // now print the buffer to Serial
  len = 0;
  if (enableSerialLogs)
  {
    len = Serial.print(temp);
  }
  if (enableTelnetDebugging)
  {
    if (debugClient)
    {
      debugClient.print(temp);
    }
  }

  if (temp != loc_buf)
  {
    free(temp);
  }

  va_end(arg);
  return len;
}


void handleTelnetDebugging()
{
  if (enableTelnetDebugging)
  {
    if (!debugClient)
    {
      debugClient = debugServer.available();
    }
  }
}