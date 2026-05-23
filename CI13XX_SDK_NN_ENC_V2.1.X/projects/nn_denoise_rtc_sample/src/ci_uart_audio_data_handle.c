

#include "ci_uart_audio_data_handle.h"
#include "audio_play_process.h"
#include "stream_buffer.h"
#include "semphr.h"

#define SPEEX_SAMPLE_POINT_NUM 160
#if AUDIO_COMPRESS_SPEEX_ENABLE
#include "ci110x_speex.h"
#include "sb_celp.h"
#include "nb_celp.h"
extern SpeexMode cias_speex_wb_mode;
extern const SpeexSBMode sb_wb_mode;
extern void *sb_encoder_init(const SpeexMode *m);
extern void sb_encoder_destroy(void *state);
extern int sb_decode(void *state, SpeexBits *bits, void *vout);
extern int sb_encoder_ctl(void *state, int request, void *ptr);
extern int sb_decoder_ctl(void *state, int request, void *ptr);
extern int sb_encode(void *state, void *vin, SpeexBits *bits);
extern void *sb_decoder_init(const SpeexMode *m);
extern void sb_decoder_destroy(void *state);
ci_speex_t *ci_speex_encode_hander = NULL;
ci_speex_t *ci_speex_decode_hander = NULL;
#endif // AUDIO_COMPRESS_SPEEX_ENABLE
QueueHandle_t network_msg_recv_queue = NULL;
QueueHandle_t speex_msg_recv_queue = NULL;
#define UART_VOICE_LEN 43

static int8_t src_addr[SPEEX_NEED_PCM_LEN * 2] = {0};
static int8_t dst_addr[64] = {0};
uint8_t speex_init_flag = 0;
int8_t g_audio_rx_temp[UART_VOICE_LEN]; // AUDIO_CAP_POINT_NUM_PER_FRM*2*2
short speexOutDecFrame[320] = {0};
int8_t recv_pcm_buf[UART_VOICE_LEN] = {0};
SpeexMode *speex_wb_mode_new;
int g_audio_rx_index = 0;
StreamBufferHandle_t xSpeexPlayStreamBuffer = NULL;

#if UART_NUM_SEND_AUDIO_NUMBER == HAL_UART0_BASE
void UART0_IRQHandler(void)
#elif UART_NUM_SEND_AUDIO_NUMBER == HAL_UART1_BASE
void UART1_IRQHandler(void)
#elif UART_NUM_SEND_AUDIO_NUMBER == HAL_UART2_BASE
void UART2_IRQHandler(void)
#else
#error "please choose UART\n"
#endif
{
    UART_TypeDef *uart = (UART_TypeDef *)UART_NUM_SEND_AUDIO_NUMBER;
    /*接受数据*/
    if ((uart->UARTMIS & (1UL << UART_RXInt)) || (uart->UARTMIS & (1UL << UART_RXTimeoutInt)))
    {
        while (!(((UART_TypeDef *)UART_NUM_SEND_AUDIO_NUMBER)->UARTFlag & (0x1 << 6))) // read fifo have data
        {
            g_audio_rx_temp[g_audio_rx_index++] = (int8_t)UartPollingReceiveData(uart);
            // mprintf("%02x ", g_audio_rx_temp[g_audio_rx_index-1]);
            if (g_audio_rx_index == UART_VOICE_LEN)
            {
                g_audio_rx_index = 0;
                if (xQueueSendFromISR(network_msg_recv_queue, g_audio_rx_temp, pdFALSE) == pdFALSE)
                    mprintf("----queue send err------\n");
                memset(g_audio_rx_temp, 0, UART_VOICE_LEN);
            }
            // else
            // {
            //      memset(g_audio_rx_temp, 0, UART_VOICE_LEN);
            // }
            // mprintf("rx_data = 0x%x\r\n", rx_data);
        }
        UART_IntClear(uart, UART_RXInt);
    }
    (uart)->UARTICR = 0xFFF;
}

