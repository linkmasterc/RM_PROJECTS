#ifndef PARAM_CONFIG_H
#define PARAM_CONFIG_H



/*YAW电机设定中值*/
#define DN_YAW_MID      					1364									// 下云台YAW电机的设定中值（枪管朝正前方时YAW电机编码器的返回原始值）
#define UP_YAW_MID							2732									// 上云台YAW电机的设定中值（枪管朝正前方时YAW电机编码器的返回原始值）


/*YAW电机角度范围*/
#define UP_YAW_ANGLE_MAX					60.0f									// 上云台YAW轴限幅


/*PITCH电机遥控角度范围*/
#define UP_PITCH_RC_ANGLE_MAX				(5.0f)									// 上云台PITCH遥控角度最大值
#define UP_PITCH_RC_ANGLE_MIN				(-32.0f)								// 上云台PITCH遥控角度最小值
#define DN_PITCH_RC_ANGLE_MAX				(30.0f)									// 下云台PITCH遥控角度最大值
#define DN_PITCH_RC_ANGLE_MIN				(-30.0f)									// 下云台PITCH遥控角度最小值


/*PITCH电机扫描角度范围*/
#define UP_PITCH_SCAN_ANGLE_MAX				(0.0f)									// 上云台PITCH扫描角度最大值
#define UP_PITCH_SCAN_ANGLE_MIN				(-30.0f)								// 上云台PITCH扫描角度最小值
#define DN_PITCH_SCAN_ANGLE_MAX				(40.0f)									// 下云台PITCH扫描角度最大值
#define DN_PITCH_SCAN_ANGLE_MIN				(0.0f)									// 下云台PITCH扫描角度最小值

/*PITCH电机扫描速度大小*/
#define DN_PITCH_LINE_MOVE_SPEED			0.10f									// °/ms
#define UP_PITCH_LINE_MOVE_SPEED			0.10f									// °/ms


/*YAW电机扫描角度范围*/
#define UP_YAW_SCAN_ANGLE_MAX				(60.0f)									// 上云台YAW扫描角度最大值
#define UP_YAW_SCAN_ANGLE_MIN				(-60.0f)								// 上云台YAW扫描角度最小值
#define DN_YAW_SCAN_ANGLE_MAX				(180.0f)								// 下云台YAW扫描角度最大值
#define DN_YAW_SCAN_ANGLE_MIN				(-180.0f)								// 下云台YAW扫描角度最小值


/*YAW电机直线扫描速度大小*/
#define DN_YAW_LINE_MOVE_SPEED				0.12f									// °/ms
#define UP_YAW_LINE_MOVE_SPEED				0.10f									// °/ms


/*扫描巡逻运动下YAW轴曲线运动的极值*/
#define UP_YAW_CURVE_RANGE_SPEED_MAX		0.25f									// 曲线运动
#define UP_YAW_CURVE_RANGE_SPEED_MIN		0.08f
#define DN_YAW_CURVE_RANGE_SPEED_MAX		0.30f
#define DN_YAW_CURVE_RANGE_SPEED_MIN		0.08f


/*定点巡逻两点时间间隔*/
#define UP_FIXED_PATROL_GAPTIME				700
#define DN_FIXED_PATROL_GAPTIME				480


/*上云台拨弹电机单弹占角*/
#define TOP_SUPPLY_STEP         			(29491 * 5 / 3)
#define BOTTOM_SUPPLY_STEP      			(-29491)


/*射击允许跟随误差*/
#define DN_ATTACK_PITCH_ERROR				1.5f
#define DN_ATTACK_YAW_ERROR					2.5f
#define UP_ATTACK_PITCH_ERROR				1.0f
#define UP_ATTACK_YAW_ERROR					2.5f


/*轨道禁止打弹区域*/
#define BAN_SHOOT_AREA 						(rush.RankLength / 8)


/*底盘标定*/
#define CHASSIS_POSITION_DETECT_ERROR		100					// 底盘位置信息标定允许误差

/*加速到满足撞柱需求的最近位置*/
#define ACC_HIT_SITE_MIN					(rush.RankLength / 2)					

/*底盘速度系数*/
#define SPEED_CONTROL_SAFE_PARA				1.05f									// 安全状态系数
#define SPEED_CONTROL_DANGER_PARA			1.15f									// 危险状态系数


/*底盘运动状态*/
#define CHASSIS_STATE_WAIT_START			0x00									// 底盘状态————等待启动
#define CHASSIS_STATE_CALIBRATE				0x01									// 底盘状态————标定
#define CHASSIS_STATE_WAIT_CMD				0x02									// 底盘状态————等待指令
#define CHASSIS_STATE_RUSH					0x03									// 底盘状态————运动

#define 	CAPACITOR_CALIBRATE_WAIT	0x00	//电容标定状态————等待
#define		CAPACITOR_CALIBRATE_STATE	0x01	//电容标定状态————进行中
#define		CAPACITOR_CALIBRATE_STOP	0x02	//电容标定状态————暂停
#define		CAPACITOR_CALIBRATE_FINISH	0x03	//电容标定状态————完成
#define		CAPACITOR_CALIBRATE_FAULT	0x04	//电容标定状态————错误
	
#define CAPACITOR_CALIBRATE_INPUT_POWER_MAX		95
#define CAPACITOR_CALIBRATE_INPUT_POWER_MIN		15
#define CAPACITOR_CALIBRATE_INPUT_POWER_MEAN	40
#define CAPACITOR_COMPENSATE_INPUT_POWER_OVUT_MAX	400
#define CAPACITOR_COMPENSATE_INPUT_POWER_MAX	200
#define CAPACITOR_COMPENSATE_INPUT_POWER_MIN	35
#define CAPACITOR_COMPENSATE_OUTPUT_POWER		50


