/**
 * @file canraw.hpp
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2020-08-24
 * 
 * @copyright Copyright () 2020
 * 
 */
#include <cstdint>

#pragma once

#pragma pack(push, 1)
typedef struct
{
    char data[8];
} TL0x1830FF21Struct;

/**
 *激活状态   00b/01b/11b<--->未激活/激活/无效
 * */
typedef struct
{
    uint8_t wire_status_vehicle_ctrl_mode; //车辆控制模式

    uint8_t wire_status_engine : 2;     //发动机激活状态
    uint8_t wire_status_hand_brake : 2; //手刹激活状态
    uint8_t wire_status_ebs : 2;        // EBS激活状态
    uint8_t wire_status_retarder : 2;   //缓速器激活状态

    uint8_t wire_status_transmission : 2; //变速箱的激活状态
    uint8_t wire_status_eps : 2;          // EPS激活状态
    uint8_t wire_status_lift : 2;         //举升激活状态
    uint8_t res0 : 2;
} TL0x1830B021Struct;

/**
 *
 *故障状态   0x00/0x01/0x02/0x03<--->无故障/一般故障/严重故障/致命故障
 *
 * */
typedef struct
{
    uint8_t wire_status_engine_falut;       //发动机故障
    uint8_t wire_status_transmission_falut; //变速箱故障
    uint8_t wire_status_ebs_falut;          // EBS故障
    uint8_t wire_status_eps_falut;          // EPS故障
} TL0x1830E021Struct;

/**
 *
 *故障等级    0x00/0x01/0x02/0x03<--->无故障/一般故障/严重故障/致命故障
 *故障编码    0x00~0xFA<--->0xFA表示掉线故障,0xFF表示未知的故障代码
 *
 * */
typedef struct
{
    uint8_t wire_status_vehicle_falut;     //整车故障等级
    uint8_t wire_status_engine_falut_code; //发动机故障编码
    uint8_t res0;
    uint8_t res1;
    uint8_t res2;
    uint8_t wire_status_transmission_falut_code; //变速箱故障编码
    uint8_t res3;
    uint8_t res4;
} TL0x18FECA21Struct;

/**
 *
 *故障等级    0x00/0x01/0x02/0x03<--->无故障/一般故障/严重故障/致命故障
 *故障编码    0x00~0xFA<--->0xFA表示掉线故障,0xFF表示未知的故障代码
 *
 * */
typedef struct
{
    uint8_t res0;
    uint8_t wire_status_eps_falut_code; // EPS故障编码
    uint8_t wire_status_ebs_falut_code; // EBS故障编码
    uint8_t res1;
    uint8_t res2;
    uint8_t res3;
    uint8_t wire_status_electrical_system_falut_code; // 电气系统故障编码
    uint8_t res4;
} TL0x18FECB21Struct;

typedef struct
{
    uint8_t wire_status_acc_pedal_percent_raw;       //加速踏板行程百分比
    uint8_t wire_status_engine_torque_percent_raw;   //发动机扭矩百分比
    uint16_t wire_status_engine_speed_raw;           //发动机转速
    uint8_t wire_status_actually_accelerate_pad_raw; //实际踩踏板百分比
    uint8_t res0;
    uint8_t res1;
    uint8_t check_sum_0C300021 : 4;       //报文校验
    uint8_t rollint_counter_0C300021 : 4; //指令循环数
} TL0x0C300021Struct;

// clang-format off
typedef struct {
  uint16_t wire_status_front_axle_aver_velocity_raw;  //前轴平均速度
  uint8_t wire_status_front_axle_left_wheel_relatively_velocity_raw;  //前轴左侧轮相对速度
  uint8_t wire_status_front_axle_right_wheel_relatively_velocity_raw;  //前轴右侧轮相对速度
  uint8_t res0;
  uint8_t res1;
  uint8_t wire_status_rear_axle_left_wheel_relatively_velocity_raw;  //后轴左侧轮相对速度
  uint8_t wire_status_rear_axle_right_wheel_relatively_velocity_raw;  //后轴右侧轮相对速度
} TL0x0C301121Struct;

