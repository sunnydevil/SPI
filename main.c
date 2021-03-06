#include <main.h>


int main(void) {

	//const float alpha=0.5;

	sensor=BMX055;
	received_ch=0;
	aRes = 4.0/2048.0;
	gRes = 2000.0/32768.0;
	mRes = 10.0 * 4219.0/32760.0;
	read =0;


	InitSPI();
	InitDisplay();

	lcdInitialise(LCD_ORIENTATION0);
	lcdClearDisplay(decodeRgbValue(255,255,255));

	lcdPutS("ax=        mG",(lcdTextX(1)),(lcdTextY(1)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));
	lcdPutS("ay=        mG",(lcdTextX(1)),(lcdTextY(3)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));
	lcdPutS("az=        mG",(lcdTextX(1)),(lcdTextY(5)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));
	lcdPutS("gx=        Grad/s",(lcdTextX(1)),(lcdTextY(7)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));
	lcdPutS("gy=        Grad/s",(lcdTextX(1)),(lcdTextY(9)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));
	lcdPutS("gz=        Grad/s",(lcdTextX(1)),(lcdTextY(11)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));

	switch (sensor) {
		case MPU9250:
		{
			Init_MPU9250();
			Setup_Wake_on_Motion_Interrupt();
			break;
		}
		case LSM9DS1:
		{
			Init_LSM9DS1();
			break;
		}
		case BMX055:
		{
			Init_BMX055();
			break;
		}
		case BMI160:
		{
			Init_BMI160();
			break;
		}
		default:
			break;
	}

	__enable_interrupt();

	while(1)
	{

		switch (sensor) {
				case MPU9250:
				{
					whoami= SPI_Read(MPU9250_AGM,MPUREG_WHOAMI);				//should be 0x71
					Init_MPU9250();
					Read_Accelorameter(accelorameter_raw);
					ax=accelorameter_raw[0]*aRes;
					ay=accelorameter_raw[1]*aRes;
					az=accelorameter_raw[2]*aRes;
					Read_Gyroscope(gyroscope_raw);
					gx=gyroscope_raw[0]*gRes;
					gy=gyroscope_raw[1]*gRes;
					gz=gyroscope_raw[2]*gRes;
					Read_Magnetometer(magnetometer_raw);
					mx=magnetometer_raw[0]*mRes;
					my=magnetometer_raw[1]*mRes;
					mz=magnetometer_raw[2]*mRes;
					temperature = ((float)Read_Temp()/333.87+ 21.0);
					break;
				}
				case LSM9DS1:
				{
					whoami=SPI_Read(LSM9DS1_M,0x0F);								//0x3D
					Read_Accelorameter(accelorameter_raw);
					ax=accelorameter_raw[0]*aRes;
					ay=accelorameter_raw[1]*aRes;
					az=accelorameter_raw[2]*aRes;
					Read_Gyroscope(gyroscope_raw);
					gx=gyroscope_raw[0]*gRes;
					gy=gyroscope_raw[1]*gRes;
					gz=gyroscope_raw[2]*gRes;
					Read_Magnetometer(magnetometer_raw);
					mx=magnetometer_raw[0]*0.58;
					my=magnetometer_raw[1]*0.58;
					mz=magnetometer_raw[2]*0.58;
					temperature = ((float)Read_Temp()/16+ 25.0);
					break;
				}
				case BMX055:
				{
					//whoami=SPI_Read(BMX055_A,0x00);								//0xFA
					//whoami=SPI_Read(BMX055_M,0x40);									//0x32
					if (read == 1) {
						Read_Accelorameter(accelorameter_raw);
						ax=accelorameter_raw[0]*aRes;
						ay=accelorameter_raw[1]*aRes;
						az=accelorameter_raw[2]*aRes;
						read=0;
					}

					/*Read_Gyroscope(gyroscope_raw);
					gx=gyroscope_raw[0]*gRes;
					gy=gyroscope_raw[1]*gRes;
					gz=gyroscope_raw[2]*gRes;
					test_0=SPI_Read(BMX055_M,0x4e);
					test_1=SPI_Read(BMX055_M,BMX055_MAG_PWR_CNTL1);
					test_2=SPI_Read(BMX055_M,0x4c);
					Read_Magnetometer(magnetometer_raw);
					mx=magnetometer_raw[0]*0.079;
					my=magnetometer_raw[1]*0.079;
					mz=magnetometer_raw[2]*0.152;
					temperature = ((float)Read_Temp()/2 + 23.0);*/
					break;
				}
				case BMI160:
				{
					whoami=SPI_Read(CS_0,0x00);							//0xD1
					test_0=SPI_Read(BMI160_AG,BMI160_USER_ERROR_ADDR);
					test_1=SPI_Read(BMI160_AG,BMI160_USER_PMU_STAT_ADDR);
					Read_Accelorameter(accelorameter_raw);
					ax=accelorameter_raw[0]*aRes;
					ay=accelorameter_raw[1]*aRes;
					az=accelorameter_raw[2]*aRes;
					Read_Gyroscope(gyroscope_raw);
					gx=gyroscope_raw[0]*gRes;
					gy=gyroscope_raw[1]*gRes;
					gz=gyroscope_raw[2]*gRes;
					//temperature = ((float)Read_Temp()/2 + 23.0);
					break;
				}
				default:
					break;
			}


	//-------BMI160------------------------------------------------------------------
		//whoami=SPI_Read(CS_0,0x00);							//0xD1

		char str[7];

		sprintf(str,"%d",((int) (ax*1000)));
		String_number_rightify((ax*1000),str);
		lcdPutS(str,(lcdTextX(5)),(lcdTextY(1)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));
		sprintf(str,"%d",((int) (ay*1000)));
		String_number_rightify((ay*1000),str);
		lcdPutS(str,(lcdTextX(5)),(lcdTextY(3)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));
		sprintf(str,"%d",((int) (az*1000)));
		String_number_rightify((az*1000),str);
		lcdPutS(str,(lcdTextX(5)),(lcdTextY(5)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));

		sprintf(str,"%d",((int) gx));
		String_number_rightify(gx,str);
		lcdPutS(str,(lcdTextX(5)),(lcdTextY(7)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));
		sprintf(str,"%d",((int) gy));
		String_number_rightify(gy,str);
		lcdPutS(str,(lcdTextX(5)),(lcdTextY(9)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));
		sprintf(str,"%d",((int) gz));
		String_number_rightify(gz,str);
		lcdPutS(str,(lcdTextX(5)),(lcdTextY(11)),(decodeRgbValue(0,0,0)),(decodeRgbValue(255,255,255)));

	}
}

