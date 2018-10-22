/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   i2c mpu6050应用函数bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "./i2c/bsp_i2c.h"
#include "./systick/bsp_SysTick.h"

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define I2C_SCL_PIN		GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			/* 连接到SDA数据线的GPIO */




/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 0	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif

#define Delay mdelay

/* STM32 I2C 快速模式 */
#define I2C_Speed              400000  //*

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2Cx_OWN_ADDRESS7      0X0A   


static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;    



/********************************* Prototypes *********************************/
unsigned long ST_Hard_Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue);
unsigned long ST_Hard_Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue);
/*******************************  Function ************************************/

//若定义了这个宏，使用硬件iic，否则使用软件iic。这个宏在bsp_i2c.h文件定义
//使用硬件IIC时不能与液晶屏共用，因为FSMC的NADV与IIC1的SDA 是同一个引脚，互相影响了

#ifdef HARD_IIC

#define ST_Sensors_I2C_WriteRegister  ST_Hard_Sensors_I2C_WriteRegister
#define ST_Sensors_I2C_ReadRegister ST_Hard_Sensors_I2C_ReadRegister

static uint8_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


#else
//
#define Soft_I2C_SDA_STATE   	GPIO_ReadInputDataBit(Soft_I2C_PORT, Soft_I2C_SDA)
#define Soft_I2C_DELAY 				Soft_I2C_Delay(100000)
#define Soft_I2C_NOP					Soft_I2C_Delay(10) 
//
#define Soft_I2C_READY		0x00
#define Soft_I2C_BUS_BUSY	0x01	
#define Soft_I2C_BUS_ERROR	0x02
//
#define Soft_I2C_NACK	  0x00 
#define Soft_I2C_ACK		0x01


//
static void Soft_I2C_Configuration(void);
static void Soft_I2C_Delay(uint32_t dly);
static uint8_t Soft_I2C_START(void);
static void Soft_I2C_STOP(void);
static void Soft_I2C_SendACK(void);
static void Soft_I2C_SendNACK(void);
static uint8_t Soft_I2C_SendByte(uint8_t anbt_i2c_data);
static uint8_t Soft_I2C_ReceiveByte_WithACK(void);
static uint8_t Soft_I2C_ReceiveByte(void);
static uint8_t Soft_DMP_I2C_Write(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len,unsigned char *soft_i2c_data_buf);
static uint8_t Soft_DMP_I2C_Read(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len,unsigned char *soft_i2c_data_buf);


#define ST_Sensors_I2C_WriteRegister  Soft_DMP_I2C_Write
#define ST_Sensors_I2C_ReadRegister Soft_DMP_I2C_Read

#endif



#ifdef HARD_IIC
/**
  * @brief  I2C1 I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* 使能与 I2C1 有关的时钟 */
	SENSORS_I2C_APBxClock_FUN ( SENSORS_I2C_CLK, ENABLE );
	SENSORS_I2C_GPIO_APBxClock_FUN ( SENSORS_I2C_GPIO_CLK, ENABLE );
	
    
  /* PB6-I2C1_SCL、PB7-I2C1_SDA*/
  GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(SENSORS_I2C_SCL_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(SENSORS_I2C_SDA_PORT, &GPIO_InitStructure);	
	
	
}


/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	
	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* 通信速率 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C1 初始化 */
  I2C_Init(SENSORS_I2Cx, &I2C_InitStructure);
  
	/* 使能 I2C1 */
  I2C_Cmd(SENSORS_I2Cx, ENABLE);   
}



/**
  * @brief  向IIC设备的寄存器连续写入数据
  * @param  Address: IIC设备地址
  * @param  RegisterAddr: 寄存器地址
  * @param  RegisterLen: 要写入数据的长度
  * @param  RegisterValue: 要指向写入数据的指针
  * @retval 0正常，非0异常
  */