// clang-format on
typedef struct
{
    uint8_t res0;
    uint8_t wire_status_mechanical_brake_percent_raw; //制动踏板位置百分比
    uint8_t res1;
    uint8_t res2;
    uint8_t res3;
    uint8_t res4;
    uint8_t res5;
    uint8_t check_sum_0C301021 : 4;
    uint8_t rollint_counter_0C301021 : 4;
} TL0x0C301021Struct;

typedef struct
{
    uint8_t wire_status_retarder_torque_percent_raw; //缓速器扭矩百分比
    uint8_t res0;
    uint8_t res1;
    uint8_t res2;
    uint8_t res3;
    uint8_t res4;
    uint8_t res5;
    uint8_t check_sum_0C301221 : 4;
    uint8_t rollint_counter_0C301221 : 4;
} TL0x0C301221Struct;

typedef struct
{
    uint16_t wire_status_steering_wheel_angle_raw;    //方向盘当前转角
    uint16_t wire_status_steering_wheel_torque_raw;   //电机助力扭矩
    uint8_t wire_status_steering_axis_speed_rate_raw; //方向盘反馈角速度
    uint8_t res0;
    uint8_t res1;
    uint8_t check_sum_0C302021 : 4;
    uint8_t rollint_counter_0C302021 : 4;
} TL0x0C302021Struct;

typedef struct
{
    uint8_t wire_status_gear_shift_raw;     //变速箱当前档位
    uint8_t wire_status_max_gear_shift_raw; //变速箱最高前进挡
    uint16_t wire_status_truck_speed_raw;   //车速(通过变速箱计算)
    uint8_t res0;
    uint8_t res1;
    uint8_t res2;
    uint8_t check_sum_0C303021 : 4;
    uint8_t rollint_counter_0C303021 : 4;
} TL0x0C303021Struct;

// clang-format off
typedef struct {
  uint16_t wire_status_truck_horizontal_angle_raw;    //车辆水平纵向角度
  uint16_t wire_status_truck_longitudinal_angle_raw;  //车辆水平横向角度
  uint16_t wire_status_truck_lift_anle_raw;           //车辆水平纵向角度
  uint8_t wire_status_bucket_pos_raw;                 //货箱举升角度
  uint8_t check_sum_0C304021 : 4;
  uint8_t rollint_counter_0C304021 : 4;
} TL0x0C304021Struct;
// clang-format on

typedef struct
{
    char data[8]; //车辆识别码
} TL0x1830F021Struct;

/**
 *
 *00关/01开
 *
 * */
typedef struct
{
    uint8_t wire_status_emergency_switch : 2;       //紧急双闪开关输入
    uint8_t wire_status_air_filter_alarm : 2;       //空滤堵塞报警
    uint8_t wire_status_steer_oil_filter_alarm : 2; //转向油滤报警
    uint8_t res0 : 2;
    uint8_t res1;
    uint8_t res2;
    uint8_t res3;
    uint8_t res4;
    uint8_t res5;
    uint8_t res6;
    uint8_t res7;
} TL0x1830A321Struct;

/**
 *
 * 11b:正常开 01b:短路  00b:正常关
 *
 * */
typedef struct
{
    uint8_t wire_status_left_light : 2;  //左转灯
    uint8_t wire_status_right_light : 2; //右转灯
    uint8_t wire_status_low_beam : 2;    //近光灯
    uint8_t wire_status_high_beam : 2;   //远光灯

    uint8_t wire_status_rotate_alarm_light : 2; //旋转报警灯
    uint8_t wire_status_air_horns : 2;          //气喇叭
    uint8_t wire_status_front_fog_lamp : 2;     //前雾灯
    uint8_t wire_status_front_work_lamp : 2;    //前工作灯

    uint8_t wire_status_side_lamp : 2;     //侧照灯
    uint8_t wire_status_positoin_lamp : 2; //位置灯
    uint8_t res0 : 4;

    uint8_t wire_status_wiper_motor_power : 2;     //雨刮电机供电
    uint8_t wire_status_ebs_power : 2;             // ebs供电
    uint8_t wire_status_pressure_sensor_power : 2; //气压传感器1-2供电
    uint8_t res1 : 2;

    uint8_t wire_status_rise_solenoid_savle_up : 2;   //举升磁阀上
    uint8_t wire_status_rise_solenoid_savle_down : 2; //举升磁阀下
    uint8_t wire_status_manual_solenoid_savle : 2;    //手制动磁阀
    uint8_t res2 : 2;

    uint8_t res3;
    uint8_t res4;
    uint8_t res5;
} TL0x1830A021Struct;