void String_number_rightify(float number, char str[]){


	if ((((int) (number))<=99999 && ((int) (number))>=10000)  || (((int) (number))>=-99999 && ((int) (number))<=-10000 ) ) {
		if (str[0]==0x2d) {
			str[6]=0;
			str[5]=str[5];
			str[4]=str[4];
			str[3]=str[3];
			str[2]=str[2];
			str[1]=str[1];
			str[0]=str[0];
		}
		else
		{str[6]=0;
		str[5]=str[4];
		str[4]=str[3];
		str[3]=str[2];
		str[2]=str[1];
		str[1]=str[0];
		str[0]=0x20;}
	}
	else if ((((int) (number))<=9999 && ((int) (number))>=1000) || (((int) (number))>=-9999 && ((int) (number))<=-1000)) {
		if (str[0]==0x2d) {
			str[6]=0;
			str[5]=str[4];
			str[4]=str[3];
			str[3]=str[2];
			str[2]=str[1];
			str[1]=0x20;
			str[0]=str[0];
		}
		else{
		str[6]=0;
		str[5]=str[3];
		str[4]=str[2];
		str[3]=str[1];
		str[2]=str[0];
		str[1]=0x20;
		str[0]=0x20;}
	}
	else if ((((int) (number))<=999 && ((int) (number))>=100) || (((int) (number))>=-999 && ((int) (number))<=-100)) {
		if (str[0]==0x2d) {
			str[6]=0;
			str[5]=str[3];
			str[4]=str[2];
			str[3]=str[1];
			str[2]=0x20;
			str[1]=0x20;
			str[0]=str[0];
		}
		else{
		str[6]=0;
		str[5]=str[2];
		str[4]=str[1];
		str[3]=str[0];
		str[2]=0x20;
		str[1]=0x20;
		str[0]=0x20;}
	}
	else if ((((int) (number))<=99 && ((int) (number))>=10) || (((int) (number))>=-99 && ((int) (number))<=-10)) {
		if (str[0]==0x2d) {
			str[6]=0;
			str[5]=str[2];
			str[4]=str[1];
			str[3]=0x20;
			str[2]=0x20;
			str[1]=0x20;
			str[0]=str[0];
		}
		else{
		str[6]=0;
		str[5]=str[1];
		str[4]=str[0];
		str[3]=0x20;
		str[2]=0x20;
		str[1]=0x20;
		str[0]=0x20;}
	}
	else if ((((int) (number))<=9 && ((int) (number))>=0) || (((int) (number))>=-9 && ((int) (number))<=-1)) {
		if (str[0]==0x2d) {
			str[6]=0;
			str[5]=str[1];
			str[4]=0x20;
			str[3]=0x20;
			str[2]=0x20;
			str[1]=0x20;
			str[0]=str[0];
		}
		else{
		str[6]=0;
		str[5]=str[0];
		str[4]=0x20;
		str[3]=0x20;
		str[2]=0x20;
		str[1]=0x20;
		str[0]=0x20;}
	}
}

