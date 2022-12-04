#ifndef RM_RS_TYPES_H
#define RM_RS_TYPES_H
#include "rm_redefine_types.h"

#define	Red_1_Hero		1
#define	Red_2_Engineer	2
#define Red_3_Standard 	3
#define Red_4_Standard  4
#define Red_5_Standard  5
#define Red_6_Aerial    6
#define Red_7_Sentry    7
#define	Red_9_Radar     9
    
#define	Blue_1_Hero     101
#define Blue_2_Engineer 102
#define Blue_3_Standard 103
#define Blue_4_Standard 104
#define Blue_5_Standard 105
#define Blue_6_Aerial   106
#define Blue_7_Sentry   107
#define	Blue_9_Radar    109


#define GameStatusID    			0x0001
#define GameResultID    			0x0002
#define GameRobotHPID   			0x0003
#define DartStatusID    			0x0004
#define ICRABuffDebuffZoneStatusID  0x0005
#define EventDataID     			0x0101
#define SupplyProjectileActionID    0x0102
#define ApplySupplyProjectoleID		0x0103
#define RefereeWaringID     		0x0104
#define DartRemainingTimeID     	0x0105
#define GameRobotStatusID       	0x0201
#define PowerHeatDataID     		0x0202
#define GameRobotPosID      		0x0203
#define BuffID      				0x0204
#define AerialRobotEnergyID     	0x0205
#define RobotHurtID     			0x0206
#define ShootDataID     			0x0207
#define BulletRemainingID   		0x0208
#define RFIDStatusID        		0x0209
#define DartClientCmdID     		0x020A
#define RobotInteractiveDataID  	0x0301

#define ClientCustomGraphicDeleteID		0x0100
#define ClientCustomGraphicSingleID		0x0101
#define ClientCustomGraphicDoubleID		0x0102
#define ClientCustomGraphicFiveID		0x0103
#define ClientCustomGraphicSevenID		0x0104
#define ClientCustomCharacterID			0x0110

#define Custom_Data_Length_MAX			113

/** ģ�����߱�־�ṹ�� */
typedef struct
{
	bool RSState;			//����ϵͳ����
	bool UPGimbalState;		//����̨�ƿ�����
	bool UPComputerState;	//����̨С��������
	bool DNGimbalState;		//����̨�ƿ�����
	bool DNComputerState;	//����̨С��������
	bool Can1State;			//CAN1ģ������
	bool Can2State;			//CAN2ģ������
	bool CapacitorState; 	//����ģ���쳣
} ST_ERROR;
/** ȫ�ֱ�־�ṹ�� */
typedef struct
{
	bool EXTI1_IT_ENABLE_FLAG;
	bool EXTI2_IT_ENABLE_FLAG;


	bool RunFlag_Pre;		//ǰ��pitch����˶����
	bool RunFlag;			//pitch����˶����
	bool ShootFlag;			//��������˶����
	bool FrictionFlag;		//Ħ����������־
	bool SniperFlag_Pre;		//ǰ��ʶ���־
	bool SniperFlag;			//�Ӿ�ʶ��������

	bool CS_RunFlag;			//����������־
	bool CS_RunFlag_Pre;		//ǰ�ε���������־
	
	bool PT_UPFlag;				//����̨�յ���ͣ�籣����־
	bool PT_DNFlag;				//����̨�յ���ͣ�籣����־
	
	bool ResetFlag;				//��λ��־λ
	bool InitFlag;				//��ʼ���ɹ���־λ
	
} ST_FLAG;

