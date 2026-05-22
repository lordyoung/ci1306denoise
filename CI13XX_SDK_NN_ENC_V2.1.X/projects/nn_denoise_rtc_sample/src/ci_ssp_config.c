/**
  ******************************************************************************
  * @file    ci_ssp_config.c
  * @version V1.0.0
  * @date    2021.08.17
  * @brief 
  ******************************************************************************
  */

#include <stdint.h>
#include <stdbool.h>

#include "alg_preprocess.h"
#include "ci_audio_wrapfft.h"
#include "ci_denoise.h"
#include "ci_bf.h"
#include "ci_dereverb.h"
#include "alc_auto_switch.h"
#include "ci_adapt_aec.h"
#include "ci_doa.h"
#include "ai_denoise_api_rtc.h"
#include "ci_drc.h"
#include "ci_eq.h"
#include "ci_agc.h"

#include "sdk_default_config.h"
#include "status_share.h"
#include "user_config.h"
//采音板音频输出配置
const iis_out_audio_config_t iis_out_audio_config =
{
	.iis_out_enable = USE_IIS0_OUT_PRE_RSLT_AUDIO || USE_HP_OUT_PRE_RSLT_AUDIO || USE_UART_SEND_PRE_RSLT_AUDIO,			 //用于采音。
	#if RTC_OUT_IS_VIA_DENOISE
	.iis_left_channel = DST1,		 //左通道输出音频。MICL、MICR、REFL、REFR、DST1、DST2
	.iis_right_channel =MICL,		 //右通道输出音频。MICL、MICR、REFL、REFR、DST1、DST2
	#else
	.iis_left_channel = MICL,		 //左通道输出音频。MICL、MICR、REFL、REFR、DST1、DST2
	.iis_right_channel =DST1,		 //右通道输出音频。MICL、MICR、REFL、REFR、DST1、DST2
	#endif
	.vad_mark_enable = false,		 //是否附带vad标签，默认左通道输出vad标签。
	.ssp_dst_cover_micl_enble = false //处理过后的音频dst覆盖原始micl数据,16k采样数据
};

//stft_istft模块配置
//#define FFT_MODEL 512
const stft_istft_config_t stft_istft_config =
{
//#if 512 == FFT_MODEL
#if !INNER_CODEC_AUDIO_IN_USE_RESAMPLE
	.sample_frequency = 16000, //采样率
	.frame_size = 512,		   //窗长
	.frame_shift = AUDIO_CAP_POINT_NUM_PER_FRM,		   //帧移
	.fft_frm_size = 512,	   //fft输入有效长度的点数
	.fft_size = 257,		   //fft的正频分量点数-1+直流分量点数
	.result_out_channel = 1,   // 输出通道数result_out_channel;
	.psd_compute_channel_num = 1,//计算mic的psd
	.time_pre_emphasis_enable = false,
	.downsampled_enable = false, 
	.fe_psd_enable = true,		 //是否计算psd，以计算特征
	#if USE_AHS_MODULE
	.compute_ahs = true,
	#endif
#else
	.sample_frequency = 32000, //采样率
	.frame_size = 1024,		   //窗长
	.frame_shift = AUDIO_CAP_POINT_NUM_PER_FRM * 2,		   //帧移
	.fft_frm_size = 1024,	   //fft输入有效长度的点数
	.fft_size = 513,		   //fft的正频分量点数-1+直流分量点数
	.result_out_channel = 1,   // 输出通道数result_out_channel;
	.psd_compute_channel_num = 0,//计算原mic的psd
	.time_pre_emphasis_enable = false,
	.downsampled_enable = true, 
	.fe_psd_enable = false		//是否计算psd，以计算特征
#endif
};


//ai_denoise模块配置
denoise_nn_config_t ai_denoise_config = 
{
	.denoise_mode = 0,             //降噪等级可配，0为一级降噪，1为二级降噪，适配denoise_version=1
	.denoise_version = 2		  //降噪模型版本可配，1为适配[60003]nn_denoise_rtc_va00x.bin降噪模型,适用于远场降噪；2为适配[60003]nn_denoise_rtc_vb00x.bin降噪模型,适用于近场降噪，默认为2
};

//EQ 默认配置
///tag4-EQ-param-set
//EQ_param_set
eq_config_t eq_config = 
{
	.gain[0] = 0.0f, .bw[0] = 35.0f, .fc[0] = 62.0f, .type[0] = PEAKING,
	.gain[1] = 0.0f, .bw[1] = 65.0f, .fc[1] = 125.0f, .type[1] = PEAKING,
	.gain[2] = 0.0f, .bw[2] = 150.0f, .fc[2] = 250.0f, .type[2] = PEAKING,
	.gain[3] = 0.0f, .bw[3] = 300.0f, .fc[3] = 500.0f, .type[3] = PEAKING,
	.gain[4] = 0.0f, .bw[4] = 650.0f, .fc[4] = 1000.0f, .type[4] = PEAKING,
	.gain[5] = 0.0f, .bw[5] = 1250.0f, .fc[5] = 2000.0f, .type[5] = PEAKING,
	.gain[6] = 0.0f, .bw[6] = 1650.0f, .fc[6] = 3000.0f, .type[6] = PEAKING,
	.gain[7] = 0.0f, .bw[7] = 2250.0f, .fc[7] = 4000.0f, .type[7] = PEAKING,
	.gain[8] = 0.0f, .bw[8] = 3500.0f, .fc[8] = 7000.0f, .type[8] = PEAKING,
	.eq_gain_from_uart = false
};



