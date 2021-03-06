/* Copyright (c) 2014 Quanta Research Cambridge, Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "LedControllerRequest.h"
#include "GeneratedTypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <portal.h>

int main(int argc, const char **argv)
{
  LedControllerRequestProxy *device = new LedControllerRequestProxy(IfcNames_LedControllerRequestS2H);

  printf("Starting LED test\n");

  FILE *pipe = popen("ifconfig eth0", "r");
  char buf[256];
  // read the first line and discard it
  char *line = fgets(buf, sizeof(buf), pipe);
  // read the second line
  line = fgets(buf, sizeof(buf), pipe);
  printf("address line: %s", line);
  // done with the pipe, close it
  fclose(pipe);
  int addr[5];
  memset(addr, 0, sizeof(addr));
  int status = sscanf(buf, "          inet addr:%d.%d.%d.%d", &addr[0], &addr[1], &addr[2], &addr[3]);
  if (status != 4) {
    fprintf(stderr, "Failed to scan the IP address returned from ifconfig, converted %d out of 4. error=%d:%s\n", status, errno, strerror(errno));
  }
  printf("eth0 addr %d.%d.%d.%d\n", addr[0], addr[1], addr[2], addr[3]);

#ifdef BSIM
  // BSIM does not run very many cycles per second
  int blinkinterval = 10;
#else
  int blinkinterval = 100000000; // 100MHz cycles
#endif
  int sleepinterval = 1; // seconds
  for (int i = 0; i < 20; i++) {
    printf("led value %x\n", addr[i % 5]);
    device->setLeds(addr[i % 5], blinkinterval);
    sleep(sleepinterval);
  }
  printf("Done.\n");
}