void InitSPI(){
	//--------------Init SPI ----------------------------------------------------------------------

	// Sensor			MPU9250			LSM9DS1			BMX055			BMI160
	//Port 1.7	CS											M
	//Port 1.6	CS							M				G
	//Port 1.5  CS		AGM					AG				A
	//Port 1.4	CLK
	//Port 1.3  Int
	//Port 1.2	MOSI
	//Port 1.1	MISO

	WDTCTL = WDTPW + WDTHOLD; 								// Stop WDT

	P1OUT |= BIT5 + BIT6 + BIT7;							//Port 1.5,1.6,1.7 as High
	P1DIR |= BIT5 + BIT6 + BIT7;							//Port 1.5,1.6,1.7 as Output
	P1SEL = BIT1 | BIT2 | BIT4;								//Port Bit 1,2,4 as SPI Interface
	P1SEL2 = BIT1 | BIT2 | BIT4;							//Port Bit 1,2,4 as SPI Interfasce

	UCA0CTL1 = UCSWRST;
	UCA0CTL0 |= /*UCCKPH +*/ UCCKPL + UCMSB + UCMST + UCSYNC; 	// 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2; 									// SMCLK
	UCA0BR0 |= 0x02; 										// /2
	UCA0BR1 = 0; 											//
	UCA0MCTL = 0; 											// No modulation
	UCA0CTL1 &= ~UCSWRST; 									// **Initialize USCI state machine**


	//P1DIR |= BIT6;											//LED2 as OUtput
	//P1OUT &= ~BIT6;											//LED2 as off
	P1IE |= BIT3;											//P1.3 Interrupt enabled
	P1IES &= ~BIT3;											//Interrupt direction from low to high
	P1IFG &= ~BIT3;											//P1.3 IFG is cleared

	//----------------Init SPI End----------------------------------------------------------------------

		/*
		 * SPI  INTERFACE
		 *
		 * 		MSB	|	6	|	5	|	4	|	3	|	2	|	1	|	LSB
		 * 	------------------------------------------------------------------
		 * 		R/W	|	A6	|	A5	|	A4	|	A3	|	A2	|	A1	|	A0
		 *
		 *
		 * 		MSB	|	6	|	5	|	4	|	3	|	2	|	1	|	LSB
		 * 	------------------------------------------------------------------
		 * 		D7	|	D6	|	D5	|	D4	|	D3	|	D2	|	D1	|	D0
		 *
		 * 		Read	1
		 * 		Write	0
		 * 		A		Address
		 * 		D		Data
		 * 		*/
}

