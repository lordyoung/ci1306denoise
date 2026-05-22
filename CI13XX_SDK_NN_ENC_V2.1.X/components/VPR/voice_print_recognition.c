
/**
 * @file voice_print_recognition.c
 * @brief 声纹识别模块
 * @version 1.0.0
 * @date 2023-03-08
 *
 * @copyright Copyright (c) 2023 Chipintelli Technology Co., Ltd.
 *
 */

#include "FreeRTOS.h"
#include "timers.h"
#include "voice_print_recognition.h"
#include "ci_nvdata_manage.h"
#include "status_share.h"
#include "ci_flash_data_info.h"
#include "vp_nn_cmpt_port_host.h"
#include "remote_api_for_host.h"
#include "user_config.h"


#define VPT_SIZE                        (192*sizeof(float))             // 模板大小
#define NVDATA_ID_VP_NUMBER             NVDATA_ID_VP_MOULD_INFO         // 存储已添加了的模板数量
#define VP_SLIDE_TIME_PER_CMPT          4                               //声纹每次计算，滑窗的次数

// 声纹模块状态枚举
typedef enum
{
    VPR_STATUS_REC,         // 识别状态
    VPR_STATUS_REG,         // 注册状态
    VPR_STATUS_IDLE,        // 空闲状态
}vpr_status_t;

// 声纹模块请求类型枚举
typedef enum
{
    VPR_REQ_NONE,           // 没有请求
    VPR_REQ_REC,            // 开始识别请求
    VPR_REQ_REG,            // 开始注册请求
    VPR_REQ_STOP,           // 停止请求
}vpr_req_t;

typedef struct
{
    vpr_status_t status;                // 模块状态
    vpr_req_t request;                  // 请求类型
    vpr_callback_t callback_func;       // 回调函数
    xTimerHandle reg_timer;             // 用于注册超时计数的TIMER
    uint8_t vpt_data[MAX_VP_TEMPLATE_NUM][VPT_SIZE];  // 存放模板的buffer
    uint8_t vpt_number;                 // 模板数量
}vpr_info_t;


static int asr_start_frm = 0;
static int asr_valid_frm = 0;
void get_asr_start_end_frm(int start,int valid)
{
    asr_start_frm = start;
    asr_valid_frm = valid;
}


static int vpr_tmp_init();
static int vpr_tmp_add();
static int vpr_tmp_del(int index);
static int vpr_tmp_delete_all();
static int vpr_tmp_get(int index, uint8_t* vpt_data_buffer, uint32_t vpt_data_buf_len);
static int vpr_tmp_get_number();
static void vpr_reg_timeout_callback(TimerHandle_t xTimer);

vpr_info_t vpr_info = {0};


/**
 * @brief 初始化声纹模块
 * @para callback_func 函数指针，用于传入事件回调函数。事件回调函数用于对外通知声纹识别相关事件，比如"模板注册成功"、"模板注册失败"、"声纹匹配成功"。
 * @return 0:初始化成功; 非0:错误号。 
 */
int vpr_init(vpr_callback_t callback_func)
{
    REMOTE_CALL(vp_nn_cmpt_cfg(VP_USE_FRM_LEN/10,VP_CMPT_SKIP_NUM));
    mprintf("VP_USE_FRM_LEN = %d\n",VP_USE_FRM_LEN);
    vpr_info.status = VPR_STATUS_IDLE;
    vpr_info.request = VPR_REQ_NONE;
    vpr_info.callback_func = callback_func;
    vpr_info.reg_timer = xTimerCreate("vpr_reg_timout", pdMS_TO_TICKS(MAX_VP_REG_TIME*1000),
        pdFALSE, (void *)0, vpr_reg_timeout_callback);
    {
        uint32_t addr;
        uint32_t size;
        get_dnn_addr_by_id(VOICE_PRINT_DNN_ID,&addr,&size);
        if((addr >= 0x1000000) || (addr <= 0x6000))
        {
            mprintf("VOICE_PRINT_DNN_ID = %d addr = %x, please check\n",VOICE_PRINT_DNN_ID,addr);
            CI_ASSERT(0,"\n");
        }
        ciss_set(CI_SS_VP_MODEL_ADDR,addr);
    }
    vpr_tmp_init();     // 模板管理初始化
    vpr_info.vpt_number = vpr_tmp_get_number();
    if (vpr_info.vpt_number > 0)
    {
        for (int i = 0;i < vpr_info.vpt_number;i++)
        {
            vpr_tmp_get(i, (uint8_t*)vpr_info.vpt_data[i], VPT_SIZE);
        }
        vpr_info.status = VPR_STATUS_REC;
        mprintf("load vpt:%d\n", vpr_info.vpt_number);
    }
    else
    {
        vpr_info.vpt_number = 0;
    }

    return 0;
}

