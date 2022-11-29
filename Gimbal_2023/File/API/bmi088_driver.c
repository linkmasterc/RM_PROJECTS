#include "bmi088_driver.h"

int GetOffset(void);

u8 BMI088_Read_Gyro_Data(u8 reg_id)
{
    u8 rec;

    GYRO_CS_L;//GPIO_ResetBits(Open_SPIx_CSB2_PORT, Open_SPIx_CSB2_PIN)//GPIOB   GPIO_Pin_12
    rec = (u8)(0x00ff & spi_read_write_byte((u16)((reg_id<<8 | 0x00) | 0x8000)));//0b1000000000000000)));
    GYRO_CS_H;//GPIO_SetBits(Open_SPIx_CSB2_PORT, Open_SPIx_CSB2_PIN)//GPIOB   GPIO_Pin_12
    NOP;


    return rec;
}

void BMI088_Write_Gyro_Data(u8 reg_id,u8 data)
{
    GYRO_CS_L;
    spi_read_write_byte((u16)((reg_id<<8 | data) &0x7fff));// 0b0111111111111111));
    GYRO_CS_H;
    NOP;
}

u8 BMI088_Read_Acc_Data(u8 reg_id)
{
    u8 rec = 0;

    ACC_CS_L;
    spi_read_write_byte((u16)((reg_id<<8 | 0xff) | 0x8000));//0b1000000000000000));				//Dummy_Byte
    rec = (u8)(spi_read_write_byte(0x0000)>>8) & 0xff;
    ACC_CS_H;
    NOP;

    return rec;
}

void BMI088_Write_Acc_Data(u8 reg_id,u8 data)
{
    ACC_CS_L;
    spi_read_write_byte((u16)((reg_id<<8 | data) &0x7fff));// & 0b0111111111111111));
    ACC_CS_H;
    NOP;
}

u32 Acc_Range_Coe;

u8 Acc_ID,Gyro_ID;
int  GetStill = 0;
bool bmi088_init(void)								//IMU��ʼ��
{
    BMI088_Read_Acc_Data(ACC_ID);
    BMI088_Read_Gyro_Data(GYRO_ID);

    delay_ms(100);

    BMI088_Write_Acc_Data(ACC_PWR_CONF,0x00);
    BMI088_Write_Acc_Data(ACC_PWR_CTRL,0x04);

    BMI088_Read_Acc_Data(ACC_ID);
    BMI088_Read_Gyro_Data(GYRO_ID);

    //��ʼ��ʱҪ������AccΪNormalģʽ����ҪдPWR�Ĵ���

    Acc_ID = BMI088_Read_Acc_Data(ACC_ID);
    Gyro_ID = BMI088_Read_Gyro_Data(GYRO_ID);


    if(Acc_ID != 0x1e || Gyro_ID != 0x0f)		//ID_Wrong
    {
        BMI088_Write_Acc_Data(ACC_SOFT_R,0XB6);
        BMI088_Write_Gyro_Data(GYRO_SOFT_R,0XB6);
        delay_ms(50);
        return TRUE;
    }

    if((BMI088_Read_Acc_Data(ACC_ERR) & 0x1D/*0b00011101*/ )!= 0)				 //Acc_Err_Ocur
    {
        BMI088_Write_Acc_Data(ACC_SOFT_R,0XB6);
        BMI088_Write_Gyro_Data(GYRO_SOFT_R,0XB6);
        delay_ms(50);
        return TRUE;
    }

//Acc����
    //����ODR��Filter
    BMI088_Write_Acc_Data(ACC_CONF,0XAC);//0b10101100);										//���ü��ٶȼ��������1600Hz��Normal���ģʽ
    delay_ms(10);
    //���ò�����Χ
    BMI088_Write_Acc_Data(ACC_RANGE,0X01);//0b00000001);								  //���ü��ٶȼ������Χ+-6g
    delay_ms(10);
    //���õ�Դ����
    BMI088_Write_Acc_Data(ACC_PWR_CONF,0x00);															//���ý���ģʽ
    delay_ms(10);
    //���õ�Դ����
    BMI088_Write_Acc_Data(ACC_PWR_CTRL,0x04);															//�򿪼��ٶȼƵ�Դ
    delay_ms(10);
    //������ٶȼ�����ϵ��
    Acc_Range_Coe = pow(2,(BMI088_Read_Acc_Data(ACC_RANGE)+1)) + 2;
    delay_ms(10);

//Gyro����
    //���ò�����Χ
    BMI088_Write_Gyro_Data(GYRO_RANGE,0X01);															//��������������+-1000��/s
    delay_ms(10);
    //���õ�ͨ�˲�����
    BMI088_Write_Gyro_Data(GYRO_BANDW,0X00);															//�����������2000Hz������532Hz
    delay_ms(10);
    //���õ�Դģʽ
    BMI088_Write_Gyro_Data(GYRO_POWER,0x00);															//����ģʽ
    delay_ms(10);

    delay_ms(100);

    while(!GetStill)             //ֱ����⾲ֹ�ų���ѭ��
    {
        GetStill = GetOffset();
    }
    return FALSE;
}

