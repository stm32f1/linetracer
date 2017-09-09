#include <stdio.h>
#include "common/device.h"
#include "common/utility.h"
#include "common/comutil.h"
#include "setup.h"
#include "common/sensor.h"
#include "common/servo.h"
#include "common/motor.h"


static void GPIO_setting(void);
static void DMA_setting(void);
static void ADC_setting(void);
static void DAC_setting(void);
static void USART_setting(void);
static void I2C1_setting(void);
static void SysTick_setting(void);
static void Timer2_setting(void);
static void Timer3_setting(void);
static void Timer4_setting(void);
static void Timer5_setting(void);
static void Timer6_setting(void);
static void Timer7_setting(void);
static void tof_setting(void);
static void battery_check(void);

void setup(void)
{
	GPIO_setting();
	//I2C1_setting();
	DMA_setting();
	ADC_setting();
	DAC_setting();
	USART_setting();
	SysTick_setting();
	Timer2_setting();
	Timer3_setting();
	Timer4_setting();
	Timer5_setting();
	Timer6_setting();
	Timer7_setting();
	//tof_setting();


#ifdef USE_SLEEP
	CoreDebug->DHCSR |= 0x7;
#endif
	wake_up_all_servo();
	normalize_arm();
	MOTOR_SLEEP();
	/*
	Break_Lump = 1;
	//delay_ms(200);
	PSD_LOW_GPIO = 0;
	PSD_HIGH_GPIO = 0;
	PSD_JUDGE_CAPTURED_GPIO = 0;
	delay_ms(160);
	PSD_LOW_GPIO = 1;
	PSD_HIGH_GPIO = 1;
	PSD_JUDGE_CAPTURED_GPIO = 1;
	//delay_ms(40);
	 */
	
	color_GATE = 0;
	delay_ms(500);
	sleep_all_servo();

	if (ButtonA) battery_check();
	else while (!ButtonA);
}

static void GPIO_setting(void)
{
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
			RCC_APB2Periph_GPIOB |
			RCC_APB2Periph_GPIOC |
			RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //JTAGでしか使わないピンをI/Oに振る
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);


	// Button A and B
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//micro switch A and B
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// color sensor DOUT
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// USART1 TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART1 RX
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART3 TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// USART3 RX
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*
	//I2C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/

	// ADC
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

	//for on board battery checker
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =
			GPIO_Pin_0 | GPIO_Pin_1 |
			GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | 
			GPIO_Pin_4 | GPIO_Pin_5; //DAC
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =
			GPIO_Pin_2 | GPIO_Pin_3; //IR
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Servo
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

static void DMA_setting(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) sensor_val;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = NUMBER_OF_SENSORS; //number of sensor
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

static void ADC_setting(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	ADC_StructInit(&ADC_InitStructure);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = NUMBER_OF_SENSORS; //読み込むチャンネル数
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, 13, 1, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, 1, 2, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, 3, 3, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, 2, 4, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, 0, 5, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, 12, 6, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, 6, 7, ADC_SampleTime_13Cycles5); //marker
	ADC_RegularChannelConfig(ADC1, 7, 8, ADC_SampleTime_13Cycles5); //marker
	ADC_RegularChannelConfig(ADC1, 14, 9, ADC_SampleTime_13Cycles5); //psd
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	/* ADC2 Configuration ------------------------------------------------*/
	ADC_StructInit(&ADC_InitStructure);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);

	ADC_Cmd(ADC2, ENABLE);

	ADC_ResetCalibration(ADC2);
	while (ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while (ADC_GetCalibrationStatus(ADC2));

}

static void DAC_setting(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_InitTypeDef DAC_InitStructure;

	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_SetChannel1Data(DAC_Align_12b_R, 0);

	DAC_Cmd(DAC_Channel_1, ENABLE); //PA4
}

static void USART_setting(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	// RX interrupt
	// USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

	USART_Init(USART3, &USART_InitStructure);
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
}

static void I2C1_setting(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	I2C_InitTypeDef I2C_InitStructure;

	/* I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	//I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_CLOCK;

	I2C_DeInit(I2C1);
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);

	//interrupt setting
	/*
	NVIC_InitTypeDef NVIC_InitStructure;

	//Configure the I2C event priority
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//*/
}

static void SysTick_setting(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	NVIC_SetPriority(SysTick_IRQn, 0);
}

static void Timer2_setting(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM2->CR1 = 0b0010000000;
	TIM2->ARR = (100000000 / (29366 * 16)) / 2; //29366
	TIM2->PSC = 72 - 1;
	TIM2->DIER = 1;
}

static void Timer3_setting(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM3->CR1 = 0b0010000000;
	TIM3->ARR = 20000 - 1;
	TIM3->PSC = 72;

	TIM3->SR = (uint16_t) ~1;
	TIM3->DIER = 0;

	TIM3->EGR |= 1;

	TIM3->CCMR1 = 0b0110100001101000;
	TIM3->CCMR2 = 0b0110100001101000;

	//TIM3->CCER = 0b0001000100010001; // 起動時ON
	TIM3->CCER = 0b0000000000000000; // 起動時OFF

	TIM3->CR1 |= 1;
}

static void Timer4_setting(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM4->CR1 = 0b0010000000;
	TIM4->ARR = 2000 - 1;
	TIM4->PSC = 7200 - 1;
	TIM4->DIER = 1;
}

static void Timer5_setting(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM5->CR1 = 0b0010000000;
	TIM5->ARR = 100 - 1;
	TIM5->PSC = 7200 - 1;
	TIM5->DIER = 1;
}

static void Timer6_setting(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM6->CR1 = 0b10000000;
	TIM6->ARR = 200 - 1;
	TIM6->PSC = (uint16_t) (motorR.prescaler - 1); //720
	TIM6->DIER = 1;
}

static void Timer7_setting(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM7->CR1 = 0b10000000;
	TIM7->ARR = 200 - 1;
	TIM7->PSC = (uint16_t) (motorL.prescaler - 1); //720
	TIM7->DIER = 1;
}

static void battery_check(void)
{
	int val_LiPo = read_ADC(10);

	if (val_LiPo > 1800 || ADC_to_V(val_LiPo) < (10.8 / 11 * 1)) {
		inform_error();
	}
}