int speex_recv_data_task(void)
{
    int decLen = 0;
    speex_init_flag = 1;


//    // cm_config_pcm_buffer(PLAY_CODEC_ID, CODEC_OUTPUT, &pcm_buffer_info);

    UARTDMAConfig((UART_TypeDef *)UART_NUM_SEND_AUDIO_NUMBER, UART_OUT_PRE_RSLT_AUDIO_BAUDRATE);
    pad_config_for_uart(UART_NUM_SEND_AUDIO_NUMBER);
   // audio_play_hw_start(ENABLE);
    //audio_play_hw_start(ENABLE, &audio_format_info);
    //   int16_t left_data[UART_VOICE_LEN/4] = {0};
    //   int16_t right_data[UART_VOICE_LEN/4] = {0};
    network_msg_recv_queue = xQueueCreate(200, UART_VOICE_LEN);
    mprintf("ci_speex_decode_hander->_frame_size = %d\r\n", ci_speex_decode_hander->_frame_size);
#if 1
    while (1)
    {
        // mprintf("start recv...\r\n");
        xQueueReceive(network_msg_recv_queue, recv_pcm_buf, portMAX_DELAY);
        for (int i = 0; i < UART_VOICE_LEN / 43; i++)
        {
            //  mprintf("start recv1...\r\n");
            // 解码
            // mprintf("recv_pcm_buf[0] = %x\r\n", recv_pcm_buf[0]);
            if (recv_pcm_buf[0] != 0x2a) // 错误数据
            {
                mprintf("error data..\r\n");
                continue;
            }
            // for(int i = 0; i < 43; i++)
            // {
            //     mprintf("%02x ", recv_pcm_buf[i]);
            // }
            speex_bits_read_from(&ci_speex_decode_hander->_bits, &recv_pcm_buf[1], 42);
            // mprintf("start recv2...\r\n");
            memset(speexOutDecFrame, 0, 320 * sizeof(short));
            //  mprintf("start recv3...\r\n");
            decLen = speex_decode_int(ci_speex_decode_hander->_decoder, &ci_speex_decode_hander->_bits, speexOutDecFrame);
            int8_t *src_addr = (int8_t *)speexOutDecFrame;
            int ret = xStreamBufferSend(xSpeexPlayStreamBuffer, src_addr, 320 * sizeof(int16_t), 1000);
            if (ret != 320 * sizeof(int16_t))
            {
                mprintf("xSpeexPlayStreamBuffer send error: ret = %d\r\n", ret);
            }
            // mprintf("decLen = %d\r\n", decLen);
            // uint32_t write_pcm_addr_cpy = 0;
            // // for(int i = 320 - 16; i < 320; i++)
            // // {
            // //     mprintf("%04x ", speexOutDecFrame[i]);
            // // }
            // cm_get_pcm_buffer(PLAY_CODEC_ID, &write_pcm_addr_cpy, portMAX_DELAY); // TODO HSL
            // int16_t *pcm_data_p_cpy = (int16_t *)write_pcm_addr_cpy;
            // memcpy(pcm_data_p_cpy, speexOutDecFrame, ci_speex_decode_hander->_frame_size * sizeof(short));
#if 0
        for(int i = 0; i < UART_VOICE_LEN/4; i++)
        {
            left_data[i] = recv_pcm_buf[4*i];
            left_data[i] |= (recv_pcm_buf[4*i + 1] << 8);
            right_data[i] = recv_pcm_buf[4*i + 2];
            right_data[i] |= recv_pcm_buf[4*i + 3] << 8;
        }
        for(int i = 0; i < UART_VOICE_LEN/4; i++)
        {
           // mprintf("i= %d\r\n", i);
            pcm_data_p_cpy[2*i] = left_data[i];
            pcm_data_p_cpy[2*i + 1] = right_data[i];
        }
#endif

            //  memset(left_data, 0, 256);
            // memset(right_data, 0, 256);

            // cm_config_pcm_buffer(PLAY_CODEC_ID, CODEC_OUTPUT, &pcm_buffer_info);
            // audio_play_hw_start(ENABLE, &audio_format_info);
            // xQueueReceive(network_msg_recv_queue, recv_pcm_buf, portMAX_DELAY);
            // uint32_t ret_buf;
            // while (!(cm_get_codec_empty_buffer_number(PLAY_CODEC_ID, CODEC_OUTPUT) > 1))
            //     ;
            // cm_get_pcm_buffer(PLAY_CODEC_ID, &ret_buf, portMAX_DELAY);
            // memcpy(ret_buf, recv_pcm_buf, 256);
            // cm_write_codec(PLAY_CODEC_ID, ret_buf, portMAX_DELAY);
            // memset(recv_pcm_buf, 0, 256);
        }
    }
#endif
}

