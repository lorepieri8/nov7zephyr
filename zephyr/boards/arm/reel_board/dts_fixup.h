#define CONFIG_FXOS8700_NAME			NORDIC_NRF_I2C_40003000_NXP_FXOS8700_1D_LABEL
#define CONFIG_FXOS8700_I2C_NAME		NORDIC_NRF_I2C_40003000_NXP_FXOS8700_1D_BUS_NAME
#define CONFIG_FXOS8700_I2C_ADDRESS		NORDIC_NRF_I2C_40003000_NXP_FXOS8700_1D_BASE_ADDRESS
#define CONFIG_FXOS8700_GPIO_NAME		NORDIC_NRF_I2C_40003000_NXP_FXOS8700_1D_INT1_GPIOS_CONTROLLER
#define CONFIG_FXOS8700_GPIO_PIN		NORDIC_NRF_I2C_40003000_NXP_FXOS8700_1D_INT1_GPIOS_PIN

#define CONFIG_HDC1008_NAME			NORDIC_NRF_I2C_40003000_TI_HDC1008_43_LABEL
#define CONFIG_HDC1008_I2C_MASTER_DEV_NAME 	NORDIC_NRF_I2C_40003000_TI_HDC1008_43_BUS_NAME
#define CONFIG_HDC1008_I2C_ADDR 		NORDIC_NRF_I2C_40003000_TI_HDC1008_43_BASE_ADDRESS
#define CONFIG_HDC1008_GPIO_DEV_NAME		NORDIC_NRF_I2C_40003000_TI_HDC1008_43_DRDY_GPIOS_CONTROLLER
#define CONFIG_HDC1008_GPIO_PIN_NUM		NORDIC_NRF_I2C_40003000_TI_HDC1008_43_DRDY_GPIOS_PIN
#define CONFIG_HDC1008_GPIO_FLAGS		NORDIC_NRF_I2C_40003000_TI_HDC1008_43_DRDY_GPIOS_FLAGS

#define CONFIG_APDS9960_DRV_NAME		NORDIC_NRF_I2C_40003000_AVAGO_APDS9960_29_LABEL
#define CONFIG_APDS9960_I2C_DEV_NAME		NORDIC_NRF_I2C_40003000_AVAGO_APDS9960_29_BUS_NAME
#define CONFIG_APDS9960_GPIO_DEV_NAME		NORDIC_NRF_I2C_40003000_AVAGO_APDS9960_29_INT_GPIOS_CONTROLLER
#define CONFIG_APDS9960_GPIO_PIN_NUM		NORDIC_NRF_I2C_40003000_AVAGO_APDS9960_29_INT_GPIOS_PIN

#define CONFIG_SSD1673_DEV_NAME			NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_LABEL
#define CONFIG_SSD1673_SPI_FREQ			NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_SPI_MAX_FREQUENCY
#define CONFIG_SSD1673_SPI_DEV_NAME		NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_BUS_NAME
#define CONFIG_SSD1673_SPI_SLAVE_NUMBER		NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_BASE_ADDRESS
#define CONFIG_SSD1673_SPI_GPIO_CS		y
#define CONFIG_SSD1673_SPI_GPIO_CS_DRV_NAME	NORDIC_NRF_SPI_4002B000_CS_GPIOS_CONTROLLER
#define CONFIG_SSD1673_SPI_GPIO_CS_PIN		NORDIC_NRF_SPI_4002B000_CS_GPIOS_PIN

#define CONFIG_SSD1673_RESET_GPIO_PORT_NAME	NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_RESET_GPIOS_CONTROLLER
#define CONFIG_SSD1673_RESET_PIN		NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_RESET_GPIOS_PIN
#define CONFIG_SSD1673_DC_GPIO_PORT_NAME	NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_DC_GPIOS_CONTROLLER
#define CONFIG_SSD1673_DC_PIN			NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_DC_GPIOS_PIN
#define CONFIG_SSD1673_BUSY_GPIO_PORT_NAME	NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_BUSY_GPIOS_CONTROLLER
#define CONFIG_SSD1673_BUSY_PIN			NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_BUSY_GPIOS_PIN
#define CONFIG_SSD1673_ORIENTATION_FLIPPED	NORDIC_NRF_SPI_4002B000_SOLOMON_SSD1673FB_SPI_0_ORIENTATION_FLIPPED