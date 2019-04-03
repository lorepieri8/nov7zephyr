/* SoC level DTS fixup file */

#define CONFIG_NUM_IRQ_PRIO_BITS	ARM_V7M_NVIC_E000E100_ARM_NUM_IRQ_PRIORITY_BITS
#define CONFIG_UART_CC32XX_NAME		TI_CC32XX_UART_4000C000_LABEL

#define CONFIG_I2C_0_LABEL		TI_CC32XX_I2C_40020000_LABEL
#define CONFIG_I2C_0_BASE_ADDRESS	TI_CC32XX_I2C_40020000_BASE_ADDRESS
#define CONFIG_I2C_0_BITRATE		TI_CC32XX_I2C_40020000_CLOCK_FREQUENCY
#define CONFIG_I2C_0_IRQ		TI_CC32XX_I2C_40020000_IRQ_0
#define CONFIG_I2C_0_IRQ_PRIORITY	TI_CC32XX_I2C_40020000_IRQ_0_PRIORITY

#define CONFIG_GPIO_CC32XX_A0_BASE_ADDRESS	TI_CC32XX_GPIO_40004000_BASE_ADDRESS
#define CONFIG_GPIO_CC32XX_A0_IRQ		TI_CC32XX_GPIO_40004000_IRQ_0
#define CONFIG_GPIO_CC32XX_A0_IRQ_PRI		TI_CC32XX_GPIO_40004000_IRQ_0_PRIORITY
#define CONFIG_GPIO_CC32XX_A0_NAME		TI_CC32XX_GPIO_40004000_LABEL

#define CONFIG_GPIO_CC32XX_A1_BASE_ADDRESS	TI_CC32XX_GPIO_40005000_BASE_ADDRESS
#define CONFIG_GPIO_CC32XX_A1_IRQ		TI_CC32XX_GPIO_40005000_IRQ_0
#define CONFIG_GPIO_CC32XX_A1_IRQ_PRI		TI_CC32XX_GPIO_40005000_IRQ_0_PRIORITY
#define CONFIG_GPIO_CC32XX_A1_NAME		TI_CC32XX_GPIO_40005000_LABEL

#define CONFIG_GPIO_CC32XX_A2_BASE_ADDRESS	TI_CC32XX_GPIO_40006000_BASE_ADDRESS
#define CONFIG_GPIO_CC32XX_A2_IRQ		TI_CC32XX_GPIO_40006000_IRQ_0
#define CONFIG_GPIO_CC32XX_A2_IRQ_PRI		TI_CC32XX_GPIO_40006000_IRQ_0_PRIORITY
#define CONFIG_GPIO_CC32XX_A2_NAME		TI_CC32XX_GPIO_40006000_LABEL

#define CONFIG_GPIO_CC32XX_A3_BASE_ADDRESS	TI_CC32XX_GPIO_40007000_BASE_ADDRESS
#define CONFIG_GPIO_CC32XX_A3_IRQ		TI_CC32XX_GPIO_40007000_IRQ_0
#define CONFIG_GPIO_CC32XX_A3_IRQ_PRI		TI_CC32XX_GPIO_40007000_IRQ_0_PRIORITY
#define CONFIG_GPIO_CC32XX_A3_NAME		TI_CC32XX_GPIO_40007000_LABEL

/* End of SoC Level DTS fixup file */