void InitDisplay(){

	/**
	 * 2.0		Read "L" Write "H"
	 * 2.1		Reset
	 * 2.2		CS
	 * 2.3		SCK
	 * 2.4		MOSI
	 */

	P2OUT |= BIT0 + BIT1 + BIT2 + BIT4;							//Port 2.0,2.1,2.2 as High
	P2OUT &= ~BIT3;
	P2DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;							//Port 2.0,2.1,2.2 as Output


	P2OUT &= ~(LCD_RESET);									//Reset auf Low
	__delay_cycles(100000);
	P2OUT |= LCD_RESET;										//Rest High
	__delay_cycles(200000);
}

void Init_MPU9250(){

	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_1,BIT_H_RESET);			//RESET_All

	_delay_cycles(120000);								//100ms delay

	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_1,0x00);					//CYCLE =1 -> Clock Source

	_delay_cycles(120000);								//100ms delay

	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_2,0x00);					//Enable ACC,Gyro

	SPI_Write(MPU9250_AGM,MPUREG_CONFIG,0x03);						//

	SPI_Write(MPU9250_AGM,MPUREG_GYRO_CONFIG,0x18);					//Gyro


	SPI_Write(MPU9250_AGM,MPUREG_ACCEL_CONFIG,0x08);				//Accel -> 4G Range


	SPI_Write(MPU9250_AGM,MPUREG_ACCEL_CONFIG_2,0x09);				//Accel 2


	SPI_Write(MPU9250_AGM,MPUREG_INT_PIN_CFG,0x22);					//int




	SPI_Write(MPU9250_AGM,MPUREG_USER_CTRL,0x20);					//user -> Enable fifo operation mode

	SPI_Write(MPU9250_AGM,MPUREG_I2C_MST_CTRL,0x0d);				//ctrl -> MPU-9250 clock divider for i2C -> /20 -> 400kHz

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR);	//addr -> Write AK8963 I2C Address into Register for I2C Communication with Slave

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_REG,AK8963_CNTL2);		//reg -> I2C slave register address from where to begin data transfer

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_DO,0x01);					//do -> Data to be written if I2C Slave 0 enabled -> Reset magnetometer

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_CTRL,0x81);				//ctrl ->

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_REG,AK8963_CNTL1);		//reg

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_DO,0x12);					//do	-> Magnetometer continous Measurement 16 bit

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_CTRL,0x81);				//ctrl

	MagID= Read_Magnetometer_Id();

}

void Init_LSM9DS1(){

	SPI_Write(LSM9DS1_AG,CTRL_REG8,0x01);		//Reset A&G Device
	__delay_cycles(100000);

	SPI_Write(LSM9DS1_AG,CTRL_REG1_G,((G_ODR_952<<5)+(G_SCALE_2000DPS<<3)));	//Gyroscope Setup
	SPI_Write(LSM9DS1_AG,CTRL_REG6_XL,((XL_ODR_952<<5)+(A_SCALE_4G<<3)));		//Accelorameter Setup
	SPI_Write(LSM9DS1_M,CTRL_REG3_M,0x80);										//Mag i2C disable + Contionus mode
	SPI_Write(LSM9DS1_M,CTRL_REG1_M,0x7C);										//Magnetometer UHP-Mode+80HZ ODR
	SPI_Write(LSM9DS1_M,CTRL_REG2_M,(M_SCALE_16GS<<5));

}

