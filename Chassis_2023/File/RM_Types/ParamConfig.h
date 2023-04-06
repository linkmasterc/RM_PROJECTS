#ifndef PARAM_CONFIG_H
#define PARAM_CONFIG_H

#define Vision_Use_Vofa
//#define Gimbal_Check_Vofa
//#define Normal_Use_Vofa

/*YAW����趨��ֵ*/
#define First_YAW_MID      					1242					// ��̨YAW������趨��ֵ��ǹ�ܳ���ǰ��ʱYAW����������ķ���ԭʼֵ��

#define GM_YAW_SENSIBILITY_TEST		0.05f						// ң����yaw������(�������ã�
#define GM_PITCH_SENSIBILITY_TEST	25.0f						// ң����pitch�����ȣ��������ã�

/*PITCH���ң�ؽǶȷ�Χ*/
#define PITCH_RC_ANGLE_MAX				(30.0f)					// ����̨PITCHң�ؽǶ����ֵ
#define PITCH_RC_ANGLE_MIN				(-27.0f)				// ����̨PITCHң�ؽǶ���Сֵ


/*PITCH���ɨ��Ƕȷ�Χ*/
#define PITCH_SCAN_ANGLE_MAX				(10.0f)				// ����̨PITCHɨ��Ƕ����ֵ
#define PITCH_SCAN_ANGLE_MIN				(-10.0f)			// ����̨PITCHɨ��Ƕ���Сֵ

/*YAW���ɨ��Ƕȷ�Χ*/
#define YAW_SCAN_ANGLE_MAX				(180.0f)				// ����̨YAWɨ��Ƕ����ֵ
#define YAW_SCAN_ANGLE_MIN				(-180.0f)				// ����̨YAWɨ��Ƕ���Сֵ


/*PITCH���ɨ���ٶȴ�С*/
#define PITCH_LINE_MOVE_SPEED			0.10f						// ��/ms

/*YAW���ֱ��ɨ���ٶȴ�С*/
#define YAW_LINE_MOVE_SPEED				0.10f						// ��/ms


/*ɨ��Ѳ���˶���YAW�������˶��ļ�ֵ*/
#define YAW_CURVE_RANGE_SPEED_MAX		0.30f
#define YAW_CURVE_RANGE_SPEED_MIN		0.08f


/*����Ѳ������ʱ����*/
#define FIXED_PATROL_GAPTIME				900


/*��̨�����������ռ��*/	
#define BOTTOM_SUPPLY_STEP      			(29491 * 5 / 3)


/*�������������*/
#define ATTACK_PITCH_ERROR				1.5f
#define ATTACK_YAW_ERROR					2.5f



/*�����ֹ������*/
#define BAN_SHOOT_AREA 						(rush.RankLength / 8)


/*���̱궨*/
#define CHASSIS_POSITION_DETECT_ERROR		100					// ����λ����Ϣ�궨�������

/*���ٵ�����ײ����������λ��*/
#define ACC_HIT_SITE_MIN					(rush.RankLength / 2)					

/*�����ٶ�ϵ��*/
#define SPEED_CONTROL_SAFE_PARA				1.05f									// ��ȫ״̬ϵ��
#define SPEED_CONTROL_DANGER_PARA			1.15f									// Σ��״̬ϵ��


/*�����˶�״̬*/
#define CHASSIS_STATE_WAIT_START			0x00								// ����״̬���������ȴ�����
#define CHASSIS_STATE_CALIBRATE				0x01								// ����״̬���������궨
#define CHASSIS_STATE_WAIT_CMD				0x02								// ����״̬���������ȴ�ָ��
#define CHASSIS_STATE_RUSH					0x03									// ����״̬���������˶�

#define CAPACITOR_CALIBRATE_WAIT	0x00								//���ݱ궨״̬���������ȴ�
#define	CAPACITOR_CALIBRATE_STATE	0x01										//���ݱ궨״̬��������������
#define	CAPACITOR_CALIBRATE_STOP	0x02										//���ݱ궨״̬����������ͣ
#define	CAPACITOR_CALIBRATE_FINISH	0x03									//���ݱ궨״̬�����������
#define	CAPACITOR_CALIBRATE_FAULT	0x04										//���ݱ궨״̬������������
	
#define CAPACITOR_CALIBRATE_INPUT_POWER_MAX		95
#define CAPACITOR_CALIBRATE_INPUT_POWER_MIN		15
#define CAPACITOR_CALIBRATE_INPUT_POWER_MEAN	40
#define CAPACITOR_COMPENSATE_INPUT_POWER_OVUT_MAX	400
#define CAPACITOR_COMPENSATE_INPUT_POWER_MAX	200
#define CAPACITOR_COMPENSATE_INPUT_POWER_MIN	35
#define CAPACITOR_COMPENSATE_OUTPUT_POWER		50


/*���̻���������ȫ��ֵ*/
#define CHASSIS_BUFF_POWER_LIMIT			200.0f

#define CHASSIS_BUFF_POWER_SAFE_THRE		0.9f

/*���̻�������Σ����ֵ*/
#define CHASSIS_BUFF_POWER_DANGER_THRE		0.2f