unsigned long ST_Hard_Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue)
{
	 uint32_t i=0;
	
	 /* Send STRAT condition */
	  I2C_GenerateSTART(SENSORS_I2Cx, ENABLE);

		I2CTimeout = I2CT_FLAG_TIMEOUT;


	  /* Test on EV5 and clear it */
	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	  {
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
	  }

	  /* Send slave address for write */
	  I2C_Send7bitAddress(SENSORS_I2Cx, (Address<<1), I2C_Direction_Transmitter);

		I2CTimeout = I2CT_FLAG_TIMEOUT;
	  /* Test on EV6 and clear it */
	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
	  }

	  /* Send the slave's internal address to write to */
	  I2C_SendData(SENSORS_I2Cx, RegisterAddr);

		I2CTimeout = I2CT_FLAG_TIMEOUT;
	  /* Test on EV8 and clear it */
	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
	  }

	  /* Send the byte to be written */
	  for( i=0; i<(RegisterLen); i++)
	   {
		  	  I2CTimeout = I2CT_FLAG_TIMEOUT;
		  	  /* Test on EV8 and clear it */
		  	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		  		{
		  	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		  	  }

	     /* Prepare the register value to be sent */
	     I2C_SendData(SENSORS_I2Cx, RegisterValue[i]);
	   }

		I2CTimeout = I2CT_FLAG_TIMEOUT;

	  /* Test on EV8 and clear it */
	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
	  }

	  /* Send STOP condition */
	  I2C_GenerateSTOP(SENSORS_I2Cx, ENABLE);

		return 0; //正常返回0
}


/**
  * @brief  向IIC设备的寄存器连续读出数据
  * @param  Address: IIC设备地址
  * @param  RegisterAddr: 寄存器地址
  * @param  RegisterLen: 要读取的数据长度
  * @param  RegisterValue: 指向存储读出数据的指针
  * @retval 0正常，非0异常
  */
unsigned long ST_Hard_Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue)
{
	 I2CTimeout = I2CT_LONG_TIMEOUT;

	  while(I2C_GetFlagStatus(SENSORS_I2Cx, I2C_FLAG_BUSY)) // Added by Najoua 27/08/2008
	  {
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
	   }

	  I2C_GenerateSTART(SENSORS_I2Cx, ENABLE);

		I2CTimeout = I2CT_FLAG_TIMEOUT;

	  /* Test on EV5 and clear it */
	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		{
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
	   }

	  /* Send slave address for write */
	  I2C_Send7bitAddress(SENSORS_I2Cx, (Address<<1), I2C_Direction_Transmitter);

		I2CTimeout = I2CT_FLAG_TIMEOUT;

	  /* Test on EV6 and clear it */
	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);
	   }

	  /* Clear EV6 by setting again the PE bit */
	  I2C_Cmd(SENSORS_I2Cx, ENABLE);

	  /* Send the slave's internal address to write to */
	  I2C_SendData(SENSORS_I2Cx, RegisterAddr);

		I2CTimeout = I2CT_FLAG_TIMEOUT;

	  /* Test on EV8 and clear it */
	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);
	   }

	  /* Send STRAT condition a second time */
	  I2C_GenerateSTART(SENSORS_I2Cx, ENABLE);

		I2CTimeout = I2CT_FLAG_TIMEOUT;
	  /* Test on EV5 and clear it */
	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		{
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
	   }

	  /* Send slave address for read */
	  I2C_Send7bitAddress(SENSORS_I2Cx, (Address<<1), I2C_Direction_Receiver);

		I2CTimeout = I2CT_FLAG_TIMEOUT;

	  /* Test on EV6 and clear it */
	  while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
	    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
	   }

	  /* While there is data to be read */
	  while(RegisterLen)
	  {
	    if(RegisterLen == 1)
	    {
	      /* Disable Acknowledgement */
	      I2C_AcknowledgeConfig(SENSORS_I2Cx, DISABLE);

	      /* Send STOP Condition */
	      I2C_GenerateSTOP(SENSORS_I2Cx, ENABLE);
	    }

			I2CTimeout = I2CT_LONG_TIMEOUT;
			while(!I2C_CheckEvent(SENSORS_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
			{
				if((I2CTimeout--) == 0) 
				{

						return I2C_TIMEOUT_UserCallback(10);
				}
			 }
			
	    {
	      /* Read a byte from the slave */
	      *RegisterValue = I2C_ReceiveData(SENSORS_I2Cx);

	      /* Point to the next location where the byte read will be saved */
	      RegisterValue++;

	      /* Decrement the read bytes counter */
	      RegisterLen--;
	    }

	  }

	  /* Enable Acknowledgement to be ready for another reception */
	  I2C_AcknowledgeConfig(SENSORS_I2Cx, ENABLE);

		return 0; //正常，返回0
}






/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
static  uint8_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
	/*重置IIC*/
  I2C_GenerateSTOP(SENSORS_I2Cx, ENABLE);
  I2C_SoftwareResetCmd(SENSORS_I2Cx, ENABLE);
  I2C_SoftwareResetCmd(SENSORS_I2Cx, DISABLE);
	
	I2C_Bus_Init();
	
  /* Block communication and all processes */
  MPU_ERROR("I2C Timeout error! error code = %d",errorCode);
  
  return 1;
}

