# This file is maked by run generate_makefile.lua
OBJS += build/objs/ci130x_init.o
-include build/objs/ci130x_init.d
build/objs/ci130x_init.o : $(ROOT_DIR)/startup/ci130x_init.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_vtable.o
-include build/objs/ci130x_vtable.d
build/objs/ci130x_vtable.o : $(ROOT_DIR)/startup/ci130x_vtable.S
	$(CC_PREFIX)$(AS) $(S_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_startup.o
-include build/objs/ci130x_startup.d
build/objs/ci130x_startup.o : $(ROOT_DIR)/startup/ci130x_startup.S
	$(CC_PREFIX)$(AS) $(S_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_it.o
-include build/objs/ci130x_it.d
build/objs/ci130x_it.o : $(ROOT_DIR)/system/ci130x_it.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_system.o
-include build/objs/ci130x_system.d
build/objs/ci130x_system.o : $(ROOT_DIR)/system/ci130x_system.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/platform_config.o
-include build/objs/platform_config.d
build/objs/platform_config.o : $(ROOT_DIR)/system/platform_config.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_handlers.o
-include build/objs/ci130x_handlers.d
build/objs/ci130x_handlers.o : $(ROOT_DIR)/system/ci130x_handlers.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/baudrate_calibrate.o
-include build/objs/baudrate_calibrate.d
build/objs/baudrate_calibrate.o : $(ROOT_DIR)/system/baudrate_calibrate.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/debug_time_consuming.o
-include build/objs/debug_time_consuming.d
build/objs/debug_time_consuming.o : $(ROOT_DIR)/components/assist/debug_time_consuming.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/croutine.o
-include build/objs/croutine.d
build/objs/croutine.o : $(ROOT_DIR)/components/freertos/croutine.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/event_groups.o
-include build/objs/event_groups.d
build/objs/event_groups.o : $(ROOT_DIR)/components/freertos/event_groups.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/list.o
-include build/objs/list.d
build/objs/list.o : $(ROOT_DIR)/components/freertos/list.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/queue.o
-include build/objs/queue.d
build/objs/queue.o : $(ROOT_DIR)/components/freertos/queue.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/stream_buffer.o
-include build/objs/stream_buffer.d
build/objs/stream_buffer.o : $(ROOT_DIR)/components/freertos/stream_buffer.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/tasks.o
-include build/objs/tasks.d
build/objs/tasks.o : $(ROOT_DIR)/components/freertos/tasks.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/timers.o
-include build/objs/timers.d
build/objs/timers.o : $(ROOT_DIR)/components/freertos/timers.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/heap_4.o
-include build/objs/heap_4.d
build/objs/heap_4.o : $(ROOT_DIR)/components/freertos/portable/MemMang/heap_4.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci_log.o
-include build/objs/ci_log.d
build/objs/ci_log.o : $(ROOT_DIR)/components/log/ci_log.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ota_aes.o
-include build/objs/ota_aes.d
build/objs/ota_aes.o : $(ROOT_DIR)/components/ota/ota_aes.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ota_partition_verify.o
-include build/objs/ota_partition_verify.d
build/objs/ota_partition_verify.o : $(ROOT_DIR)/components/ota/ota_partition_verify.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/FreeRTOS_CLI.o
-include build/objs/FreeRTOS_CLI.d
build/objs/FreeRTOS_CLI.o : $(ROOT_DIR)/components/assist/cli/FreeRTOS_CLI.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/UARTCommandConsole.o
-include build/objs/UARTCommandConsole.d
build/objs/UARTCommandConsole.o : $(ROOT_DIR)/components/assist/cli/UARTCommandConsole.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/serial.o
-include build/objs/serial.d
build/objs/serial.o : $(ROOT_DIR)/components/assist/cli/port/serial.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/Sample_CLI_commands.o
-include build/objs/Sample_CLI_commands.d
build/objs/Sample_CLI_commands.o : $(ROOT_DIR)/components/assist/cli/port/Sample_CLI_commands.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci_flash_data_info.o
-include build/objs/ci_flash_data_info.d
build/objs/ci_flash_data_info.o : $(ROOT_DIR)/components/flash_control/flash_control_src/ci_flash_data_info.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/flash_control_inner_port.o
-include build/objs/flash_control_inner_port.d
build/objs/flash_control_inner_port.o : $(ROOT_DIR)/components/flash_control/flash_control_src/flash_control_inner_port.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/audio_play_api.o
-include build/objs/audio_play_api.d
build/objs/audio_play_api.o : $(ROOT_DIR)/components/player/audio_play/audio_play_api.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/audio_play_decoder.o
-include build/objs/audio_play_decoder.d
build/objs/audio_play_decoder.o : $(ROOT_DIR)/components/player/audio_play/audio_play_decoder.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/audio_play_process.o
-include build/objs/audio_play_process.d
build/objs/audio_play_process.o : $(ROOT_DIR)/components/player/audio_play/audio_play_process.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/audio_play_os_port.o
-include build/objs/audio_play_os_port.d
build/objs/audio_play_os_port.o : $(ROOT_DIR)/components/player/audio_play/audio_play_os_port.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/audio_play_device.o
-include build/objs/audio_play_device.d
build/objs/audio_play_device.o : $(ROOT_DIR)/components/player/audio_play/audio_play_device.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/get_play_data.o
-include build/objs/get_play_data.d
build/objs/get_play_data.o : $(ROOT_DIR)/components/player/audio_play/get_play_data.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/adpcmdec.o
-include build/objs/adpcmdec.d
build/objs/adpcmdec.o : $(ROOT_DIR)/components/player/adpcm/adpcmdec.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/adpcm.o
-include build/objs/adpcm.d
build/objs/adpcm.o : $(ROOT_DIR)/components/player/adpcm/adpcm.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/parse_m4a_atom_containers_port.o
-include build/objs/parse_m4a_atom_containers_port.d
build/objs/parse_m4a_atom_containers_port.o : $(ROOT_DIR)/components/player/m4a/parse_m4a_atom_containers_port.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/parse_m4a_atom_containers.o
-include build/objs/parse_m4a_atom_containers.d
build/objs/parse_m4a_atom_containers.o : $(ROOT_DIR)/components/player/m4a/parse_m4a_atom_containers.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/bitstreamf.o
-include build/objs/bitstreamf.d
build/objs/bitstreamf.o : $(ROOT_DIR)/components/player/flacdec/bitstreamf.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/flacdecoder.o
-include build/objs/flacdecoder.d
build/objs/flacdecoder.o : $(ROOT_DIR)/components/player/flacdec/flacdecoder.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/tables.o
-include build/objs/tables.d
build/objs/tables.o : $(ROOT_DIR)/components/player/flacdec/tables.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/status_share.o
-include build/objs/status_share.d
build/objs/status_share.o : $(ROOT_DIR)/components/status_share/status_share.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci_nvdata_manage.o
-include build/objs/ci_nvdata_manage.d
build/objs/ci_nvdata_manage.o : $(ROOT_DIR)/components/ci_nvdm/ci_nvdata_manage.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci_nvdata_port.o
-include build/objs/ci_nvdata_port.d
build/objs/ci_nvdata_port.o : $(ROOT_DIR)/components/ci_nvdm/ci_nvdata_port.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/command_file_reader.o
-include build/objs/command_file_reader.d
build/objs/command_file_reader.o : $(ROOT_DIR)/components/cmd_info/command_file_reader.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/command_info.o
-include build/objs/command_info.d
build/objs/command_info.o : $(ROOT_DIR)/components/cmd_info/command_info.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/prompt_player.o
-include build/objs/prompt_player.d
build/objs/prompt_player.o : $(ROOT_DIR)/components/cmd_info/prompt_player.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/audio_in_manage_inner.o
-include build/objs/audio_in_manage_inner.d
build/objs/audio_in_manage_inner.o : $(ROOT_DIR)/components/audio_in_manage/audio_in_manage_inner.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci_key.o
-include build/objs/ci_key.d
build/objs/ci_key.o : $(ROOT_DIR)/components/ci_key/ci_key.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/asr_malloc_port.o
-include build/objs/asr_malloc_port.d
build/objs/asr_malloc_port.o : $(ROOT_DIR)/components/asr/asr_malloc_port.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/asr_process_callback_decoder.o
-include build/objs/asr_process_callback_decoder.d
build/objs/asr_process_callback_decoder.o : $(ROOT_DIR)/components/asr/asr_process_callback_decoder.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/asr_process_callback.o
-include build/objs/asr_process_callback.d
build/objs/asr_process_callback.o : $(ROOT_DIR)/components/asr/asr_process_callback.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/codec_manager.o
-include build/objs/codec_manager.d
build/objs/codec_manager.o : $(ROOT_DIR)/components/codec_manager/codec_manager.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/codec_manage_inner_port.o
-include build/objs/codec_manage_inner_port.d
build/objs/codec_manage_inner_port.o : $(ROOT_DIR)/components/codec_manager/codec_manage_inner_port.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/voice_module_uart_protocol_g1.o
-include build/objs/voice_module_uart_protocol_g1.d
build/objs/voice_module_uart_protocol_g1.o : $(ROOT_DIR)/components/msg_com/voice_module_uart_protocol_g1.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/i2c_protocol_module.o
-include build/objs/i2c_protocol_module.d
build/objs/i2c_protocol_module.o : $(ROOT_DIR)/components/msg_com/i2c_protocol_module.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/voice_module_uart_protocol.o
-include build/objs/voice_module_uart_protocol.d
build/objs/voice_module_uart_protocol.o : $(ROOT_DIR)/components/msg_com/voice_module_uart_protocol.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/color_light_control.o
-include build/objs/color_light_control.d
build/objs/color_light_control.o : $(ROOT_DIR)/components/led/color_light_control.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/led_light_control.o
-include build/objs/led_light_control.d
build/objs/led_light_control.o : $(ROOT_DIR)/components/led/led_light_control.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_audio_pre_rslt_out.o
-include build/objs/ci130x_audio_pre_rslt_out.d
build/objs/ci130x_audio_pre_rslt_out.o : $(ROOT_DIR)/components/audio_pre_rslt_iis_out/ci130x_audio_pre_rslt_out.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_core_eclic.o
-include build/objs/ci130x_core_eclic.d
build/objs/ci130x_core_eclic.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_core_eclic.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_core_timer.o
-include build/objs/ci130x_core_timer.d
build/objs/ci130x_core_timer.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_core_timer.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_dma.o
-include build/objs/ci130x_dma.d
build/objs/ci130x_dma.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_dma.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_codec.o
-include build/objs/ci130x_codec.d
build/objs/ci130x_codec.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_codec.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_pdm.o
-include build/objs/ci130x_pdm.d
build/objs/ci130x_pdm.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_pdm.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_alc.o
-include build/objs/ci130x_alc.d
build/objs/ci130x_alc.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_alc.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_iisdma.o
-include build/objs/ci130x_iisdma.d
build/objs/ci130x_iisdma.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_iisdma.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_core_misc.o
-include build/objs/ci130x_core_misc.d
build/objs/ci130x_core_misc.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_core_misc.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_iis.o
-include build/objs/ci130x_iis.d
build/objs/ci130x_iis.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_iis.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_adc.o
-include build/objs/ci130x_adc.d
build/objs/ci130x_adc.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_adc.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_gpio.o
-include build/objs/ci130x_gpio.d
build/objs/ci130x_gpio.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_gpio.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_iic.o
-include build/objs/ci130x_iic.d
build/objs/ci130x_iic.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_iic.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_pwm.o
-include build/objs/ci130x_pwm.d
build/objs/ci130x_pwm.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_pwm.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_timer.o
-include build/objs/ci130x_timer.d
build/objs/ci130x_timer.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_timer.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_uart.o
-include build/objs/ci130x_uart.d
build/objs/ci130x_uart.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_uart.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_lowpower.o
-include build/objs/ci130x_lowpower.d
build/objs/ci130x_lowpower.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_lowpower.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_iwdg.o
-include build/objs/ci130x_iwdg.d
build/objs/ci130x_iwdg.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_iwdg.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_spiflash.o
-include build/objs/ci130x_spiflash.d
build/objs/ci130x_spiflash.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_spiflash.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci130x_dtrflash.o
-include build/objs/ci130x_dtrflash.d
build/objs/ci130x_dtrflash.o : $(ROOT_DIR)/driver/ci130x_chip_driver/src/ci130x_dtrflash.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/board.o
-include build/objs/board.d
build/objs/board.o : $(ROOT_DIR)/driver/boards/board.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/board_default.o
-include build/objs/board_default.d
build/objs/board_default.o : $(ROOT_DIR)/driver/boards/board_default.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/dichotomy_find.o
-include build/objs/dichotomy_find.d
build/objs/dichotomy_find.o : $(ROOT_DIR)/utils/dichotomy_find.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/crc.o
-include build/objs/crc.d
build/objs/crc.o : $(ROOT_DIR)/utils/crc.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/main.o
-include build/objs/main.d
build/objs/main.o : $(ROOT_DIR)/projects/nn_denoise_rtc_sample/src/main.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/iis0_sample.o
-include build/objs/iis0_sample.d
build/objs/iis0_sample.o : $(ROOT_DIR)/projects/nn_denoise_rtc_sample/src/iis0_sample.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/system_hook.o
-include build/objs/system_hook.d
build/objs/system_hook.o : $(ROOT_DIR)/projects/nn_denoise_rtc_sample/src/system_hook.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/system_msg_deal.o
-include build/objs/system_msg_deal.d
build/objs/system_msg_deal.o : $(ROOT_DIR)/projects/nn_denoise_rtc_sample/src/system_msg_deal.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/user_msg_deal.o
-include build/objs/user_msg_deal.d
build/objs/user_msg_deal.o : $(ROOT_DIR)/projects/nn_denoise_rtc_sample/src/user_msg_deal.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci_ssp_config.o
-include build/objs/ci_ssp_config.d
build/objs/ci_ssp_config.o : $(ROOT_DIR)/projects/nn_denoise_rtc_sample/src/ci_ssp_config.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci_uart_audio_data_handle.o
-include build/objs/ci_uart_audio_data_handle.d
build/objs/ci_uart_audio_data_handle.o : $(ROOT_DIR)/projects/nn_denoise_rtc_sample/src/ci_uart_audio_data_handle.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci_uart_eq_param.o
-include build/objs/ci_uart_eq_param.d
build/objs/ci_uart_eq_param.o : $(ROOT_DIR)/projects/nn_denoise_rtc_sample/src/ci_uart_eq_param.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/bits.o
-include build/objs/bits.d
build/objs/bits.o : $(ROOT_DIR)/components/cias_speex/libspeex/bits.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/cb_search.o
-include build/objs/cb_search.d
build/objs/cb_search.o : $(ROOT_DIR)/components/cias_speex/libspeex/cb_search.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/exc_5_64_table.o
-include build/objs/exc_5_64_table.d
build/objs/exc_5_64_table.o : $(ROOT_DIR)/components/cias_speex/libspeex/exc_5_64_table.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/exc_5_256_table.o
-include build/objs/exc_5_256_table.d
build/objs/exc_5_256_table.o : $(ROOT_DIR)/components/cias_speex/libspeex/exc_5_256_table.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/exc_8_128_table.o
-include build/objs/exc_8_128_table.d
build/objs/exc_8_128_table.o : $(ROOT_DIR)/components/cias_speex/libspeex/exc_8_128_table.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/exc_10_16_table.o
-include build/objs/exc_10_16_table.d
build/objs/exc_10_16_table.o : $(ROOT_DIR)/components/cias_speex/libspeex/exc_10_16_table.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/exc_10_32_table.o
-include build/objs/exc_10_32_table.d
build/objs/exc_10_32_table.o : $(ROOT_DIR)/components/cias_speex/libspeex/exc_10_32_table.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/exc_20_32_table.o
-include build/objs/exc_20_32_table.d
build/objs/exc_20_32_table.o : $(ROOT_DIR)/components/cias_speex/libspeex/exc_20_32_table.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/filters.o
-include build/objs/filters.d
build/objs/filters.o : $(ROOT_DIR)/components/cias_speex/libspeex/filters.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/gain_table.o
-include build/objs/gain_table.d
build/objs/gain_table.o : $(ROOT_DIR)/components/cias_speex/libspeex/gain_table.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/gain_table_lbr.o
-include build/objs/gain_table_lbr.d
build/objs/gain_table_lbr.o : $(ROOT_DIR)/components/cias_speex/libspeex/gain_table_lbr.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/hexc_10_32_table.o
-include build/objs/hexc_10_32_table.d
build/objs/hexc_10_32_table.o : $(ROOT_DIR)/components/cias_speex/libspeex/hexc_10_32_table.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/hexc_table.o
-include build/objs/hexc_table.d
build/objs/hexc_table.o : $(ROOT_DIR)/components/cias_speex/libspeex/hexc_table.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/high_lsp_tables.o
-include build/objs/high_lsp_tables.d
build/objs/high_lsp_tables.o : $(ROOT_DIR)/components/cias_speex/libspeex/high_lsp_tables.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/kiss_fft.o
-include build/objs/kiss_fft.d
build/objs/kiss_fft.o : $(ROOT_DIR)/components/cias_speex/libspeex/kiss_fft.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/kiss_fftr.o
-include build/objs/kiss_fftr.d
build/objs/kiss_fftr.o : $(ROOT_DIR)/components/cias_speex/libspeex/kiss_fftr.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/lpc.o
-include build/objs/lpc.d
build/objs/lpc.o : $(ROOT_DIR)/components/cias_speex/libspeex/lpc.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/lsp.o
-include build/objs/lsp.d
build/objs/lsp.o : $(ROOT_DIR)/components/cias_speex/libspeex/lsp.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/lsp_tables_nb.o
-include build/objs/lsp_tables_nb.d
build/objs/lsp_tables_nb.o : $(ROOT_DIR)/components/cias_speex/libspeex/lsp_tables_nb.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ltp.o
-include build/objs/ltp.d
build/objs/ltp.o : $(ROOT_DIR)/components/cias_speex/libspeex/ltp.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/modes.o
-include build/objs/modes.d
build/objs/modes.o : $(ROOT_DIR)/components/cias_speex/libspeex/modes.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/modes_wb.o
-include build/objs/modes_wb.d
build/objs/modes_wb.o : $(ROOT_DIR)/components/cias_speex/libspeex/modes_wb.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/nb_celp.o
-include build/objs/nb_celp.d
build/objs/nb_celp.o : $(ROOT_DIR)/components/cias_speex/libspeex/nb_celp.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/quant_lsp.o
-include build/objs/quant_lsp.d
build/objs/quant_lsp.o : $(ROOT_DIR)/components/cias_speex/libspeex/quant_lsp.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/sb_celp.o
-include build/objs/sb_celp.d
build/objs/sb_celp.o : $(ROOT_DIR)/components/cias_speex/libspeex/sb_celp.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/smallft.o
-include build/objs/smallft.d
build/objs/smallft.o : $(ROOT_DIR)/components/cias_speex/libspeex/smallft.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/speex.o
-include build/objs/speex.d
build/objs/speex.o : $(ROOT_DIR)/components/cias_speex/libspeex/speex.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/speex_callbacks.o
-include build/objs/speex_callbacks.d
build/objs/speex_callbacks.o : $(ROOT_DIR)/components/cias_speex/libspeex/speex_callbacks.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/stereo.o
-include build/objs/stereo.d
build/objs/stereo.o : $(ROOT_DIR)/components/cias_speex/libspeex/stereo.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/vbr.o
-include build/objs/vbr.d
build/objs/vbr.o : $(ROOT_DIR)/components/cias_speex/libspeex/vbr.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/vorbis_psy.o
-include build/objs/vorbis_psy.d
build/objs/vorbis_psy.o : $(ROOT_DIR)/components/cias_speex/libspeex/vorbis_psy.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/vq.o
-include build/objs/vq.d
build/objs/vq.o : $(ROOT_DIR)/components/cias_speex/libspeex/vq.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/window.o
-include build/objs/window.d
build/objs/window.o : $(ROOT_DIR)/components/cias_speex/libspeex/window.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

OBJS += build/objs/ci110x_speex.o
-include build/objs/ci110x_speex.d
build/objs/ci110x_speex.o : $(ROOT_DIR)/components/cias_speex/port/ci110x_speex.c
	$(CC_PREFIX)$(CC) $(C_FLAGS) -c -o "$@" "$<"

C_FLAGS += -I$(ROOT_DIR)/components/cias_speex/include/speex
C_FLAGS += -I$(ROOT_DIR)/components/cias_speex/include/ogg
C_FLAGS += -I$(ROOT_DIR)/components/cias_speex/include
C_FLAGS += -I$(ROOT_DIR)/components/cias_speex/libspeex
C_FLAGS += -I$(ROOT_DIR)/components/cias_speex/port
LIB_FILES += $(ROOT_DIR)/$(LIBS_PATH)/libasr.a
LD_FLAGS += -L$(ROOT_DIR)/$(LIBS_PATH)
LIBS += -lasr
LIB_FILES += $(ROOT_DIR)/$(LIBS_PATH)/libeq.a
LD_FLAGS += -L$(ROOT_DIR)/$(LIBS_PATH)
LIBS += -leq
LIB_FILES += $(ROOT_DIR)/$(LIBS_PATH)/libdrc.a
LD_FLAGS += -L$(ROOT_DIR)/$(LIBS_PATH)
LIBS += -ldrc
LIB_FILES += $(ROOT_DIR)/$(LIBS_PATH)/libnewlib_port.a
LD_FLAGS += -L$(ROOT_DIR)/$(LIBS_PATH)
LIBS += -lnewlib_port
LIB_FILES += $(ROOT_DIR)/$(LIBS_PATH)/libfreertos_port.a
LD_FLAGS += -L$(ROOT_DIR)/$(LIBS_PATH)
LIBS += -lfreertos_port
LIB_FILES += $(ROOT_DIR)/$(LIBS_PATH)/libdsu.a
LD_FLAGS += -L$(ROOT_DIR)/$(LIBS_PATH)
LIBS += -ldsu
LIB_FILES += $(ROOT_DIR)/$(LIBS_PATH)/libflash_encrypt.a
LD_FLAGS += -L$(ROOT_DIR)/$(LIBS_PATH)
LIBS += -lflash_encrypt
C_FLAGS += -I$(ROOT_DIR)/driver/ci130x_chip_driver/inc
C_FLAGS += -I$(ROOT_DIR)/driver/boards
C_FLAGS += -I$(ROOT_DIR)/driver/third_device_driver/outside_codec
C_FLAGS += -I$(ROOT_DIR)/system
C_FLAGS += -I$(ROOT_DIR)/components/log
C_FLAGS += -I$(ROOT_DIR)/components/assist
C_FLAGS += -I$(ROOT_DIR)/components/assist/cli
C_FLAGS += -I$(ROOT_DIR)/components/assist/cli/port
C_FLAGS += -I$(ROOT_DIR)/components/freertos/include
C_FLAGS += -I$(ROOT_DIR)/components/freertos/portable/GCC/N307
C_FLAGS += -I$(ROOT_DIR)/components
C_FLAGS += -I$(ROOT_DIR)/components/alg/ai_denoise_rtc
C_FLAGS += -I$(ROOT_DIR)/components/asr
C_FLAGS += -I$(ROOT_DIR)/components/asr/asr_top
C_FLAGS += -I$(ROOT_DIR)/components/asr/asr_top/asr_top_inc
C_FLAGS += -I$(ROOT_DIR)/components/asr/decoder/decoder_inc
C_FLAGS += -I$(ROOT_DIR)/components/asr/vad_fe
C_FLAGS += -I$(ROOT_DIR)/components/asr/vad_fe/vad_fe_inc
C_FLAGS += -I$(ROOT_DIR)/components/asr/dnn
C_FLAGS += -I$(ROOT_DIR)/components/asr/dnn/dnn_inc
C_FLAGS += -I$(ROOT_DIR)/components/asr/cinn/cinn_inc
C_FLAGS += -I$(ROOT_DIR)/components/asr/npu/npu_inc
C_FLAGS += -I$(ROOT_DIR)/components/asr/nn_and_flash
C_FLAGS += -I$(ROOT_DIR)/components/asr/nn_and_flash/nn_and_flash_inc
C_FLAGS += -I$(ROOT_DIR)/components/fft
C_FLAGS += -I$(ROOT_DIR)/components/ci_key
C_FLAGS += -I$(ROOT_DIR)/components/alg/drc
C_FLAGS += -I$(ROOT_DIR)/components/alg/eq
C_FLAGS += -I$(ROOT_DIR)/components/alg/agc
C_FLAGS += -I$(ROOT_DIR)/components/msg_com
C_FLAGS += -I$(ROOT_DIR)/components/led
C_FLAGS += -I$(ROOT_DIR)/components/player/audio_play
C_FLAGS += -I$(ROOT_DIR)/components/player/mp3lib/mp3pub
C_FLAGS += -I$(ROOT_DIR)/components/player/aaclib/aacpub
C_FLAGS += -I$(ROOT_DIR)/components/player/flacdec
C_FLAGS += -I$(ROOT_DIR)/components/player/m4a
C_FLAGS += -I$(ROOT_DIR)/components/player/adpcm
C_FLAGS += -I$(ROOT_DIR)/components/status_share
C_FLAGS += -I$(ROOT_DIR)/components/flash_control/flash_control_inc
C_FLAGS += -I$(ROOT_DIR)/components/flash_encrypt
C_FLAGS += -I$(ROOT_DIR)/components/codec_manager
C_FLAGS += -I$(ROOT_DIR)/components/ci_nvdm
C_FLAGS += -I$(ROOT_DIR)/components/cmd_info
C_FLAGS += -I$(ROOT_DIR)/components/sys_monitor
C_FLAGS += -I$(ROOT_DIR)/components/ota
C_FLAGS += -I$(ROOT_DIR)/components/audio_pre_rslt_iis_out
C_FLAGS += -I$(ROOT_DIR)/components/audio_in_manage
C_FLAGS += -I$(ROOT_DIR)/components/assist/SEGGER
C_FLAGS += -I$(ROOT_DIR)/components/assist/SEGGER/config
C_FLAGS += -I$(ROOT_DIR)/components/nuclear_com
C_FLAGS += -I$(ROOT_DIR)/components/audio_pre_rslt_iis_out
C_FLAGS += -I$(ROOT_DIR)/projects/nn_denoise_rtc_sample/src
C_FLAGS += -I$(ROOT_DIR)/utils
C_FLAGS += -I$(ROOT_DIR)/components/alg
C_FLAGS += -I$(ROOT_DIR)/components/alg/denoise
C_FLAGS += -I$(ROOT_DIR)/components/alg/beamforming
C_FLAGS += -I$(ROOT_DIR)/components/alg/dereverb
C_FLAGS += -I$(ROOT_DIR)/components/alg/drc
C_FLAGS += -I$(ROOT_DIR)/components/alg/alc_auto_switch
C_FLAGS += -I$(ROOT_DIR)/components/alg/basic_alg
C_FLAGS += -I$(ROOT_DIR)/components/alg/aec
C_FLAGS += -I$(ROOT_DIR)/components/alg/doa
C_FLAGS += -I$(ROOT_DIR)/components/alg/eq
