#include "mode_choose_task.h"
#include "global_declare.h"
//#include "data_send_task.h"




#define DN_OFF			TRUE
#define DN_ON			FALSE
#define CHASSIS_OFF		TRUE
#define CHASSIS_ON		FALSE
#define DN_SHOOT_ON		TRUE
#define DN_SHOOT_OFF	FALSE





static void 	MotorInit(bool DNOff, bool ChassisOff);								// �˶������ʼ��

static void 	ShootInit(bool UPShoot);											// ���������ʼ��

//static void 	ShootSpeedStrategy(void);														// ���پ���

//static void 	UPShootSpeedDistanceStraegy(void);												// ����̨���پ��ߣ��з����룩

//static void 	UPShootSpeedHeatStraegy(void);													// ����̨���پ��ߣ���ǰ������

//static void 	DNShootSpeedDistanceStraegy(void);												// ����̨���پ��ߣ��з����룩

//static void 	DNShootSpeedHeatStraegy(void);													// ����̨���پ��ߣ���ǰ������

//static void 	ChassisMoveStrategy(void);														// �����˶���ʽ����

//static void 	DangerStateJudge(void);															// Σ��״̬�ж�

//static void		EnemyDistanceJudge(void);														// �з������ж�

//static void 	RecognizeBanlanceSoldier(void);													// ƽ�ⲽ���ж�


void ModeChoose(void)
{
	if(g_StDbus.stRC.SW_L==RC_SW_DOWN && g_StDbus.stRC.SW_R==RC_SW_DOWN)
	{
		ControlMode=0x00;
		MotorInit(DN_OFF,CHASSIS_OFF);
		ShootInit(DN_SHOOT_OFF);
	}
	else if (g_StDbus.stRC.SW_L==RC_SW_MID && g_StDbus.stRC.SW_R==RC_SW_DOWN)
	{
		ControlMode=0x01;
		MotorInit(DN_ON,CHASSIS_OFF);
		ShootInit(DN_SHOOT_ON);
	}
	else if(g_StDbus.stRC.SW_L==RC_SW_UP && g_StDbus.stRC.SW_R==RC_SW_DOWN)
	{
		ControlMode=0x02;
		MotorInit(DN_ON,CHASSIS_OFF);
		ShootInit(DN_SHOOT_ON);
	}

}




/** --------------------------------------------------------------------------
  * @brief  ��̨&���̵��ģʽ��ʼ��
			
  * @note	1000Hz
			ʹ��ң��������̨����ģʽ����ѡ��
 -------------------------------------------------------------------------- **/
void MotorInit(bool DNOff, bool ChassisOff)
{	
	if(DNOff)
	{
		stFlag.RunFlag 		= FALSE;	// ����̨���ʧ��
	}
	else	{	stFlag.RunFlag = TRUE;}
	
	
//	/*���̵��*/
//	if(ChassisOff)
//	{
//		stFlag.CS_RunFlag 		= FALSE;													// ���̵��ʧ��
//		rush.StartFlag 			= FALSE;													// ����������־λ����
//		StartTimes				= 500;														// �����궨	
//	}
//	else	{	stFlag.CS_RunFlag = TRUE;}
}





/** --------------------------------------------------------------------------
  * @brief  ���������ʼ��
			
  * @note	
 -------------------------------------------------------------------------- **/
void ShootInit(bool DNShoot)
{
	if(DNShoot)
	{
		stFlag.ShootFlag = TRUE;								// �������������־λ
	}
	else
	{
		//�ⵯģʽ����		
		stFlag.ShootFlag = FALSE;											// �������������־λ
		Friction_State_UP 	= 500;												// Ħ���ֿ�����־λ
		DNSTC.BoDan_Flag 	= FALSE;											// ������־λ
		FrictFirstIn 		= TRUE;										// ����Ħ������ʱ����
		BoDanFirstIn		= TRUE;	
	}
	

}





