#ifndef CI_AGC_H
#define CI_AGC_H
#endif

#include <stdio.h>
typedef struct 
{
	float compress_gain;
	float target_db;
	float agc_gain;
	int32_t vox_Th;
	int16_t vox_Wake;
	int16_t vox_Delay;
	bool compute_agc;
}agc_config_t;

#ifdef __cplusplus
extern "C" {
#endif
    int ci_agc_version(void);
	void* ci_agc_create(void* module_config);
	int ci_agc_deal(void* handle, short* pcm_in, short* pcm_out);

#ifdef __cplusplus
}
#endif


