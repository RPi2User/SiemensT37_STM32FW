/*
 * booTY.h
 *
 *  Created on: Jul 6, 2025
 *      Author: florian
 */

#ifndef INC_BOOTY_H_
#define INC_BOOTY_H_

/*
const int VALID_VALS[];
const int VALID_MODES[];
const int VALID_LINEBREAKS[];
*/

int* booTYinit(int* currentBuffer);
int* booTYshell(int* currentBuffer);
int* readCommand(int cmd_terminator);

#endif /* INC_BOOTY_H_ */
