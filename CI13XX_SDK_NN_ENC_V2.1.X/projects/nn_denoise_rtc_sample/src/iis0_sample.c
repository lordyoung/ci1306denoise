/**
 * @file main.c
 * @brief 示例程序
 * @version 1.0.0
 * @date 2021-03-19
 *
 * @copyright Copyright (c) 2019  Chipintelli Technology Co., Ltd.
 *
 */
#include <stdio.h> 
#include <malloc.h>
#include "FreeRTOS.h" 
#include "task.h"
#include "sdk_default_config.h"
#include "ci130x_core_eclic.h"
#include "ci130x_spiflash.h"
#include "ci130x_gpio.h"
#include "audio_play_api.h"
#include "audio_play_decoder.h"
#include "ci_flash_data_info.h"
// #include "ci130x_audio_capture.h"
#include "board.h"
#include "ci130x_uart.h"
#include "flash_manage_outside_port.h"
#include "system_msg_deal.h"
#include "ci130x_dpmu.h"
#include "ci130x_mailbox.h"
#include "ci130x_nuclear_com.h"
#include "flash_control_inner_port.h"
#include "romlib_runtime.h"
#include "audio_in_manage_inner.h"
#include "ci_log.h"
#include "status_share.h"
#include "platform_config.h"
#include "asr_api.h"
#include "alg_preprocess.h"
#include "ci130x_iwdg.h"
#include "ci130x_codec.h"
#include "codec_manager.h"
#include "user_config.h"

typedef enum
{
    AUDIO_PLAY_STATUS_ON = 1,
    AUDIO_PLAY_STATUS_OFF = 0,
}audio_play_status_t;


#define IIS0_CODEC_ID             0


volatile uint16_t audio_block_size = 0;
volatile audio_play_status_t audio_play_status = AUDIO_PLAY_STATUS_OFF;


/**
 * @brief 默认麦克采音喇叭播音的CODEC配置
 *
 */
const cm_codec_hw_info_t host_iis0_audio_in_hw_info_test = 
{
    .IICx = IIC_NULL,
    .input_iis.IISx = IIS0,
    .input_iis.iis_mode_sel = USE_IIS0_AUDIO_IN_SPEAKER_OUT_MODE, // <IIS_MASTER , IIS_SLAVE>
    .input_iis.over_sample = IIS_MCLK_FS_256,
    .input_iis.clk_source = AUDIO_PLAY_CLK_SOURCE_IPCORE,
    .input_iis.mclk_out_en = IIS_MCLK_OUT,
    .input_iis.iis_data_format = IIS_DF_IIS,
    .input_iis.sck_lrck_ratio = IIS_SCK_LRCK_64,
    .input_iis.tx_cha = IIS_RX_CHANNAL_RX0,
    .input_iis.scklrck_out_en = IIS_SCKLRCK_MODENULL,   

    .output_iis.IISx = IIS0,
    .output_iis.iis_mode_sel = USE_IIS0_AUDIO_IN_SPEAKER_OUT_MODE, // <IIS_MASTER, IIS_SLAVE>
    .output_iis.over_sample = IIS_MCLK_FS_256,
    #if !INNER_CODEC_AUDIO_IN_USE_RESAMPLE
    .output_iis.clk_source = AUDIO_PLAY_CLK_SOURCE_OSC_OR_INEER_RC,
    #else
    .output_iis.clk_source = AUDIO_PLAY_CLK_SOURCE_IPCORE,
    #endif
    .output_iis.mclk_out_en = USE_IIS0_AUDIO_IN_SPEAKER_OUT_MODE,
    .output_iis.iis_data_format = IIS_DF_IIS,
    .output_iis.sck_lrck_ratio = IIS_SCK_LRCK_64,
    .output_iis.tx_cha = IIS_TX_CHANNAL_TX0,
    .output_iis.scklrck_out_en = IIS_SCKLRCK_OUT,// <IIS_SCKLRCK_OUT , IIS_SCKLRCK_MODENULL>选择输出，输入
}; 


/**
 * @brief 录音音频参数表
 *
 */
const cm_sound_info_t host_iis0_audio_in_sound_info_test = {
    .sample_rate = 16000,
    .channel_flag = 3,          //bit[0] = 1选择左声道，bit[1]=1选择右声道，可以用或运算组合
    .sample_depth = IIS_DW_16BIT,
};


void audio_block_size_set()
{
    if(3 == host_iis0_audio_in_sound_info_test.channel_flag)
    {
        audio_block_size = AUDIO_CAP_POINT_NUM_PER_FRM * sizeof(int16_t) *  2;/*双通道*/
    }
    else
    {
        audio_block_size = AUDIO_CAP_POINT_NUM_PER_FRM * sizeof(int16_t); /*单通道*/
    }
}


uint16_t audio_block_size_get()
{
    return audio_block_size;
}


/**
 * @brief 录音播音codec注册
 *
 */