///** --------------------------------------------------------------------------
//  * @brief  ��Ƶ����
//			
//  * @note	
// -------------------------------------------------------------------------- **/
//void ShootSpeedStrategy(void)
//{
//	if(stFlag.SniperFlag)										// ʶ��Ŀ��
//	{
//		DNShootSpeedDistanceStraegy();																						// ���ݵз�����ѡ��Ƶ
//		DNShootSpeedHeatStraegy();																							// ����ǹ������ѡ��Ƶ
//	}
//	if(stFlag.SniperFlag)										// ʶ��Ŀ��
//	{
//		UPShootSpeedDistanceStraegy();																						// ���ݵз�����ѡ��Ƶ
//		UPShootSpeedHeatStraegy();																							// ����ǹ������ѡ��Ƶ	
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  ����̨��Ƶ���ߣ��з����룩
//			
//  * @note	���ȼ�����ǰ����>�з�����
// -------------------------------------------------------------------------- **/
//void UPShootSpeedDistanceStraegy(void)
//{
//	if(Enemy_Distance_UP < DEFEND_THIRD_LINE_DISTANCE_UP)
//	{
//		Shoot_Frequency_UP = DEFEND_THIRD_LINE_SHOOT_FRC_UP;
//	}
//	else if(Enemy_Distance_UP < DEFEND_SECOND_LINE_DISTANCE_UP)
//	{
//		Shoot_Frequency_UP = DEFEND_SECOND_LINE_SHOOT_FRC_UP;
//	}
//	else if(Enemy_Distance_UP < DEFEND_FIRST_LINE_DISTANCE_UP)
//	{
//		Shoot_Frequency_UP = DEFEND_FIRST_LINE_SHOOT_FRC_UP;
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  ����̨��Ƶ���ߣ���ǰǹ��������
//			
//  * @note	���ȼ�����ǰ����>�з�����
//			������������
// -------------------------------------------------------------------------- **/
//void UPShootSpeedHeatStraegy(void)
//{
//	static bool RecoverFlag = FALSE;
//	if((Barrel_Heat_Cur_UP >= BARREL_HEAT_THIRD_LINE_UP) || (RecoverFlag && (Barrel_Heat_Cur_UP >= BARREL_HEAT_FIRST_LINE_UP)))
//	{
//		if(Shoot_Frequency_UP >= BARREL_HEAT_THIRD_LINE_SHOOT_FRC_UP)
//		{
//			Shoot_Frequency_UP = BARREL_HEAT_THIRD_LINE_SHOOT_FRC_UP;
//			RecoverFlag = TRUE;
//		}
//	}
//	else if(Barrel_Heat_Cur_UP >= BARREL_HEAT_SECOND_LINE_UP)
//	{
//		if(Shoot_Frequency_UP >= BARREL_HEAT_SECOND_LINE_SHOOT_FRC_UP)
//		{
//			Shoot_Frequency_UP = BARREL_HEAT_SECOND_LINE_SHOOT_FRC_UP;
//		}
//		RecoverFlag = FALSE;
//	}
//	else if(Barrel_Heat_Cur_UP < BARREL_HEAT_SECOND_LINE_UP)
//	{
//		if(Shoot_Frequency_UP > BARREL_HEAT_FIRST_LINE_SHOOT_FRC_UP)
//		{
//			Shoot_Frequency_UP = BARREL_HEAT_FIRST_LINE_SHOOT_FRC_UP;
//		}
//		RecoverFlag = FALSE;
//	}
//	else
//	{
//		RecoverFlag = FALSE;
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  ����̨��Ƶ���ߣ��з����룩
//			
//  * @note	���ȼ�����ǰ����>�з�����
// -------------------------------------------------------------------------- **/
//void DNShootSpeedDistanceStraegy(void)
//{
//	if(Enemy_Distance_DN < DEFEND_THIRD_LINE_DISTANCE_DN)
//	{
//		Shoot_Frequency_DN = DEFEND_THIRD_LINE_SHOOT_FRC_DN;
//	}
//	else if(Enemy_Distance_DN < DEFEND_SECOND_LINE_DISTANCE_DN)
//	{
//		Shoot_Frequency_DN = DEFEND_SECOND_LINE_SHOOT_FRC_DN;
//	}
//	else if(Enemy_Distance_DN < DEFEND_FIRST_LINE_DISTANCE_DN)
//	{
//		Shoot_Frequency_DN = DEFEND_FIRST_LINE_SHOOT_FRC_DN;
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  ����̨��Ƶ���ߣ���ǰǹ��������
//			
//  * @note	���ȼ�����ǰ����>�з�����
//			������������
// -------------------------------------------------------------------------- **/
//void DNShootSpeedHeatStraegy(void)
//{
//	static bool RecoverFlag = FALSE;
//	
//	if((Barrel_Heat_Cur_DN >= BARREL_HEAT_THIRD_LINE_DN) || (RecoverFlag && (Barrel_Heat_Cur_DN >= BARREL_HEAT_FIRST_LINE_DN)))
//	{
//		if(Shoot_Frequency_DN >= BARREL_HEAT_THIRD_LINE_SHOOT_FRC_DN)
//		{
//			Shoot_Frequency_DN = BARREL_HEAT_THIRD_LINE_SHOOT_FRC_DN;
//			RecoverFlag = TRUE;
//		}
//	}
//	else if(Barrel_Heat_Cur_DN >= BARREL_HEAT_SECOND_LINE_DN)
//	{
//		if(Shoot_Frequency_DN >= BARREL_HEAT_SECOND_LINE_SHOOT_FRC_DN)
//		{
//			Shoot_Frequency_DN = BARREL_HEAT_SECOND_LINE_SHOOT_FRC_DN;
//		}
//		RecoverFlag = FALSE;
//	}
//	else if(Barrel_Heat_Cur_DN < BARREL_HEAT_SECOND_LINE_DN)
//	{
//		if(Shoot_Frequency_DN >= BARREL_HEAT_FIRST_LINE_SHOOT_FRC_DN)
//		{
//			Shoot_Frequency_DN = BARREL_HEAT_FIRST_LINE_SHOOT_FRC_DN;
//		}
//		RecoverFlag = FALSE;
//	}
//	else
//	{
//		RecoverFlag = FALSE;
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  Σ��״̬�ж�
//			
//  * @note	װ�װ��ܵ��˺�����ΪΣ��״̬��20s��û�ܵ��˺���Ϊ��ȫ״̬
// -------------------------------------------------------------------------- **/
//void DangerStateJudge(void)													
//{
//	static u32 DangerTime = 0;
//	
//	if(RobotHurt.hurt_type == 0)
//	{
//		rush.DangerFlag = TRUE;
//		DangerTime = systemMonitor.SysTickTime;
//		RobotHurt.hurt_type = 9;
//	}
//	if(systemMonitor.SysTickTime - DangerTime > 20000)
//	{
//		rush.DangerFlag = FALSE;
//	}
//}




