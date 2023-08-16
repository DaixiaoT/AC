#ifndef _SPI_H_
#define _SPI_H_

void SPI_Init_Core(SPI_TypeDef *SPIx);

/* 等待忙标志位为零和发送缓冲为未满 */
static inline U8 spi_send_byte_x(SPI_TypeDef *SPIx, U8 byte);

static inline void WAIT_TX(SPI_TypeDef *SPIx,int retry_cnt=100) {
	while (((SPIx)->SR & (SPI_I2S_FLAG_TXE | SPI_I2S_FLAG_BSY)) != SPI_I2S_FLAG_TXE) {
		if((retry_cnt--)<=0) break;


	}
}

/* 等待忙标志位为零和接收缓冲器非空 */
static inline void WAIT_RX(SPI_TypeDef *SPIx, int retry_cnt=100) {
	while (((SPIx)->SR & (SPI_I2S_FLAG_BSY | SPI_I2S_FLAG_RXNE)) != SPI_I2S_FLAG_RXNE) {
		if ((retry_cnt--)<=0) break;
		

	}
}

static inline U8 spi_send_byte_x(SPI_TypeDef *SPIx, U8 byte)
{
    WAIT_TX(SPIx);
    SPIx->DR = byte;       // 发送 read X 命令
    //sleep(1);
    WAIT_RX(SPIx);
	return (U8)(SPIx->DR & 0xFF);
}

#endif //_SPI_H_