void iis0_audio_in_codec_registe()
{
    audio_block_size_set();
    uint16_t block_size = audio_block_size_get();

    cm_reg_codec(IIS0_CODEC_ID, (cm_codec_hw_info_t*)&host_iis0_audio_in_hw_info_test);

    //配置录音PCM buffer
    cm_pcm_buffer_info_t record_buffer_info_str;
    record_buffer_info_str.record_buffer_info.block_num = AUDIO_IN_BUFFER_NUM;
    record_buffer_info_str.record_buffer_info.block_size = block_size;//576;
    record_buffer_info_str.record_buffer_info.buffer_size = record_buffer_info_str.record_buffer_info.block_size * record_buffer_info_str.record_buffer_info.block_num;
    record_buffer_info_str.record_buffer_info.pcm_buffer = (void*)pvPortMalloc(record_buffer_info_str.record_buffer_info.buffer_size);
    cm_config_pcm_buffer(IIS0_CODEC_ID, CODEC_INPUT, &record_buffer_info_str);
    //配置录音音频格式
    cm_config_codec(IIS0_CODEC_ID, CODEC_INPUT, (cm_sound_info_t*)&host_iis0_audio_in_sound_info_test);
    
    #if USE_IIS0_OUT_PRE_RSLT_AUDIO
    //配置放音PCM buffer
    cm_pcm_buffer_info_t pcm_buffer_info_iis0;
    pcm_buffer_info_iis0.play_buffer_info.block_num = 1;
    pcm_buffer_info_iis0.play_buffer_info.buffer_num = 6;
    pcm_buffer_info_iis0.play_buffer_info.block_size = block_size;
    pcm_buffer_info_iis0.play_buffer_info.buffer_size = pcm_buffer_info_iis0.play_buffer_info.block_size * pcm_buffer_info_iis0.play_buffer_info.block_num;
    pcm_buffer_info_iis0.play_buffer_info.pcm_buffer = pvPortMalloc(pcm_buffer_info_iis0.play_buffer_info.buffer_size * pcm_buffer_info_iis0.play_buffer_info.buffer_num);
    CI_ASSERT(pcm_buffer_info_iis0.play_buffer_info.pcm_buffer,"\n");
    cm_config_pcm_buffer(IIS0_CODEC_ID, CODEC_OUTPUT, &pcm_buffer_info_iis0);
    //配置IIS放音音频格式
    cm_config_codec(IIS0_CODEC_ID, CODEC_OUTPUT, &host_iis0_audio_in_sound_info_test);
    #endif

    //配置播音PCM buffer
    cm_pcm_buffer_info_t pcm_buffer_info;
    pcm_buffer_info.play_buffer_info.block_num = 1;
    pcm_buffer_info.play_buffer_info.buffer_num = 6;
    pcm_buffer_info.play_buffer_info.block_size = block_size;
    pcm_buffer_info.play_buffer_info.buffer_size = pcm_buffer_info.play_buffer_info.block_size * pcm_buffer_info.play_buffer_info.block_num;
    pcm_buffer_info.play_buffer_info.pcm_buffer = pvPortMalloc(pcm_buffer_info.play_buffer_info.buffer_size * pcm_buffer_info.play_buffer_info.buffer_num);
    CI_ASSERT(pcm_buffer_info.play_buffer_info.pcm_buffer,"\n");
    cm_config_pcm_buffer(PLAY_CODEC_ID, CODEC_OUTPUT, &pcm_buffer_info);
    //配置播音音频格式
    cm_config_codec(PLAY_CODEC_ID, CODEC_OUTPUT, &host_iis0_audio_in_sound_info_test);
    mprintf("iis0_audio_in_codec_registe ok\r\n");
}   


void audio_play_status_set(audio_play_status_t status)
{
    if(AUDIO_PLAY_STATUS_ON == status)
    {
        //开始播音
        cm_start_codec(PLAY_CODEC_ID, CODEC_OUTPUT);
    }
    else
    {
        //暂停播音
        cm_stop_codec(PLAY_CODEC_ID, CODEC_OUTPUT);
    }

    audio_play_status = status;
}


audio_play_status_t audio_play_status_get()
{
    return audio_play_status;
}


uint32_t audio_is_not_empty(uint32_t data_addr, uint32_t size)
{
    uint32_t *addr = (uint32_t*)data_addr;
    uint32_t data = 0;
    for(int i = 0; i < (size / 4); i++)
    {
        data = *(addr + i);
        if(data != 0)
        {
            return 1;
        }
    }
    return 0;
}

void iis0_speaker_out_task(void *p)
{   
    iis0_audio_in_codec_registe();
    //开始录音
    cm_start_codec(IIS0_CODEC_ID, CODEC_INPUT);
    
    //开始播音
    cm_start_codec(PLAY_CODEC_ID, CODEC_OUTPUT);

    for (;;)
    {
        uint32_t data_addr = 0, data_size;
        cm_read_codec(IIS0_CODEC_ID, &data_addr, &data_size);
        if (!data_addr)
        {
            mprintf("iisdma int too slow\n");
            continue;
        }
        else
        {   
            if(audio_is_not_empty(data_addr,data_size) == 1)
            {
                cm_set_codec_mute(PLAY_CODEC_ID, CODEC_OUTPUT, 3, DISABLE);
                //iis0输入音频通过喇叭输出
                uint32_t write_pcm_addr = 0;
                mprintf("5---------\r\n");
                cm_get_pcm_buffer(PLAY_CODEC_ID,&write_pcm_addr,0);    //TODO HSL
                if (write_pcm_addr)
                {
                    MASK_ROM_LIB_FUNC->newlibcfunc.memcpy_p((void*)write_pcm_addr, (void*)data_addr, data_size);
                    cm_write_codec(PLAY_CODEC_ID, (void*)write_pcm_addr,0); 
                }
            }
            else
            {
                cm_set_codec_mute(PLAY_CODEC_ID, CODEC_OUTPUT, 3, ENABLE);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