#endif //endof #ifdef HARD_IIC



/**
  * @brief  I2C 外设初始化
  * @param  无
  * @retval 无
  */
void I2C_Bus_Init(void)
{	
	
	Set_I2C_Retry(5);
	
	#ifdef HARD_IIC
	MPU_DEBUG("hard iic");
	
  I2C_GPIO_Config(); 
 
  I2C_Mode_Configu();
	
	#else
	
  MPU_DEBUG("soft iic");
	Soft_I2C_Configuration();
	
	#endif

}






/**
  * @brief  向IIC设备的寄存器连续写入数据，带超时重试设置，供mpu接口调用
  * @param  Address: IIC设备地址
  * @param  RegisterAddr: 寄存器地址
  * @param  RegisterLen: 要写入数据的长度
  * @param  RegisterValue: 要指向写入数据的指针
  * @retval 0正常，非0异常
  */
int Sensors_I2C_WriteRegister(unsigned char slave_addr,
                                        unsigned char reg_addr,
                                        unsigned short len,
                                        const unsigned char *data_ptr)
{
  char retries=0;
  int ret = 0;
  unsigned short retry_in_mlsec = Get_I2C_Retry();

tryWriteAgain:
  ret = 0;
  ret = ST_Sensors_I2C_WriteRegister( slave_addr, reg_addr, len, ( unsigned char *)data_ptr);

  if(ret && retry_in_mlsec)
  {
    if( retries++ > 4 )
        return ret;

    mdelay(retry_in_mlsec);
    goto tryWriteAgain;
  }
  return ret;
}


/**
  * @brief  向IIC设备的寄存器连续读出数据,带超时重试设置，供mpu接口调用
  * @param  Address: IIC设备地址
  * @param  RegisterAddr: 寄存器地址
  * @param  RegisterLen: 要读取的数据长度
  * @param  RegisterValue: 指向存储读出数据的指针
  * @retval 0正常，非0异常
  */
int Sensors_I2C_ReadRegister(unsigned char slave_addr,
                                       unsigned char reg_addr,
                                       unsigned short len,
                                       unsigned char *data_ptr)
{
  char retries=0;
  int ret = 0;
  unsigned short retry_in_mlsec = Get_I2C_Retry();

tryReadAgain:
  ret = 0;
  ret = ST_Sensors_I2C_ReadRegister( slave_addr, reg_addr, len, ( unsigned char *)data_ptr);

  if(ret && retry_in_mlsec)
  {
    if( retries++ > 4 )
        return ret;

    mdelay(retry_in_mlsec);
    goto tryReadAgain;
  }
  return ret;
}


static unsigned short RETRY_IN_MLSEC  = 55;

/**
  * @brief  设置iic重试时间
  * @param  ml_sec：重试的时间，单位毫秒
  * @retval 重试的时间，单位毫秒
  */
void Set_I2C_Retry(unsigned short ml_sec)
{
  RETRY_IN_MLSEC = ml_sec;
}