/**
 * @brief 开始注册声纹模板
 * @return 0:启动声纹注册成功; -1:启动声纹注册失败。
 */
int vpr_start_regist()
{
    mprintf("vpr_start_regist\n");
    if (vpr_info.vpt_number < MAX_VP_TEMPLATE_NUM)
    {
        vpr_info.request = VPR_REQ_REG;
        xTimerStart(vpr_info.reg_timer, 0);
        return 0;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief 停止注册声纹模板
 * @return 0:没有错误; 非0:错误号。
 */
int vpr_stop_regist()
{
    vpr_info.request = VPR_REQ_REC;
    return 0;
}

/**
 * @brief 删除指定的声纹模板
 * @return 0:没有错误; -1:指定的模板号非有效模板(未注册或者已被删除)。
 */
int vpr_delete(int index)
{
    return vpr_tmp_del(index);
}

/**
 * @brief 删除所有声纹模板
 * @return 0:没有错误; 非0:错误号。
 */
int vpr_clear()
{
    vpr_tmp_delete_all();
    return 0;
}

/**
 * @brief 执行一次声纹识别
 * @return 0:没有错误; 非0:错误号。
 */
int vpr_run_one_recognition()
{
    static float vp_buffer_identify[192];
    static float cos_distance[VP_SLIDE_TIME_PER_CMPT][MAX_VP_TEMPLATE_NUM];
    switch(vpr_info.request)
    {
    case VPR_REQ_REC:
        vpr_info.status = VPR_STATUS_REC;
        xTimerStop(vpr_info.reg_timer, 0);
        break;
    case VPR_REQ_REG:
        vpr_info.status = VPR_STATUS_REG;
        break;
    case VPR_REQ_STOP:
        vpr_info.status = VPR_STATUS_IDLE;
        break;
    default:
        break;
    }
    vpr_info.request = VPR_REQ_NONE;

    int index = 0;
    switch(vpr_info.status)
    {
    case VPR_STATUS_REG:
        index = vpr_info.vpt_number;
        bool ret = vp_nn_cmpt(VP_CMPT_TYPE_REG,(float*)&(vpr_info.vpt_data[index]),
                                100,0,asr_start_frm,asr_valid_frm);
	    // vp_nn_cmpt_clear();
        vpr_tmp_add(&(vpr_info.vpt_data[index]), VPT_SIZE);
        vpr_info.status = VPR_STATUS_REC;
        xTimerStop(vpr_info.reg_timer, 0);
        if (vpr_info.callback_func)
        {
            vpr_info.callback_func(vpr_reg_successed, index);
        }
        mprintf("vpr reg successed\n");
        break;
    case VPR_STATUS_REC:
        if (vpr_info.vpt_number)
        {
            for(int k=0;k<VP_SLIDE_TIME_PER_CMPT;k++)
	        {
		        bool ret = vp_nn_cmpt(VP_CMPT_TYPE_IDENTIFY,vp_buffer_identify,
                                    100,k,asr_start_frm,asr_valid_frm);
		        if(ret)
		        {
                    for(int i = 0;i < vpr_info.vpt_number;i++)
                    {
			            cosin_metric((float*)&(vpr_info.vpt_data[i][0]),vp_buffer_identify,&(cos_distance[k][i]));
                        mprintf("%d,%d: %d\n", i,k,(int)(cos_distance[k][i]*1000));
                    }
		        }
	        }
            // 查找每个人的最大分数
            for (int k = 1;k < VP_SLIDE_TIME_PER_CMPT;k++)
            {
                for (int i = 0;i < vpr_info.vpt_number;i++)
                {
                    if (cos_distance[k][i] > cos_distance[0][i])
                    {
                        cos_distance[0][i] = cos_distance[k][i];
                    }
                }
            }
            int max_score_vpt_index = 0;
            // 查找最大分数的人
            for (int i = 1;i < vpr_info.vpt_number;i++)
            {
                if (cos_distance[0][i] > cos_distance[0][0])
                {
                    cos_distance[0][0] = cos_distance[0][i];
                    max_score_vpt_index = i;
                }
            }
            if (cos_distance[0][0] > 0.58f)
            {
                if (vpr_info.callback_func)
                {
                    vpr_info.callback_func(vpr_rec_successed, max_score_vpt_index);
                }
                mprintf("vpr rst: %d\n", max_score_vpt_index);
            }
            else
            {
                if (vpr_info.callback_func)
                {
                    vpr_info.callback_func(vpr_rec_failed, 0);
                }
            }
        }
	    // vp_nn_cmpt_clear();
        break;
    case VPR_STATUS_IDLE:
        break;
    default:
        break;
    }
    return 0;
}

void vpr_reg_timeout_callback(TimerHandle_t xTimer)
{
    vpr_info.request = VPR_REQ_REC;
    if (vpr_info.callback_func)
    {
        vpr_info.callback_func(vpr_reg_failed, 0);
    }
    mprintf("vrp reg timeout!\n");
}

//////////////////template manage//////////////////////
int vpr_tmp_init()
{
    uint8_t num = 0;
    uint16_t real_len;
    if(CINV_OPER_SUCCESS != cinv_item_read(NVDATA_ID_VP_NUMBER, sizeof(num), &num, &real_len))
    {
        num = 0;
        cinv_item_init(NVDATA_ID_VP_NUMBER, sizeof(num), &num);
    }
    vpr_info.vpt_number = num;
}

int vpr_tmp_add(uint8_t * vpt_data, uint32_t vpt_data_len)
{
    uint32_t save_id = NVDATA_ID_VP_NUMBER + vpr_info.vpt_number+1;
    uint32_t saved_len = 0;
    if (CINV_OPER_SUCCESS != cinv_item_write(save_id, vpt_data_len, vpt_data))
    {
        cinv_item_init(save_id, vpt_data_len, vpt_data);
    }
    vpr_info.vpt_number++;
    if (CINV_OPER_SUCCESS != cinv_item_write(NVDATA_ID_VP_NUMBER, sizeof(vpr_info.vpt_number), &(vpr_info.vpt_number)))
    {
        cinv_item_init(NVDATA_ID_VP_NUMBER, sizeof(vpr_info.vpt_number), &(vpr_info.vpt_number));
    }
    return save_id;
}

int vpr_tmp_del(int index)
{
    if (index < vpr_info.vpt_number)
    {
        uint32_t save_id = NVDATA_ID_VP_NUMBER + 1 + index;
        if(CINV_OPER_SUCCESS == cinv_item_delete(save_id))
        {
            vpr_info.vpt_number -= 1;
            cinv_item_write(NVDATA_ID_VP_NUMBER, sizeof(vpr_info.vpt_number), &(vpr_info.vpt_number));
            return 0;
        }
    }
    return -1;
}

int vpr_tmp_delete_all()
{
    for (int i = 0;i < vpr_info.vpt_number;i++)
    {
        uint32_t save_id = NVDATA_ID_VP_NUMBER + 1 + i;
        cinv_item_delete(save_id);
    }
    vpr_info.vpt_number = 0;
    cinv_item_write(NVDATA_ID_VP_NUMBER, sizeof(vpr_info.vpt_number), &(vpr_info.vpt_number));
    return 0;
}

int vpr_tmp_get(int index, uint8_t* vpt_data_buffer, uint32_t vpt_data_buf_len)
{
    uint32_t save_id = NVDATA_ID_VP_NUMBER + 1 + index;
    uint16_t real_len = 0;
    if (CINV_OPER_SUCCESS != cinv_item_read(save_id, vpt_data_buf_len, vpt_data_buffer, &real_len))
    {
        return -1;
    }
    return 0;
}

int vpr_tmp_get_number()
{
    int ret = -1;
    uint32_t num;
    uint16_t real_len;
    if (CINV_OPER_SUCCESS == cinv_item_read(NVDATA_ID_VP_NUMBER, sizeof(num), &num, &real_len))
    {
        ret = num;
    }
    return ret;
}


