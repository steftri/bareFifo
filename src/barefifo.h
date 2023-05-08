#ifndef _LIB_FIFO_H_
#define _LIB_FIFO_H_

#include <inttypes.h>



class BareFifo
{
  uint8_t *mpu8_Buffer;
  uint32_t mu32_BufferSize;
  uint32_t mu32_StartPtr;
  uint32_t mu32_Used;
  bool mb_Overflow; 

public:
  BareFifo(uint8_t pu8_FifoBuffer[], const uint32_t u32_FifoBufferSize);

  void begin(void);
  void end(void);

  bool isEmpty(void);
  bool isAlmostEmpty(void);
  bool isAlmostFull(void);
  bool isFull(void);
  bool isOverflow(void);
  void reset(void);

  uint32_t available(void);
  uint32_t availableForWrite(void);

  uint32_t write(const uint8_t pu8_Buffer[], const uint32_t u32_Size);
  uint32_t read(uint8_t pu8_Buffer[], const uint32_t u32_BufferSize);
};



#endif