/**
  * @brief  获取设置的iic重试时间
  * @param  none
  * @retval none
  */
unsigned short Get_I2C_Retry(void)
{
  return RETRY_IN_MLSEC;
}


/************************软件IIC驱动函数****************************************/

#ifndef HARD_IIC

static void Soft_I2C_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	//
	//
  GPIO_InitStructure.GPIO_Pin = Soft_I2C_SCL | Soft_I2C_SDA;					//配置使用的I2C口
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   //设置I2C口最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	  //设置I2C为开漏输出
  GPIO_Init(Soft_I2C_PORT, &GPIO_InitStructure); 
	//
	//
	Soft_I2C_SCL_1; 
	Soft_I2C_SDA_1;
	Soft_I2C_DELAY; 
}

static void Soft_I2C_Delay(uint32_t dly) 
{
	while(--dly);	//dly=100: 8.75us; dly=100: 85.58 us (SYSCLK=72MHz)
}

static uint8_t Soft_I2C_START(void)
{ 
	Soft_I2C_SDA_1; 
 	Soft_I2C_NOP;
  // 
 	Soft_I2C_SCL_1; 
 	Soft_I2C_NOP;    
	//
 	if(!Soft_I2C_SDA_STATE) return Soft_I2C_BUS_BUSY;
	//
 	Soft_I2C_SDA_0;
 	Soft_I2C_NOP;
  //
 	Soft_I2C_SCL_0;  
 	Soft_I2C_NOP; 
	//
 	if(Soft_I2C_SDA_STATE) return Soft_I2C_BUS_ERROR;
	//
 	return Soft_I2C_READY;
}

static void Soft_I2C_STOP(void)
{
 	Soft_I2C_SDA_0; 
 	Soft_I2C_NOP;
  // 
 	Soft_I2C_SCL_1; 
 	Soft_I2C_NOP;    
	//
 	Soft_I2C_SDA_1;
 	Soft_I2C_NOP;
}

static void Soft_I2C_SendACK(void)
{
 	Soft_I2C_SDA_0;
 	Soft_I2C_NOP;
 	Soft_I2C_SCL_1;
 	Soft_I2C_NOP;
 	Soft_I2C_SCL_0; 
 	Soft_I2C_NOP;  
}

static void Soft_I2C_SendNACK(void)
{
	Soft_I2C_SDA_1;
	Soft_I2C_NOP;
	Soft_I2C_SCL_1;
	Soft_I2C_NOP;
	Soft_I2C_SCL_0; 
	Soft_I2C_NOP;  
}




/**
  * @brief  等待应答信号到来
  * @retval 返回值：1，接收应答失败
	*									0，接收应答成功
  */
uint8_t Soft_I2C_Wait_Ack(void)
{
	uint8_t ucErrTime=0;

	Soft_I2C_SDA_1;
	Soft_I2C_NOP;	   
	Soft_I2C_SCL_1;
	Soft_I2C_NOP;	 
	
	while(Soft_I2C_SDA_STATE)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			Soft_I2C_STOP();
			return Soft_I2C_BUS_ERROR;
		}
	}
	Soft_I2C_SCL_0;//时钟输出0 	   
	return 0;  
} 

static uint8_t Soft_I2C_SendByte(uint8_t soft_i2c_data)
{
 	uint8_t i;
 	
	Soft_I2C_SCL_0;
 	for(i=0;i<8;i++)
 	{  
  		if(soft_i2c_data&0x80) Soft_I2C_SDA_1;
   		else Soft_I2C_SDA_0;
			//
  		soft_i2c_data<<=1;
  		Soft_I2C_NOP;
			//
  		Soft_I2C_SCL_1;
  		Soft_I2C_NOP;
  		Soft_I2C_SCL_0;
  		Soft_I2C_NOP; 
 	}
	//
// 	Soft_I2C_SDA_1; 
// 	Soft_I2C_NOP;
// 	Soft_I2C_SCL_1;
// 	Soft_I2C_NOP;   
// 	if(Soft_I2C_SDA_STATE)
// 	{
//  		Soft_I2C_SCL_0;
//  		return Soft_I2C_NACK;
// 	}
// 	else
// 	{
//  		Soft_I2C_SCL_0;
//  		return Soft_I2C_ACK;  
// 	}  
	return Soft_I2C_Wait_Ack();  
}