void Init_BMX055(){

	SPI_Write(BMX055_A,BMX055_ACC_BGW_SOFTRESET,0xB6);
	SPI_Write(BMX055_G,BMX055_GYRO_BGW_SOFTRESET,0xB6);
	SPI_Write(BMX055_M,BMX055_MAG_PWR_CNTL1,0x82);
	__delay_cycles(100000);
	SPI_Write(BMX055_A,BMX055_ACC_PMU_RANGE,AFS_4G);
	SPI_Write(BMX055_A,BMX055_ACC_PMU_BW,ABW_1000Hz);
	SPI_Write(BMX055_G,BMX055_GYRO_RANGE,GFS_2000DPS);
	SPI_Write(BMX055_G,BMX055_GYRO_BW,(0x80|G_2000Hz230Hz));
	SPI_Write(BMX055_M,BMX055_MAG_PWR_CNTL1,0x01);
	__delay_cycles(5000);
	SPI_Write(BMX055_M,BMX055_MAG_PWR_CNTL2,0x00);
	SPI_Write(BMX055_M,BMX055_MAG_REP_XY,0x04);								//Rep x/y 	=9  -> 2 x 4 +1 =9
	SPI_Write(BMX055_M,BMX055_MAG_REP_Z,0x0E);								//Rep z 	=15 -> 14 +1 =15

	//-------- ACC Low Power Mode 1 100ms sleep interval

	SPI_Write(BMX055_A,BMX055_ACC_PMU_LPW,0x5A);							//ACC Low_power enabled + sleep duration 100ms
	SPI_Write(BMX055_A,BMX055_ACC_PMU_LOW_POWER,0x00);						//ACC LPM1 + event driven time base mode

	SPI_Write(BMX055_A,BMX055_ACC_INT_EN_1,0x10);							//ACC interrupt new data ready enabled
	SPI_Write(BMX055_A,BMX055_ACC_INT_MAP_1,0x01);							//ACC Data ready interrupt Int1 Pin

	//------- Gyro in deep suspend mode

	SPI_Write(BMX055_G,BMX055_GYRO_LPM1,0x20);								//Gyro in deep-suspend mode
	SPI_Write(BMX055_G,BMX055_GYRO_LPM2,0x10);								//Gyro external Wakeup Int3 Pin
	SPI_Write(BMX055_G,BMX055_GYRO_INT_EN_0,0x80);							//Gyro enable Data Ready interrupt
	SPI_Write(BMX055_G,BMX055_GYRO_INT_MAP_1,0x80);							//Gyro Interrupt on Int4 Pin

	//------- Magnetometer

	SPI_Write(BMX055_M,BMX055_MAG_PWR_CNTL1,0x00);							//MAG Suspend mode


}

void Init_BMI160(){

	SPI_Write(BMI160_AG,BMI160_CMD_COMMANDS_ADDR,0xB6);			//softreset
	__delay_cycles(200000);
	SPI_Write(BMI160_AG,BMI160_USER_ACCEL_CONFIG_ADDR,0x23);	//ACC CONFIG: 2 -> normal mode, 3 ODR 3,125Hz
	SPI_Write(BMI160_AG,BMI160_USER_ACCEL_RANGE_ADDR,0x05);		//ACC Range 0x03 -> 2g, 0x05 -> 4g, 0x08 ->8g
	SPI_Write(BMI160_AG,BMI160_USER_GYRO_CONFIG_ADDR,0x26);		//GYRO Config 2 -> normal Mode, 6 -> 25HZ output rate
	SPI_Write(BMI160_AG,BMI160_USER_GYRO_RANGE_ADDR,0x00);		//Gyro Range: 2000�/s
	SPI_Write(BMI160_AG,BMI160_CMD_COMMANDS_ADDR,0x11);			//Start ACC
	__delay_cycles(10000);
	SPI_Write(BMI160_AG,BMI160_CMD_COMMANDS_ADDR,0x15);			//Start Gyro


}