/** ���в���ģʽ�ṹ�� */
typedef struct
{
	bool BoDan_Flag;			//�򵯿���
	bool PreBoDan_Flag;			//ǰ�ο���״̬

	float Fact_PreShootPos;		//ǰ�β���λ��
	float Fact_ShootPos;		//��ǰ����λ��

	s32 Fact_ShootSumNumber;	//�����ۻ�����
	s32 Fact_ShootNumber;		//�����ۻ����굯����
	s32 Fact_pShootNumber;		//����ͳ�ƴ���
	s32 Des_pShootNumber;		//���������޴���

}ST_ShootTestControl;
/* ����ϵͳ���ݽ����� */
/** ����״̬���ݣ�0x0001    ����Ƶ�ʣ�1Hz
  * @param	game_type	�������ͣ�	1-RoboMaster���״�ʦ��
  *									2-RoboMaster���״�ʦ������
  *									3-ICRA RoboMaster�˹�������ս��
  *									4-RoboMaster������3V3
  *									5-RoboMaster������1V1
  *			game_progress	��ǰ�����׶Σ�	0-δ��ʼ����
  *											1-׼���׶�
  *											2-�Լ�׶�
  *											3-5s����ʱ
  *											4-��ս��
  *											5-����������
  *			stage_remain_time	��ǰ�׶�ʣ��ʱ�䣬��λs
  *			SyncTimeStamp		�����˽��յ���ָ��ľ�ȷUnixʱ�䣬�����ض��յ���Ч��NTP��������ʱ����Ч
  */
typedef __packed struct {
	uint8_t game_type: 4;
	uint8_t game_progress: 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
} ext_game_status_t;



/** ����������ݣ�0x0002    ����Ƶ�ʣ�������������
  *	@param	winner	0-ƽ��	1-�췽ʤ��	2-����ʤ��
  */
typedef __packed struct {
	uint8_t winner;
} ext_game_result_t;



/** ������Ѫ�����ݣ�0x0003   ����Ƶ�ʣ�1Hz
* @brief	�ṩ���ӻ�����Ѫ�����ݣ�δ�ϳ��Լ�����Ѫ��Ϊ0
  */
typedef __packed struct {
	uint16_t red_1_robot_HP;
	uint16_t red_2_robot_HP;
	uint16_t red_3_robot_HP;
	uint16_t red_4_robot_HP;
	uint16_t red_5_robot_HP;
	uint16_t red_7_robot_HP;
	uint16_t red_outpost_HP;
	uint16_t red_base_HP;
	uint16_t blue_1_robot_HP;
	uint16_t blue_2_robot_HP;
	uint16_t blue_3_robot_HP;
	uint16_t blue_4_robot_HP;
	uint16_t blue_5_robot_HP;
	uint16_t blue_7_robot_HP;
	uint16_t blue_outpost_HP;
	uint16_t blue_base_HP;
} ext_game_robot_HP_t;



/** �˹�������ս���ӳ���ͷ���״̬��0x0005   ����Ƶ�ʣ�1Hz���ڷ���    ���ͷ�Χ�����л����� */
typedef __packed struct {
	uint8_t F1_zone_status: 1;
	uint8_t F1_zone_buff_debuff_status: 3;
	uint8_t F2_zone_status: 1;
	uint8_t F2_zone_buff_debuff_status: 3;
	uint8_t F3_zone_status: 1;
	uint8_t F3_zone_buff_debuff_status: 3;
	uint8_t F4_zone_status: 1;
	uint8_t F4_zone_buff_debuff_status: 3;
	uint8_t F5_zone_status: 1;
	uint8_t F5_zone_buff_debuff_status: 3;
	uint8_t F6_zone_status: 1;
	uint8_t F6_zone_buff_debuff_status: 3;

	uint16_t red1_bullet_left;
	uint16_t red2_bullet_left;
	uint16_t blue1_bullet_left;
	uint16_t blue2_bullet_left;
} ext_ICRA_buff_debuff_zone_status_t;



/** �����¼����ݣ�0x0101    ����Ƶ�ʣ��¼��ı����
  *	@param	bit 0-2��	������Ѫ��״̬
  * 			bit 0��	��������վ1�Ų�Ѫ��ռ��״̬��1Ϊ��ռ��
  *				bit 1��	��������վ2�Ų�Ѫ��ռ��״̬��1Ϊ��ռ��
  *				bit 2��	��������վ3�Ų�Ѫ��ռ��״̬��1Ϊ��ռ��
  *			bit 3-5��	������������״̬
  *				bit 3��	�����ռ��״̬��1Ϊռ��
  *				bit 4��	С�������ؼ���״̬��1Ϊ�Ѽ���
  *				bit 5��	���������ؼ���״̬��1Ϊ�Ѽ���
  *			bit 6��		����R2���θߵ�ռ��״̬��1Ϊ��ռ��
  *			bit 7��		����R3���θߵ�ռ��״̬��1Ϊ��ռ��
  *			bit 8��		����R4���θߵ�ռ��״̬��1Ϊ��ռ��
  *			bit 9��		�������ػ���״̬
  *				1Ϊ���������⻤��Ѫ��
  *				0Ϊ���������⻤��Ѫ��
  *			bit 10��	����ǰ��վ״̬
  *				1Ϊǰ��վ���
  *				0Ϊǰ��վ������
  */
