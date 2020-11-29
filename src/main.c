/*
RPI MPS Final Project

Member: Jiahong Ji, Lizheng Liu

Project name: RF double security door Lock
 */

#include "rfid.h"
#include "rfid_functions.h"
#include "init.h"

char password[4];
char right_password[4] = "1234";
int check = 0;



//initiazted all the required variable
TIM_HandleTypeDef htim7;
int counter_1=0;
int trigger=0;
int timeout = 0;
int frozen = 0;


int main(void)
{

	/* Recognized card ID */
  uint8_t CardID[4];
  uint8_t type;
  char *result;


  //initialize the system and the clock
  Sys_Init();
  Init_Timer();
  Init_GPIO();



  // init the SPI  & RC522
  SPI_Init();
  RFID_RC522_Init();
  printf("\r\033[2J\033[1;1HRFID Test2\r\n");fflush(stdout);
  /* Infinite loop */
  while (1)
  {
	//keep reading the RFID until we got somethings
    int status = TM_MFRC522_Check(CardID, &type);

    //if the account has been freezon, we need to lcok the user over here
    if(frozen)
    {
    	printf("The bank system has been frozen due to mutiple incoorect enter\r\n");fflush(stdout);
    	printf("Plz come back later 20s\r\n");fflush(stdout);
    	HAL_Delay(20000);
    	printf("The system is now back online, swipt your tag!\r\n");fflush(stdout);
    	frozen = 0;
    }


    //if the reading function read something, it means we got something from the user
  	if (status == MI_OK)
  	{

  		//print the ID and let the user input his password
  		printf("The ID is %x%x%x%x.\r\n", CardID[0], CardID[1], CardID[2], CardID[3]);fflush(stdout);
  		printf("Plz enter your password\r\n");fflush(stdout);

  		// i stand for the number of time the user has tried, it can try four times in total
  		for(int i = 0; i<4;i++)
  		{
  			//the predefined keypad function, will change the global variable passowrd
  			keypad_enter();

  			//if time out happened, continue the code
  			if(timeout)
  			{
  				printf("TIMEOUT! Reenter your password\r\n");fflush(stdout);
  				timeout = 0;
  				continue;
  			}

  			//conpare each byte with the correct password
  			for(int j =0; j<4; j++)
  			{
  				if(password[j] != right_password[j])
  					check = 1;//indicate the password is wrong
  			}

  			//if the password sis wrong, do the following things
  			if(check)
  			{
  				printf("the password is incorrect, keep entering\r\n");fflush(stdout);
  				check = 0;

  				//i the user fialed for four time, freeze its accout
  				if(i == 3)
  					frozen = 1;//frezzing indicator
  				//light up the RED LED and turn off Green LED
  				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);
  				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_RESET);

  			}
  			else//else, the password is right
  			{
  				//light up the green LED
  				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_SET);
  				HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_RESET);
  				printf("the password is correct_ Unlock the ACCOUNT\r\n");fflush(stdout);
  				check = 0;
  				i = 4;//for jumping out of the for loop
  			}

  		}


  	}

  }
}

//the keypad enter subsystem, it will change the global varaible password
void keypad_enter(void){

    int i = 0;
    counter_1 =0;//reset the 10s counter



    //Generally, we fired the four row of the key pad altinatively, than see if there is any
    //reading from each colum, if so, stored it, otherwise keep waiting for input
    while(i<4)
    {
    	//first row
    	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET);
    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6))
    			{
    		     HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);
    			 password[i] = 66;
    			 HAL_Delay(500);
    			 i++;
    			continue;}
    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7))
    			{
    			 HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);
				 password[i] = 65;
				 HAL_Delay(500);
				 i++;
    			continue;}
    	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1))
    			{
    		     HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);
				 password[i] = 67;
				 HAL_Delay(500);
				 i++;
    			continue;}
    	if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6))
    			{
    			 HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);
				 password[i] = 68;
				 HAL_Delay(500);
				 i++;
				 continue;}
    	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);

    	//second row
    	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_4, GPIO_PIN_SET);
    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6))
    			{
    			 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_4, GPIO_PIN_RESET);
				 password[i] = 54;
				 HAL_Delay(500);
				 i++;
				 continue;}
    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7))
    			{
    			 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_4, GPIO_PIN_RESET);
				 password[i] = 51;
				 HAL_Delay(500);
				 i++;
				 continue;}
    	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1))
    			{
    			 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_4, GPIO_PIN_RESET);
				 password[i] = 57;
				 HAL_Delay(500);
				 i++;
				 continue;}
    	if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6))
    			{HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_4, GPIO_PIN_RESET);
				 password[i] = 35;
				 HAL_Delay(500);
				 i++;
				 continue;}
    	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_4, GPIO_PIN_RESET);

    	//third row
    	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_SET);
    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6))
    			{
    			 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);
				 password[i] = 53;
				 HAL_Delay(500);
				 i++;
				 continue;}
    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) )
    			{
    			 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);
				 password[i] = 50;
				 HAL_Delay(500);
				 i++;
				 continue;}
    	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1))
    			{
    			 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);
				 password[i] = 56;
				 HAL_Delay(500);
				 i++;
				 continue;}
    	if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6) )
    			{
    			 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);
				 password[i] = 48;
				 HAL_Delay(500);
				 i++;
				 continue;}
    	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);

    	//forth row
    	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET);
    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6))
    			{
    			 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);
				 password[i] = 52;
				 HAL_Delay(500);
				 i++;continue;}
    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7))
    			{
    			 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);
				 password[i] = 49;
				 HAL_Delay(500);
				 i++;continue;}
    	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1))
    			{
    			 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);
				 password[i] = 55;
				 HAL_Delay(500);
				 i++;continue;}
    	if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6))
    			{
    			 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);
				 password[i] = 42;
				 HAL_Delay(500);
				 i++;continue;}
    	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);

    	//if the 10 second is reach, break the function
    	if(counter_1 == 10)
    	{
    		timeout = 1;//indicate the time is out
    		break;
    	}

    }
    //print out the user inputs
    printf("your inputs is %c%c%c%c\r\n",password[0],password[1],password[2],password[3]);fflush(stdout);

}

//initializtion for the timer, we set to 1s counter
void Init_Timer() {

	HAL_Init();
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 2472;
	htim7.Init.Period = 43690;




	__HAL_RCC_TIM7_CLK_ENABLE();

//set the timer to interrut mode
	HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_TIM_Base_Init(&htim7);
	HAL_TIM_Base_Start_IT(&htim7);

}


//initiazte the GPIO pin for the timer
void Init_GPIO() {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	asm ("nop");
	asm ("nop");

	// Set Pin 13/5 to output. (LED1 and LED2)

	//enable J8 setting mode
	//GPIOJ->MODER
	GPIO_InitTypeDef gpio_init;
	gpio_init.Pin = GPIO_PIN_1;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &gpio_init);

	//enable the I
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);


}

//
//
// -- ISRs (IRQs) -------------
//
void TIM7_IRQHandler(void) {

	HAL_TIM_IRQHandler(&htim7);
	// Other code here:

}

//1s call back function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if(htim->Instance == TIM7)
	{
		counter_1++;
		//indicate 1s has passed
		printf("1s \r\n");fflush(stdout);

		//this is for the main swtich which may cluse the whole circuit
		if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_8))
				printf("and it's on \r\n");fflush(stdout);
	}
}


//HAL - GPIO/EXTI Handler
void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(8);

	if(__HAL_GPIO_EXTI_GET_FLAG(EXTI_PR_PR8)	)
	{
		trigger++;
		__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI_PR_PR8);
	}


}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

//none

}