int Read_Temp(){

	int rawData[2];

	switch (sensor) {
		case MPU9250:
		{
			rawData[0]=(int)SPI_Read(MPU9250_AGM,MPUREG_TEMP_OUT_H);
			rawData[1]=(int)SPI_Read(MPU9250_AGM,MPUREG_TEMP_OUT_L);
			break;
		}
		case LSM9DS1:
		{
			rawData[0]=(int)SPI_Read(LSM9DS1_AG,OUT_TEMP_H);
			rawData[1]=(int)SPI_Read(LSM9DS1_AG,OUT_TEMP_L);
			break;
		}
		case BMX055:
				{
					rawData[0]=0;
					rawData[1]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_TEMP);
					break;
				}
		default:
			break;
	}


	return ((rawData[0]<<8)|rawData[1]);
}

void Read_Accelorameter(int * destination){

	int rawData[6];

	switch (sensor) {
		case MPU9250:
		{	rawData[0]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_XOUT_H);
			rawData[1]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_XOUT_L);
			rawData[2]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_YOUT_H);
			rawData[3]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_YOUT_L);
			rawData[4]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_ZOUT_H);
			rawData[5]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_ZOUT_L);

			destination[0]=(rawData[0]<<8)|rawData[1];
			destination[1]=(rawData[2]<<8)|rawData[3];
			destination[2]=(rawData[4]<<8)|rawData[5];
			break;
		}
		case LSM9DS1:
		{
			rawData[0]=(int)SPI_Read(LSM9DS1_AG,OUT_X_H_XL);
			rawData[1]=(int)SPI_Read(LSM9DS1_AG,OUT_X_L_XL);
			rawData[2]=(int)SPI_Read(LSM9DS1_AG,OUT_Y_H_XL);
			rawData[3]=(int)SPI_Read(LSM9DS1_AG,OUT_Y_L_XL);
			rawData[4]=(int)SPI_Read(LSM9DS1_AG,OUT_Z_H_XL);
			rawData[5]=(int)SPI_Read(LSM9DS1_AG,OUT_Z_L_XL);

			destination[0]=(rawData[0]<<8)|rawData[1];
			destination[1]=(rawData[2]<<8)|rawData[3];
			destination[2]=(rawData[4]<<8)|rawData[5];
			break;

		}
		case BMX055:
		{
			rawData[1]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_X_LSB);
			rawData[0]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_X_MSB);
			rawData[3]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_Y_LSB);
			rawData[2]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_Y_MSB);
			rawData[5]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_Z_LSB);
			rawData[4]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_Z_MSB);

			destination[0]=(rawData[0]<<4)|((rawData[1]>>4)&0x0F);
			destination[1]=(rawData[2]<<4)|((rawData[3]>>4)&0x0F);
			destination[2]=(rawData[4]<<4)|((rawData[5]>>4)&0x0F);
			break;
		}
		case BMI160:
		{
			rawData[0]=(int)SPI_Read(BMI160_AG,BMI160_ACC_X_MSB);
			rawData[1]=(int)SPI_Read(BMI160_AG,BMI160_ACC_X_LSB);
			rawData[2]=(int)SPI_Read(BMI160_AG,BMI160_ACC_Y_MSB);
			rawData[3]=(int)SPI_Read(BMI160_AG,BMI160_ACC_Y_LSB);
			rawData[4]=(int)SPI_Read(BMI160_AG,BMI160_ACC_Z_MSB);
			rawData[5]=(int)SPI_Read(BMI160_AG,BMI160_ACC_Z_LSB);

			destination[0]=(rawData[0]<<8)|rawData[1];
			destination[1]=(rawData[2]<<8)|rawData[3];
			destination[2]=(rawData[4]<<8)|rawData[5];
			break;
		}
		default:
			break;
	}
}

