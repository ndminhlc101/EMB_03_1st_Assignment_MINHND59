################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CAN.c \
../src/FTM.c \
../src/GPIO.c \
../src/LPIT.c \
../src/Middleware.c \
../src/PORT.c \
../src/UART.c \
../src/clock_and_mode.c \
../src/main.c 

OBJS += \
./src/CAN.o \
./src/FTM.o \
./src/GPIO.o \
./src/LPIT.o \
./src/Middleware.o \
./src/PORT.o \
./src/UART.o \
./src/clock_and_mode.o \
./src/main.o 

C_DEPS += \
./src/CAN.d \
./src/FTM.d \
./src/GPIO.d \
./src/LPIT.d \
./src/Middleware.d \
./src/PORT.d \
./src/UART.d \
./src/clock_and_mode.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/CAN.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


