#include "main.h"
#include "shoot_task.h"
#include "global_declare.h"
#include "rm_algorithm.h"
#include "mode_choose_task.h"
#include "ParamConfig.h"



static void 	RCShootBullet(void);											// ң�����
static void 	AutoShootBullet(void);											// �Զ����
static void 	GBShoot(int Shoot_f, bool mode);								// ��Ƶ�ʲ���
static void		RammerDetect(void);												// �ӵ�װ��������
static void 	ShootNumberRecord(bool mode);									// ʵ�ʷ����ӵ�����¼
static void 	ShootAreaDetect(void);											// �������������
static void 	UPShooterPID(void);												// ����ò���PID����

/** --------------------------------------------------------------------------
  * @brief  �����ӵ�����
			
  * @note	ң����ͨ��4�²�����Ħ���ּ����������ϲ��ر�
			ң����ͨ��2��3���Ʋ���Ƶ��
			
			�Զ���״̬��ֻ�и������С��һ��ֵ�Ż�������
			�Զ�����ĵ�ƵΪ9hz
 -------------------------------------------------------------------------- **/
void ShootBullet(void)
{

	/* ң�ش� */
	if(ControlMode == 0x01	|| ControlMode == 0x02 )
	{
		RCShootBullet();
	}
		
//	/* �Զ��� */
//	else if(ControlMode == 0x03 || ControlMode == 0x06 || ControlMode == 0x08 || 
//			ControlMode == 0x09)
//	{	
//		AutoShootBullet();
//	}
	
	
/****************/
	

//	/*���������*/
//	ShootAreaDetect();

//	/*�ӵ�װ��������*/
//	RammerDetect();
	
	/*��¼״̬*/
	ShootNumberRecord(DNShootMode);																							// ��¼����̨ʵ�ʷ����ӵ���	
	DNSTC.PreBoDan_Flag = DNSTC.BoDan_Flag;
	Bullet_Des_Pre 	= Bullet_Des;
}

void RCShootBullet()
{
	static u32	FrictionTick 	= 0;																						// ����Ħ������ʱ����
	static bool BoDanFirstIn			= FALSE;																					

	/*ң����ͨ��4����Ħ���ּ���������*/
	if(g_StDbus.stRC.Ch4 - RC_CH_VALUE_OFFSET > 300)						
	{
		if(stFlag.ShootFlag)
		{
			Friction_State 	= 700;																						// �²�ͨ��4����Ħ����
		}

		FrictionTick			= systemMonitor.SysTickTime;
		BoDanFirstIn					= TRUE;
	}
	else if(g_StDbus.stRC.Ch4 - RC_CH_VALUE_OFFSET < -300)																	// �ϲ�ͨ��4�ر�Ħ���ֺͲ���
	{
		if(stFlag.ShootFlag)
		{
			Friction_State 	= 500;
			DNSTC.BoDan_Flag 	= FALSE;
		}	
	}
	
	
	/*��ʱ�򿪲�������*/
	if(systemMonitor.SysTickTime - FrictionTick > 0 && (Friction_State == 700 || Friction_State_UP == 700) && BoDanFirstIn)// ����Ħ���ֺ���ʱ0s������������
	{
		if(Friction_State == 700)	{DNSTC.BoDan_Flag 		= TRUE;}
		BoDanFirstIn											= FALSE;
	}
	/*ң����ͨ��2��3���Ʋ���Ƶ��*/

	if(DNSTC.BoDan_Flag)																									// ����̨����
	{
		if((Friction_State == 700) && stFlag.ShootFlag)												
		{
			if(g_StDbus.stRC.Ch3 - RC_CH_VALUE_OFFSET < -300 )
			{	GBShoot(1, DNShootMode);}
			else if(g_StDbus.stRC.Ch2 - RC_CH_VALUE_OFFSET > 300)
			{	GBShoot(9, DNShootMode);}
			else if(g_StDbus.stRC.Ch3 - RC_CH_VALUE_OFFSET > 300)
			{	GBShoot(13, DNShootMode);}
			else if(g_StDbus.stRC.Ch2 - RC_CH_VALUE_OFFSET < -300)
			{	GBShoot(17, DNShootMode);}
		}
	}
}


/** --------------------------------------------------------------------------
	* @brief  	��������
  
	* @retval 	None
  
	* @param	Shoot_f		����Ƶ��Hz
	 			mode		����ģʽ��	UPShootMode ����̨����
										DNShootMode ����̨����
			
	* @note		��ͬ�Ĳ���Ƶ�ʶ�Ӧ��ͬ�Ĳ���ʱ����
 -------------------------------------------------------------------------- **/
void GBShoot(int Shoot_f, bool mode)
{
	static u32 time_Shoot = 0;

	if(mode == DNShootMode)
	{
		if(systemMonitor.SysTickTime - time_Shoot > 1000 / Shoot_f)
		{
			Bullet_Des += 1;															// ����̨����Ŀ����Ŀ����
			time_Shoot= systemMonitor.SysTickTime;
		}
	}
}

/** --------------------------------------------------------------------------
  * @brief  ����ʵ�ʷ�������¼
  
  * @retval None
  
  * @param	None

  * @note	���������̨�ڿ���״̬��ʵ������ӵ���
			������Shoot_Cnt_UP_FB
 -------------------------------------------------------------------------- **/
void ShootNumberRecord(bool mode)
{
	if(mode == DNShootMode)
	{
		if(DNSTC.BoDan_Flag && !DNSTC.PreBoDan_Flag)
		{
			DNSTC.Fact_PreShootPos 	= ShootEncoder.fpSumValue;
			DNSTC.Fact_ShootPos 	= DNSTC.Fact_PreShootPos;
		}
		else if(DNSTC.BoDan_Flag)
		{
			DNSTC.Fact_PreShootPos 	= DNSTC.Fact_ShootPos;
			DNSTC.Fact_ShootPos = ShootEncoder.fpSumValue;
		}
		DNSTC.Fact_pShootNumber 	= (s32)((DNSTC.Fact_ShootPos - DNSTC.Fact_PreShootPos) / (-(BOTTOM_SUPPLY_STEP)) + 0.5f);
		DNSTC.Fact_ShootSumNumber 	+= DNSTC.Fact_pShootNumber;
	}
}