static uint8_t Soft_I2C_ReceiveByte(void)
{
	uint8_t i,soft_i2c_data;
	//
 	Soft_I2C_SDA_1;
 	Soft_I2C_SCL_0; 
 	soft_i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
  		Soft_I2C_SCL_1;
  		Soft_I2C_NOP; 
  		soft_i2c_data<<=1;
			//
  		if(Soft_I2C_SDA_STATE)	soft_i2c_data|=0x01; 
  
  		Soft_I2C_SCL_0;  
  		Soft_I2C_NOP;         
 	}
	Soft_I2C_SendNACK();
 	return soft_i2c_data;
}

static uint8_t Soft_I2C_ReceiveByte_WithACK(void)
{
	uint8_t i,soft_i2c_data;
	//
 	Soft_I2C_SDA_1;
 	Soft_I2C_SCL_0; 
 	soft_i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
  		Soft_I2C_SCL_1;
  		Soft_I2C_NOP; 
  		soft_i2c_data<<=1;
			//
  		if(Soft_I2C_SDA_STATE)	soft_i2c_data|=0x01; 
  
  		Soft_I2C_SCL_0;  
  		Soft_I2C_NOP;         
 	}
	Soft_I2C_SendACK();
 	return soft_i2c_data;
}


//static void Soft_DMP_Delay_us(uint32_t dly)
//{
//	uint8_t i;
//	while(dly--) for(i=0;i<10;i++);
//}
////
//static void Soft_DMP_Delay_ms(uint32_t dly)
//{
//	while(dly--) Soft_DMP_Delay_us(1000);
//}
//

static uint8_t Soft_DMP_I2C_Write(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len,unsigned char *soft_i2c_data_buf)
{		
		uint8_t i, result=0;
		Soft_I2C_START();
		result  = Soft_I2C_SendByte(soft_dev_addr << 1 | I2C_Direction_Transmitter);	
		if(result != 0) return result;
	
		result = Soft_I2C_SendByte(soft_reg_addr);  
		if(result != 0) return result;
	
		for (i=0;i<soft_i2c_len;i++) 
		{
			result = Soft_I2C_SendByte(soft_i2c_data_buf[i]); 
			if (result != 0) return result;
		}
		Soft_I2C_STOP();
		return 0x00;
}

static uint8_t Soft_DMP_I2C_Read(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len,unsigned char *soft_i2c_data_buf)
{
		uint8_t result;
	
		Soft_I2C_START();
		result  = Soft_I2C_SendByte(soft_dev_addr << 1 | I2C_Direction_Transmitter);			
		if(result != 0) return result;

		result = Soft_I2C_SendByte(soft_reg_addr); 
		if(result != 0) return result;

		Soft_I2C_START();
		result = Soft_I2C_SendByte(soft_dev_addr << 1 | I2C_Direction_Receiver);
		if(result != 0) return result;
	
		//
    while (soft_i2c_len)
		{
			if (soft_i2c_len==1) *soft_i2c_data_buf =Soft_I2C_ReceiveByte();  
      else *soft_i2c_data_buf =Soft_I2C_ReceiveByte_WithACK();
      soft_i2c_data_buf++;
      soft_i2c_len--;
    }
		Soft_I2C_STOP();
    return 0x00;
}
/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
		CPU主频72MHz时，在内部Flash运行, MDK工程不优化
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
        
    IAR工程编译效率高，不能设置为7
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(u8 ack)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	if(ack==0)
		i2c_NAck();
	else
		i2c_Ack();
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_GPIO_Config
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_GPIO_Config();		/* 配置GPIO */
	
	i2c_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address|I2C_WR);
	ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */

	i2c_Stop();			/* 发送停止信号 */

	return ucAck;
}


#endif //endof #ifndef HARD_IIC
/*********************************************END OF FILE**********************/

