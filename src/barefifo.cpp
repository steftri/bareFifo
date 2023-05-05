#include <memory.h>

#include "barefifo.h" 



BareFifo::BareFifo(uint8_t *pu8_FifoBuffer, const uint32_t u32_FifoBufferSize)
  : mpu8_Buffer{pu8_FifoBuffer}
  , mu32_BufferSize{u32_FifoBufferSize}
  , mu32_StartPtr{0}
  , mu32_Used{0}
{
}



bool BareFifo::isEmpty(void)
{
  return (mu32_Used==0);
}



bool BareFifo::isAlmostEmpty(void)
{
  return (mu32_Used*10<=mu32_BufferSize*2);  // less or equal 20% of buffer size
}



bool BareFifo::isAlmostFull(void)
{
  return (mu32_Used*10>=mu32_BufferSize*8);  // more or equal 80% of buffer size
}



bool BareFifo::isFull(void)
{
  return (mu32_Used==mu32_BufferSize);
}

bool BareFifo::isOverflow(void)
{
  return mb_Overflow;
}


void BareFifo::reset(void)
{
  mu32_StartPtr = 0;
  mu32_Used = 0;
  mb_Overflow = false;
}



uint32_t BareFifo::available(void)
{
  return mu32_Used;
}



uint32_t BareFifo::availableForWrite(void)
{
  return mu32_BufferSize-mu32_Used; 
}




uint32_t BareFifo::write(const uint8_t *pu8_Buffer, const uint32_t u32_BufferSize)
{
  uint32_t u32_BytesToWrite = u32_BufferSize;

  if(u32_BufferSize>mu32_BufferSize-mu32_Used)
  {
    u32_BytesToWrite = mu32_BufferSize-mu32_Used;
    mb_Overflow = true;
    if(u32_BytesToWrite==0)
      return 0;
  }

  if(mu32_StartPtr+mu32_Used+u32_BytesToWrite<=mu32_BufferSize)
  { // bytes fit compelety into remaining fifo buffer
    memcpy(&mpu8_Buffer[mu32_StartPtr+mu32_Used], pu8_Buffer, u32_BytesToWrite);
  }
  else if(mu32_StartPtr+mu32_Used>=mu32_BufferSize)
  { // bytes fits compelety into section before start pointer
    memcpy(&mpu8_Buffer[mu32_StartPtr+mu32_Used-mu32_BufferSize], pu8_Buffer, u32_BytesToWrite);
  }
  else 
  { // bytes must be split      
    memcpy(&mpu8_Buffer[mu32_StartPtr+mu32_Used], pu8_Buffer, mu32_BufferSize-(mu32_StartPtr+mu32_Used));
    memcpy(&mpu8_Buffer[0], &pu8_Buffer[mu32_BufferSize-(mu32_StartPtr+mu32_Used)], u32_BytesToWrite-(mu32_BufferSize-(mu32_StartPtr+mu32_Used)));
  }
  mu32_Used += u32_BytesToWrite;

  return u32_BytesToWrite;
}



uint32_t BareFifo::read(uint8_t *pu8_Buffer, const uint32_t u32_BufferSize)
{
  uint32_t u32_BytesToRead = u32_BufferSize;  

  if(u32_BufferSize>mu32_Used)
    u32_BytesToRead = mu32_Used;  

  if(mu32_StartPtr+u32_BytesToRead<=mu32_BufferSize)
  { // bytes can be read compelety from remaining fifo buffer
    memcpy(pu8_Buffer, &mpu8_Buffer[mu32_StartPtr], u32_BytesToRead);
    mu32_StartPtr += u32_BytesToRead;
  }
  else
  { // bytes must be build together
    memcpy(pu8_Buffer, &mpu8_Buffer[mu32_StartPtr], mu32_BufferSize-mu32_StartPtr);
    memcpy(&pu8_Buffer[mu32_BufferSize-mu32_StartPtr], &mpu8_Buffer[0], u32_BytesToRead-(mu32_BufferSize-mu32_StartPtr));
    mu32_StartPtr = mu32_StartPtr+u32_BytesToRead-mu32_BufferSize;
  }
  mu32_Used -= u32_BytesToRead;

  return u32_BytesToRead;  
}

