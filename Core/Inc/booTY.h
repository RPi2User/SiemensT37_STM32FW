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

sbf_t booTYinit(sbf_t currentBuffer);
sbf_t booTYshell(sbf_t currentBuffer);
int8_t readCommand(int8_t cmd_terminator);

#endif /* INC_BOOTY_H_ */