/*���̻��������ָ�ϵ��*/
#define CHASSIS_BUFF_POWER_RECOVER_PARA1     0.95f
#define CHASSIS_BUFF_POWER_RECOVER_PARA2     0.90f

/*���̻��������������ϵ��*/
#define CHASSIS_BUFF_POWER_NORMAL_PARA     	1.0f

/*���̻�������Σ��״̬����ϵ��*/
#define CHASSIS_BUFF_POWER_DANGER_PARA     	0.95f

/*�����ٶ�ƽ���˲�ϵ��*/
#define SPEED_SMOOTH_PARA					0.01f

/*����λ���������*/
#define TURNING_SITE_ERROR_MAX				50

/* �����ֹ�������� */
#define BAN_TURNROUND_AREA 					(rush.RankLength / 10)

/* �����ٶ�������� */
#define TURNING_SPEED_MIN					500

/* �����ٶ�ƽ���˲�����С */
#define ESPEEDBOXSIZE						20
#define MSPEEDBOXSIZE						10

/* ��̨�Ƕ�ƽ���˲�����С */
#define UPYAWANGLEBOXSIZE						1
#define DNYAWANGLEBOXSIZE						1

/* ײ���󲻴�ʱ��*/
#define UNSHOOTTIMES						500

/* ����ʼ���� */
#define RUSHRANK							5

/* ��Ѫ */
#define Hero_HP_Threshold		90							//�Ӿ�ʶ��Ӣ��Ѫ��
#define Engi_HP_Threshold		100							//�Ӿ�ʶ�𹤳�Ѫ��
#define Stan_HP_Threshold		70							//�Ӿ�ʶ�𲽱�Ѫ��

#define RECAPTIMES				3							// �����ر����

#define CAPPOWERDES				40							// ���ݿ���Ŀ�깦��


/********************************�������******************************************/
#define DEFEND_FIRST_LINE_DISTANCE_UP			10000			// ����̨��һ�����߾��루�ط�ǰ��վ�Ĵ�ֱ���룩
#define DEFEND_SECOND_LINE_DISTANCE_UP			7000		// ����̨�ڶ������߾��루�ҷ�ǰ��վ�Ĵ�ֱ���룩
#define DEFEND_THIRD_LINE_DISTANCE_UP			3000			// ����̨���������߾��루�ҷ����θߵصĴ�ֱ���룩
#define DEFEND_FIRST_LINE_SHOOT_FRC_UP			5				// ����̨��һ������������
#define DEFEND_SECOND_LINE_SHOOT_FRC_UP			9				// ����̨�ڶ�������������
#define DEFEND_THIRD_LINE_SHOOT_FRC_UP			9				// ����̨����������������

#define DEFEND_FIRST_LINE_DISTANCE_DN			10000			// ����̨��һ�����߾��루�ط�ǰ��վ�Ĵ�ֱ���룩
#define DEFEND_SECOND_LINE_DISTANCE_DN			7000		// ����̨�ڶ������߾��루�ҷ�ǰ��վ�Ĵ�ֱ���룩
#define DEFEND_THIRD_LINE_DISTANCE_DN			3000			// ����̨���������߾��루�ҷ����θߵصĴ�ֱ���룩
#define DEFEND_FIRST_LINE_SHOOT_FRC_DN			9				// ����̨��һ������������
#define DEFEND_SECOND_LINE_SHOOT_FRC_DN			17			// ����̨�ڶ�������������
#define DEFEND_THIRD_LINE_SHOOT_FRC_DN			17			// ����̨����������������

#define BARREL_HEAT_LIMIT_UP					320						// ����̨ǹ����������
#define BARREL_HEAT_FIRST_LINE_UP				160					// ����̨ǹ��������һ��ֵ��
#define BARREL_HEAT_SECOND_LINE_UP				280				// ����̨ǹ�������ڶ���ֵ��
#define BARREL_HEAT_THIRD_LINE_UP				310					// ����̨ǹ������������ֵ��
#define BARREL_HEAT_FIRST_LINE_SHOOT_FRC_UP		9			// ����̨ǹ��������һ��ֵ���ڵ��������
#define BARREL_HEAT_SECOND_LINE_SHOOT_FRC_UP	9			// ����̨ǹ�������ڶ���ֵ���ڵ��������
#define BARREL_HEAT_THIRD_LINE_SHOOT_FRC_UP		5			// ����̨ǹ������������ֵ���ڵ��������

#define BARREL_HEAT_LIMIT_DN					320						// ����̨ǹ����������
#define BARREL_HEAT_FIRST_LINE_DN				160					// ����̨ǹ��������һ��ֵ��
#define BARREL_HEAT_SECOND_LINE_DN				280				// ����̨ǹ�������ڶ���ֵ��
#define BARREL_HEAT_THIRD_LINE_DN				310					// ����̨ǹ������������ֵ��
#define BARREL_HEAT_FIRST_LINE_SHOOT_FRC_DN		17		// ����̨ǹ��������һ��ֵ���ڵ��������
#define BARREL_HEAT_SECOND_LINE_SHOOT_FRC_DN	9			// ����̨ǹ�������ڶ���ֵ���ڵ��������
#define BARREL_HEAT_THIRD_LINE_SHOOT_FRC_DN		5			// ����̨ǹ������������ֵ���ڵ��������


#endif
