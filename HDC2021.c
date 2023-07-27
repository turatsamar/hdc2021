
#include <msp430.h>
#include "HDC2021.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <driverlib.h>


/**�ｿｽ�ｿｽ�ｿｽW�ｿｽX�ｿｽ^�ｿｽ}�ｿｽb�ｿｽv�ｿｽ@Ref P.20 7.6 **/
static const uint8_t HDC2021_REG_TEMP_L                 = {0x00};
static const uint8_t HDC2021_REG_TEMP_H                 = {0x01};
static const uint8_t HDC2021_REG_HUM_L                  = {0x02};
static const uint8_t HDC2021_REG_HUM_H                  = {0x03};
static const uint8_t HDC2021_REG_STATUS                 = {0x04};
static const uint8_t HDC2021_REG_TEMP_MAX               = {0x04};
static const uint8_t HDC2021_REG_HUM_MAX                = {0x05};
static const uint8_t HDC2021_REG_INTERRUPT_ENABLE       = {0x07};
static const uint8_t HDC2021_REG_TEMP_OFFSET_ADJUST     = {0x08};
static const uint8_t HDC2021_REG_HUM_OFFSET_ADJUST      = {0x09};
static const uint8_t HDC2021_REG_TEMP_THR_L             = {0x0A};
static const uint8_t HDC2021_REG_TEMP_THR_H             = {0x0B};
static const uint8_t HDC2021_REG_RH_THR_L               = {0x0C};
static const uint8_t HDC2021_REG_RH_THR_H               = {0x0D};
static const uint8_t HDC2021_REG_DEVICE_CONFIG          = {0x0E};
static const uint8_t HDC2021_REG_MEASUR_CONFIG          = {0x0F};
static const uint8_t HDC2021_REG_MANUFACTURER_ID_L      = {0xFC};
static const uint8_t HDC2021_REG_MANUFUCTURER_ID_H      = {0xFD};
static const uint8_t HDC2021_REG_DEVICE_ID_LOW          = {0xFE};
static const uint8_t HDC2021_REG_DEVICE_ID_HIGH          = {0xFF};

/**�ｿｽ�ｿｽ�ｿｽW�ｿｽX�ｿｽ^�ｿｽﾅ抵ｿｽl **/
static const uint8_t HDC2021_DEVICE_ID[2]             = {0xD0,0x07};
static const uint8_t HDC2021_MANUFACTURER_ID[2]         = {0x49, 0x54};

/**�ｿｽ�ｿｽ�ｿｽW�ｿｽX�ｿｽ^�ｿｽﾝ抵ｿｽ@�ｿｽf�ｿｽt�ｿｽH�ｿｽ�ｿｽ�ｿｽg **/
static const uint8_t HDC2021_DEFAULT_STATUS             = {0x00};
static const uint8_t HDC2021_DEFAULT_TEMP_MAX           = {0x00};
static const uint8_t HDC2021_DEFAULT_HUM_MAX            = {0x00};
static const uint8_t HDC2021_DEFAULT_INTERRUPT_ENABLE   = {0x00};
static const uint8_t HDC2021_DEFAULT_TEMP_OFFSET_ADJUST = {0x00};
static const uint8_t HDC2021_DEFAULT_HUM_OFFSET_ADJUST  = {0x00};
static const uint8_t HDC2021_DEFAULT_TEMP_THR_L         = {0x01};
static const uint8_t HDC2021_DEFAULT_TEMP_THR_H         = {0xFF};
static const uint8_t HDC2021_DEFAULT_RH_THR_L           = {0x00};
static const uint8_t HDC2021_DEFAULT_RH_THR_H           = {0xFF};
static const uint8_t HDC2021_DEFAULT_DEVICE_CONFIG      = {0x0E};
static const uint8_t HDC2021_DEFAULT_MEASUR_CONFIG      = {0x0F};