void Read_Gyroscope(int * destination){

	int rawData[6];

	switch (sensor) {
		case MPU9250:
		{
			rawData[0]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_XOUT_H);
			rawData[1]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_XOUT_L);
			rawData[2]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_YOUT_H);
			rawData[3]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_YOUT_L);
			rawData[4]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_ZOUT_H);
			rawData[5]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_ZOUT_L);
			break;
		}
		case LSM9DS1:
		{
			rawData[0]=(int)SPI_Read(LSM9DS1_AG,OUT_X_H_G);
			rawData[1]=(int)SPI_Read(LSM9DS1_AG,OUT_X_L_G);
			rawData[2]=(int)SPI_Read(LSM9DS1_AG,OUT_Y_H_G);
			rawData[3]=(int)SPI_Read(LSM9DS1_AG,OUT_Y_L_G);
			rawData[4]=(int)SPI_Read(LSM9DS1_AG,OUT_Z_H_G);
			rawData[5]=(int)SPI_Read(LSM9DS1_AG,OUT_Z_L_G);
			break;
		}
		case BMX055:
		{
			rawData[0]=(int)SPI_Read(BMX055_G,BMX055_GYRO_RATE_X_MSB);
			rawData[1]=(int)SPI_Read(BMX055_G,BMX055_GYRO_RATE_X_LSB);
			rawData[2]=(int)SPI_Read(BMX055_G,BMX055_GYRO_RATE_Y_MSB);
			rawData[3]=(int)SPI_Read(BMX055_G,BMX055_GYRO_RATE_Y_LSB);
			rawData[4]=(int)SPI_Read(BMX055_G,BMX055_GYRO_RATE_Z_MSB);
			rawData[5]=(int)SPI_Read(BMX055_G,BMX055_GYRO_RATE_Z_LSB);
			break;
		}
		case BMI160:
		{
			rawData[0]=(int)SPI_Read(BMI160_AG,BMI160_GYRO_X_MSB);
			rawData[1]=(int)SPI_Read(BMI160_AG,BMI160_GYRO_X_LSB);
			rawData[2]=(int)SPI_Read(BMI160_AG,BMI160_GYRO_Y_MSB);
			rawData[3]=(int)SPI_Read(BMI160_AG,BMI160_GYRO_Y_LSB);
			rawData[4]=(int)SPI_Read(BMI160_AG,BMI160_GYRO_Z_MSB);
			rawData[5]=(int)SPI_Read(BMI160_AG,BMI160_GYRO_Z_LSB);
			break;
		}
		default:
			break;
	}

	destination[0]=(rawData[0]<<8)|rawData[1];
	destination[1]=(rawData[2]<<8)|rawData[3];
	destination[2]=(rawData[4]<<8)|rawData[5];
}

void Read_Magnetometer(int * destination){


	int rawData[6];

	switch (sensor) {
		case MPU9250:
		{
			SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_ADDR,(AK8963_I2C_ADDR|0x80));			//Read from Magnetometer
			SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_REG,AK8963_HXL);						//Write Address of to read Mag register
			SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_CTRL,0x87);							// 0x8- Read Data from AK8963 0x-7 Read 7 bytes

			rawData[0]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_01);				//HXH
			rawData[1]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_00);				//HXL
			rawData[2]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_03);				//HYH
			rawData[3]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_02);				//HYL
			rawData[4]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_05);				//HZH
			rawData[5]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_04);				//HZL

			destination[0]=(rawData[0]<<8)|rawData[1];
			destination[1]=(rawData[2]<<8)|rawData[3];
			destination[2]=(rawData[4]<<8)|rawData[5];
			break;
		}
		case LSM9DS1:
		{
			rawData[0]=(int)SPI_Read(LSM9DS1_M,OUT_X_H_M);				//HXH
			rawData[1]=(int)SPI_Read(LSM9DS1_M,OUT_X_L_M);				//HXL
			rawData[2]=(int)SPI_Read(LSM9DS1_M,OUT_Y_H_M);				//HYH
			rawData[3]=(int)SPI_Read(LSM9DS1_M,OUT_Y_L_M);				//HYL
			rawData[4]=(int)SPI_Read(LSM9DS1_M,OUT_Z_H_M);				//HZH
			rawData[5]=(int)SPI_Read(LSM9DS1_M,OUT_Z_L_M);				//HZL

			destination[0]=(rawData[0]<<8)|rawData[1];
			destination[1]=(rawData[2]<<8)|rawData[3];
			destination[2]=(rawData[4]<<8)|rawData[5];
			break;
		}
		case BMX055:
		{
			rawData[0]=(int)SPI_Read(BMX055_M,BMX055_MAG_XOUT_MSB);				//HXH
			rawData[1]=((int)SPI_Read(BMX055_M,BMX055_MAG_XOUT_LSB)>>3);				//HXL
			rawData[2]=(int)SPI_Read(BMX055_M,BMX055_MAG_YOUT_MSB);				//HYH
			rawData[3]=((int)SPI_Read(BMX055_M,BMX055_MAG_YOUT_LSB)>>3);				//HYL
			rawData[4]=(int)SPI_Read(BMX055_M,BMX055_MAG_ZOUT_MSB);				//HZH
			rawData[5]=((int)SPI_Read(BMX055_M,BMX055_MAG_ZOUT_LSB)>>1);				//HZL

			destination[0]=(rawData[0]<<5)|rawData[1];
			destination[1]=(rawData[2]<<5)|rawData[3];
			destination[2]=(rawData[4]<<7)|rawData[5];
			break;
		}
		default:
			break;
	}


}

