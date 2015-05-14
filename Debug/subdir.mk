################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../font_control.c \
../main.c \
../particle_ctrl.c \
../scene_control.c \
../snake_control.c \
../texture_mgr.c 

OBJS += \
./font_control.o \
./main.o \
./particle_ctrl.o \
./scene_control.o \
./snake_control.o \
./texture_mgr.o 

C_DEPS += \
./font_control.d \
./main.d \
./particle_ctrl.d \
./scene_control.d \
./snake_control.d \
./texture_mgr.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