/** 繧ｹ繝�繝ｼ繧ｿ繧ｹ 繝ｬ繧ｸ繧ｹ繧ｿ險ｭ螳壼�､ Ref P.23 7.6.5 **/
typedef union
{
    uint8_t data;
    struct
    {
        uint8_t res     :3;
        uint8_t hl      :1;
        uint8_t hh      :1;
        uint8_t tl      :1;
        uint8_t th      :1;
        uint8_t drdy    :1;
    }bit;
}HDC2021RegStatus;

/** �ｿｽ�ｿｽ�ｿｽ闕橸ｿｽﾝイ�ｿｽl�ｿｽ[�ｿｽu�ｿｽ�ｿｽ �ｿｽ�ｿｽ�ｿｽW�ｿｽX�ｿｽ^�ｿｽﾝ抵ｿｽl Ref P.25 7.6.8 **/
typedef union
{
    uint8_t data;
    struct
    {
        uint8_t res  :3;
        uint8_t hl   :1;
        uint8_t hh   :1;
        uint8_t tl   :1;
        uint8_t th   :1;
        uint8_t drdy :1;
    }bits;

}HDC2021DEPT_RegInterruptEanable;

/**�ｿｽf�ｿｽo�ｿｽC�ｿｽX�ｿｽ\�ｿｽ�ｿｽ�ｿｽ@�ｿｽ�ｿｽ�ｿｽW�ｿｽX�ｿｽ^�ｿｽﾝ抵ｿｽl�ｿｽ@Ref�ｿｽ@P.30�ｿｽ@�ｿｽV�ｿｽD�ｿｽU�ｿｽD�ｿｽP�ｿｽT**/
typedef union
{
    uint8_t data;
    struct
    {
        uint8_t int_mode :1;
        uint8_t int_pol  :1;
        uint8_t drdy     :1;
        uint8_t heat     :1;
        uint8_t CC       :3;
        uint8_t reset    :1;
    }bits;
}HDC2021DEPT_Reg_Device_Config;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t meas_trigq      :1;
        uint8_t meas_config     :2;
        uint8_t res             :1;
        uint8_t hacc            :2;
        uint8_t tacc            :2;
    }bit;
}HDC2021DEPT_RegMeasuremtConfig;

#pragma pack(1)
typedef struct
{

    uint8_t temp_max;
    uint8_t hum_max;
    HDC2021DEPT_RegInterruptEanable interrept_enable;
    uint8_t temp_adjust_offset;
    uint8_t hum_adjust_offset;
    uint8_t temp_thr_l;
    uint8_t temp_thr_h;
    uint8_t rh_thr_l;
    uint8_t rh_thr_h;
    HDC2021DEPT_RegMeasuremtConfig meas_config;
    HDC2021DEPT_Reg_Device_Config config;
}hdc2021_registers;
#pragma pack(0)

//static HDC2021DEPTErr Hdc2021debt_Check_Device_id(void);
static HDC2021DEPTErr Hdc2021dept_SoftReset(void);
static void hdc2021_Default_Reg(void);


static hdc2021_registers reg;

HDC2021DEPTErr Hdc2021debt_Init(void)
{
    HDC2021DEPTErr err = HDC2021DEPT_ERR_NOT_CONNECT;
    do
    {
        hdc2021_Default_Reg();
        if((err = Hdc2021dept_SoftReset()) !=  HDC2021DEPT_ERR_NONE)
            break;
        __delay_cycles(3500);
        err = Hdc2021debt_Check_Device_id();
    }while(0);
    return err;
}

