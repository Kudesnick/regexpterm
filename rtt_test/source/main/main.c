/**
 * @file main.c
 * @author Stulov Tikhon (aka Kudesnick) (kudesnick@inbox.ru)
 * @brief 
 * @version 0.1
 * @date 11.12.2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <stdio.h>
#include "Board_Buttons.h"


/**
 * @brief   Main function
 * @return  Error code
 */
int main(void)
{
    Buttons_Initialize();

	do
	{
		for (int i = 0; i < 10; i++)
			printf("\033[31mC\033[32mO\033[33mL\033[34mO\033[35mR\033[42m \033[0m\033[36mT\033[37mE\033[30m\033[47mS\033[0mT "); // Color test
		printf("\n");
	}
	while (Buttons_GetState());
	printf("Insert please_");

    for(;;)
    {
		int inp = getchar();
		if (inp > 0)
		{
			printf("<%02X>\n", inp);
		}
    }

    return 0;
}