///** --------------------------------------------------------------------------
//  * @brief  �з������ж�
//			
//  * @note	��1s��ʱ�俴���з�����ĳ��������Ϊ�з����ڸ÷���
// -------------------------------------------------------------------------- **/
//void EnemyDistanceJudge(void)														
//{
//	static u32 UPSniperTime  = 0;
//	static u32 DNSniperTime  = 0;
//	static u32 UPEnemyDis1 = 0; 
//	static u32 UPEnemyDis2 = 0; 
//	static u32 UPEnemyDis3 = 0; 
//	static u32 DNEnemyDis1 = 0; 
//	static u32 DNEnemyDis2 = 0; 
//	static u32 DNEnemyDis3 = 0; 
//	
//	
//	/**����̨**/
//	/*�з��ڷ���������ʱ���ۻ�*/
//	if(stFlag.DN_SniperFlag)
//	{
//		if(Enemy_Distance_DN < DEFEND_THIRD_LINE_DISTANCE_DN)
//		{
//			DNEnemyDis3++;
//		}
//		else if(Enemy_Distance_DN < DEFEND_SECOND_LINE_DISTANCE_DN)
//		{
//			DNEnemyDis2++;
//		}
//		else if(Enemy_Distance_DN < DEFEND_FIRST_LINE_DISTANCE_DN)
//		{
//			DNEnemyDis1++;
//		}
//		DNSniperTime	= systemMonitor.SysTickTime;	
//	}

