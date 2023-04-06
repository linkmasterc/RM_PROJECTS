#ifndef PARAM_CONFIG_H
#define PARAM_CONFIG_H

//摩擦轮
#define fric_speed1 -12200.0
#define Fric 139

//pitch轴电机
#define	PITCH_USE_TD
#define GBDN_PITCH_MAX	30.0f
#define GBDN_PITCH_MIN	-27.0f
#define GM_PITCH_MIDPOS 4106

/*YAW电机设定中值*/
#define SECOND_YAW_MID           3450

/*云台拨弹电机单弹占角*/	
#define BOTTOM_SUPPLY_STEP      			(29491 * 5 / 3)

/* 云台角度平滑滤波器大小 */
#define YAWANGLEBOXSIZE						1

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
