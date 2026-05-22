#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "system_msg_deal.h"
#include "prompt_player.h"
#include "voice_module_uart_protocol.h"
#include "i2c_protocol_module.h"
#include "ci_nvdata_manage.h"
#include "ci_log.h"
#include "ci130x_gpio.h"
#include "user_msg_deal.h"
#include "user_control.h"
#include "ci_key.h"

// 接收到协议后的播报
#if RTC_PLAY_VOICE_BY_UART_ENABLE
static recv_send_pro_t recv_play[] = {
    ///tag1-play-voice-after-recv-by-uart
};

void play_voice_by_recv_protocol(char *p_data, int* vol_id)
{
    int recv_pro_len = sizeof(recv_play)/sizeof(recv_send_pro_t);
    for(int i=0;i<recv_pro_len;i++)
    {
        if(memcmp(p_data,recv_play[i].pro_buf,recv_play[i].pro_len) == 0)
        {
            *vol_id = recv_play[i].pro_id;
        }
    }
}
#endif

// 录音及录音播报功能
#if RTC_RECORD_PLAY_VOICE_ENABLE
static recv_send_pro_t recv_record[] = {
    ///tag2-record-and-play-after-recv-by-uart
};

// 接收到协议后的录音
void record_play_by_recv_protocol(char *p_data, int* vol_id)
{
    int recv_pro_len = sizeof(recv_record)/sizeof(recv_send_pro_t);
    for(int i=0;i<recv_pro_len;i++)
    {
        if(memcmp(p_data,recv_record[i].pro_buf,recv_record[i].pro_len) == 0)
        {
            *vol_id = recv_record[i].pro_id;
        }
    }
}
#endif

// 语音检测功能
#if RTC_VOX_ENABLE
static recv_send_pro_t recv_vox[] = {
    ///tag3-vox-cmd-after-recv-by-uart
};

static recv_send_pro_t send_vox[] = {
    ///tag4-send-uart-cmd-after-vox
};

// 接收到协议后的检测
void vox_voice_by_recv_protocol(char *p_data, int* vol_id)
{
    *vol_id = -1;
    int recv_pro_len = sizeof(recv_vox)/sizeof(recv_send_pro_t);
    for(int i=0;i<recv_pro_len;i++)
    {
        if(memcmp(p_data,recv_vox[i].pro_buf,recv_vox[i].pro_len) == 0)
        {
            *vol_id = recv_vox[i].pro_id;
        }
    }
}

// 检测后的发送
void vox_voice_send_protocol(int id)
{
    for (int i = 0;i < send_vox[id].pro_len; i++)
    {
        UartPollingSenddata(UART_PROTOCOL_NUMBER, send_vox[id].pro_buf[i]);
    }
}
#endif

/**
 * @brief 用户初始化
 *
 */
void userapp_initial(void)
{
    #if CPU_RATE_PRINT
    init_timer3_getresource();
    #endif

    #if MSG_COM_USE_UART_EN
    #if (UART_PROTOCOL_VER == 1)
    uart_communicate_init();
    #elif (UART_PROTOCOL_VER == 2)
    vmup_communicate_init();
    #elif (UART_PROTOCOL_VER == 255)
    vmup_communicate_init();
    #endif
    #endif

    #if MSG_USE_I2C_EN
    i2c_communicate_init();
    #endif
	
	
    //按键初始化
    ci_key_init();
    mprintf("HEMY-%s %d\n",__FUNCTION__,__LINE__);

    ///tag-gpio-init
    // user_pin_control_init();
}

/**
 * @brief 处理按键消息（目前未实现该demo）
 *
 * @param key_msg 按键消息
 */
void userapp_deal_key_msg(sys_msg_key_data_t  *key_msg)
{
    (void)(key_msg);
}



/**
 * @brief 按语义ID响应asr消息处理
 *
 * @param asr_msg
 * @param cmd_handle
 * @param semantic_id
 * @return uint32_t
 */