typedef struct
{
    uint8_t wire_status_pressure_sensor1_raw; //气压传感器1
    uint8_t wire_status_pressure_sensor2_raw; //气压传感器2
    uint8_t wire_status_oil_sensor_raw;       //油量传感器
    uint8_t res0;
    uint8_t res1;
    uint8_t res2;
    uint8_t res3;
    uint8_t res4;
} TL0x1830A221Struct;

/**
 *
 * 11b:正常开 01b:短路  00b:正常关
 *
 * */
typedef struct
{
    uint8_t wire_status_rear_left_light : 2;  //左后转向灯
    uint8_t wire_status_rear_right_light : 2; //右后转向灯
    uint8_t wire_status_rear_pos_light : 2;   //后位置灯
    uint8_t wire_status_brake_light : 2;      //制动灯

    uint8_t wire_status_reverse_light_horns : 2; //倒车灯,倒车喇叭
    uint8_t wire_status_rear_work : 2;           //后工作灯
    uint8_t res0 : 4;

    uint8_t wire_status_heat_sink1 : 2;          //辅助散热器1
    uint8_t wire_status_heat_sink2 : 2;          //辅助散热器2
    uint8_t wire_status_diff_solenoid_savle : 2; //轴差电磁阀
    uint8_t res1 : 2;

    uint8_t wire_status_dryer_power : 2;               //干燥器供电
    uint8_t wire_status_inclinator_power : 2;          //倾角仪供电
    uint8_t wire_status_cold_water_treasure_power : 2; //水寒宝供电
    uint8_t res2 : 2;

    uint8_t wire_status_exhaust_brake : 2; //排气制动输出
    uint8_t res3 : 6;
} TL0x1830A121Struct;

/*
 *
 *自研VCU用于北方股份的车辆
 *
 */
typedef struct
{
    uint16_t wire_status_acc_pedal_percent_raw;
    uint16_t wire_status_elctric_brake_pedal_percent_raw;

    uint8_t wire_status_parking_status : 1;
    uint8_t wire_status_throttle_status : 1;
    uint8_t wire_status_electric_brake_status : 1;
    uint8_t wire_status_steerwheelmotor_status : 1;
    uint8_t res0 : 4;

    uint16_t wire_status_front_wheel_angle_raw;

    uint8_t wire_status_is_auto_drive_status : 1;
    uint8_t res1 : 7;
} NT0x0C305121Struct;

typedef struct
{
    uint16_t wire_status_mechanical_brake_pedal_percent_raw;
    uint16_t wire_status_mechanical_brake_motor_load_rate_raw;
    uint16_t wire_status_mechanical_brake_motor_error_num_raw;
    uint8_t wire_status_mechanical_brake_motor_enable_status;
    uint8_t res0;
} NT0x0C305221Struct;

typedef struct
{
    uint16_t wire_status_left_wheel_speed_raw;
    uint16_t wire_status_right_wheel_speed_raw;
    uint8_t res0;
    uint8_t res1;
    uint8_t res2;

    uint8_t res3 : 2;
    uint8_t wire_status_ruler_status_raw : 2;
    uint8_t res4 : 4;
} NT0x0C305321Struct;

typedef struct
{
    uint8_t wire_status_gear_shift_raw;
    uint8_t wire_status_gear_shift_lamp_status;
    uint16_t wire_status_truck_lift_anle_raw;

    uint8_t res1 : 1;
    uint8_t wire_status_inclinator_status : 2;
    uint8_t wire_status_lift_status : 2;
    uint8_t res2 : 3;

    uint8_t res3;

    uint16_t wire_status_steer_speed_raw;
} NT0x0C305421Struct;

