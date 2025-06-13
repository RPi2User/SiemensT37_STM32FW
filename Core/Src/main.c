#include "main.h"
#include <stdio.h>
#include <stdlib.h>



UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);

// --- GOBLAL VARIABLES --------------------------------------------

char* writeBuffer;  // Create empty writeBuffer

// Teletype Variables
int rx_figs = 0;    // whether or not currently in figs or ltrs mode
int tx_figs = 0;    // ebd.
int tty_baud = 100;	// default Baudrate for TTYs


// Mode-specific vars
int mode = 0; // 0 -> Local != 0 -> Serial


// -----------------------------------------------------------------


// ---I/O SECTION---------------------------------------------------
void setLED_MLOCAL(int state){	// LED @ A0
	if (state != 0) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	}
}

void setLED_MSERIAL(int state){ // LED @ A1
	if (state != 0) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	}
}

void setLED_BSY(int state){		// LED @ A2
	if (state != 0) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	}
}

void setTTY(int state){			// TTY @ A3
	if (state != 0) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	}
}

// -----------------------------------------------------------------


// ---STR MANIPULATION----------------------------------------------
int strLen(char* str){
	int n = 0;
	while(*str != '\0') {
		n++;
		str++;
	}
	return n;
}
// -----------------------------------------------------------------


// ---TTY-FUNCTIONS-------------------------------------------------
void TTY_DELAY(int cycles){
	HAL_Delay(cycles * ( 1000 / tty_baud));
}

void TTY_WRITE(int symbol){
	setTTY(0);		// Startbit
	TTY_DELAY(1);	// wait for transmit

	// send those 5 bits
	for (int i = 0; i < 5; i++){
		int current_bit = (symbol >> i) & 1; // take first bit
		setTTY(current_bit);
		TTY_DELAY(1); 	// wait for transmit
	}

	// stop bits
	setTTY(1);
	TTY_DELAY(2); 	// send two stop bits
	setTTY(0);		// set to zero , or new startbit
}

void ryLoop(){
	while (1){
		TTY_WRITE(10); 	// send 'r'
		TTY_WRITE(21);	// send 'y'
	}
}

int convertToTTY(char c){
	// writeBuffer.lower();
	return 0;
}

void SEND_TTYC(char c){
	// writes a 7Bit ASCII to CCITT-2
	int out = (int) c;
	out &= ( 11111 << 0);	// Let just the 5 lower bits through
							// out should be between 0-31
	out = convertToTTY(c);
	TTY_WRITE(out);
}


// ---SENDERS-------------------------------------------------------
void SEND_TTY(){
	// sends writeBuffer to tty @ A3

	// foreach char in writeBuffer…
	int i = 0;
	while(*writeBuffer != '\0') {	
		// 1. writeBuffer.toLower();
		char c = writeBuffer[i];
        if (writeBuffer[i] >= 'A' && writeBuffer[i] <= 'Z') 
            c += 'a' - 'A'; 	// offset via the difference a-A
        writeBuffer[i] = c; 	// replace c in writeBuffer
        
		
		SEND_TTYC(writeBuffer[i]);
		i++;
	}

}

void SEND_SERIAL(){
	// placeholder for future code
}

void SEND(){
	if (mode != 0){ // LOCAL MODE
		SEND_TTY();
	}
	else {			// SERIAL MODE
		SEND_SERIAL();
	}
}
// -----------------------------------------------------------------


// ---INTERNAL LOGIC------------------------------------------------
void manageIO(){

	// poll Button @ D10
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_SET){
		// 10ms delay for debounce, prolly main delay.
		HAL_Delay(10);
		mode = mode != 0 ? 0 : 1;
		// wait until BT is released
		while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_SET){
			HAL_Delay(20);
		}
	}
	if (mode != 0){
		setLED_MLOCAL(0);
		setLED_MSERIAL(1);
	}
	else {
		setLED_MLOCAL(1);
		setLED_MSERIAL(0);
	}
}

void debugger(){
	ryLoop();
}

void _mode(){
    return;
}

int main(void)
{
	// scary ST-Stuff
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_USART1_UART_Init();
    //--------------------------------------------------------------

    // init vars
    writeBuffer = malloc(0);

    // init i/o stuff
    setLED_BSY(0);
    setLED_MLOCAL(0);
    setLED_MSERIAL(0);

    setTTY(0);

    // init ESP8266 uart
    while(1){
        manageIO();    // Like toggle LEDs, poll Button, etc.
		debugger();
        _mode();
        // do smth important, like initializing
        // 0. poll teletype
        // 1. poll RS232 Port
        // 2. signalize ESP8266 for transmit
    }
    free(writeBuffer);
    return 0;

}


// ---SCARY ST STUFF :C---------------------------------------------
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/*
    * @brief USART1 Initialization Function
    * @param None
    * @retval None
*/
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_EVEN;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 38400;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */

    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, LED_MLOCAL_Pin|LED_MSERIAL_Pin|LED_BSY_Pin|TTY_SEND_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : LED_MLOCAL_Pin LED_MSERIAL_Pin LED_BSY_Pin TTY_SEND_Pin */
    GPIO_InitStruct.Pin = LED_MLOCAL_Pin|LED_MSERIAL_Pin|LED_BSY_Pin|TTY_SEND_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : TTY_RECEIVE_Pin BT_Mode_Pin */
    GPIO_InitStruct.Pin = TTY_RECEIVE_Pin|BT_MODE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */

    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     * printf("Wrong parameters value: file %s on line %d\r\n", file, line); */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
// -----------------------------------------------------------------
