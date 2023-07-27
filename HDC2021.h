/*
 * HDC2021.h
 *
 *  Created on: 25 Jul 2023
 *      Author: turabek
 */

#ifndef HDC2021_H_
#define HDC2021_H_

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"


/** �ｿｽG�ｿｽ�ｿｽ�ｿｽ[ **/
typedef enum
{
    HDC2021DEPT_ERR_NONE = 0,       /** �ｿｽG�ｿｽ�ｿｽ�ｿｽ[�ｿｽ�ｿｽ�ｿｽ�ｿｽ **/
    HDC2021DEPT_ERR_NOT_CONNECT,    /** �ｿｽﾚ托ｿｽ�ｿｽG�ｿｽ�ｿｽ�ｿｽ[ **/
    HDC2021DEPT_ERR_NOT_RESPONSE,   /** �ｿｽ�ｿｽ�ｿｽX�ｿｽ|�ｿｽ�ｿｽ�ｿｽX�ｿｽﾙ擾ｿｽ **/
    HDC2021DEPT_ERR_ID_MISMATCH     /** ID�ｿｽs�ｿｽ�ｿｽv **/
}HDC2021DEPTErr;


/** �ｿｽ�ｿｽ�ｿｽ闕橸ｿｽ�ｿｽ **/
#define HDC2021_INT_EN_NONE    (0)         /** �ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾝ厄ｿｽ�ｿｽ�ｿｽ **/
#define HDC2021_INT_EN_HL      (1 << 3)    /** �ｿｽ�ｿｽ�ｿｽx�ｿｽX�ｿｽ�ｿｽ�ｿｽb�ｿｽV�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽh Low **/
#define HDC2021_INT_EN_HH      (1 << 4)    /** �ｿｽ�ｿｽ�ｿｽx�ｿｽX�ｿｽ�ｿｽ�ｿｽb�ｿｽV�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽh High **/
#define HDC2021_INT_EN_TL      (1 << 5)    /** �ｿｽ�ｿｽ�ｿｽx�ｿｽX�ｿｽ�ｿｽ�ｿｽb�ｿｽV�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽh Low **/
#define HDC2021_INT_EN_TH      (1 << 6)    /** �ｿｽ�ｿｽ�ｿｽx�ｿｽX�ｿｽ�ｿｽ�ｿｽb�ｿｽV�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽh High **/
#define HDC2021_INT_EN_DRDY    (1 << 7)    /** �ｿｽf�ｿｽ[�ｿｽ^�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ **/

/** �ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾝピ�ｿｽ�ｿｽ�ｿｽﾉ撰ｿｽ **/
typedef enum
{
    HDC2021_INT_POL_LOW = 0,                /** �ｿｽA�ｿｽN�ｿｽe�ｿｽB�ｿｽu Low **/
    HDC2021_INT_POL_HIGH,                   /** �ｿｽA�ｿｽN�ｿｽe�ｿｽB�ｿｽu High **/

}HDC2021IntPol;

/** �ｿｽv�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ[�ｿｽh **/
typedef enum
{
    HDC2021_CC_ONESHOT = 0,
    HDC2021_CC_2MIN,
    HDC2021_CC_1MIN,
    HDC2021_CC_10SEC,
    HDC2021_CC_5SEC,
    HDC2021_CC_1SEC,
    HDC2021_CC_500MSEC,
    HDC2021_CC_200MSEC
}Hdc2021debt_Setting_Sample_Rate;
HDC2021DEPTErr Hdc2021debt_Check_Device_id(void);
HDC2021DEPTErr Hdc2021debt_Init(void);
HDC2021DEPTErr Hdc2021debt_Start(Hdc2021debt_Setting_Sample_Rate Rate);
HDC2021DEPTErr Hdc2021debt_Get_Status(uint8_t* status);
void Hdc2021debt_SetInterrupt(uint8_t interrupt_en, HDC2021IntPol pol);
HDC2021DEPTErr Hdc2021debt_ReadHumidity(uint16_t* shitsu);
HDC2021DEPTErr Hdc2021debt_ReadTemp(int16_t* ondo);
const char* hdc2021_GetErrorString(HDC2021DEPTErr err);
//HDC2021DEPTErr Hdc2021debt_Check_Device_id(void);

bool readReg(const uint8_t reg, uint8_t *dst, size_t size, uint32_t timer);
bool writeReg(uint8_t reg, const uint8_t* src,size_t size, uint32_t txTimer);




#endif /* HDC2021_H_ */
