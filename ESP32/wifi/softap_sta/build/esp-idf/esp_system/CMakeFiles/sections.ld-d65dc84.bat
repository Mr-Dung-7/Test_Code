@echo off
cd /D D:\Test_Code\ESP32\wifi\softap_sta\build\esp-idf\esp_system || (set FAIL_LINE=2& goto :ABORT)
F:\Espressif\python_env\idf5.4_py3.11_env\Scripts\python.exe F:/Espressif/frameworks/esp-idf-v5.3.1/tools/ldgen/ldgen.py --config D:/Test_Code/ESP32/wifi/softap_sta/sdkconfig --fragments-list F:/Espressif/frameworks/esp-idf-v5.3.1/components/xtensa/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_gpio/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_pm/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_mm/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/spi_flash/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_system/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_system/app.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_common/common.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_common/soc.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_rom/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/hal/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/log/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/heap/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/soc/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_hw_support/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_hw_support/dma/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_hw_support/ldo/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/freertos/linker_common.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/freertos/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/newlib/newlib.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/newlib/system_libs.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_gptimer/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_ringbuf/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_uart/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/app_trace/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_event/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_pcnt/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_spi/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_mcpwm/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_ana_cmpr/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_dac/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_rmt/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_sdm/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_i2c/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_ledc/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_parlio/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/driver/twai/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_phy/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/vfs/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/lwip/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_netif/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/wpa_supplicant/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_wifi/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_adc/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_driver_isp/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_eth/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_gdbstub/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_psram/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/esp_lcd/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/espcoredump/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/ieee802154/linker.lf;F:/Espressif/frameworks/esp-idf-v5.3.1/components/openthread/linker.lf --input D:/Test_Code/ESP32/wifi/softap_sta/build/esp-idf/esp_system/ld/sections.ld.in --output D:/Test_Code/ESP32/wifi/softap_sta/build/esp-idf/esp_system/ld/sections.ld --kconfig F:/Espressif/frameworks/esp-idf-v5.3.1/Kconfig --env-file D:/Test_Code/ESP32/wifi/softap_sta/build/config.env --libraries-file D:/Test_Code/ESP32/wifi/softap_sta/build/ldgen_libraries --objdump F:/Espressif/tools/xtensa-esp-elf/esp-14.2.0_20240906/xtensa-esp-elf/bin/xtensa-esp32-elf-objdump.exe || (set FAIL_LINE=3& goto :ABORT)
goto :EOF

:ABORT
set ERROR_CODE=%ERRORLEVEL%
echo Batch file failed at line %FAIL_LINE% with errorcode %ERRORLEVEL%
exit /b %ERROR_CODE%