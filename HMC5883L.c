/*
 * HMC5883L.c
 *
 *  Created on: Oct 1, 2014
 *      Author: gwilson
 */
#include <driverlib.h>
#include "HMC5883L.h"
#include "BackChannel.h"

uint8_t R_Data[6];          // Rx data array
uint8_t ReadTx[2];          // Request read data
bool dataReady;

//private functions
bool I2C_masterSendMultiple(uint8_t hmcRegister, uint8_t txData[],
		uint16_t txLength, uint32_t timeout) {
	uint8_t byte = 0;
	bool status = STATUS_SUCCESS;
	if (txLength < 1)
		return STATUS_FAIL;
	USCI_B_I2C_setSlaveAddress(HMCI2C_BASE, HMC5883L_ADDRESS);
	USCI_B_I2C_setMode(HMCI2C_BASE, USCI_B_I2C_TRANSMIT_MODE);
	USCI_B_I2C_enableInterrupt(HMCI2C_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
	status &= USCI_B_I2C_masterMultiByteSendStartWithTimeout(HMCI2C_BASE,
			hmcRegister, timeout);

	while (status == STATUS_SUCCESS && txLength-- > 1)
		status &= USCI_B_I2C_masterMultiByteSendNextWithTimeout(HMCI2C_BASE,
				txData[byte++], timeout);
	if (status == STATUS_SUCCESS)
		status &= USCI_B_I2C_masterMultiByteSendFinishWithTimeout(HMCI2C_BASE,
				txData[byte], timeout);
	return status;
}

bool I2C_masterSendByte(uint8_t hmcRegister, uint8_t txData, uint32_t timeout) {
	uint8_t data[1];
	data[0] = txData;
	return I2C_masterSendMultiple(hmcRegister, data, 1, timeout);
}

uint8_t mode;

uint8_t I2C_masterReadMultiple(uint8_t hmcRegister, uint8_t rxData[],
		uint16_t rxLength, uint32_t timeout) {
	uint8_t byte = 0;
	uint16_t passedInLength = rxLength;
	if (rxLength < 1) {
		if (BackChannel_Connected())
			BackChannel_WriteLine("Bad length passed in.");
		return STATUS_FAIL;
	}

	USCI_B_I2C_setMode(HMCI2C_BASE, USCI_B_I2C_TRANSMIT_MODE);
	USCI_B_I2C_disableInterrupt(HMCI2C_BASE, USCI_B_I2C_RECEIVE_INTERRUPT);
	USCI_B_I2C_enableInterrupt(HMCI2C_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
	if (USCI_B_I2C_masterMultiByteSendStartWithTimeout(HMCI2C_BASE, hmcRegister,
			timeout) == STATUS_FAIL) {
		if (BackChannel_Connected())
			BackChannel_WriteLine("Sending register address to slave failed.");
		return STATUS_FAIL;
	}

	USCI_B_I2C_setMode(HMCI2C_BASE, USCI_B_I2C_RECEIVE_MODE);
	USCI_B_I2C_disableInterrupt(HMCI2C_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
	USCI_B_I2C_enableInterrupt(HMCI2C_BASE, USCI_B_I2C_RECEIVE_INTERRUPT);
	USCI_B_I2C_masterMultiByteReceiveStart(HMCI2C_BASE);   //Not sure if needed
	while (rxLength > 1) {
		rxLength--;
		while (~USCI_B_I2C_RECEIVE_INTERRUPT & UCRXIFG)
			;
		rxData[byte++] = USCI_B_I2C_masterMultiByteReceiveNext(HMCI2C_BASE);
	}
	while (~USCI_B_I2C_RECEIVE_INTERRUPT & UCRXIFG)
		;
	if (USCI_B_I2C_masterMultiByteReceiveFinishWithTimeout(
	HMCI2C_BASE, (uint8_t *) (*rxData + byte), timeout) == STATUS_SUCCESS) {
		rxLength = passedInLength;
		return rxLength;
	}
	if (BackChannel_Connected())
		BackChannel_WriteLine("Final recieve byte failed.");
	return STATUS_FAIL;
}

uint8_t I2C_masterReadByte(uint8_t hmcRegister, uint32_t timeout) {
	uint8_t b;
	bool ret = I2C_masterReadMultiple(hmcRegister, &b, 1, timeout);
	if (ret == STATUS_FAIL) {
		if (BackChannel_Connected())
			BackChannel_WriteLine("Read byte failed.");
		return STATUS_FAIL;
	}
	return b;
}

bool I2C_masterWriteBits(uint8_t hmcRegister, uint8_t bitStart, uint8_t length,
		uint8_t data, uint32_t timeout) {
	uint8_t b;
	b = I2C_masterReadByte(hmcRegister, timeout);
	uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
	data <<= (bitStart - length + 1); // shift data into correct position
	data &= mask; // zero all non-important bits in data
	b &= ~(mask); // zero all important bits in existing byte
	b |= data; // combine data with existing byte
	return I2C_masterSendByte(hmcRegister, b, timeout);
}

/** Read multiple bits from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t I2C_masterReadBits(uint8_t regAddr, uint8_t bitStart, uint8_t length,
		uint8_t *data, uint16_t timeout) {
	// 01101001 read byte
	// 76543210 bit numbers
	//    xxx   args: bitStart=4, length=3
	//    010   masked
	//   -> 010 shifted
	uint8_t b = I2C_masterReadByte(regAddr, 100000);
	uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
	b &= mask;
	b >>= (bitStart - length + 1);
	*data = b;
	return STATUS_SUCCESS;
}

typedef bool (*configFunctionType)(uint8_t);

bool HMC_ConfigureAndCheck(configFunctionType * configFunctions, uint8_t * args,
		uint8_t length) {
	bool status = STATUS_SUCCESS;
	uint8_t i;
	for (i = 0; i < length; i++) {
		status &= (*configFunctions)(*args++);
		if (status)
			continue;
		else {
			if (BackChannel_Connected()) {
				BackChannel_Write("Config step Failed.");
				unsigned char * stepNum = "0";
				*stepNum += (i + 1);
				BackChannel_Write(stepNum);
				BackChannel_WriteLine(" Failed.");
			}
			return STATUS_FAIL;
		}
	}
	return status;
}

//public functions
bool HMC_initialize() {
//	//P4DIR = 0xFF;// |= BIT1 + BIT2;
//	if (HMCI2C_BASE == USCI_B1_BASE) {
//		P4DIR |= BIT1 + BIT2;         // Assign I2C pins to USCI_B1
//		P4OUT = (BIT1 + BIT2);
		P4SEL |= BIT1 + BIT2;         // Assign I2C pins to USCI_B1
//	}
//	if (HMCI2C_BASE == USCI_B0_BASE) {
//		P3DIR = 0xFF;         // |= BIT1 + BIT2;
//		P3OUT = (BIT1 + BIT0);
//		P3SEL |= BIT1 + BIT0;         // Assign I2C pins to USCI_B0
//	}
	P2DIR &= ~BIT6;
	P2IE |= BIT6;
	P2IES |= BIT6;

	//Initialize I2C to 100khz master mode
	USCI_B_I2C_masterInit(HMCI2C_BASE, USCI_B_I2C_CLOCKSOURCE_SMCLK,
			UCS_getSMCLK(), USCI_B_I2C_SET_DATA_RATE_100KBPS);
	USCI_B_I2C_enable(HMCI2C_BASE);

	//Initialize the settings of the HMC5883
	ReadTx[0] = 0x02;
	ReadTx[1] = 0x01;         // send start address for data in the slave device
	R_Data[0] = 0;
	R_Data[1] = 0;
	R_Data[2] = 0;
	R_Data[3] = 0;
	R_Data[4] = 0;
	R_Data[5] = 0; //define array for data receive

	//Send configuration data
//	HMC_setSampleAveraging(2);
//	HMC_setDataRate(75);
//	HMC_setMeasurementBias(0);
//	HMC_setGain(5);
	//I2C_masterSendMultiple(HMC5883L_RA_CONFIG_A, powerOn, 4, 10000);
	configFunctionType initFunctions[] = { *HMC_setSampleAveraging,
			*HMC_setDataRate, *HMC_setMeasurementBias, *HMC_setGain };
	uint8_t funcArgs[] = { 2, 75, 0, 5 };
	if (HMC_ConfigureAndCheck(initFunctions, funcArgs, 4) == STATUS_SUCCESS) {
		while (!dataReady)
			LPM0; //Wait for dataReady
		return STATUS_SUCCESS;
	}
	if (BackChannel_Connected())
		BackChannel_WriteLine("HMC_initialize Failed.");
	return STATUS_FAIL;

}

bool HMC_testConnection() {
	if (BackChannel_Connected())
		BackChannel_WriteLine("Testing HMC connection.");

	int test = I2C_masterReadMultiple(HMC5883L_RA_ID_A, R_Data, 3, 100000);
	if (BackChannel_Connected())
		BackChannel_WriteLine(itoa(test));

	if (test == 3) {
		if (BackChannel_Connected()) {
			BackChannel_Write("ID returned: ");
			BackChannel_WriteLine((uint8_t *) R_Data);
		}

		return (R_Data[0] == 'H' && R_Data[1] == '4' && R_Data[2] == '3') ?
				STATUS_SUCCESS : STATUS_FAIL;
	}
	if (BackChannel_Connected())
		BackChannel_WriteLine("Test Connection Failed.");
	return STATUS_FAIL;
}

//// CONFIG_A register
/** Get number of samples averaged per measurement.
 * @return Current samples averaged per measurement (0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_AVERAGING_8
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
uint8_t HMC_getSampleAveraging() {
	return I2C_masterReadByte(HMC5883L_RA_CONFIG_A, 10000);
}
/** Set number of samples averaged per measurement.
 * @param averaging New samples averaged per measurement setting(0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
bool HMC_setSampleAveraging(uint8_t averaging) {
	bool ret = I2C_masterWriteBits(HMC5883L_RA_CONFIG_A,
	HMC5883L_CRA_AVERAGE_BIT,
	HMC5883L_CRA_AVERAGE_LENGTH, averaging, 100000);
	if (ret == STATUS_FAIL) {
		if (BackChannel_Connected())
			BackChannel_WriteLine("HMC_setSampleAveraging Failed.");
		return ret;	//Breakpoint here
	}
	return ret;
}
/** Get data output rate value.
 * The Table below shows all selectable output rates in continuous measurement
 * mode. All three channels shall be measured within a given output rate. Other
 * output rates with maximum rate of 160 Hz can be achieved by monitoring DRDY
 * interrupt pin in single measurement mode.
 *
 * Value | Typical Data Output Rate (Hz)
 * ------+------------------------------
 * 0     | 0.75
 * 1     | 1.5
 * 2     | 3
 * 3     | 7.5
 * 4     | 15 (Default)
 * 5     | 30
 * 6     | 75
 * 7     | Not used
 *
 * @return Current rate of data output to registers
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
uint8_t HMC_getDataRate() {
	uint8_t b;
	if (I2C_masterReadBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT,
	HMC5883L_CRA_RATE_LENGTH, &b, 10000) == STATUS_SUCCESS)
		return b;
	if (BackChannel_Connected())
		BackChannel_WriteLine("HMC_getDataRate Failed.");
	return STATUS_FAIL;
}
/** Set data output rate value.
 * @param rate Rate of data output to registers
 * @see getDataRate()
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
bool HMC_setDataRate(uint8_t rate) {
	if (I2C_masterWriteBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT,
	HMC5883L_CRA_RATE_LENGTH, rate, 10000) == STATUS_SUCCESS)
		return STATUS_SUCCESS;
	if (BackChannel_Connected())
		BackChannel_WriteLine("HMC_setDataRate Failed.");
	return STATUS_FAIL;
}
/** Get measurement bias value.
 * @return Current bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
uint8_t HMC_getMeasurementBias() {
	uint8_t b;
	I2C_masterReadBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_BIAS_BIT,
	HMC5883L_CRA_BIAS_LENGTH, &b, 10000);
	return b;
}
/** Set measurement bias value.
 * @param bias New bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
bool HMC_setMeasurementBias(uint8_t bias) {
	if (I2C_masterWriteBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_BIAS_BIT,
	HMC5883L_CRA_BIAS_LENGTH, bias, 10000) == STATUS_SUCCESS)
		return STATUS_SUCCESS;
	if (BackChannel_Connected())
		BackChannel_WriteLine("HMC_setMeasurementBias Failed.");
	return STATUS_FAIL;
}

// CONFIG_B register

/** Get magnetic field gain value.
 * The table below shows nominal gain settings. Use the "Gain" column to convert
 * counts to Gauss. Choose a lower gain value (higher GN#) when total field
 * strength causes overflow in one of the data output registers (saturation).
 * The data output range for all settings is 0xF800-0x07FF (-2048 - 2047).
 *
 * Value | Field Range | Gain (LSB/Gauss)
 * ------+-------------+-----------------
 * 0     | +/- 0.88 Ga | 1370
 * 1     | +/- 1.3 Ga  | 1090 (Default)
 * 2     | +/- 1.9 Ga  | 820
 * 3     | +/- 2.5 Ga  | 660
 * 4     | +/- 4.0 Ga  | 440
 * 5     | +/- 4.7 Ga  | 390
 * 6     | +/- 5.6 Ga  | 330
 * 7     | +/- 8.1 Ga  | 230
 *
 * @return Current magnetic field gain value
 * @see HMC5883L_GAIN_1090
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
uint8_t HMC_getGain() {
	uint8_t b;
	I2C_masterReadBits(HMC5883L_RA_CONFIG_B, HMC5883L_CRB_GAIN_BIT,
	HMC5883L_CRB_GAIN_LENGTH, &b, 10000);
	return b;
}
/** Set magnetic field gain value.
 * @param gain New magnetic field gain value
 * @see getGain()
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
bool HMC_setGain(uint8_t gain) {
	if (I2C_masterWriteBits(HMC5883L_RA_CONFIG_B, HMC5883L_CRB_GAIN_BIT,
	HMC5883L_CRB_GAIN_LENGTH, gain, 10000) == STATUS_SUCCESS)
		return STATUS_SUCCESS;
	if (BackChannel_Connected())
		BackChannel_WriteLine("HMC_setGain Failed.");
	return STATUS_FAIL;
}

// MODE register

/** Get measurement mode.
 * In continuous-measurement mode, the device continuously performs measurements
 * and places the result in the data register. RDY goes high when new data is
 * placed in all three registers. After a power-on or a write to the mode or
 * configuration register, the first measurement set is available from all three
 * data output registers after a period of 2/fDO and subsequent measurements are
 * available at a frequency of fDO, where fDO is the frequency of data output.
 *
 * When single-measurement mode (default) is selected, device performs a single
 * measurement, sets RDY high and returned to idle mode. Mode register returns
 * to idle mode bit values. The measurement remains in the data output register
 * and RDY remains high until the data output register is read or another
 * measurement is performed.
 *
 * @return Current measurement mode
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
uint8_t HMC_getMode() {
	uint8_t b;
	I2C_masterReadBits(HMC5883L_RA_MODE, HMC5883L_MODEREG_BIT,
	HMC5883L_MODEREG_LENGTH, &b, 10000);
	return b;
}
/** Set measurement mode.
 * @param newMode New measurement mode
 * @see getMode()
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
bool HMC_setMode(uint8_t newMode) {
	// use this method to guarantee that bits 7-2 are set to zero, which is a
	// requirement specified in the datasheet; it's actually more efficient than
	// using the I2Cdev.writeBits method
	if (I2C_masterSendByte(HMC5883L_RA_MODE,
			mode << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1),
			10000)==STATUS_SUCCESS) {
		mode = newMode; // track to tell if we have to clear bit 7 after a read
		return STATUS_SUCCESS;
	}
	if (BackChannel_Connected())
		BackChannel_WriteLine("HMC_setMode Failed.");
	return STATUS_FAIL;
}

// DATA* registers
void HMC_getHeading(int16_t *x, int16_t *y, int16_t *z) {
	I2C_masterReadMultiple(3, R_Data, 6, 10000);
	if (mode == HMC5883L_MODE_SINGLE)
		I2C_masterSendByte(HMC5883L_RA_MODE,
				HMC5883L_MODE_SINGLE
						<< (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1),
				10000);
	*x = (((int16_t) R_Data[0]) << 8) | R_Data[1];
	*y = (((int16_t) R_Data[4]) << 8) | R_Data[5];
	*z = (((int16_t) R_Data[2]) << 8) | R_Data[3];
}
int16_t HMC_getHeadingX() {
	int16_t *x = 0, *y = 0, *z = 0;
	HMC_getHeading(x, y, z);
	return *x;
}
int16_t HMC_getHeadingY() {
	int16_t *x = 0, *y = 0, *z = 0;
	HMC_getHeading(x, y, z);
	return *y;
}
int16_t HMC_getHeadingZ() {
	int16_t *x = 0, *y = 0, *z = 0;
	HMC_getHeading(x, y, z);
	return *z;
}

// STATUS register

/** Get data output register lock status.
 * This bit is set when this some but not all for of the six data output
 * registers have been read. When this bit is set, the six data output registers
 * are locked and any new data will not be placed in these register until one of
 * three conditions are met: one, all six bytes have been read or the mode
 * changed, two, the mode is changed, or three, the measurement configuration is
 * changed.
 * @return Data output register lock status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_LOCK_BIT
 */
bool HMC_getLockStatus() {
	uint8_t b = I2C_masterReadByte(HMC5883L_RA_STATUS, 10000);
	return (b & (1 << HMC5883L_STATUS_LOCK_BIT)) ? true : false;
}
/** Get data ready status.
 * This bit is set when data is written to all six data registers, and cleared
 * when the device initiates a write to the data output registers and after one
 * or more of the data output registers are written to. When RDY bit is clear it
 * shall remain cleared for 250 us. DRDY pin can be used as an alternative to
 * the status register for monitoring the device for measurement data.
 * @return Data ready status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_READY_BIT
 */
bool HMC_getReadyStatus() {
	uint8_t b = I2C_masterReadByte(HMC5883L_RA_STATUS, 10000);
	return (b & (1 << HMC5883L_STATUS_READY_BIT)) ? true : false;
}

// ID_* registers
uint8_t HMC_getIDA();
uint8_t HMC_getIDB();
uint8_t HMC_getIDC();

uint8_t HMC_devAddr;

#pragma vector = PORT2_VECTOR
__interrupt void HMC_PORT2_ISR(void) {
	if (P2IFG & BIT6)
		dataReady = true;
	P2IFG &= ~BIT6;
	LPM0_EXIT;
}
