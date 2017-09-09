#ifndef PORT_H
#define PORT_H

#define Buzzer			BITBAND_GPIO(&GPIOC->ODR, 13)
#define ButtonA			BITBAND_GPIO(&GPIOC->IDR, 14)
#define ButtonB			BITBAND_GPIO(&GPIOC->IDR, 15)

#define LED_A			BITBAND_GPIO(&GPIOB->ODR, 8)
#define LED_B			BITBAND_GPIO(&GPIOB->ODR, 9)

#define motorL_clock		BITBAND_GPIO(&GPIOB->ODR, 5)
#define motorL_direction	BITBAND_GPIO(&GPIOB->ODR, 3)
#define motorR_clock		BITBAND_GPIO(&GPIOC->ODR, 12)
#define motorR_direction	BITBAND_GPIO(&GPIOC->ODR, 10)
//DAC FOR MOTOR DRIVER = PA4

///*
#define Piezo_Buzzer		BITBAND_GPIO(&GPIOB->ODR, 15)
#define Left_Blinker		BITBAND_GPIO(&GPIOB->ODR, 13)
#define Break_Lump		BITBAND_GPIO(&GPIOB->ODR, 11)
#define Right_Blinker		BITBAND_GPIO(&GPIOB->ODR, 2)
//*/

//color sensor
#define color_LED		BITBAND_GPIO(&GPIOB->ODR, 6)
#define color_GATE		BITBAND_GPIO(&GPIOB->ODR, 4)
#define color_CK		BITBAND_GPIO(&GPIOD->ODR, 2)
#define color_DOUT		BITBAND_GPIO(&GPIOC->IDR, 11)
//#define color_RANGE		BITBAND_GPIO(&GPIOA->ODR, 15)

#define micro_switchR		BITBAND_GPIO(&GPIOA->IDR, 12)
#define micro_switchL		BITBAND_GPIO(&GPIOA->IDR, 11)

//#define PSD_LOW_GPIO		BITBAND_GPIO(&GPIOB->ODR, 0)
//#define PSD_HIGH_GPIO		BITBAND_GPIO(&GPIOB->ODR, 1)
//#define PSD_JUDGE_CAPTURED_GPIO	BITBAND_GPIO(&GPIOC->ODR, 4)


#endif /* PORT_H */