uint32_t deal_asr_msg_by_semantic_id(sys_msg_asr_data_t *asr_msg, cmd_handle_t cmd_handle, uint32_t semantic_id)
{
    uint32_t ret = 1;
    if (PRODUCT_GENERAL == get_product_id_from_semantic_id(semantic_id))
    {
        uint8_t vol;
        int select_index = -1;
        switch(get_function_id_from_semantic_id(semantic_id))
        {
        case VOLUME_UP:        //增大音量
            vol = vol_set(vol_get() + 1);
            select_index = (vol == VOLUME_MAX) ? 1:0;
            break;
        case VOLUME_DOWN:      //减小音量
            vol = vol_set(vol_get() - 1);
            select_index = (vol == VOLUME_MIN) ? 1:0;
            break;
        case MAXIMUM_VOLUME:   //最大音量
            vol_set(VOLUME_MAX);
            break;
        case MEDIUM_VOLUME:  //中等音量
            vol_set(VOLUME_MID);
            break;
        case MINIMUM_VOLUME:   //最小音量
            vol_set(VOLUME_MIN);
            break;
        case TURN_ON_VOICE_BROADCAST:    //开启语音播报
            prompt_player_enable(ENABLE);
            break;
        case TURN_OFF_VOICE_BROADCAST:    //关闭语音播报
            prompt_player_enable(DISABLE);
            break;
        default:
            ret = 0;
            break;
        }
        if (ret)
        {
            #if PLAY_OTHER_CMD_EN
            pause_voice_in();
            prompt_play_by_cmd_handle(cmd_handle, select_index, default_play_done_callback,true);
            #endif
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}


/**
 * @brief 按命令词id响应asr消息处理
 *
 * @param asr_msg
 * @param cmd_handle
 * @param cmd_id
 * @return uint32_t
 */
uint32_t deal_asr_msg_by_cmd_id(sys_msg_asr_data_t *asr_msg, cmd_handle_t cmd_handle, uint16_t cmd_id)
{
    uint32_t ret = 1;
    int select_index = -1;
    switch(cmd_id)
    {
        ///tag-asr-msg-deal-by-cmd-id-start
        case 2://“打开空调”
        {
            break;
        }
        case 3://“关闭空调”
        {
            break;
        }
        case 14://“除湿模式”
        {
            break;
        }
        case 15://"关闭除湿"
        {
            break;
        }
        case 22://"关闭睡眠模式"
        {
            break;
        }
        ///tag-asr-msg-deal-by-cmd-id-end
        default:
            ret = 0;
            break;
    }

    if (ret && select_index >= -1)
    {
        #if PLAY_OTHER_CMD_EN
        pause_voice_in();
        prompt_play_by_cmd_handle(cmd_handle, select_index, default_play_done_callback,true);
        #endif
    }

    return ret;
}

/**
 * @brief 用户自定义消息处理
 *
 * @param msg
 * @return uint32_t
 */
uint32_t deal_userdef_msg(sys_msg_t *msg)
{
    uint32_t ret = 1;
    switch(msg->msg_type)
    {
    /* 按键消息 */
    case SYS_MSG_TYPE_KEY:
    {
        sys_msg_key_data_t *key_rev_data;
        key_rev_data = &msg->msg_data.key_data;
        userapp_deal_key_msg(key_rev_data);
        break;
    }
    #if MSG_COM_USE_UART_EN
    /* CI串口协议消息 */
    case SYS_MSG_TYPE_COM:
    {
		#if ((UART_PROTOCOL_VER == 1) || (UART_PROTOCOL_VER == 2))
    	sys_msg_com_data_t *com_rev_data;
        com_rev_data = &(msg->msg_data.com_data);
        userapp_deal_com_msg(com_rev_data);
        #elif (UART_PROTOCOL_VER == 255)
        com_msg_process(&msg->msg_data.com_data); //串口数据接收时,rx_buffer[0]存放的是接收的数据长度信息，串口数据从rx_buffer[1]开始存放
        #endif
        break;
    }
    #endif
    /* CI IIC 协议消息 */
    #if MSG_USE_I2C_EN
    case SYS_MSG_TYPE_I2C:
    {
        sys_msg_i2c_data_t *i2c_rev_data;
        i2c_rev_data = &msg->msg_data.i2c_data;
        userapp_deal_i2c_msg(i2c_rev_data);
        break;
    }
    #endif
    default:
        break;
    }
    return ret;
}


ci_rtc_state_t rtc_state;

void set_rtc_mode(ci_rtc_mode_t mode)
{
    rtc_state.mode = mode;
}


#include "ci130x_iwdg.h"
#include "audio_in_manage_inner.h"
#include "status_share.h"
#include "codec_manager.h"


uint8_t vol_set_rtc_out(char vol)
{
    int32_t gain = 67*vol/VOLUME_MAX + 7;
    cm_set_codec_dac_gain(PLAY_CODEC_ID, CM_CHA_TWO_CHA, gain);
}


void audio_in_manage_inner_task_rtc(void *p)
{
    cm_start_codec(HOST_MIC_RECORD_CODEC_ID, CODEC_INPUT);
    ciss_set(CI_SS_MIC_VOICE_STATUE, CI_SS_MIC_VOICE_NORMAL);

    audio_pre_rslt_out_play_card_init();

    cm_set_codec_dac_gain(PLAY_CODEC_ID,CM_CHA_TWO_CHA,RTC_HPOUT_DEFAULT_VOL);

    for (;;)
    {
        if(rtc_state.mode == CI_RTC_MODE_UP)
        {
            //上行模式，MIC收音，处理之后发送出去
            uint32_t data_addr = 0, data_size;

            cm_read_codec(HOST_MIC_RECORD_CODEC_ID, &data_addr, &data_size);

            if (!data_addr)
            {
                mprintf("iisdma int too slow\n");
                continue;
            }

            audio_in_get_voice_t voice_info_str;
            voice_info_str.channel_flag = 1;
            
            voice_info_str.addr1 = data_addr;
            voice_info_str.cha_num = 1;
            voice_info_str.addr2 = 0;
            voice_info_str.size = data_size;

            #if RTC_MIC_LEFT_RIGHT_CHOOSE
            int16_t* data_to_asr = (int16_t*)data_addr;
            int num = data_size / 2 / 2;
            extern bool get_src_from_right_mic(void);
            if(get_src_from_right_mic())
            {
                for(int n=0;n<num;n++)
                {
                    data_to_asr[n] = data_to_asr[2*n + 1];
                }
            }
            else
            {
                for(int n=0;n<num;n++)
                {
                    data_to_asr[n] = data_to_asr[2*n];
                }
            }
            #endif
        
            send_voice_info_to_audio_in_manage(&voice_info_str);
        }
        else if(rtc_state.mode == CI_RTC_MODE_DOWN)
        {

        }
        
        iwdg_feed(IWDG);
    }
}