typedef struct
{
    uint16_t wire_status_thr_feedback_AD;
    uint16_t wire_status_ebrk_feedback_AD;
    uint16_t wire_status_zhongchuang_motorspeed;
    uint16_t wire_status_zhongchuang_motorcur;
} NT0x0C305621Struct;

typedef struct
{
    uint8_t wire_status_product_type;
    uint8_t wire_status_module_type;
    uint8_t wire_status_vehicle_type;
    uint8_t wire_status_main_version;
    uint8_t wire_status_sub_version;
    uint8_t wire_status_func_version;
    uint8_t res1;
    uint8_t res2;
} NT0x0C305521Struct;

/*
 *
 * 北方股份线控VCU
 *
 * */
typedef struct
{
    uint8_t wire_status_error_level : 4;
    uint8_t res0 : 4;

    uint8_t wire_satus_vehicle_load;
    uint8_t wire_status_mechanical_brake_pedal_percent_raw;

    uint8_t wire_status_is_auto_drive_status : 1;
    uint8_t wire_status_urgent_brake : 1;
    uint8_t wire_status_lift_up_ctl : 1;
    uint8_t wire_status_lift_fall_ctl : 1;
    uint8_t wire_status_lift_up_feedback : 1;
    uint8_t wire_status_lift_fall_feedback : 1;
    uint8_t wire_status_lift_keep_feedback : 1;
    uint8_t wire_status_electric_brake_status : 1;

    uint8_t wire_status_load_brake_status : 1;
    uint8_t wire_status_park_brake_status : 1;
    uint8_t wire_status_gear_shift_raw : 2;
    uint8_t res1 : 4;

    uint8_t wire_status_acc_pedal_actuator_percent_raw;
    uint8_t wire_status_acc_pedal_percent_raw;

    uint8_t res2;
} NT0x18FD0821Struct;

typedef struct
{
    uint16_t wire_status_steering_wheel_angle_raw;
    uint8_t res0;
    uint8_t res1;
    uint8_t wire_status_steering_valve_status;
    uint8_t res2;
    uint8_t res3;
    uint8_t res4;
} NT0x18FD0921Struct;

typedef struct
{
    uint16_t wire_status_truck_speed_raw;
    uint8_t res0;
    uint8_t res1;
    uint8_t res2;
    uint8_t res3;
    uint8_t res4;
    uint8_t res5;
} NT0x18FF0407Struct;

union CanFrameMsg
{
    TL0x1830FF21Struct tl0x1830ff21St;
    TL0x1830B021Struct tl0x1830b021St;
    TL0x1830E021Struct tl0x1830e021St;
    TL0x18FECA21Struct tl0x18feca21St;
    TL0x18FECB21Struct tl0x18fecb21St;
    TL0x0C300021Struct tl0x0c300021St;
    TL0x0C301121Struct tl0x0C301121St;
    TL0x0C301021Struct tl0x0c301021St;
    TL0x0C301121Struct tl0x0c301121St;
    TL0x0C301221Struct tl0x0c301221St;
    TL0x0C302021Struct tl0x0c302021St;
    TL0x0C303021Struct tl0x0c303021St;
    TL0x0C304021Struct tl0x0c304021St;
    TL0x1830F021Struct tl0x1830f021St;
    TL0x1830A321Struct tl0x1830a321St;
    TL0x1830A021Struct tl0x1830a021St;
    TL0x1830A221Struct tl0x1830a221St;
    TL0x1830A121Struct tl0x1830a121St;
    NT0x0C305121Struct nt0x0c305121St;
    NT0x0C305221Struct nt0x0c305221St;
    NT0x0C305321Struct nt0x0c305321St;
    NT0x0C305421Struct nt0x0c305421St;
    NT0x0C305621Struct nt0x0c305621St;
    NT0x0C305521Struct nt0x0c305521St;
    NT0x18FD0821Struct nt0x18fd0821St;
    NT0x18FD0921Struct nt0x18fd0921St;
    NT0x18FF0407Struct nt0x18ff0407St;
    uint8_t canFrameData[8];
};

#pragma pack(pop)