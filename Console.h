#ifndef CONSOLE_H
#define CONSOLE_H

// Copyright 2017 Luc Chouinard

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <Arduino.h>
#include <stdint.h>
#include "esp_console.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "esp_vfs_fat.h"

esp_err_t esp_console_init(const esp_console_config_t* config);



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
  

#endif  //#ifndef CONSOLE_H
