################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ReadFCIDUMP.cpp \
../src/diagonalizer.cpp \
../src/main.cpp \
../src/rhf.cpp \
../src/rhf_equations.cpp \
../src/test_ReadFCIDUMP.cpp \
../src/test_diagonalizer.cpp \
../src/test_rhf.cpp \
../src/test_rhf_equations.cpp \
../src/testhelpers.cpp 

OBJS += \
./src/ReadFCIDUMP.o \
./src/diagonalizer.o \
./src/main.o \
./src/rhf.o \
./src/rhf_equations.o \
./src/test_ReadFCIDUMP.o \
./src/test_diagonalizer.o \
./src/test_rhf.o \
./src/test_rhf_equations.o \
./src/testhelpers.o 

CPP_DEPS += \
./src/ReadFCIDUMP.d \
./src/diagonalizer.d \
./src/main.d \
./src/rhf.d \
./src/rhf_equations.d \
./src/test_ReadFCIDUMP.d \
./src/test_diagonalizer.d \
./src/test_rhf.d \
./src/test_rhf_equations.d \
./src/testhelpers.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/work/TACO/taco-master/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


