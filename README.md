<!--
//
// Copyright 2017 Luc Chouinard
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
-->

# Console is a class implementing partially esp32-idf console with linenoise as a line reader.

Have a look at https://github.com/lumostor/arduino-esp32-LinenoiseBitlash.git for an example of a full implementation.

```C++
class Console {
 private:
  String prompt;
  TaskHandle_t console_task_handle;

 public:
   
  Console();

  void begin(int baud, String p, int hlen);
  int termProbe();
  void termDumb( bool set );
  void historyLen( int len );
  void historyAdd(char *line);
  char *lineNoise();
  void freeLine(char*);
  void consoleTaskStart();
  void consoleTaskStop();
  virtual void consoleTask();
};
```


## Example of use

```C++
#include <Console.h>

Console CON;


setup() {

	CON.begin(115200, "HAL> ", 10);

	if (CON.termProbe()) { /* zero indicates success */
          printf("\n"
	    "Your terminal application does not support escape sequences.\n"
	    "Line editing and history features are disabled.\n"
	    "On linux , try screen.\n"
	    "On Windows, try using Putty instead.\n");
          CON.termDumb(true);
  	}

	CON.consoleTaskStart( );  // will start a task waiting for input but only print back
}

loop() {

       // put your loop code here

}


```