#define ONE_AUDIO_FRAME_LEN 512
int8_t speex_play_temp[ONE_AUDIO_FRAME_LEN] = {0};
int16_t speex_play_data[256] = {0};
int speex_play_data_task()
{
    xSpeexPlayStreamBuffer = xStreamBufferCreate(640 * 10, 640);
    int index = 0;
    if (xSpeexPlayStreamBuffer == NULL)
    {
        mprintf("xSpeexRecordStreamBuffer create error...\r\n");
        // 处理错误情况
    }
    int stream_aviable_len = 0;
    while (1)
    {
        // speex_mutex_take();
        stream_aviable_len = xStreamBufferBytesAvailable(xSpeexPlayStreamBuffer);
        //  mprintf("stream_aviable_len = %d\r\n", stream_aviable_len);
        // speex_mutex_give();
        // mprintf("len = %d\r\n", len);
        if (stream_aviable_len >= ONE_AUDIO_FRAME_LEN)
        {
            memset(speex_play_temp, 0, ONE_AUDIO_FRAME_LEN);
            int rx_size = xStreamBufferReceive(xSpeexPlayStreamBuffer, speex_play_temp, ONE_AUDIO_FRAME_LEN, portMAX_DELAY);
            if (rx_size != ONE_AUDIO_FRAME_LEN)
            {
                mprintf("xSpeexPlayStreamBuffer rcv error\r\n");
            }
            else
            {
                uint32_t write_pcm_addr_cpy = 0;
                cm_get_pcm_buffer(PLAY_CODEC_ID, &write_pcm_addr_cpy, portMAX_DELAY); // TODO HSL
                // while(!(cm_get_codec_empty_buffer_number(PLAY_CODEC_ID,CODEC_OUTPUT) > 1))
                // {
                //     ;
                // }
                memcpy(speex_play_data, speex_play_temp, ONE_AUDIO_FRAME_LEN);
                
                int16_t *pcm_data_p_cpy = (int16_t *)write_pcm_addr_cpy;
                for(int i = 0; i < 256; i++)
                {
                    pcm_data_p_cpy[2*i] = speex_play_data[i];
                    pcm_data_p_cpy[2*i + 1] = speex_play_data[i];

                }
                //memcpy(pcm_data_p_cpy, speex_play_temp, ONE_AUDIO_FRAME_LEN);
                if (0 == write_pcm_addr_cpy)
                {
                    continue;
                }
                cm_write_codec(PLAY_CODEC_ID, (void *)write_pcm_addr_cpy, 0);
                if(index++ == 0)
                cm_start_codec(PLAY_CODEC_ID, CODEC_OUTPUT);


            }
        }
        else
        {
            /* 无数据时必须延时，否则该任务在优先级4持续忙等，
             * 抢占其他音频任务的CPU，导致IIS DMA underrun和周期性截断 */
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}
StreamBufferHandle_t xSpeexRecordStreamBuffer = NULL;
int8_t speex_dst_addr[64] = {0};
int8_t speex_rx_temp[SPEEX_SAMPLE_POINT_NUM * 2 * 2] = {0}; // AUDIO_CAP_POINT_NUM_PER_FRM*2*2
int speex_record_data_task(void)
{
    // speex_msg_recv_queue = xQueueCreate(10, 128);
    xSpeexRecordStreamBuffer = xStreamBufferCreate(1024 * 20, 640);
    if (xSpeexRecordStreamBuffer == NULL)
    {
        mprintf("xSpeexRecordStreamBuffer create error...\r\n");
        vTaskDelete(NULL);
        return 0;
        // 处理错误情况
    }
    //  while(1)
    // {

    // }

    int speex_rx_index = 0;
    int encode_len = 0;
    int stream_aviable_len = 0;
    int rx_size = 0;
    while (1)
    {
        // speex_mutex_take();
        stream_aviable_len = xStreamBufferBytesAvailable(xSpeexRecordStreamBuffer);
        //  mprintf("stream_aviable_len = %d\r\n", stream_aviable_len);
        // speex_mutex_give();
        // mprintf("len = %d\r\n", len);
        if (stream_aviable_len >= SPEEX_SAMPLE_POINT_NUM * 2 * 2)
        {
            //  speex_mutex_take();
            memset(speex_rx_temp, 0, sizeof(speex_rx_temp));
            int rx_size = xStreamBufferReceive(xSpeexRecordStreamBuffer, speex_rx_temp, SPEEX_SAMPLE_POINT_NUM * 2 * 2, portMAX_DELAY);
            if (rx_size != SPEEX_SAMPLE_POINT_NUM * 2 * 2)
            {
                mprintf("xSpeexRecordStreamBuffer rcv error\r\n");
            }
            else
            {
                //    int encode_len = cias_speex_compressed_data(ci_speex_encode_hander, src_addr, dst_addr, 320);     //压缩音频数据
                //   memset(speex_dst_addr, 0, sizeof(speex_dst_addr));
                //   int encode_len = cias_speex_compressed_data(ci_speex_encode_hander, speex_rx_temp, speex_dst_addr, SPEEX_SAMPLE_POINT_NUM*2);     //压缩音频数据
                //    audio_pre_rslt_write_data_from_uart((uint32_t)speex_dst_addr, encode_len);
                for (int k = 0; k < SPEEX_SAMPLE_POINT_NUM / 160; k++)
                {
                    memset(speex_dst_addr, 0, sizeof(speex_dst_addr));
                    encode_len = cias_speex_compressed_data(ci_speex_encode_hander, speex_rx_temp, speex_dst_addr, 320); // 压缩音频数据
                    audio_pre_rslt_write_data_from_uart((uint32_t)speex_dst_addr, encode_len);
                    vTaskDelay(1); // 必须加延时，不然会丢数据
                    // mprintf("--------------------------------------\r\n");
                    // for(int i = 0; i < encode_len; i++)
                    // {
                    //     mprintf("%02x ", speex_dst_addr[i]);
                    // }
                    // network_send(speex_dst_addr, encode_len);
                    // audio_pre_rslt_write_data_from_uart((uint32_t)(&speex_rx_temp[320*k]), 320);
                    // vTaskDelay(5);
                    // mprintf("rx_size = %d, k = %d, encode_len = %d\r\n", rx_size, k, encode_len);
                }
                // mprintf("=================================done==================================\r\n");
                // audio_pre_rslt_write_data_from_uart((uint32_t)speex_rx_temp, SPEEX_SAMPLE_POINT_NUM*2);
                // memset(speex_rx_temp, 0, sizeof(speex_rx_temp));
            }
            //  speex_mutex_give();
        }
        else { 
            taskYIELD(); 
        }
        // xQueueReceive(speex_msg_recv_queue, &speex_rx_temp[speex_rx_index], portMAX_DELAY);
        // speex_rx_index++;
        // if(speex_rx_index == 160)
        // {
        // //    int encode_len = cias_speex_compressed_data(ci_speex_encode_hander, speex_rx_temp, dst_addr, 320);     //压缩音频数据
        // //    mprintf("encode_len = %d\r\n", encode_len);
        // //    audio_pre_rslt_write_data_from_uart((uint32_t)dst_addr, encode_len);
        //    speex_rx_index = 0;
        // }
    }
}
bool audio_speex_task_init(void)
{
#if AUDIO_COMPRESS_SPEEX_ENABLE
    cias_speex_wb_mode.mode = &sb_wb_mode;
    cias_speex_wb_mode.query = wb_mode_query;
    cias_speex_wb_mode.modeName = pvPortMalloc(strlen("wideband (sub-band CELP)"));
    memcpy(cias_speex_wb_mode.modeName, "wideband (sub-band CELP)", strlen("wideband (sub-band CELP)"));
    cias_speex_wb_mode.modeID = 1;
    cias_speex_wb_mode.bitstream_version = 4;
    cias_speex_wb_mode.enc_init = sb_encoder_init;
    cias_speex_wb_mode.enc_destroy = sb_encoder_destroy;
    cias_speex_wb_mode.enc = sb_encode;
    cias_speex_wb_mode.enc_ctl = sb_encoder_ctl;
    cias_speex_wb_mode.dec_init = sb_decoder_init;
    cias_speex_wb_mode.dec_destroy = sb_decoder_destroy;
    cias_speex_wb_mode.dec = sb_decode;
    cias_speex_wb_mode.dec_ctl = sb_decoder_ctl;

    ci_speex_encode_hander = ci_speex_encode_create(); // 初始化编码speex
    if (NULL == ci_speex_encode_hander)
    {
        mprintf("ci_speex_encode_hander is null\r\n");
        return false;
    }
    ci_speex_encode_hander->ci_speex_mode = CI_SPEEX_INT;
    ci_speex_decode_hander = ci_speex_decode_create(); // 初始化解码speex

    if (NULL == ci_speex_decode_hander)
    {
        mprintf("ci_speex_decode_hander is null\r\n");

        return false;
    }
    ci_speex_decode_hander->ci_speex_mode = CI_SPEEX_INT;
    xTaskCreate(speex_recv_data_task, "speex_recv_data_task", 1024 * 2, NULL, 4, NULL);
    xTaskCreate(speex_record_data_task, "speex_record_data_task", 1024 * 2, NULL, 4, NULL);
    xTaskCreate(speex_play_data_task, "speex_play_data_task", 1024 * 2, NULL, 4, NULL);
#endif
    return true;
}