void GetValue()
{
    u8 Acc_X_MSB;
    u8 Acc_X_LSB;
    u8 Acc_Y_MSB;
    u8 Acc_Y_LSB;
    u8 Acc_Z_MSB;
    u8 Acc_Z_LSB;

    u8 Gyro_X_MSB;
    u8 Gyro_X_LSB;
    u8 Gyro_Y_MSB;
    u8 Gyro_Y_LSB;
    u8 Gyro_Z_MSB;
    u8 Gyro_Z_LSB;

    u8 Temp_MSB;
    u8 Temp_LSB;
    u16 Temp_Ori;


    //ԭʼ���ݻ�ȡ

    //�ж�����״̬��Acc_Data_Rdy_Reg   ACC_STATUS
    Acc_X_MSB = BMI088_Read_Acc_Data(ACC_X_MSB);
    Acc_X_LSB = BMI088_Read_Acc_Data(ACC_X_LSB);
    Acc_Y_MSB = BMI088_Read_Acc_Data(ACC_Y_MSB);
    Acc_Y_LSB = BMI088_Read_Acc_Data(ACC_Y_LSB);
    Acc_Z_MSB = BMI088_Read_Acc_Data(ACC_Z_MSB);
    Acc_Z_LSB = BMI088_Read_Acc_Data(ACC_Z_LSB);

    Gyro_X_MSB = BMI088_Read_Gyro_Data(GYRO_X_MSB);
    Gyro_X_LSB = BMI088_Read_Gyro_Data(GYRO_X_LSB);
    Gyro_Y_MSB = BMI088_Read_Gyro_Data(GYRO_Y_MSB);
    Gyro_Y_LSB = BMI088_Read_Gyro_Data(GYRO_Y_LSB);
    Gyro_Z_MSB = BMI088_Read_Gyro_Data(GYRO_Z_MSB);
    Gyro_Z_LSB = BMI088_Read_Gyro_Data(GYRO_Z_LSB);

    Acc_X_Ori = ((s16)(Acc_X_MSB<<8 | Acc_X_LSB))/32767.0f*1000.0f*Acc_Range_Coe;			//RangeΪ���ٶȼ��趨�Ĳ�����Χ����λm*g

    Acc_Y_Ori = ((s16)(Acc_Y_MSB<<8 | Acc_Y_LSB))/32767.0f*1000.0f*Acc_Range_Coe;			//Range = 6g;
    Acc_Z_Ori = ((s16)(Acc_Z_MSB<<8 | Acc_Z_LSB))/32767.0f*1000.0f*Acc_Range_Coe;

    Gyro_X_Ori = ((s16)(Gyro_X_MSB<<8 | Gyro_X_LSB))/32767.0f*1000.0f;									//��λ��/s
    Gyro_Y_Ori = ((s16)(Gyro_Y_MSB<<8 | Gyro_Y_LSB))/32767.0f*1000.0f;									//Range = 1000��/s
    Gyro_Z_Ori = ((s16)(Gyro_Z_MSB<<8 | Gyro_Z_LSB))/32767.0f*1000.0f;

    //�׳���NaN
    Temp_MSB = BMI088_Read_Acc_Data(TEMP_MSB);
    Temp_LSB = BMI088_Read_Acc_Data(TEMP_LSB);

    Temp_Ori = (Temp_MSB*8) + (Temp_LSB/32);
    if(Temp_Ori > 1023)
        Real_Temp = (Temp_Ori - 2048)*0.125 + 23;
    else
        Real_Temp = Temp_Ori*0.125 + 23;
}


