# CMSIS-Driver configuration

There are multiple ways to configure a CMSIS-Driver.


## Via the RTE_Device.h file

The classical method is using the *RTE_Device.h* file that comes with the device support. The 

file configures the drivers to the actual pin connection of the microcontroller device. 

Using the *Configuration Wizard* view of *RTE_Device.h*, it can configure the driver 

interfaces in a graphical mode without the need to edit manually the #defines in this 

header file. 


## Via STM32CubeMX

The next method is  CMSIS-Driver configuring using **STM32CubeMX**. This graphical software 

configuration tool allows you to generate C initialization code using graphical wizards 

for STMicroelectronics devices.



> Keil MDK-ARM supports using the *STM32Cube Framework*  for STM32 devices that can be 
> 
> configured with a classical method using the *RTE_Device.h* configuration file or 
> 
> by using *STM32CubeMX*. Based on the configuration data *RTE_Components*.h file is generated 
> 
> and *STM32 HAL library* is included. 


## This Project

The use of *STM32Cube Framework* and  *STM32 HAL library* is not intended in the Project.

The classical method via the *RTE_Device.h* file and the *Configuration Wizard* is applied.

Further the #defines in *RTE_Device.h* file is manually used in Driver files.


