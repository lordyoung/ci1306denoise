/**/
#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define CI_CHIP_TYPE              1306
#define BOARD_PORT_FILE           "CI-D06GT01D.c" 

#define AEC_MODULE                  1 //  0不带AEC    1带AEC
#if AEC_MODULE == 0
#define MIC_DIFF_SINGLE             1 //1，单端。0，差分
#else
#define MIC_DIFF_SINGLE             0 //1，单端。0，差分
#endif

#define USE_INNER_LDO3              1   //0 外置1.1V   1内置1.1V
#define AUDIO_PLAYER_ENABLE         0   //用于屏蔽播放器任务相关代码      0：屏蔽，1：开启

#if AUDIO_PLAYER_ENABLE
#define USE_PROMPT_DECODER          1   //播放器是否支持prompt解码器
#define USE_MP3_DECODER             1   //为1时加入mp3解码器
#define AUDIO_PLAY_SUPPT_MP3_PROMPT 1   //播放器默认开启mp3播报音

#define AUDIO_PLAY_BLOCK_CONT 4         //播放器底层缓冲区个数
#endif

#if AEC_MODULE== 0
#define USE_AHS_MODULE             1        //使用啸叫抑制模块:1开启，0关闭
#define USE_AEC_MODULE             0        //使用回声消除模块:1开启，0关闭
#else
#define USE_AHS_MODULE             0        //使用啸叫抑制模块:1开启，0关闭
#define USE_AEC_MODULE             1        //使用回声消除模块:1开启，0关闭
#endif
#define USE_AGC_MODULE             0        //使用自动增益控制模块:1开启，0关闭
#define USE_DRC_MODULE             0        //使用动态范围压缩模块:1开启，0关闭
#define USE_EQ_MODULE              1         //使用均衡器算法模块:1开启，0关闭

#if USE_BEAMFORMING_MODULE || USE_AEC_MODULE || USE_DOA_MODULE ||USE_DEREVERB_MODULE
#define HOST_CODEC_CHA_NUM  2
#endif


#define CONFIG_CI_LOG_UART                   HAL_UART0_BASE//HAL_UART1_BASE 

#define MSG_COM_USE_UART_EN 0 
#define UART_PROTOCOL_NUMBER    HAL_UART0_BASE  // HAL_UART0_BASE ~ HAL_UART2_BASE
#define UART_PROTOCOL_BAUDRATE  UART_BaudRate9600
#define UART_PROTOCOL_VER 2 //串口协议版本号，1：一代协议，2：二代协议，255：平台生成协议(只有发送没有接收)

#define RTC_EQ_UART_PORT                     UART_PROTOCOL_NUMBER    //通话降噪EQ等参数配置使用的串口

#define USE_EXTERNAL_CRYSTAL_OSC             1      //使用外部晶振

#if (USE_EXTERNAL_CRYSTAL_OSC == 0)
#define UART_BAUDRATE_CALIBRATE         0           // 是否使能波特率校准功能
#define BAUDRATE_SYNC_PERIOD            300000      // 波特率同步周期，单位毫秒
#define BAUDRATE_FAST_SYNC_PERIOD       5000         // 一次波特率校准识别后，下一次同步周期，单位毫秒
#define BAUD_CALIBRATE_MAX_WAIT_TIME    400          // 等待反馈包的超时时间，单位毫秒


#endif

//**OTA配置
#define CI_OTA_ENABLE                (0)                 //开启OTA功能，需修改打包升级工具版本，在firmware\config.ini文件中设置成firmware_version=FW_V1，升级与被升级固件均要FW_V1版本工具打包生成
#define CI_OTA_UART_NUMBER           (1)                 //uart0:0  uart1:1  uart2:2 ,引脚需使板级配置文件中复用配置为该UART的引脚 \
                                                         (注意:bootloader中: uart1-(1306使用PB7和PC0, 其他型号使用PA2和PA3) \
                                                                            uart2-(1306使用PB1和PB2, 其他型号使用PA5和PA6)  \
                                                                            请严格按照上面管脚设计，否则会出现管脚不对应无法和 \
                                                                            bootloadr握手成功)
#define CI_OTA_UART_BAUD          (UART_BaudRate921600)  //同bootloader握手波特率

#define USE_DENOISE_NN                      1   //不可修改
#define NO_ASR_FLOW                         1   //不可修改
#define USE_DENOISE_NN_RTC                  1   //不可修改
#define INNER_CODEC_AUDIO_IN_USE_RESAMPLE   0    //不可修改

