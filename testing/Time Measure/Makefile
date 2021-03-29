# Define CROSS_CC_PATH to be the path of cross-compiler on your system
CROSS_CC_PATH=/home/ubuntu/openwrt-zsun-zsun/staging_dir/toolchain-mips_mips32_gcc-4.8-linaro_uClibc-0.9.33.2/bin

#Define STAGING_DIR variable with the path on your system
#export STAGING_DIR=/home/linuxbill/Embedded_Systems/openwrt-zsun-zsun/staging_dir/toolchain-mips_mips32_gcc-4.8-linaro_uClibc-0.9.33.2/bin
CC1 = gcc
CC2 = $(CROSS_CC_PATH)/./mips-openwrt-linux-gcc 
 

EXECUTABLES = client_sender client_receiver server_threads_time_meas

all: $(EXECUTABLES)

client_sender: client_sender.c
		$(CC1) $< -o $@	

client_receiver: client_receiver.c
		$(CC1) $< -o $@	

server_threads_time_meas: server_threads_time_meas.c
		$(CC2) $< -o $@ -lpthread