//	/*�з����������ж�*/
//	if(DNEnemyDis3 > 1000)
//	{
//		DNEnemyPosition = 3;											// �з����ڵ�����������
//		DNEnemyDis3 = 0;
//		DNEnemyDis2 = 0;
//		DNEnemyDis1 = 0;
//	}
//	else if(DNEnemyDis2 > 1000)
//	{
//		DNEnemyPosition = 2;
//		DNEnemyDis3 = 0;
//		DNEnemyDis2 = 0;
//		DNEnemyDis1 = 0;		
//	}
//	else if(DNEnemyDis1 > 1000)
//	{
//		DNEnemyPosition = 1;
//		DNEnemyDis3 = 0;
//		DNEnemyDis2 = 0;
//		DNEnemyDis1 = 0;		
//	}
//	
//	/*5s��δʶ�𵽵з���Ϊ�з�δ�������*/
//	if(systemMonitor.SysTickTime - DNSniperTime > 5000)
//	{
//		DNEnemyPosition = 0;
//		DNEnemyDis3 = 0;
//		DNEnemyDis2 = 0;
//		DNEnemyDis1 = 0;	
//	}
//	
//	
//	
//	/**����̨**/
//	/*�з��ڷ���������ʱ���ۻ�*/
//	if(stFlag.UP_SniperFlag)
//	{
//		if(Enemy_Distance_UP < DEFEND_THIRD_LINE_DISTANCE_UP)
//		{
//			UPEnemyDis3++;
//		}
//		else if(Enemy_Distance_UP < DEFEND_SECOND_LINE_DISTANCE_UP)
//		{
//			UPEnemyDis2++;
//		}
//		else if(Enemy_Distance_UP < DEFEND_FIRST_LINE_DISTANCE_UP)
//		{
//			UPEnemyDis1++;
//		}
//		UPSniperTime	= systemMonitor.SysTickTime;	
//	}

//	/*�з����������ж�*/
//	if(UPEnemyDis3 > 1000)
//	{
//		UPEnemyPosition = 3;											// �з����ڵ�����������
//		UPEnemyDis3 = 0;
//		UPEnemyDis2 = 0;
//		UPEnemyDis1 = 0;
//	}
//	else if(UPEnemyDis2 > 1000)
//	{
//		UPEnemyPosition = 2;
//		UPEnemyDis3 = 0;
//		UPEnemyDis2 = 0;
//		UPEnemyDis1 = 0;		
//	}
//	else if(UPEnemyDis1 > 1000)
//	{
//		UPEnemyPosition = 1;
//		UPEnemyDis3 = 0;
//		UPEnemyDis2 = 0;
//		UPEnemyDis1 = 0;		
//	}

//	
//	/*5s��δʶ�𵽵з���Ϊ�з�δ�������*/
//	if(systemMonitor.SysTickTime - UPSniperTime > 5000)
//	{
//		UPEnemyPosition = 0;
//		UPEnemyDis3 = 0;
//		UPEnemyDis2 = 0;
//		UPEnemyDis1 = 0;
//	}
//}




///** --------------------------------------------------------------------------
//  * @brief  ƽ�ⲽ���������ж�
//			
//  * @note	���ݿ��ֵ�Ѫ�������ж�
// -------------------------------------------------------------------------- **/
//void RecognizeBanlanceSoldier(void)
//{
//	static bool FirstIn			= TRUE;
//	static u32	RecognizeTimes 	= 5000;
//	static bool	Num3Reconize 	= FALSE;
//	static bool	Num4Reconize 	= FALSE;
//	static bool	Num5Reconize 	= FALSE;

//	if(FirstIn && GameStatus.game_progress == 4)
//	{
//		FirstIn = FALSE;
//	}
//	
//	if(!FirstIn && RecognizeTimes > 0)
//	{
//		if(RobotIDMessage == Blue_7_Sentry)
//		{
//			if(GameRobotHP.red_3_robot_HP == 300 && !Num3Reconize)
//			{
//				BalancesoldierNumber += 3;
//				Num3Reconize = TRUE;
//			}
//			if(GameRobotHP.red_4_robot_HP == 300 && !Num4Reconize)
//			{
//				BalancesoldierNumber += 4; 
//				Num4Reconize = TRUE;
//			}
//			if(GameRobotHP.red_5_robot_HP == 300 && !Num5Reconize)
//			{
//				BalancesoldierNumber += 5; 
//				Num5Reconize = TRUE;
//			}
//		}
//		else
//		{
//			if(GameRobotHP.blue_3_robot_HP == 300 && !Num3Reconize)
//			{
//				BalancesoldierNumber += 3;
//				Num3Reconize = TRUE;
//			}
//			if(GameRobotHP.blue_3_robot_HP == 300 && !Num4Reconize)
//			{
//				BalancesoldierNumber += 4; 
//				Num4Reconize = TRUE;
//			}
//			if(GameRobotHP.blue_3_robot_HP == 300 && !Num5Reconize)
//			{
//				BalancesoldierNumber += 5; 
//				Num5Reconize = TRUE;
//			}
//		}
//		
//		RecognizeTimes--;
//	}
//	
//	if(RecognizeTimes == 0)
//	{
//		BanlanReconizeFinshFlag = TRUE;
//	}
//}