int Read_Magnetometer_Id(){

	int id;

	_delay_cycles(100);

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_ADDR,(AK8963_I2C_ADDR|0x80));			//Read from Magnetometer
	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_REG,0x00);				//Write Address of to read Mag register
	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_CTRL,0x81);							// 0x8- Read Data from AK8963 0x-1 Read 1 bytes

	_delay_cycles(100);

	id= SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_00);

	return(id);
}

void Setup_Wake_on_Motion_Interrupt(){

	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_1,0x00);			//Powermanagement sleep=0, cycle=0,
	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_2,0x07);			//Disable Gyro
	SPI_Write(MPU9250_AGM,MPUREG_ACCEL_CONFIG_2,0x09);		//Set ACC to 184HZ

	SPI_Write(MPU9250_AGM,MPUREG_MOT_DETECT_CTRL,0xc0);		//Enable Motion Detection Logic
	SPI_Write(MPU9250_AGM,MPUREG_MOT_THR,0x0f);				//Motion Threshold LSB -> 15
	SPI_Write(MPU9250_AGM,MPUREG_LP_ACCEL_ODR,0x04);		//Wake up intervall 3.91HZ
	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_1,0x20);			//Cycle =1;

	SPI_Write(MPU9250_AGM,MPUREG_INT_ENABLE,0x40);			//Enable Motion Interrupt
}


void SPI_Write (char cs_signal,char reg, char data){

	SPI_Transceive(cs_signal,reg,data);
}

char SPI_Read (char cs_signal, char reg){


	return(SPI_Transceive(cs_signal,(reg|0x80),0x55));
}

char SPI_Transceive(char cs_signal,char reg,char data) {



	P1OUT &= (~cs_signal); 							// Pin 1.5 LOW

	while (!(IFG2 & UCA0TXIFG)); 					// USCI_A0 TX buffer ready?
	UCA0TXBUF = reg; 								// Send variable "reg" over SPI to Slave
	while (!(IFG2 & UCA0RXIFG)); 					// USCI_A0 RX Received?
	received_ch = UCA0RXBUF;						// Store received data

	while (!(IFG2 & UCA0TXIFG)); 					// USCI_A0 TX buffer ready?
	UCA0TXBUF = data; 								// Send variable "data" over SPI to Slave
	while (!(IFG2 & UCA0RXIFG)); 					// USCI_A0 RX Received?
	received_ch = UCA0RXBUF;						// Store received data


	P1OUT |= (cs_signal); 							// Pin 1.5 High

	_delay_cycles(150);

	return (received_ch);
}



#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){

	//MPU9250
	/*P1OUT ^= BIT6;				//Toggle LED
	P1IFG &= ~BIT3;				//Clear Interrupt Flag
	SPI_Read(MPU9250_AGM,MPUREG_INT_STATUS);//Clear INterrupt MPU9250*/

	//BMX055

	read=1;
	P1IFG &= ~BIT3;				//Clear Interrupt Flag
}