/*底盘缓冲能量安全阈值*/
#define CHASSIS_BUFF_POWER_LIMIT			200.0f

#define CHASSIS_BUFF_POWER_SAFE_THRE		0.9f

/*底盘缓冲能量危险阈值*/
#define CHASSIS_BUFF_POWER_DANGER_THRE		0.2f

/*底盘缓冲能量恢复系数*/
#define CHASSIS_BUFF_POWER_RECOVER_PARA1     0.95f
#define CHASSIS_BUFF_POWER_RECOVER_PARA2     0.90f

/*底盘缓冲能量常规控制系数*/
#define CHASSIS_BUFF_POWER_NORMAL_PARA     	1.0f

/*底盘缓冲能量危险状态控制系数*/
#define CHASSIS_BUFF_POWER_DANGER_PARA     	0.95f

/*底盘速度平缓滤波系数*/
#define SPEED_SMOOTH_PARA					0.01f

/*变向位置允许误差*/
#define TURNING_SITE_ERROR_MAX				50

/* 轨道禁止变向区域 */
#define BAN_TURNROUND_AREA 					(rush.RankLength / 10)

/* 变向速度最低需求 */
#define TURNING_SPEED_MIN					500

/* 底盘速度平滑滤波器大小 */
#define ESPEEDBOXSIZE						20
#define MSPEEDBOXSIZE						10

/* 云台角度平滑滤波器大小 */
#define UPYAWANGLEBOXSIZE						1
#define DNYAWANGLEBOXSIZE						1

/* 撞柱后不打弹时间*/
#define UNSHOOTTIMES						500

/* 变向开始趟数 */
#define RUSHRANK							5

/* 残血 */
#define Hero_HP_Threshold		90							//视觉识别英雄血线
#define Engi_HP_Threshold		100							//视觉识别工程血线
#define Stan_HP_Threshold		70							//视觉识别步兵血线

#define RECAPTIMES				3							// 电容重标次数

#define CAPPOWERDES				40							// 电容控制目标功率


/********************************比赛相关******************************************/
#define DEFEND_FIRST_LINE_DISTANCE_UP			10000									// 上云台第一道防线距离（地方前哨站的垂直距离）
#define DEFEND_SECOND_LINE_DISTANCE_UP			7000									// 上云台第二道防线距离（我方前哨站的垂直距离）
#define DEFEND_THIRD_LINE_DISTANCE_UP			3000									// 上云台第三道防线距离（我方环形高地的垂直距离）
#define DEFEND_FIRST_LINE_SHOOT_FRC_UP			5										// 上云台第一道防线内射速
#define DEFEND_SECOND_LINE_SHOOT_FRC_UP			9										// 上云台第二道防线内射速
#define DEFEND_THIRD_LINE_SHOOT_FRC_UP			9										// 上云台第三道防线内射速

#define DEFEND_FIRST_LINE_DISTANCE_DN			10000									// 下云台第一道防线距离（地方前哨站的垂直距离）
#define DEFEND_SECOND_LINE_DISTANCE_DN			7000									// 下云台第二道防线距离（我方前哨站的垂直距离）
#define DEFEND_THIRD_LINE_DISTANCE_DN			3000									// 下云台第三道防线距离（我方环形高地的垂直距离）
#define DEFEND_FIRST_LINE_SHOOT_FRC_DN			9										// 下云台第一道防线内射速
#define DEFEND_SECOND_LINE_SHOOT_FRC_DN			17										// 下云台第二道防线内射速
#define DEFEND_THIRD_LINE_SHOOT_FRC_DN			17										// 下云台第三道防线内射速

#define BARREL_HEAT_LIMIT_UP					320										// 上云台枪管热量上限
#define BARREL_HEAT_FIRST_LINE_UP				160										// 上云台枪管热量第一阈值线
#define BARREL_HEAT_SECOND_LINE_UP				280										// 上云台枪管热量第二阈值线
#define BARREL_HEAT_THIRD_LINE_UP				310										// 上云台枪管热量第三阈值线
#define BARREL_HEAT_FIRST_LINE_SHOOT_FRC_UP		9										// 上云台枪管热量第一阈值线内的最高射速
#define BARREL_HEAT_SECOND_LINE_SHOOT_FRC_UP	9										// 上云台枪管热量第二阈值线内的最高射速
#define BARREL_HEAT_THIRD_LINE_SHOOT_FRC_UP		5										// 上云台枪管热量第三阈值线内的最高射速

#define BARREL_HEAT_LIMIT_DN					320										// 下云台枪管热量上限
#define BARREL_HEAT_FIRST_LINE_DN				160										// 下云台枪管热量第一阈值线
#define BARREL_HEAT_SECOND_LINE_DN				280										// 下云台枪管热量第二阈值线
#define BARREL_HEAT_THIRD_LINE_DN				310										// 下云台枪管热量第三阈值线
#define BARREL_HEAT_FIRST_LINE_SHOOT_FRC_DN		17										// 下云台枪管热量第一阈值线内的最高射速
#define BARREL_HEAT_SECOND_LINE_SHOOT_FRC_DN	9										// 下云台枪管热量第二阈值线内的最高射速
#define BARREL_HEAT_THIRD_LINE_SHOOT_FRC_DN		5										// 下云台枪管热量第三阈值线内的最高射速


#endif
