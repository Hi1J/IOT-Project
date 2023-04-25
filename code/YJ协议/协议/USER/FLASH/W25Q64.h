#ifndef __W25Q64_H_
#define __W25Q64_H_

#include "stm32f10x.h"
#include "spi.h"
#include "usart.h"

#define sFLASH_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define sFLASH_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define sFLASH_CMD_WREN           0x06  /*!< Write enable instruction */
#define sFLASH_CMD_READ           0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define sFLASH_CMD_RDID           0x9F  /*!< Read identification */
#define sFLASH_CMD_SE             0x20  /*!< Sector Erase instruction */
#define sFLASH_CMD_BE             0xC7  /*!< Bulk Erase instruction */

#define sFLASH_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE         0xFF
#define sFLASH_SPI_PAGESIZE       0x100

#define sFLASH_W25Q16_ID          0xEF4015
#define sFLASH_W25Q64_ID          0xEF4017  
/**
  * @}
  */ 
  
/** @defgroup STM32_EVAL_SPI_FLASH_Exported_Macros
  * @{
  */
/**
  * @brief  Select sFLASH: Chip Select pin low
  */
#define sFLASH_CS_LOW()       (GPIO_WriteBit(GPIOD,GPIO_Pin_11,Bit_RESET))
/**
  * @brief  Deselect sFLASH: Chip Select pin high
  */
#define sFLASH_CS_HIGH()      (GPIO_WriteBit(GPIOD,GPIO_Pin_11,Bit_SET)) 
/**
  * @}
  */ 
  


/** @defgroup STM32_EVAL_SPI_FLASH_Exported_Functions
  * @{
  */
/**
  * @brief  High layer functions
  */
void sFLASH_DeInit(void);
void sFLASH_Init(void);//flash³õÊ¼»¯
void sFLASH_EraseSector(uint32_t SectorAddr);//ÉÈÇø²Á³ý
void sFLASH_EraseBulk(void);//Õû¸öÐ¾Æ¬
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);//Ò³Ð´
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);//Ð´
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);//¶Á
uint32_t sFLASH_ReadID(void);//¶ÁÐ¾Æ¬ID
void sFLASH_StartReadSequence(uint32_t ReadAddr);//¶Á²Ù×÷ÃüÁî+µØÖ·

/**
  * @brief  Low layer functions
  */
uint8_t sFLASH_ReadByte(void);//flash¶Á×Ö½Ú
uint8_t sFLASH_SendByte(uint8_t byte);//flashÐ´×Ö½Ú
uint16_t sFLASH_SendHalfWord(uint16_t HalfWord);//Ð´°ë×Ö
void sFLASH_WriteEnable(void);//Ð´Ê¹ÄÜ
void sFLASH_WaitForWriteEnd(void);//busyÎ»

void Flash_Test(void);
u8 sFLASH_Read_Byte(u32 ReadAddr);


#endif