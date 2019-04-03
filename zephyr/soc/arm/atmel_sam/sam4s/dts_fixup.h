/* This file is a temporary workaround for mapping of the generated information
 * to the current driver definitions.  This will be removed when the drivers
 * are modified to handle the generated information, or the mapping of
 * generated data matches the driver definitions.
 */

/* SoC level DTS fixup file */

#define CONFIG_NUM_IRQ_PRIO_BITS	ARM_V7M_NVIC_E000E100_ARM_NUM_IRQ_PRIORITY_BITS

#define CONFIG_GPIO_SAM_PORTA_LABEL	ATMEL_SAM_GPIO_400E0E00_LABEL
#define CONFIG_GPIO_SAM_PORTA_BASE_ADDRESS	ATMEL_SAM_GPIO_400E0E00_BASE_ADDRESS
#define CONFIG_GPIO_SAM_PORTA_IRQ	ATMEL_SAM_GPIO_400E0E00_IRQ_0
#define CONFIG_GPIO_SAM_PORTA_IRQ_PRIO	ATMEL_SAM_GPIO_400E0E00_IRQ_0_PRIORITY
#define CONFIG_GPIO_SAM_PORTA_PERIPHERAL_ID	ATMEL_SAM_GPIO_400E0E00_PERIPHERAL_ID
#define CONFIG_GPIO_SAM_PORTB_LABEL	ATMEL_SAM_GPIO_400E1000_LABEL
#define CONFIG_GPIO_SAM_PORTB_BASE_ADDRESS	ATMEL_SAM_GPIO_400E1000_BASE_ADDRESS
#define CONFIG_GPIO_SAM_PORTB_IRQ	ATMEL_SAM_GPIO_400E1000_IRQ_0
#define CONFIG_GPIO_SAM_PORTB_IRQ_PRIO	ATMEL_SAM_GPIO_400E1000_IRQ_0_PRIORITY
#define CONFIG_GPIO_SAM_PORTB_PERIPHERAL_ID	ATMEL_SAM_GPIO_400E1000_PERIPHERAL_ID
#define CONFIG_GPIO_SAM_PORTC_LABEL	ATMEL_SAM_GPIO_400E1200_LABEL
#define CONFIG_GPIO_SAM_PORTC_BASE_ADDRESS	ATMEL_SAM_GPIO_400E1200_BASE_ADDRESS
#define CONFIG_GPIO_SAM_PORTC_IRQ	ATMEL_SAM_GPIO_400E1200_IRQ_0
#define CONFIG_GPIO_SAM_PORTC_IRQ_PRIO	ATMEL_SAM_GPIO_400E1200_IRQ_0_PRIORITY
#define CONFIG_GPIO_SAM_PORTC_PERIPHERAL_ID	ATMEL_SAM_GPIO_400E1200_PERIPHERAL_ID

#define CONFIG_I2C_0_BASE_ADDRESS	ATMEL_SAM_I2C_TWI_40018000_BASE_ADDRESS
#define CONFIG_I2C_0_NAME		ATMEL_SAM_I2C_TWI_40018000_LABEL
#define CONFIG_I2C_0_BITRATE		ATMEL_SAM_I2C_TWI_40018000_CLOCK_FREQUENCY
#define CONFIG_I2C_0_IRQ		ATMEL_SAM_I2C_TWI_40018000_IRQ_0
#define CONFIG_I2C_0_IRQ_PRI		ATMEL_SAM_I2C_TWI_40018000_IRQ_0_PRIORITY
#define CONFIG_I2C_0_PERIPHERAL_ID	ATMEL_SAM_I2C_TWI_40018000_PERIPHERAL_ID
#define CONFIG_I2C_1_BASE_ADDRESS	ATMEL_SAM_I2C_TWI_4001C000_BASE_ADDRESS
#define CONFIG_I2C_1_NAME		ATMEL_SAM_I2C_TWI_4001C000_LABEL
#define CONFIG_I2C_1_BITRATE		ATMEL_SAM_I2C_TWI_4001C000_CLOCK_FREQUENCY
#define CONFIG_I2C_1_IRQ		ATMEL_SAM_I2C_TWI_4001C000_IRQ_0
#define CONFIG_I2C_1_IRQ_PRI		ATMEL_SAM_I2C_TWI_4001C000_IRQ_0_PRIORITY
#define CONFIG_I2C_1_PERIPHERAL_ID	ATMEL_SAM_I2C_TWI_4001C000_PERIPHERAL_ID

#define CONFIG_UART_SAM_PORT_0_NAME	ATMEL_SAM_UART_400E0600_LABEL
#define CONFIG_UART_SAM_PORT_0_BAUD_RATE	ATMEL_SAM_UART_400E0600_CURRENT_SPEED
#define CONFIG_UART_SAM_PORT_0_IRQ	ATMEL_SAM_UART_400E0600_IRQ_0
#define CONFIG_UART_SAM_PORT_0_IRQ_PRIO	ATMEL_SAM_UART_400E0600_IRQ_0_PRIORITY
#define CONFIG_UART_SAM_PORT_1_NAME	ATMEL_SAM_UART_400E0800_LABEL
#define CONFIG_UART_SAM_PORT_1_BAUD_RATE	ATMEL_SAM_UART_400E0800_CURRENT_SPEED
#define CONFIG_UART_SAM_PORT_1_IRQ	ATMEL_SAM_UART_400E0800_IRQ_0
#define CONFIG_UART_SAM_PORT_1_IRQ_PRIO	ATMEL_SAM_UART_400E0800_IRQ_0_PRIORITY
#define CONFIG_USART_SAM_PORT_0_NAME	ATMEL_SAM_USART_40024000_LABEL
#define CONFIG_USART_SAM_PORT_0_BAUD_RATE	ATMEL_SAM_USART_40024000_CURRENT_SPEED
#define CONFIG_USART_SAM_PORT_1_NAME	ATMEL_SAM_USART_40028000_LABEL
#define CONFIG_USART_SAM_PORT_1_BAUD_RATE	ATMEL_SAM_USART_40028000_CURRENT_SPEED

#define CONFIG_WDT_SAM_IRQ          ATMEL_SAM_WATCHDOG_400E1450_IRQ_0
#define CONFIG_WDT_SAM_IRQ_PRIORITY ATMEL_SAM_WATCHDOG_400E1450_IRQ_0_PRIORITY
#define CONFIG_WDT_SAM_LABEL        ATMEL_SAM_WATCHDOG_400E1450_LABEL
#define CONFIG_WDT_SAM_BASE_ADDRESS ATMEL_SAM_WATCHDOG_400E1450_BASE_ADDRESS
/* End of SoC Level DTS fixup file */