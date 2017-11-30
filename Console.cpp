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

#include <Console.h>

#define ANSI_COLOR_DEFAULT      39
static char* s_tmp_line_buf;
static esp_console_config_t s_config;
//char *line;


esp_err_t esp_console_init(const esp_console_config_t* config) {
  if (s_tmp_line_buf) {
    return ESP_ERR_INVALID_STATE;
  }
  memcpy(&s_config, config, sizeof(s_config));
  if (s_config.hint_color == 0) {
    s_config.hint_color = ANSI_COLOR_DEFAULT;
  }
  s_tmp_line_buf = (char *)calloc(config->max_cmdline_length, 1);
  if (s_tmp_line_buf == NULL) {
    return ESP_ERR_NO_MEM;
  }
  return ESP_OK;
}


//extern "C"
void consoleTaskWrapper(void *conptr) {

  (static_cast<Console *>(conptr))->consoleTask();

}

Console::Console() {
  
  console_task_handle = NULL;
}

void Console::begin(int baud, String p, int hlen) {

  prompt = p;

  uart_config_t uart_config;
  uart_config.baud_rate = baud;
  uart_config.data_bits = UART_DATA_8_BITS;
  uart_config.parity = UART_PARITY_DISABLE;
  uart_config.stop_bits = UART_STOP_BITS_1;
  uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
  uart_param_config(UART_NUM_1, &uart_config);

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);

  /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
  esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
  /* Move the caret to the beginning of the next line on '\n' */
  esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);
  
  /* Install UART driver for interrupt-driven reads and writes */
  ESP_ERROR_CHECK( uart_driver_install((uart_port_t)CONFIG_CONSOLE_UART_NUM,
				       256, 0, 0, NULL, 0) );
  
  /* Tell VFS to use UART driver */
  esp_vfs_dev_uart_use_driver(CONFIG_CONSOLE_UART_NUM);

  /* Initialize the console */
  esp_console_config_t console_config;
  
  console_config.max_cmdline_args = 8;
  console_config.max_cmdline_length = 256;
#if CONFIG_LOG_COLORS
  console_config.hint_color = atoi(LOG_COLOR_CYAN);
#endif

  //ESP_ERROR_CHECK( esp_console_init(&console_config) );
  ESP_ERROR_CHECK( esp_console_init(&console_config) );

  historyLen(hlen);

}

int Console::termProbe() {
  return linenoiseProbe();
}

void Console::termDumb( bool set ) {
  linenoiseSetDumbMode(set);
}

void Console::historyLen( int len ) {
  linenoiseHistorySetMaxLen( len );
}

void Console::historyAdd(char *line) {
  linenoiseHistoryAdd(line);
}

char *Console::lineNoise() {
  return linenoise(prompt.c_str());
}

void Console::freeLine(char *line) {
  linenoiseFree(line);
}

void Console::consoleTaskStart( ) {

  xTaskCreate(
	      consoleTaskWrapper,           /* Task function. */
	      "console Task",        /* name of task. */
	      10000,                    /* Stack size of task */
	      (void *)this,                     /* parameter of the task */
	      1,                        /* priority of the task */
	      &console_task_handle); /* Task handle to keep track of created task */
}

void Console::consoleTaskStop() {
  if( console_task_handle != NULL ) {
    vTaskDelete( console_task_handle );
  }
}

void Console::consoleTask() {

  char *line;
  for(;;) {
    if((line = lineNoise()) != NULL) {
      historyAdd(line);
      printf("Console dummy task! You wrote: %s\n",line);
      freeLine(line); 
    } 
  }
  
}

  