typedef __packed struct {
	uint32_t event_type;
} ext_event_data_t;



/** ����վ������ʶ��0x0102   ����Ƶ�ʣ������ı����    ������Χ������������
  * @param	supply_projectile_id	����վ��ID:	1Ϊ1�Ų����� 2Ϊ2�Ų�����
  *			supply_robot_id			����������ID: 0Ϊ��ǰ�޻����˲���
  *			supply_projectile_step	�����ڿ���״̬��0Ϊ�رգ�1Ϊ�ӵ�׼���У�2Ϊ�ӵ�����
  *			supply_projectile_num	����������50/100/150/200
  */
typedef __packed struct {
	uint8_t supply_projectile_id;
	uint8_t supply_robot_id;
	uint8_t supply_projectile_step;
	uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;



/** ���о�����Ϣ��0x0104    ����Ƶ�ʣ����淢������
  *	@param	level	����ȼ���1Ϊ���ƣ�2Ϊ���ƣ�3Ϊ�и�
  *			foul_robot_id	���������ID
  *							�и�ʱ��������IDΪ0
  *							���ơ�����ʱ��������IDΪ���������ID
  */
typedef __packed struct {
	uint8_t level;
	uint8_t foul_robot_id;
} ext_referee_warning_t;



/** ���ڷ���ڵ���ʱ��0x0105  ����Ƶ�ʣ�1Hz    ���ͷ�Χ������������ 
  *	@param	dart_remaining_time		15s����ʱ
  */
typedef __packed struct {
	uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;



/** ����������״̬��0x0201   ����Ƶ�ʣ�10Hz 
  *	@param	robot_id	��������ID
  *			robot_level	�����˵ȼ�
  *			remain_HP	������ʣ��Ѫ��
  *			max_HP		����������Ѫ��
  *			shooter_id1_17mm_cooling_rate	������1��17mmǹ��ÿ����ȴֵ
  *			shooter_id1_17mm_cooling_limit	������1��17mmǹ����������
  *			shooter_id1_17mm_speed_limit	������1��17mmǹ�������ٶ� ��λm/s
  *			shooter_id2_17mm_cooling_rate	������2��17mmǹ��ÿ����ȴֵ
  *			shooter_id2_17mm_cooling_limit	������2��17mmǹ����������
  *			shooter_id2_17mm_speed_limit	������2��17mmǹ�������ٶ� ��λm/s
  *			shooter_id1_42mm_cooling_rate	������42mmǹ��ÿ����ȴֵ
  *			shooter_id1_42mm_cooling_limit	������42mmǹ����������
  *			shooter_id1_42mm_speed_limit	������42mmǹ�������ٶ� ��λm/s
  *			chassis_power_limit			���̹�����������
  *			���ص�Դ������
  *			mains_power_gimbal_output	gimbal�����	1Ϊ��24V�����0Ϊ��24V���
  *			mains_power_chassis_output	chassis�����	1Ϊ��24V�����0Ϊ��24V���
  *			mains_power_shooter_output	shooter�����	1Ϊ��24V�����0Ϊ��24V���
  */
typedef __packed struct {
	uint8_t robot_id;
	uint8_t robot_level;
	uint16_t remain_HP;
	uint16_t max_HP;
	uint16_t shooter_id1_17mm_cooling_rate;
	uint16_t shooter_id1_17mm_cooling_limit;
	uint16_t shooter_id1_17mm_speed_limit;

	uint16_t shooter_id2_17mm_cooling_rate;
	uint16_t shooter_id2_17mm_cooling_limit;
	uint16_t shooter_id2_17mm_speed_limit;

	uint16_t shooter_id1_42mm_cooling_rate;
	uint16_t shooter_id1_42mm_cooling_limit;
	uint16_t shooter_id1_42mm_speed_limit;

	uint16_t chassis_power_limit;
	uint8_t mains_power_gimbal_output: 1;
	uint8_t mains_power_chassis_output: 1;
	uint8_t mains_power_shooter_output: 1;
} ext_game_robot_status_t;



/** ʵʱ�����������ݣ�0x0202  ����Ƶ�ʣ�50Hz 
  * @param	chassis_volt	���������ѹ ��λmV
  *			chassis_current	����������� ��λmA
  *			chassis_power	����������� ��λW
  *			chassis_power_buffer			���̹��ʻ��� ��λJ	��ע�����¸��ݹ���������250J
  *			shooter_id1_17mm_cooling_heat	1��17mmǹ������
  *			shooter_id2_17mm_cooling_heat	2��17mmǹ������
  *			shooter_id1_42mm_cooling_heat	42mmǹ������
  */
typedef __packed struct {
	uint16_t chassis_volt;
	uint16_t chassis_current;
	float chassis_power;
	uint16_t chassis_power_buffer;
	uint16_t shooter_id1_17mm_cooling_heat;
	uint16_t shooter_id2_17mm_cooling_heat;
	uint16_t shooter_id1_42mm_cooling_heat;
} ext_power_heat_data_t;



/** ������λ�ã�0x0203     ����Ƶ�ʣ�10Hz
  *	@param	x	λ��x���꣬��λm
  *			y	λ��y���꣬��λm
  *			z	λ��z���꣬��λm
  *			yaw	λ��ǹ�ڣ���λ��
  */
typedef __packed struct {
	float x;
	float y;
	float z;
	float yaw;
} ext_game_robot_pos_t;



/** ���������棺0x0204     ����Ƶ�ʣ�1Hz
  *	@param	bit 0:	������Ѫ����Ѫ״̬
  *			bit 1:	ǹ��������ȴ����
  *			bit 2:	�����˷����ӳ�
  *			bit 3:	�����˹����ӳ�
  */
typedef __packed struct {
	uint8_t power_rune_buff;
} ext_buff_t;



/** ���л���������״̬��0x0205     ����Ƶ�ʣ�10Hz
  *	@param	attack_time	�ɹ���ʱ�� ��λs	30s�ݼ���0
  */
typedef __packed struct {
	uint8_t attack_time;
} aerial_robot_energy_t;

/** �˺�״̬��0x0206          ����Ƶ�ʣ��˺���������
  *	@param	armor_id	��Ѫ���仯����Ϊװ���˺�������װ��ID��������ֵΪ0~4�Ŵ�������˵����װ��Ƭ��
  *						����Ѫ���仯���ͣ��ñ�����ֵΪ0
  *			hurt_type	Ѫ���仯����
  *							0x00 װ���˺���Ѫ
  *							0x01 ģ����߿�Ѫ
  *							0x02 �����ٿ�Ѫ
  *							0x03 ��ǹ��������Ѫ
  *							0x04 �����̹��ʿ�Ѫ
  *							0x05 װ��ײ����Ѫ
  */
typedef __packed struct {
	uint8_t armor_id: 4;
	uint8_t hurt_type: 4;
} ext_robot_hurt_t;



/** ʵʱ�����Ϣ��0x0207    ����Ƶ�ʣ��������
  *	@param	bullet_type �ӵ�����	1Ϊ17mm����	2Ϊ42mm����
  *			shooter_id	�������ID	1Ϊ1��17mm�������
  *									2Ϊ2��17mm�������
  *									3Ϊ42mm�������
  *			bullet_freq		�ӵ���Ƶ ��λHz
  *			bullet_speed 	�ӵ����� ��λm/s
  */
typedef __packed  struct {
	uint8_t bullet_type;
	uint8_t shooter_id;
	uint8_t bullet_freq;
	float bullet_speed;
} ext_shoot_data_t;



/** �ӵ�ʣ�෢������0x0208   ����Ƶ�ʣ�10Hz���ڷ���   ���ͷ�Χ�����л����˷���
*	@param	bullet_remaining_num_17mm	17mm�ӵ�ʣ�෢����Ŀ
*			bullet_remaining_num_42mm	42mm�ӵ�ʣ�෢����Ŀ
*			coin_remaining_num			ʣ��������
  */
typedef __packed struct {
	uint16_t bullet_remaining_num_17mm;
	uint16_t bullet_remaining_num_42mm;
	uint16_t coin_remaining_num;
} ext_bullet_remaining_t;



/** ������RFID״̬��0x0209     ����Ƶ�ʣ�1Hz    ���ͷ�Χ����һ������
  *	@param	bit 0:	���������RFID״̬
  *			bit 1:	�ߵ������RFID״̬
  *			bit 2:	�������ؼ����RFID״̬
  *			bit 3:	���������RFID״̬
  *			bit 4:	ǰ��վ�����RFID״̬
  *			bit 5:	��Դ�������RFID״̬
  *			bit 6:	��Ѫ�������RFID״̬
  *			bit 7:	���̻����˲�Ѫ��RFID״̬
* @brief	RFID״̬����ȫ�����Ӧ������򴦷�״̬������з���ռ��ĸߵ�����㣬���ܻ�ȡ��Ӧ������ЧӦ
  */
typedef __packed struct {
	uint32_t rfid_status;
} ext_rfid_status_t;



/** ���ڻ����˿ͻ���ָ�����ݣ�0x020A  ����Ƶ�ʣ�10Hz   ���ͷ�Χ����һ������
  *	@param	dart_launch_opening_status	��ǰ���ڷ����״̬	0Ϊ�ر�	1Ϊ���ڿ������߹ر��� 2Ϊ�Ѿ�����
  *			dart_attack_target			���ڵĴ��Ŀ�꣬Ĭ��Ϊǰ��վ	1Ϊǰ��վ 2Ϊ����
  *			target_change_time			�л����Ŀ��ʱ�ı���ʣ��ʱ�䣬��λs����δ�л�Ĭ��0
  *			first_dart_speed			��⵽�ĵ�һö�����ٶȣ���λ0.1m/s/LSB��δ�����Ϊ0
  *			second_dart_speed			��⵽�ĵڶ�ö�����ٶȣ���λ0.1m/s/LSB��δ�����Ϊ0
  *			third_dart_speed			��⵽�ĵ���ö�����ٶȣ���λ0.1m/s/LSB��δ�����Ϊ0
  *			fourth_dart_speed			��⵽�ĵ���ö�����ٶȣ���λ0.1m/s/LSB��δ�����Ϊ0
  *			last_dart_launch_time		���һ�εķ�����ڵı���ʣ��ʱ�䣬��λs����ʼֵΪ0
  *			operate_launch_cmd_time		���һ�β�����ȷ������ָ��ʱ�ı���ʣ��ʱ�䣬��λs����ʼֵ0
  */
typedef __packed struct {
	uint8_t dart_launch_opening_status;
	uint8_t dart_attack_target;
	uint16_t target_change_time;
	uint8_t first_dart_speed;
	uint8_t second_dart_speed;
	uint8_t third_dart_speed;
	uint8_t fourth_dart_speed;
	uint16_t last_dart_launch_time;
	uint16_t operate_launch_cmd_time;
} ext_dart_client_cmd_t;



/** �������ݽ�����Ϣ��0x0301 
*	@param	data_cmd_id	���ݶε�����ID
*						0x0200~0x02FF		6+n			���������˼�ͨ��
*						0x0100				6+2			�ͻ���ɾ��ͼ��
*						0x0101				6+15		�ͻ��˻���һ��ͼ��
*						0x0102				6+30		�ͻ��˻��ƶ���ͼ��
*						0x0103				6+75		�ͻ��˻������ͼ��
*						0x0104				6+105		�ͻ��˻����߸�ͼ��
*						0x0110				6+45		�ͻ��˻����ַ�ͼ��
*			sender_ID	�����ߵ�ID
*			receiver_ID	�����ߵ�ID
  */
typedef __packed struct {
	uint16_t data_cmd_id;
	uint16_t sender_ID;
	uint16_t receiver_ID;
} ext_student_interactive_header_data_t;



/** �������� �����˼�ͨ�ţ�0x0301 */
typedef __packed struct {
	uint8_t data[Custom_Data_Length_MAX];
} robot_interactive_data_t;



/** �ͻ���ɾ��ͼ�� �����˼�ͨ�ţ�0x0301 
  *	@param	data_cmd_id		0x0100
  *			operate_type	ͼ�β���	0-�ղ��� 1-ɾ��ͼ�� 2-ɾ������
  *			layer			ͼ����		0~9
  */
typedef __packed struct {
	uint8_t operate_type;
	uint8_t layer;
} ext_client_custom_graphic_delete_t;



/** ͼ������ 
  *	@param	graphic_name	ͼ����		��ɾ�����޸ĵȲ����У���Ϊ�ͻ��˵�����
  *			operate_type	ͼ�β���	0-�ղ��� 	1-���� 		2-�޸� 		3-ɾ��
  *			graphic_type	ͼ������	0-ֱ�� 		1-����		2-��Բ		3-��Բ
  *										4-Բ��		5-������	6-������	7-�ַ�
  *			layre			ͼ����		0~9
  *			color			��ɫ		0-������ɫ	1-��ɫ		2-��ɫ		3-��ɫ
  *										4-�Ϻ�ɫ	5-��ɫ		6-��ɫ		7-��ɫ
  *										8-��ɫ
  *			start_angle		��ʼ�Ƕȣ���λ�㣬��Χ[0,360]
  *			end_angle		��ֹ�Ƕȣ���λ�㣬��Χ[0,360]
  *			width			�߿�
  *			start_x			���x����
  *			start_y			���y����
  *			radius			�����С���߰뾶
  *			end_x			�յ�x����
  *			end_y			�յ�y����
  *	@table
  *		graphic_type		start_angle		end_angle		width		start_x		start_y		radius		end_x		end_y
  *		ֱ��				��				��				�������	���x����	���y����	��			�յ�x����	�յ�y����
  *		����				��				��				�������	���x����	���y����	��			�Զ���x����	�Զ���y����
  *		��Բ				��				��				�������	Բ��x����	Բ��y����	�뾶		��			��
  *		��Բ				��				��				�������	Բ��x����	Բ��y����	��			x���᳤��	y���᳤��
  *		Բ��				��ʼ�Ƕ�		��ֹ�Ƕ�		�������	Բ��x����	Բ��y����	��			x���᳤��	y���᳤��
  *		������				�����С		С��λ��Ч����	�������	���x����	���y����			32λ��������float
  *		������				�����С		��				�������	���x����	���y����			32λ��������int32_t
  *		�ַ�				�����С		�ַ�����		�������	���x����	���y����	��			��			��
  */
typedef __packed struct {
	uint8_t graphic_name[3];
	uint32_t operate_type: 3;
	uint32_t graphic_type: 3;
	uint32_t layre: 4;
	uint32_t color: 4;
	uint32_t start_angle: 9;
	uint32_t end_angle: 9;
	uint32_t width: 10;
	uint32_t start_x: 11;
	uint32_t start_y: 11;
	uint32_t radius: 10;
	uint32_t end_x: 11;
	uint32_t end_y: 11;
} graphic_data_struct_t;



/** �ͻ��˻���һ��ͼ�� */
//#define ClientCustomGraphicSingleID		0x0101
typedef __packed struct {
	graphic_data_struct_t grapic_data_struct;
} ext_client_graphic_single_t;



/** �ͻ��˻��ƶ���ͼ�� */
//#define ClientCustomGraphicDoubleID		0x0102
typedef __packed struct {
	graphic_data_struct_t grapic_data_struct[2];
} ext_client_custom_graphic_double_t;



/** �ͻ��˻������ͼ�� */
//#define ClientCustomGraphicFiveID			0x0103
typedef __packed struct {
	graphic_data_struct_t grapic_data_struct[5];
} ext_client_custom_graphic_five_t;



/** �ͻ��˻����߸�ͼ�� */
//#define ClientCustomGraphicSevenID		0x0104
typedef __packed struct {
	graphic_data_struct_t grapic_data_struct[7];
} ext_client_custom_graphic_seven_t;



/** �ͻ��˻����ַ� */
//#define ClientCustomCharacterID			0x0110
typedef __packed struct {
	graphic_data_struct_t grapic_data_struct;
	uint8_t data[30];
} ext_client_custom_character_t;


#endif
