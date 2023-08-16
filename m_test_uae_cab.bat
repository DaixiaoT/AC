call core_path.bat

call d.bat

del src/cpu_main.o

set ProjectName=ac_test_cab_uae
set AppDef="-DPLL_M=8 -DPLL_Q=7 -D_AC_BASE_ -D_HW_TEST_ -DLAN9303 -D_CAN_ -D_CAN_20A_ -D_UART6_ -D_BTIM3_ -D_UART4_  -D_BTIM5_ -D_I2C2_ -D_BTIM6_ -D_RTC_1208_ -D_NOT_G0_"

del %ProjectName%.hex
del %ProjectName%.bin
del src\cpu_main.o

make PROJECT=%ProjectName% CORE=%COER_PATH% COMM_LIB=%COMM_PATH% APP_DEF=%AppDef% LSCRIPT=iap.ld CPU

copy %ProjectName%.hex out\%ProjectName%.hex
copy %ProjectName%.bin out\%ProjectName%.bin