#define AUDIO_COMPRESS_SPEEX_ENABLE           1     //speex压缩数据使能
#define SPEEX_NEED_PCM_LEN                    512
#define USE_IIS0_OUT_PRE_RSLT_AUDIO 	      1//0   //通话降噪的结果通过IIS0输出
#define USE_IIS0_AUDIO_IN_SPEAKER_OUT         0   //IIS0采音通过SPEAKER输出,和USE_HP_OUT_PRE_RSLT_AUDIO互斥
#define USE_IIS0_AUDIO_IN_SPEAKER_OUT_MODE    0   //1-slave   0-master,语音芯片默认做主
#define USE_HP_OUT_PRE_RSLT_AUDIO             0//1   //通话降噪的结果通过HPOUT输出
#define SYS_BOOT_AUDIO_DATA_PREPROCESS        0   //开机音频数据预处理-通话降噪工程专用
#define SYS_BOOT_READ_EQ_DRC_FROM_NV          1   //开机自动从nv读数据，如果NV中没有存数据，则默认加载代码中固定写入的参数
#define USE_UART_SEND_PRE_RSLT_AUDIO          1    //通过串口输出和接收降噪后的音频

#if USE_UART_SEND_PRE_RSLT_AUDIO
#define UART_NUM_SEND_AUDIO_NUMBER            HAL_UART2_BASE     //通过串口输出音频数据串口号
#define UART_OUT_PRE_RSLT_AUDIO_BAUDRATE      UART_BaudRate1M   //通过串口输出音频数据波特率
#endif

//内部codec ALC的最大增益
#define INNER_CODEC_ALC_MAX_GAIN            INNER_CODEC_ALC_PGA_MAX_GAIN_16_5dB    //找开AEC此处无效， 需调试.alc_off_codec_adc_gain_mic
//通话降噪hp输出音量调节
#define RTC_HPOUT_DEFAULT_VOL               80

/*通话降噪输出降噪前，还是降噪后的数据*/
#define RTC_OUT_IS_VIA_DENOISE              1// 0：降噪之前的数据，1：降噪之后的数据

#define RTC_MIC_LEFT_RIGHT_CHOOSE           1//1：可以选择声音来自左MIC，还是右MIC 0：数据只来自于左MIC ,改时关闭IIS
#if USE_AEC_MODULE
#define RTC_MIC_LEFT_RIGHT_CHOOSE           0
#endif

#if RTC_MIC_LEFT_RIGHT_CHOOSE
#define HOST_CODEC_CHA_NUM  2
#endif


/*上下行通道的选择，默认PA5，高电平左声道输出，低电平右声道输出*/
/*注意 RTC_MIC_LEFT_RIGHT_CHOOSE  改为1*/
#define RTC_UPLINK_SWITCH_GPIO_DEFAULT      0 //1 ENABLE，0 DISABLE
#define RTC_UPLINK_SWITCH_GPIO_BASE         PA
#define RTC_UPLINK_SWITCH_GPIO              PA5
#define RTC_UPLINK_SWITCH_GPIO_PIN          pin_5
#define RTC_UPLINK_SWITCH_GPIO_IRQ          PA_IRQn

///tag7-insert-denoise_switch-start
//denoise_switch
/*降噪功能开关，默认PA6，高电平开降噪，低电平关降噪*/ 
#define RTC_DENOISE_FUNC_GPIO_DEFAULT       1 //1 ENABLE，0 DISABLE
#define RTC_DENOISE_FUNC_GPIO_BASE          PA
#define RTC_DENOISE_FUNC_GPIO               PA6
#define RTC_DENOISE_FUNC_GPIO_PIN           pin_6
#define RTC_DENOISE_FUNC_GPIO_IRQ           PA_IRQn




/*配置用户按键功能实例*/
#define RTC_USER_KEY_FUNC_GPIO_DEFAULT      0  //1 ENABLE，0 DISABLE   
#define RTC_USER_KEY_FUNC_GPIO_BASE         PA
#define RTC_USER_KEY_FUNC_GPIO              PA7
#define RTC_USER_KEY_FUNC_GPIO_PIN          pin_7
#define RTC_USER_KEY_FUNC_GPIO_IRQ          PA_IRQn


#if MSG_COM_USE_UART_EN
// 通过串口播报制定音频开关，1：开启；0：关闭
#define RTC_PLAY_VOICE_BY_UART_ENABLE       1
// 录音及录音播报功能开关，1：开启；0：关闭
#define RTC_RECORD_PLAY_VOICE_ENABLE        1
// 语音检测功能开关，1：开启；0：关闭
#define RTC_VOX_ENABLE                      1
#endif
//1500000：低灵敏度  100000：中灵敏度  50000：高灵敏度
#define RTC_VOX_SENSITIVITY                 100000
#if USE_AHS_MODULE && USE_AEC_MODULE
#error "AHS AEC can't use togher\n"
#endif

#endif /* _USER_CONFIG_H_ */