HDC2021DEPTErr Hdc2021debt_ReadTemp(int16_t* ondo)
{
    HDC2021DEPTErr err = HDC2021DEPT_ERR_NOT_RESPONSE;
    uint16_t read;

    do{
    	read=0;
        if (!readReg(HDC2021_REG_TEMP_L, (uint8_t*)&read,2,50))
            break;
        err = HDC2021DEPT_ERR_NONE;
    }while(0);
    *ondo = ((read * 165.0) / 65536.0) - 40.0;
    return err;
}
HDC2021DEPTErr Hdc2021debt_ReadHumidity(uint16_t* shitsu)
{
    HDC2021DEPTErr err = HDC2021DEPT_ERR_NOT_RESPONSE;
    uint8_t shitsu1[2];
    uint16_t hum=0;
    do
    {
    	memset(shitsu1, 0, sizeof(shitsu1));
        if (!readReg(HDC2021_REG_HUM_L, (uint8_t*)&shitsu1, 2,50))
            break;
        err = HDC2021DEPT_ERR_NONE;
    }while(0);
    hum=shitsu1[1];
    hum=(hum << 8) | shitsu1[0];
    *shitsu = ((hum * 100.0) / 65536.0);
    return err;
}

HDC2021DEPTErr Hdc2021debt_Start(Hdc2021debt_Setting_Sample_Rate Rate)
{
    HDC2021DEPTErr err = HDC2021DEPT_ERR_NOT_RESPONSE;

    do{
        reg.config.bits.CC=Rate;
       if(!writeReg(HDC2021_REG_DEVICE_CONFIG  ,(const uint8_t*)&reg.config,1,100))
            break;
       reg.meas_config.bit.meas_trigq=1;
       if(!writeReg(HDC2021_REG_MEASUR_CONFIG  ,(const uint8_t*)&reg.meas_config,1,100))
            break;
       err = HDC2021DEPT_ERR_NONE;
    }while(0);

    return err;
}

void Hdc2021debt_SetInterrupt(uint8_t interrupt_en, HDC2021IntPol pol)
{

   do
   {
      if(interrupt_en == HDC2021_INT_EN_NONE)
      {
           reg.config.bits.drdy = 0;
           if(!writeReg(HDC2021_REG_DEVICE_CONFIG,(const uint8_t*)&reg.config,1,100))
                break;
      }
      else
      {
          reg.config.bits.drdy = 1;
          reg.config.bits.int_pol=pol;
          reg.interrept_enable.bits.drdy=1;
          if(!writeReg(HDC2021_REG_DEVICE_CONFIG ,(const uint8_t*)&reg.config,2,100))
              break;
          if(!writeReg(HDC2021_REG_INTERRUPT_ENABLE,(const uint8_t*)&reg.interrept_enable,1,100))
              break;
       }
   }while(0);

}

HDC2021DEPTErr Hdc2021debt_Get_Status(uint8_t* status)
{
    HDC2021DEPTErr err = HDC2021DEPT_ERR_NOT_RESPONSE;
    do{
    	*status = 0;
        if(!readReg(HDC2021_REG_STATUS,status,1,50))
            break;
      err = HDC2021DEPT_ERR_NONE;
    }while(0);

    return err;
}

HDC2021DEPTErr Hdc2021debt_Check_Device_id(void)
{
    HDC2021DEPTErr err = HDC2021DEPT_ERR_NOT_CONNECT;
    uint8_t id[2];

    do {
        if (!readReg(HDC2021_REG_DEVICE_ID_LOW,id, 2, 100))
            break;

        err = HDC2021DEPT_ERR_NONE;
        if (0 != memcmp(id, HDC2021_DEVICE_ID, sizeof(id)))
            err = HDC2021DEPT_ERR_ID_MISMATCH;
    } while (0);

    return err;
}

const char* hdc2021_GetErrorString(HDC2021DEPTErr err)
{
    const char* ptr = NULL;
    switch (err)
    {
        case HDC2021DEPT_ERR_NONE:
            ptr="HDC2021DEPT_ERR_NONE";
            break;
        case HDC2021DEPT_ERR_NOT_CONNECT:
            ptr = "HDC2021DEPT_ERR_NOT_CONNECT";
            break;
        case HDC2021DEPT_ERR_NOT_RESPONSE:
            ptr = "HDC2021DEPT_ERR_NOT_RESPONSE";
            break;
        case HDC2021DEPT_ERR_ID_MISMATCH:
            ptr = "HDC2021DEPT_ERR_ID_MISMATCH ";
            break;
    }
    return ptr;
}

