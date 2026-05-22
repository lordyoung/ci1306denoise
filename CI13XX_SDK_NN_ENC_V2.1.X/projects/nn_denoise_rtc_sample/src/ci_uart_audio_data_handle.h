#ifndef __CI_UART_AUDIO_DATA_HANDLE_H__
#define __CI_UART_AUDIO_DATA_HANDLE_H__

#include <stdio.h>
#include <malloc.h>
#include "FreeRTOS.h"
#include "task.h"
#include "sdk_default_config.h"
#include "ci130x_dma.h"
#include "ci130x_uart.h"
#include "user_config.h"
#include "queue.h"
#include "codec_manager.h"


int speex_recv_data_task(void);
int speex_record_data_task(void);
bool audio_speex_task_init(void);
#endif  //__CI_UART_AUDIO_DATA_HANDLE_H__