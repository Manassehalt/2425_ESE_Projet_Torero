#define ADXL343_CS_GPIO GPIOA
#define ADXL343_CS_PIN GPIO_PIN_5

#define ADXL343_REG_DEVID (0x00)
#define ADXL343_REG_THRESH_TAP (0x1D)
#define ADXL343_REG_OFSX (0x1E)
#define ADXL343_REG_OFSY (0x1F)
#define ADXL343_REG_OFSZ (0x20)
#define ADXL343_REG_DUR (0x21)
#define ADXL343_REG_LATENT 0x22
#define ADXL343_REG_ACT_INACT_CTL (0x27)
#define ADXL343_REG_TAP_AXES (0x2A)
#define ADXL343_REG_BW_RATE (0x2C)
#define ADXL343_REG_POWER_CTL (0x2D)
#define ADXL343_REG_FIFO_CTL (0x38)
#define ADXL343_REG_INT_ENABLE (0x2E)
#define ADXL343_REG_INT_MAP (0x2F)
#define ADXL343_REG_INT_SOURCE (0x30)
#define ADXL343_REG_DATA_FORMAT (0x31)
#define ADXL343_REG_DATAX0 (0x32)
#define ADXL343_REG_DATAX1 (0x33)
#define ADXL343_REG_DATAY0 (0x34)
#define ADXL343_REG_DATAY1 (0x35)
#define ADXL343_REG_DATAZ0 (0x36)
#define ADXL343_REG_DATAZ1 (0x37)

void ADXL343_Init(void);
HAL_StatusTypeDef Read_Acceleration(float *accel_data);
uint8_t SPI_Read(uint8_t reg);
void SPI_Write(uint8_t reg, uint8_t value);