void hdc2021_Default_Reg(void)
{
    memset((void*)&reg,0x00,sizeof(hdc2021_registers));
    reg.temp_thr_l = HDC2021_DEFAULT_TEMP_THR_L;
    reg.temp_thr_h =  HDC2021_DEFAULT_TEMP_THR_H;
    reg.rh_thr_l = HDC2021_DEFAULT_RH_THR_L;
    reg.rh_thr_h = HDC2021_DEFAULT_RH_THR_H;
}
HDC2021DEPTErr Hdc2021dept_SoftReset(void)
{

    HDC2021DEPTErr err = HDC2021DEPT_ERR_NOT_RESPONSE;
    do{
        reg.config.bits.reset=1;
        if(!writeReg(HDC2021_REG_DEVICE_CONFIG,(const uint8_t*)&reg.config,1,100))
            break;
       err = HDC2021DEPT_ERR_NONE;
    }while(0);
    reg.config.bits.reset=0;
    return err;
}

bool readReg(const uint8_t reg, uint8_t *dst, size_t size, uint32_t timer)
{
    uint32_t txTimer = timer;
    uint32_t rxTimer = timer;

    // Disable transmit interrupt enable
    UCB0IE &= ~UCTXIE;
    // Send start condition.
    UCB0CTLW0 |= UCTR + UCTXSTT;
    // Send single byte data.
    while (!(UCB0IFG & UCTXIFG) && --txTimer)
        ;
    if (txTimer == 0)
        return STATUS_FAIL;

    UCB0TXBUF = reg;
    // Poll for transmit interrupt flag.
    while (!(UCB0IFG & UCTXIFG) && --txTimer)
        ;
    if (txTimer == 0)
        return STATUS_FAIL;

    // Set USCI in Receive mode
    UCB0CTLW0 &= ~UCTR;
    // Send start condition
    UCB0CTLW0 |= UCTXSTT;
    while(size--)
    {
        if(!size)
        {

        	while (UCB0CTLW0 & UCTXSTT)
        		;
            UCB0CTLW0 |= UCTXSTP;
        }
        	while (!(UCB0IFG & UCRXIFG) && --rxTimer)
        		;
        	if (rxTimer == 0)
        		return STATUS_FAIL;
        	*dst = (uint8_t)UCB0RXBUF;
        	dst++;
    }


    // Check if transfer timed out
    return STATUS_SUCCESS;
}

bool writeReg(uint8_t reg, const uint8_t* src,size_t size, uint32_t txTimer)
{

   //Store current TXIE status
    uint16_t txieStatus = UCB0IE & UCTXIE;
    //Disable transmit interrupt enable
    UCB0IE &= ~(UCTXIE);
    //Send start condition.
    UCB0CTLW0 |= UCTR +  UCTXSTT;
    //Poll for transmit interrupt flag.
    while (!(UCB0IFG & UCTXIFG) && --txTimer)
        ;
    if(txTimer == 0)
            return (STATUS_FAIL);
    UCB0TXBUF = reg;
    while (!(UCB0IFG & UCTXIFG) && --txTimer)
        ;
    if(txTimer == 0)
            return (STATUS_FAIL);
    //Send data;
    while(size--)
       {
        if(!size)
           while (!(UCB0IFG & UCTXIFG) && --txTimer)
               ;
           if (txTimer == 0)
               return (STATUS_FAIL);
           UCB0TXBUF = *src;
           src++;
           //{
             // UCB0CTLW0 |= UCTXSTP;
          // }
       }
    UCB0CTLW0 |= UCTXSTP;
    //Reinstate transmit interrupt enable
    UCB0IE |= txieStatus;
    return (STATUS_SUCCESS);
}