//��С���˷�
float leastSquareLinearFit(float x[], float y[], const int num)
{
    float a = 0;
//float b = 0;
    float sum_x = 0.0;
    float sum_y = 0.0;
    float sum_x2 = 0.0;
    float sum_xy = 0.0;

    for (int i = 0; i < num; ++i) {
        sum_x2 += x[i]*x[i];
        sum_y += y[i];
        sum_x += x[i];
        sum_xy += x[i]*y[i];
    }

    float c = num*sum_x2 - sum_x*sum_x;
    if(c == 0)
    {
        a = 10000;
    }
    else
    {
        a = (num*sum_xy - sum_x*sum_y)/(num*sum_x2 - sum_x*sum_x);
    }
//b = (sum_x2*sum_y - sum_x*sum_xy)/(num*sum_x2-sum_x*sum_x);
    return a;
}

//У׼ʱ������˸
int GetOffset(void)
{
    static u16 cnt=0;
    cnt++;
    if(cnt>1500)
    {
//        LED_BLUE_TOGGLE();
        cnt = 0;
    }

    GetValue();

    if(Cali_Cnt)
    {
        Status_offset[0][CALI_NUM-Cali_Cnt] = Gyro_X_Ori;
        Gyro_X_Offset += Gyro_X_Ori;
        Status_offset[1][CALI_NUM-Cali_Cnt] = Gyro_Y_Ori;
        Gyro_Y_Offset += Gyro_Y_Ori;
        Status_offset[2][CALI_NUM-Cali_Cnt] = Gyro_Z_Ori;
        Gyro_Z_Offset += Gyro_Z_Ori;
        Num_offset[CALI_NUM-Cali_Cnt]       = CALI_NUM-Cali_Cnt;
        Cali_Cnt--;
        //delay_us(50);
    }
    else
    {
        SEE  = Gyro_Z_Offset / CALI_NUM;
        for(int i = 0; i<3; i++)
        {

            testvalue[i] = leastSquareLinearFit(Num_offset,Status_offset[i],CALI_NUM);
            if((testvalue[i] > 0.00005l) || (testvalue[i] <  -0.00005l))
            {
                Cali_Cnt = CALI_NUM;
                Gyro_X_Offset = 0;
                Gyro_Y_Offset = 0;
                Gyro_Z_Offset = 0;
                return FAIL;
            }
        }
        Gyro_X_Offset /= CALI_NUM;
        Gyro_Y_Offset /= CALI_NUM;
        Gyro_Z_Offset /= CALI_NUM;
		
//		Gyro_X_Offset = 0.0610004216;
//        Gyro_Y_Offset = 0.0549818724;
//        Gyro_Z_Offset = 0.0410534367;
#if defined Infantry_B
//			 Gyro_X_Offset = 0.099246047f;//�����ʵ�⣬ÿ����������ϸ΢�Ĳ��
//			 Gyro_Y_Offset = 0.512502611f;
        Gyro_Z_Offset = 0.154409705f;
#elif defined Infantry_C
//			 Gyro_X_Offset = 0.099246047f;//�����ʵ�⣬ÿ����������ϸ΢�Ĳ��
//			 Gyro_Y_Offset = 0.512502611f;
        Gyro_Z_Offset = 0.207377998f;
#elif defined Infantry_D
//			 Gyro_X_Offset = 0.099246047f;//�����ʵ�⣬ÿ����������ϸ΢�Ĳ��
//			 Gyro_Y_Offset = 0.512502611f;
        Gyro_Z_Offset = 0.154409705f;
#elif defined Infantry_E
//			 Gyro_X_Offset = 0.099246047f;//�����ʵ�⣬ÿ����������ϸ΢�Ĳ��
//			 Gyro_Y_Offset = 0.512502611f;
//			 Gyro_Z_Offset = 0.07598843f;
//#else
//#error "No defined SelfType"
#endif
        Cali_Cnt = CALI_NUM;
        return SUCCESS;
    }
    return FAIL;
}


float Limit_Zero(float Target,float limit)
{
    if(Target < limit && Target > -limit)
        return 0.0f;
    else
        return Target;
}

float G_Cal;
bool Warning;

ST_LPF Gyro_X_Lpf = {0,0,0,OFF_FREQ,0.00025f};
ST_LPF Gyro_Y_Lpf = {0,0,0,OFF_FREQ,0.00025f};
ST_LPF Gyro_Z_Lpf = {0,0,0,OFF_FREQ,0.00025f};

float  ORI_Gyrox = 0;
float  ORI_Gyroy = 0;
float  ORI_Gyroz = 0;

