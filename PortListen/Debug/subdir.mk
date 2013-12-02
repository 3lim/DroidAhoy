################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
<<<<<<< HEAD
../PortListener.cpp 

OBJS += \
./PortListener.o 

CPP_DEPS += \
./PortListener.d 
=======
../cube.cpp \
../listen.cpp 

OBJS += \
./cube.o \
./listen.o 

CPP_DEPS += \
./cube.d \
./listen.d 
>>>>>>> 4165f20ef4c5318cfeff49fe95bf055730cae738


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


