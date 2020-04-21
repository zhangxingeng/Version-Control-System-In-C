################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/client.c \
../src/sendfile.c \
../src/server.c \
../src/socket_lib.c 

OBJS += \
./src/client.o \
./src/sendfile.o \
./src/server.o \
./src/socket_lib.o 

C_DEPS += \
./src/client.d \
./src/sendfile.d \
./src/server.d \
./src/socket_lib.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