float  ORI_Accx = 0;
float  ORI_Accy = 0;
float  ORI_Accz = 0;

#define Cali_Rol_CoeA 1

void Sensor_Data_Prepare()				//IMU����׼��������ת�����˲���������ƫ��
{
    GetValue();
    //������ƫ
    Acc_X_Ori = Cali_Rol_CoeA*(Acc_X_Ori - Acc_X_Offset);
    Acc_Y_Ori = Cali_Rol_CoeA*(Acc_Y_Ori - Acc_Y_Offset);
    Acc_Z_Ori = Cali_Rol_CoeA*(Acc_Z_Ori - Acc_Z_Offset);

    Gyro_X_Ori = Cali_Rol_Coe*(Gyro_X_Ori - Gyro_X_Offset);
    Gyro_Y_Ori = Cali_Pit_Coe*(Gyro_Y_Ori - Gyro_Y_Offset);
    Gyro_Z_Ori = Cali_Yaw_Coe*(Gyro_Z_Ori - Gyro_Z_Offset);

    ORI_Gyrox = Gyro_X_Ori;
    ORI_Gyroy = Gyro_Y_Ori;
    ORI_Gyroz = Gyro_Z_Ori;

    ORI_Accx = Acc_X_Ori;
    ORI_Accy = Acc_Y_Ori;
    ORI_Accz = Acc_Z_Ori;

    gyro_x.in = Gyro_X_Ori;
    LpFilter(&gyro_x);
    Gyro_X_Ori = gyro_x.out;

    gyro_y.in = Gyro_Y_Ori;
    LpFilter(&gyro_y);
    Gyro_Y_Ori = gyro_y.out;

    gyro_z.in = Gyro_Z_Ori;
    LpFilter(&gyro_z);
    Gyro_Z_Ori = gyro_z.out;

    //��ͨ�˲�
    acc_x.in = Acc_X_Ori;
    LpFilter(&acc_x);
    Acc_X_Ori = acc_x.out;

    acc_y.in = Acc_Y_Ori;
    LpFilter(&acc_y);
    Acc_Y_Ori = acc_y.out;

    acc_z.in = Acc_Z_Ori;
    LpFilter(&acc_z);
    Acc_Z_Ori = acc_z.out;


    //�����Ǽ�����	�׳���NaN
    Gyro_X_Ori = Limit_Zero(Gyro_X_Ori,0.002f);
    Gyro_Y_Ori = Limit_Zero(Gyro_Y_Ori,0.002f);
    Gyro_Z_Ori = Limit_Zero(Gyro_Z_Ori,0.002f);

    //��λת��
    Gyro_X_Real = Gyro_X_Ori*0.0174533f;
    Gyro_Y_Real = Gyro_Y_Ori*0.0174533f;
    Gyro_Z_Real = Gyro_Z_Ori*0.0174533f;

	Gyro_X_Speed = Gyro_X_Real / PI * 180.0f;
	Gyro_Y_Speed = Gyro_Y_Real / PI * 180.0f;
	Gyro_Z_Speed = Gyro_Z_Real / PI * 180.0f;

    Acc_X_Real = Acc_X_Ori;
    Acc_Y_Real = Acc_Y_Ori;
    Acc_Z_Real = Acc_Z_Ori;

    G_Cal = sqrt(Acc_X_Real*Acc_X_Real + Acc_Y_Real*Acc_Y_Real + Acc_Z_Real*Acc_Z_Real);

    if(G_Cal > 6000 * 1.74f)
        Warning = TRUE;
    else if(!(Gyro_X_Real < 2000 && Gyro_X_Real > -2000))
        Warning = TRUE;
    else
        Warning = FALSE;

    if(imu_mode == INIT)
        imu_mode = NORMAL;
    else if(imu_mode == CALIBRATION)
        imu_mode = CALIBRATION;
}


//
//	�������ƣ�LpFilter(ST_LPF* lpf)
//	�������ܣ�Ƶ���˲���
//
void LpFilter(ST_LPF* lpf) 
{                                                                   
	float fir_a = 1/(1 + lpf->off_freq * lpf->samp_tim); 
	lpf->preout = lpf->out;
  lpf->out = fir_a * lpf->preout + (1 - fir_a) * lpf->in;
}


//
//	�������ƣ�invSqrt(float x)
//	�������ܣ�ƽ�����������㷨������һ��ţ�ٵ���������
//
float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}