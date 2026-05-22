/**
 * @file system_msg_deal.c
 * @brief  
 * @version V1.0.0
 * @date 2019.01.22
 * 
 * @copyright Copyright (c) 2019  Chipintelli Technology Co., Ltd.
 * 
 */
#ifndef _SYSTEM_MSG_DEAL_H_
#define _SYSTEM_MSG_DEAL_H_

#include "FreeRTOS.h"
#include "task.h"
#include "voice_module_uart_protocol_g1.h"
#include "voice_module_uart_protocol.h"
#include "i2c_protocol_module.h"
#include "queue.h"
#include "ci_audio_wrapfft.h"
#ifdef __cplusplus
extern "C"
{
#endif


/************************************
         ASR
*************************************/
typedef enum
{
    MSG_ASR_STATUS_GOOD_RESULT = 0x00,
    MSG_ASR_STATUS_NO_RESULT,    
    MSG_ASR_STATUS_VAD_START,    
    MSG_ASR_STATUS_AUDIO_PROCESS,    
    MSG_ASR_STATUS_VAD_END,    
    MSG_CWSL_STATUS_GOOD_RESULT,
}sys_msg_asr_status_t;


typedef struct
{
    sys_msg_asr_status_t asr_status;
    uint32_t asr_cmd_handle;
	uint32_t asr_pcm_base_addr;
    short asr_score;
    uint16_t asr_frames;
}sys_msg_asr_data_t;

/************************************
         CMD_INFO
*************************************/
typedef enum
{
    MSG_CMD_INFO_STATUS_EXIT_WAKEUP = 0x00,
    MSG_CMD_INFO_STATUS_ENABLE_EXIT_WAKEUP,
    MSG_CMD_INFO_STATUS_ENABLE_PROCESS_ASR,
    MSG_CMD_INFO_STATUS_POST_CHANGE_ASR_WAKEUP_WORD,  
    MSG_CMD_INFO_STATUS_POST_CHANGE_ASR_NORMAL_WORD,  
}sys_msg_cmd_info_status_t;


typedef struct
{
    sys_msg_cmd_info_status_t cmd_info_status;
}sys_msg_cmd_info_data_t;

/************************************
         KEY
*************************************/
typedef enum
{
    MSG_KEY_STATUS_PRESS = 0x10,
    MSG_KEY_STATUS_PRESS_LONG,    
    MSG_KEY_STATUS_RELEASE,    
}sys_msg_key_status_t;


typedef struct
{
    sys_msg_key_status_t key_status;
    uint32_t key_index;
    uint32_t key_time_ms;           /*!< 按键首次被按下到现在的时间*/
}sys_msg_key_data_t;

//!宏定义，无按键值
#define KEY_NULL                (0xff) 

/************************************
         PLAY
*************************************/
typedef enum
{
    MSG_PLAY_STATUS_START = 0x20,
    MSG_PLAY_STATUS_PAUSE,    
    MSG_PLAY_STATUS_RESUME,    
    MSG_PLAY_STATUS_DONE,    
}sys_msg_play_status_t;


typedef struct
{
    sys_msg_play_status_t asr_status;
    uint32_t play_index;
}sys_msg_play_data_t;



/************************************
         NET
*************************************/
typedef enum
{
    MSG_NET_STATUS_DISCONNECT = 0x30,
    MSG_NET_STATUS_CONNECTED,    
}sys_msg_net_status_t;


typedef struct
{
    sys_msg_net_status_t net_status;
    uint32_t play_index;
}sys_msg_net_data_t;

/************************************
         MSG
*************************************/
typedef enum
{
    SYS_MSG_TYPE_ASR = 0,
    SYS_MSG_TYPE_CMD_INFO,
    SYS_MSG_TYPE_KEY,
    SYS_MSG_TYPE_COM,
    SYS_MSG_TYPE_PLAY,
    SYS_MSG_TYPE_NET,
    SYS_MSG_TYPE_MNG,
    SYS_MSG_TYPE_AUDIO_IN_STARTED,
    SYS_MSG_TYPE_I2C,
}sys_msg_type_t;


typedef struct
{
    sys_msg_type_t msg_type;/*here will be modify use union*/
    union
    {
        sys_msg_asr_data_t  asr_data;
        sys_msg_cmd_info_data_t  cmd_info_data;
        sys_msg_key_data_t  key_data;
        #ifdef UART_PROTOCOL_VER
        #if (UART_PROTOCOL_VER >= 1 && UART_PROTOCOL_VER < 255)
        sys_msg_com_data_t com_data;
        #endif
        #endif
        sys_msg_play_data_t play_data;
        sys_msg_net_data_t  net_data;
        sys_msg_i2c_data_t  i2c_data;
    }msg_data;
}sys_msg_t;

#if USE_DUAL_MIC_DOA
#   define WAKEUP_CMD_COMING()             (get_wake_up_cmd_status()==1)
#   define SET_WAKEUP_CMD_STATUS(a)        set_wake_up_cmd_status(a)
#   define WAKEUP_WHEN_PLAYING()           wakeup_when_playing()
#   define SET_WAKEUP_WHEN_PLAYING_FLAG(a) set_wakeup_when_playing_flag(a)
#endif

void userapp_initial(void);
void sys_msg_task_initial(void);


void UserTaskManageProcess(void *p_arg);


typedef enum
{
    SYS_STATE_UNWAKEUP = 0,
    SYS_STATE_WAKEUP,
}sys_wakeup_state_t;

typedef enum
{
    SYS_STATE_ASR_IDLE = 0,
    SYS_STATE_ASR_BUSY,
    SYS_STATE_ASR_SUSPEND,
}sys_asr_state_t;


typedef enum
{
    CI_RTC_MODE_UP = 0,
    CI_RTC_MODE_DOWN = 1,
}ci_rtc_mode_t;


typedef enum
{  
    CI_RTC_ALG_NULL = 0, 
    CI_RTC_ALG_AHS = 1<<0, //防啸叫算法
    CI_RTC_ALG_AEC = 1<<1, //回音消除
    CI_RTC_ALG_DN = 1<<2,  //降噪算法
    CI_RTC_ALG_EQ = 1<<3,  //均衡算法
    CI_RTC_ALG_DRC = 1<<4, //动态范围压缩算法
}ci_rtc_alg_t;


typedef struct 
{
    ci_rtc_mode_t mode;    //上行还是下行
}ci_rtc_state_t;



void exit_wakeup_deal(uint32_t asr_busy_check);
void enter_wakeup_deal(uint32_t exit_wakup_ms, cmd_handle_t cmd_handle);
sys_wakeup_state_t get_wakeup_state(void);
void update_awake_time(void);
BaseType_t send_msg_to_sys_task(sys_msg_t *send_msg,BaseType_t *xHigherPriorityTaskWoken);

bool check_current_playing(void);
bool get_mute_voice_in_state( void );

void default_play_done_callback(cmd_handle_t cmd_handle);
uint8_t vol_set(char vol);
uint8_t vol_get(void);
void pause_voice_in(void);
void resume_voice_in(void);
int ifft_data_handle_task_rtc_init(void);
void ifft_data_handle_task_rtc(void *p);

#ifdef __cplusplus
}
#endif
  
#endif


