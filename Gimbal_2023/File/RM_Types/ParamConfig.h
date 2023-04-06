#ifndef PARAM_CONFIG_H
#define PARAM_CONFIG_H

//Ħ����
#define fric_speed1 -12200.0
#define Fric 139

//pitch����
#define	PITCH_USE_TD
#define GBDN_PITCH_MAX	30.0f
#define GBDN_PITCH_MIN	-27.0f
#define GM_PITCH_MIDPOS 4106

/*YAW����趨��ֵ*/
#define SECOND_YAW_MID           3450

/*��̨�����������ռ��*/	
#define BOTTOM_SUPPLY_STEP      			(29491 * 5 / 3)

/* ��̨�Ƕ�ƽ���˲�����С */
#define YAWANGLEBOXSIZE						1

/* ��Ѫ */
#define Hero_HP_Threshold		90							//�Ӿ�ʶ��Ӣ��Ѫ��
#define Engi_HP_Threshold		100							//�Ӿ�ʶ�𹤳�Ѫ��
#define Stan_HP_Threshold		70							//�Ӿ�ʶ�𲽱�Ѫ��

#define RECAPTIMES				3							// �����ر����

#define CAPPOWERDES				40							// ���ݿ���Ŀ�깦��


/********************************�������******************************************/
#define DEFEND_FIRST_LINE_DISTANCE_UP			10000									// ����̨��һ�����߾��루�ط�ǰ��վ�Ĵ�ֱ���룩
#define DEFEND_SECOND_LINE_DISTANCE_UP			7000									// ����̨�ڶ������߾��루�ҷ�ǰ��վ�Ĵ�ֱ���룩
#define DEFEND_THIRD_LINE_DISTANCE_UP			3000									// ����̨���������߾��루�ҷ����θߵصĴ�ֱ���룩
#define DEFEND_FIRST_LINE_SHOOT_FRC_UP			5										// ����̨��һ������������
#define DEFEND_SECOND_LINE_SHOOT_FRC_UP			9										// ����̨�ڶ�������������
#define DEFEND_THIRD_LINE_SHOOT_FRC_UP			9										// ����̨����������������

#define DEFEND_FIRST_LINE_DISTANCE_DN			10000									// ����̨��һ�����߾��루�ط�ǰ��վ�Ĵ�ֱ���룩
#define DEFEND_SECOND_LINE_DISTANCE_DN			7000									// ����̨�ڶ������߾��루�ҷ�ǰ��վ�Ĵ�ֱ���룩
#define DEFEND_THIRD_LINE_DISTANCE_DN			3000									// ����̨���������߾��루�ҷ����θߵصĴ�ֱ���룩
#define DEFEND_FIRST_LINE_SHOOT_FRC_DN			9										// ����̨��һ������������
#define DEFEND_SECOND_LINE_SHOOT_FRC_DN			17										// ����̨�ڶ�������������
#define DEFEND_THIRD_LINE_SHOOT_FRC_DN			17										// ����̨����������������

#define BARREL_HEAT_LIMIT_UP					320										// ����̨ǹ����������
#define BARREL_HEAT_FIRST_LINE_UP				160										// ����̨ǹ��������һ��ֵ��
#define BARREL_HEAT_SECOND_LINE_UP				280										// ����̨ǹ�������ڶ���ֵ��
#define BARREL_HEAT_THIRD_LINE_UP				310										// ����̨ǹ������������ֵ��
#define BARREL_HEAT_FIRST_LINE_SHOOT_FRC_UP		9										// ����̨ǹ��������һ��ֵ���ڵ��������
#define BARREL_HEAT_SECOND_LINE_SHOOT_FRC_UP	9										// ����̨ǹ�������ڶ���ֵ���ڵ��������
#define BARREL_HEAT_THIRD_LINE_SHOOT_FRC_UP		5										// ����̨ǹ������������ֵ���ڵ��������

#define BARREL_HEAT_LIMIT_DN					320										// ����̨ǹ����������
#define BARREL_HEAT_FIRST_LINE_DN				160										// ����̨ǹ��������һ��ֵ��
#define BARREL_HEAT_SECOND_LINE_DN				280										// ����̨ǹ�������ڶ���ֵ��
#define BARREL_HEAT_THIRD_LINE_DN				310										// ����̨ǹ������������ֵ��
#define BARREL_HEAT_FIRST_LINE_SHOOT_FRC_DN		17										// ����̨ǹ��������һ��ֵ���ڵ��������
#define BARREL_HEAT_SECOND_LINE_SHOOT_FRC_DN	9										// ����̨ǹ�������ڶ���ֵ���ڵ��������
#define BARREL_HEAT_THIRD_LINE_SHOOT_FRC_DN		5										// ����̨ǹ������������ֵ���ڵ��������


#endif