//默认配置
drc_config_t drc_config = 
{
	.attack_time = 0.01f,
	.release_time = 0.015f,
	.thr_comp = -10.0f,          //幅值压缩阈值dB
	.thr_exp = -25.0f,			//幅值拉伸阈值dB
	.thr_gate = -35.0f,			//幅值置为0阈值dB
	.ratio = 6.0f,
	.knee = 3.0f,
	.drc_para_from_uart = false,
	.gate_denoise = false,
	.gate_denoise_only = true
};

//agc模块配置
agc_config_t agc_config = 
{
	//AGC参数
	.compress_gain = 40,
	.target_db = 15,
	.agc_gain = 1.5f,
	//VOX参数
	.vox_Th = RTC_VOX_SENSITIVITY,		//1500000：低灵敏度  100000：中灵敏度  50000：高灵敏度
	.vox_Wake = 5,			//连续(vox_Wake*0.1)秒检测到语音，进入触发状态
	.vox_Delay = 1,			//连续vox_Delay秒没有检测到语音，进入休眠状态
	#if VOX_0_AGC_1 || VOX_1_AGC_1
	.compute_agc = true,
	#endif
};

// aec模块配置
const aec_config_t aec_config =
{

	.mic_channel_num = 1,						   //麦克风信号通道数
	.ref_channel_num = 1,						   //参考信号通道数
	.aec_control_mode = COMPUTE_REF_AMPL_MODE, //ENABLE_PLAYING_STATE_MODE:根据播报状态进行aec控制;  COMPUTE_REF_AMPL_MODE:根据参考幅值大小进行aec控制
	.aec_gain = 1.0f,							   //增益数值
	.aec_enable_threshold = 50.0f,		//参考信号判断门限值
    .aec_mic_div_ref_thr = 0.0005f,    
	.fft_size = 256,						//频域处理频点数
	.alc_off_codec_adc_gain_mic = 16, 
    .alc_off_codec_adc_gain_ref = 12,
	.aec_model_a_b = 1,						//AEC算法模式，aec_model_a_b 默认配置为1
	#if USE_DENOISE_NN_RTC
	.lowpass_enable = 0,					// 开启低通滤波处理：1； 关闭低通滤波处理：0
	.aec_beta = 0.95f,
	.aec_beta_ = 0.05f,
	.aec_lamda = 0.7f,
	.aec_lamda_ = 0.3f,
	.aec_eta = 0.3f,
	.aec_eta_ = 0.7f,
	.lowpass_gain = 0.1f,					// 衰减增益
	.cutoff_freq = 6000.0f,					// 低通滤波截止频率
	#endif
};



//.module_config域配置为NULL表示关闭此项。
ci_ssp_config_t ci_ssp = {
	#if USE_ALC_AUTO_SWITCH_MODULE
	.alc_auto_switch = 	{.module_config = &alc_auto_switch_config},
	#else
	.alc_auto_switch = 	{.module_config = NULL},
	#endif

	.stft = 			{.module_config = &stft_istft_config},

	#if USE_AEC_MODULE
	.aec = 				{.module_config = &aec_config},
	#else
	.aec = 				{.module_config = NULL},
	#endif

	#if USE_DOA_MODULE
	.doa = 				{.module_config = &doa_config},
	#else
	.doa = 				{.module_config = NULL},
	#endif

	#if USE_DEREVERB_MODULE
	.dereverb = 		{.module_config = &dereverb_config},
	#else
	.dereverb = 		{.module_config = NULL},
	#endif

	#if USE_BEAMFORMING_MODULE
	.bf = 				{.module_config = &bf_config},
	#else
	.bf = 				{.module_config = NULL},
	#endif

	#if USE_DENOISE_NN
	.ai_denoise = 		{.module_config = &ai_denoise_config},
	#else
	.ai_denoise = 		{.module_config = NULL},
	#endif
	
	#if USE_DENOISE_MODULE
	.denoise = 			{.module_config = &denoise_config},
	#else
	.denoise = 			{.module_config = NULL},
	#endif

	.istft = 			{.module_config = &stft_istft_config},

	#if USE_AGC_MODULE
	.agc  =             {.module_config = &agc_config},
	#else
	.agc  =  		    {.module_config = NULL},
	#endif

	#if USE_EQ_MODULE
	.eq  =              {.module_config = &eq_config},
	#else
	.eq  =  		    {.module_config = NULL},
	#endif
	
	#if USE_DRC_MODULE
	.drc  =             {.module_config = &drc_config},
	#else
	.drc  =             {.module_config = NULL},
	#endif

	.iis_out_audio = 	{.module_config = &iis_out_audio_config},
};


audio_capture_t audio_capture =
{
    #if INNER_CODEC_AUDIO_IN_USE_RESAMPLE
    .frame_length = AUDIO_CAP_POINT_NUM_PER_FRM * 2, // frame_length;32k采样，输入点数为320
    #else
    .frame_length = AUDIO_CAP_POINT_NUM_PER_FRM, // frame_length;16k采样，输入点数为160
    #endif

    #if USE_BEAMFORMING_MODULE || USE_DOA_MODULE || USE_DEREVERB_MODULE
    .mic_channel_num = 2, // mic_channel_num;
    #else
    .mic_channel_num = 1,                        // mic_channel_num;
    #endif

    #if USE_AEC_MODULE
    .ref_channel_num = 1, //ref_channel_num;
    #else
    .ref_channel_num = 0,                        //ref_channel_num;
    #endif
    // MICL // mic_model;
};


volatile bool src_from_right_mic = false;
/**
 * @brief Set the src from right mic object//设置左MIC或者右MIC的语音作为输入
 * 
 * @param is_right  true： 右MIC
 * 					false：左MIC
 */
void set_src_from_right_mic(bool is_right)
{
	src_from_right_mic = is_right;
}


bool get_src_from_right_mic(void)
{
	return src_from_right_mic;
}

