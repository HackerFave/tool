// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_CONTROL_CONTROL_H_
#define FLATBUFFERS_GENERATED_CONTROL_CONTROL_H_

#include "flatbuffers/flatbuffers.h"

namespace control {

struct ControlOutputForImplement;
struct ControlOutputForImplementBuilder;
struct ControlOutputForImplementT;

struct ControlOutputForDecision;
struct ControlOutputForDecisionBuilder;
struct ControlOutputForDecisionT;

bool operator==(const ControlOutputForImplementT &lhs, const ControlOutputForImplementT &rhs);
bool operator!=(const ControlOutputForImplementT &lhs, const ControlOutputForImplementT &rhs);
bool operator==(const ControlOutputForDecisionT &lhs, const ControlOutputForDecisionT &rhs);
bool operator!=(const ControlOutputForDecisionT &lhs, const ControlOutputForDecisionT &rhs);

inline const flatbuffers::TypeTable *ControlOutputForImplementTypeTable();

inline const flatbuffers::TypeTable *ControlOutputForDecisionTypeTable();

struct ControlOutputForImplementT : public flatbuffers::NativeTable {
  typedef ControlOutputForImplement TableType;
  float vehicle_velocity;
  float front_wheel_angle;
  float throttle_pedal;
  float brake_pedal;
  float hydraulic_brake;
  float retarder_ctrl;
  uint8_t park_ctrl;
  uint8_t exhuast_brake;
  int16_t shift_ctrl;
  int16_t bucket_ctrl;
  uint8_t trumpet_ctrl;
  uint8_t lamp_ctrl;
  uint8_t shift_limit;
  bool can_send_flag;
  bool const_speed_downhill_enable;
  float const_speed_downhill_speed;
  ControlOutputForImplementT()
      : vehicle_velocity(0.0f),
        front_wheel_angle(0.0f),
        throttle_pedal(0.0f),
        brake_pedal(0.0f),
        hydraulic_brake(0.0f),
        retarder_ctrl(0.0f),
        park_ctrl(0),
        exhuast_brake(0),
        shift_ctrl(0),
        bucket_ctrl(0),
        trumpet_ctrl(0),
        lamp_ctrl(0),
        shift_limit(0),
        can_send_flag(false),
        const_speed_downhill_enable(false),
        const_speed_downhill_speed(0.0f) {
  }
};

inline bool operator==(const ControlOutputForImplementT &lhs, const ControlOutputForImplementT &rhs) {
  return
      (lhs.vehicle_velocity == rhs.vehicle_velocity) &&
      (lhs.front_wheel_angle == rhs.front_wheel_angle) &&
      (lhs.throttle_pedal == rhs.throttle_pedal) &&
      (lhs.brake_pedal == rhs.brake_pedal) &&
      (lhs.hydraulic_brake == rhs.hydraulic_brake) &&
      (lhs.retarder_ctrl == rhs.retarder_ctrl) &&
      (lhs.park_ctrl == rhs.park_ctrl) &&
      (lhs.exhuast_brake == rhs.exhuast_brake) &&
      (lhs.shift_ctrl == rhs.shift_ctrl) &&
      (lhs.bucket_ctrl == rhs.bucket_ctrl) &&
      (lhs.trumpet_ctrl == rhs.trumpet_ctrl) &&
      (lhs.lamp_ctrl == rhs.lamp_ctrl) &&
      (lhs.shift_limit == rhs.shift_limit) &&
      (lhs.can_send_flag == rhs.can_send_flag) &&
      (lhs.const_speed_downhill_enable == rhs.const_speed_downhill_enable) &&
      (lhs.const_speed_downhill_speed == rhs.const_speed_downhill_speed);
}

inline bool operator!=(const ControlOutputForImplementT &lhs, const ControlOutputForImplementT &rhs) {
    return !(lhs == rhs);
}


struct ControlOutputForImplement FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ControlOutputForImplementT NativeTableType;
  typedef ControlOutputForImplementBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return ControlOutputForImplementTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_VEHICLE_VELOCITY = 4,
    VT_FRONT_WHEEL_ANGLE = 6,
    VT_THROTTLE_PEDAL = 8,
    VT_BRAKE_PEDAL = 10,
    VT_HYDRAULIC_BRAKE = 12,
    VT_RETARDER_CTRL = 14,
    VT_PARK_CTRL = 16,
    VT_EXHUAST_BRAKE = 18,
    VT_SHIFT_CTRL = 20,
    VT_BUCKET_CTRL = 22,
    VT_TRUMPET_CTRL = 24,
    VT_LAMP_CTRL = 26,
    VT_SHIFT_LIMIT = 28,
    VT_CAN_SEND_FLAG = 30,
    VT_CONST_SPEED_DOWNHILL_ENABLE = 32,
    VT_CONST_SPEED_DOWNHILL_SPEED = 34
  };
  float vehicle_velocity() const {
    return GetField<float>(VT_VEHICLE_VELOCITY, 0.0f);
  }
  bool mutate_vehicle_velocity(float _vehicle_velocity) {
    return SetField<float>(VT_VEHICLE_VELOCITY, _vehicle_velocity, 0.0f);
  }
  float front_wheel_angle() const {
    return GetField<float>(VT_FRONT_WHEEL_ANGLE, 0.0f);
  }
  bool mutate_front_wheel_angle(float _front_wheel_angle) {
    return SetField<float>(VT_FRONT_WHEEL_ANGLE, _front_wheel_angle, 0.0f);
  }
  float throttle_pedal() const {
    return GetField<float>(VT_THROTTLE_PEDAL, 0.0f);
  }
  bool mutate_throttle_pedal(float _throttle_pedal) {
    return SetField<float>(VT_THROTTLE_PEDAL, _throttle_pedal, 0.0f);
  }
  float brake_pedal() const {
    return GetField<float>(VT_BRAKE_PEDAL, 0.0f);
  }
  bool mutate_brake_pedal(float _brake_pedal) {
    return SetField<float>(VT_BRAKE_PEDAL, _brake_pedal, 0.0f);
  }
  float hydraulic_brake() const {
    return GetField<float>(VT_HYDRAULIC_BRAKE, 0.0f);
  }
  bool mutate_hydraulic_brake(float _hydraulic_brake) {
    return SetField<float>(VT_HYDRAULIC_BRAKE, _hydraulic_brake, 0.0f);
  }
  float retarder_ctrl() const {
    return GetField<float>(VT_RETARDER_CTRL, 0.0f);
  }
  bool mutate_retarder_ctrl(float _retarder_ctrl) {
    return SetField<float>(VT_RETARDER_CTRL, _retarder_ctrl, 0.0f);
  }
  uint8_t park_ctrl() const {
    return GetField<uint8_t>(VT_PARK_CTRL, 0);
  }
  bool mutate_park_ctrl(uint8_t _park_ctrl) {
    return SetField<uint8_t>(VT_PARK_CTRL, _park_ctrl, 0);
  }
  uint8_t exhuast_brake() const {
    return GetField<uint8_t>(VT_EXHUAST_BRAKE, 0);
  }
  bool mutate_exhuast_brake(uint8_t _exhuast_brake) {
    return SetField<uint8_t>(VT_EXHUAST_BRAKE, _exhuast_brake, 0);
  }
  int16_t shift_ctrl() const {
    return GetField<int16_t>(VT_SHIFT_CTRL, 0);
  }
  bool mutate_shift_ctrl(int16_t _shift_ctrl) {
    return SetField<int16_t>(VT_SHIFT_CTRL, _shift_ctrl, 0);
  }
  int16_t bucket_ctrl() const {
    return GetField<int16_t>(VT_BUCKET_CTRL, 0);
  }
  bool mutate_bucket_ctrl(int16_t _bucket_ctrl) {
    return SetField<int16_t>(VT_BUCKET_CTRL, _bucket_ctrl, 0);
  }
  uint8_t trumpet_ctrl() const {
    return GetField<uint8_t>(VT_TRUMPET_CTRL, 0);
  }
  bool mutate_trumpet_ctrl(uint8_t _trumpet_ctrl) {
    return SetField<uint8_t>(VT_TRUMPET_CTRL, _trumpet_ctrl, 0);
  }
  uint8_t lamp_ctrl() const {
    return GetField<uint8_t>(VT_LAMP_CTRL, 0);
  }
  bool mutate_lamp_ctrl(uint8_t _lamp_ctrl) {
    return SetField<uint8_t>(VT_LAMP_CTRL, _lamp_ctrl, 0);
  }
  uint8_t shift_limit() const {
    return GetField<uint8_t>(VT_SHIFT_LIMIT, 0);
  }
  bool mutate_shift_limit(uint8_t _shift_limit) {
    return SetField<uint8_t>(VT_SHIFT_LIMIT, _shift_limit, 0);
  }
  bool can_send_flag() const {
    return GetField<uint8_t>(VT_CAN_SEND_FLAG, 0) != 0;
  }
  bool mutate_can_send_flag(bool _can_send_flag) {
    return SetField<uint8_t>(VT_CAN_SEND_FLAG, static_cast<uint8_t>(_can_send_flag), 0);
  }
  bool const_speed_downhill_enable() const {
    return GetField<uint8_t>(VT_CONST_SPEED_DOWNHILL_ENABLE, 0) != 0;
  }
  bool mutate_const_speed_downhill_enable(bool _const_speed_downhill_enable) {
    return SetField<uint8_t>(VT_CONST_SPEED_DOWNHILL_ENABLE, static_cast<uint8_t>(_const_speed_downhill_enable), 0);
  }
  float const_speed_downhill_speed() const {
    return GetField<float>(VT_CONST_SPEED_DOWNHILL_SPEED, 0.0f);
  }
  bool mutate_const_speed_downhill_speed(float _const_speed_downhill_speed) {
    return SetField<float>(VT_CONST_SPEED_DOWNHILL_SPEED, _const_speed_downhill_speed, 0.0f);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_VEHICLE_VELOCITY) &&
           VerifyField<float>(verifier, VT_FRONT_WHEEL_ANGLE) &&
           VerifyField<float>(verifier, VT_THROTTLE_PEDAL) &&
           VerifyField<float>(verifier, VT_BRAKE_PEDAL) &&
           VerifyField<float>(verifier, VT_HYDRAULIC_BRAKE) &&
           VerifyField<float>(verifier, VT_RETARDER_CTRL) &&
           VerifyField<uint8_t>(verifier, VT_PARK_CTRL) &&
           VerifyField<uint8_t>(verifier, VT_EXHUAST_BRAKE) &&
           VerifyField<int16_t>(verifier, VT_SHIFT_CTRL) &&
           VerifyField<int16_t>(verifier, VT_BUCKET_CTRL) &&
           VerifyField<uint8_t>(verifier, VT_TRUMPET_CTRL) &&
           VerifyField<uint8_t>(verifier, VT_LAMP_CTRL) &&
           VerifyField<uint8_t>(verifier, VT_SHIFT_LIMIT) &&
           VerifyField<uint8_t>(verifier, VT_CAN_SEND_FLAG) &&
           VerifyField<uint8_t>(verifier, VT_CONST_SPEED_DOWNHILL_ENABLE) &&
           VerifyField<float>(verifier, VT_CONST_SPEED_DOWNHILL_SPEED) &&
           verifier.EndTable();
  }
  ControlOutputForImplementT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ControlOutputForImplementT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<ControlOutputForImplement> Pack(flatbuffers::FlatBufferBuilder &_fbb, const ControlOutputForImplementT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct ControlOutputForImplementBuilder {
  typedef ControlOutputForImplement Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_vehicle_velocity(float vehicle_velocity) {
    fbb_.AddElement<float>(ControlOutputForImplement::VT_VEHICLE_VELOCITY, vehicle_velocity, 0.0f);
  }
  void add_front_wheel_angle(float front_wheel_angle) {
    fbb_.AddElement<float>(ControlOutputForImplement::VT_FRONT_WHEEL_ANGLE, front_wheel_angle, 0.0f);
  }
  void add_throttle_pedal(float throttle_pedal) {
    fbb_.AddElement<float>(ControlOutputForImplement::VT_THROTTLE_PEDAL, throttle_pedal, 0.0f);
  }
  void add_brake_pedal(float brake_pedal) {
    fbb_.AddElement<float>(ControlOutputForImplement::VT_BRAKE_PEDAL, brake_pedal, 0.0f);
  }
  void add_hydraulic_brake(float hydraulic_brake) {
    fbb_.AddElement<float>(ControlOutputForImplement::VT_HYDRAULIC_BRAKE, hydraulic_brake, 0.0f);
  }
  void add_retarder_ctrl(float retarder_ctrl) {
    fbb_.AddElement<float>(ControlOutputForImplement::VT_RETARDER_CTRL, retarder_ctrl, 0.0f);
  }
  void add_park_ctrl(uint8_t park_ctrl) {
    fbb_.AddElement<uint8_t>(ControlOutputForImplement::VT_PARK_CTRL, park_ctrl, 0);
  }
  void add_exhuast_brake(uint8_t exhuast_brake) {
    fbb_.AddElement<uint8_t>(ControlOutputForImplement::VT_EXHUAST_BRAKE, exhuast_brake, 0);
  }
  void add_shift_ctrl(int16_t shift_ctrl) {
    fbb_.AddElement<int16_t>(ControlOutputForImplement::VT_SHIFT_CTRL, shift_ctrl, 0);
  }
  void add_bucket_ctrl(int16_t bucket_ctrl) {
    fbb_.AddElement<int16_t>(ControlOutputForImplement::VT_BUCKET_CTRL, bucket_ctrl, 0);
  }
  void add_trumpet_ctrl(uint8_t trumpet_ctrl) {
    fbb_.AddElement<uint8_t>(ControlOutputForImplement::VT_TRUMPET_CTRL, trumpet_ctrl, 0);
  }
  void add_lamp_ctrl(uint8_t lamp_ctrl) {
    fbb_.AddElement<uint8_t>(ControlOutputForImplement::VT_LAMP_CTRL, lamp_ctrl, 0);
  }
  void add_shift_limit(uint8_t shift_limit) {
    fbb_.AddElement<uint8_t>(ControlOutputForImplement::VT_SHIFT_LIMIT, shift_limit, 0);
  }
  void add_can_send_flag(bool can_send_flag) {
    fbb_.AddElement<uint8_t>(ControlOutputForImplement::VT_CAN_SEND_FLAG, static_cast<uint8_t>(can_send_flag), 0);
  }
  void add_const_speed_downhill_enable(bool const_speed_downhill_enable) {
    fbb_.AddElement<uint8_t>(ControlOutputForImplement::VT_CONST_SPEED_DOWNHILL_ENABLE, static_cast<uint8_t>(const_speed_downhill_enable), 0);
  }
  void add_const_speed_downhill_speed(float const_speed_downhill_speed) {
    fbb_.AddElement<float>(ControlOutputForImplement::VT_CONST_SPEED_DOWNHILL_SPEED, const_speed_downhill_speed, 0.0f);
  }
  explicit ControlOutputForImplementBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ControlOutputForImplement> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ControlOutputForImplement>(end);
    return o;
  }
};

inline flatbuffers::Offset<ControlOutputForImplement> CreateControlOutputForImplement(
    flatbuffers::FlatBufferBuilder &_fbb,
    float vehicle_velocity = 0.0f,
    float front_wheel_angle = 0.0f,
    float throttle_pedal = 0.0f,
    float brake_pedal = 0.0f,
    float hydraulic_brake = 0.0f,
    float retarder_ctrl = 0.0f,
    uint8_t park_ctrl = 0,
    uint8_t exhuast_brake = 0,
    int16_t shift_ctrl = 0,
    int16_t bucket_ctrl = 0,
    uint8_t trumpet_ctrl = 0,
    uint8_t lamp_ctrl = 0,
    uint8_t shift_limit = 0,
    bool can_send_flag = false,
    bool const_speed_downhill_enable = false,
    float const_speed_downhill_speed = 0.0f) {
  ControlOutputForImplementBuilder builder_(_fbb);
  builder_.add_const_speed_downhill_speed(const_speed_downhill_speed);
  builder_.add_retarder_ctrl(retarder_ctrl);
  builder_.add_hydraulic_brake(hydraulic_brake);
  builder_.add_brake_pedal(brake_pedal);
  builder_.add_throttle_pedal(throttle_pedal);
  builder_.add_front_wheel_angle(front_wheel_angle);
  builder_.add_vehicle_velocity(vehicle_velocity);
  builder_.add_bucket_ctrl(bucket_ctrl);
  builder_.add_shift_ctrl(shift_ctrl);
  builder_.add_const_speed_downhill_enable(const_speed_downhill_enable);
  builder_.add_can_send_flag(can_send_flag);
  builder_.add_shift_limit(shift_limit);
  builder_.add_lamp_ctrl(lamp_ctrl);
  builder_.add_trumpet_ctrl(trumpet_ctrl);
  builder_.add_exhuast_brake(exhuast_brake);
  builder_.add_park_ctrl(park_ctrl);
  return builder_.Finish();
}

flatbuffers::Offset<ControlOutputForImplement> CreateControlOutputForImplement(flatbuffers::FlatBufferBuilder &_fbb, const ControlOutputForImplementT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct ControlOutputForDecisionT : public flatbuffers::NativeTable {
  typedef ControlOutputForDecision TableType;
  float vehicle_velocity;
  float slope_vehicle_filter;
  float lat_distance;
  float desired_speed;
  bool status_switch_flag;
  bool front_obstacle_timeout_flag;
  bool back_obstacle_timeout_flag;
  float curve_max;
  uint8_t full_load_status;
  ControlOutputForDecisionT()
      : vehicle_velocity(0.0f),
        slope_vehicle_filter(0.0f),
        lat_distance(0.0f),
        desired_speed(0.0f),
        status_switch_flag(false),
        front_obstacle_timeout_flag(false),
        back_obstacle_timeout_flag(false),
        curve_max(0.0f),
        full_load_status(0) {
  }
};

inline bool operator==(const ControlOutputForDecisionT &lhs, const ControlOutputForDecisionT &rhs) {
  return
      (lhs.vehicle_velocity == rhs.vehicle_velocity) &&
      (lhs.slope_vehicle_filter == rhs.slope_vehicle_filter) &&
      (lhs.lat_distance == rhs.lat_distance) &&
      (lhs.desired_speed == rhs.desired_speed) &&
      (lhs.status_switch_flag == rhs.status_switch_flag) &&
      (lhs.front_obstacle_timeout_flag == rhs.front_obstacle_timeout_flag) &&
      (lhs.back_obstacle_timeout_flag == rhs.back_obstacle_timeout_flag) &&
      (lhs.curve_max == rhs.curve_max) &&
      (lhs.full_load_status == rhs.full_load_status);
}

inline bool operator!=(const ControlOutputForDecisionT &lhs, const ControlOutputForDecisionT &rhs) {
    return !(lhs == rhs);
}


struct ControlOutputForDecision FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ControlOutputForDecisionT NativeTableType;
  typedef ControlOutputForDecisionBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return ControlOutputForDecisionTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_VEHICLE_VELOCITY = 4,
    VT_SLOPE_VEHICLE_FILTER = 6,
    VT_LAT_DISTANCE = 8,
    VT_DESIRED_SPEED = 10,
    VT_STATUS_SWITCH_FLAG = 12,
    VT_FRONT_OBSTACLE_TIMEOUT_FLAG = 14,
    VT_BACK_OBSTACLE_TIMEOUT_FLAG = 16,
    VT_CURVE_MAX = 18,
    VT_FULL_LOAD_STATUS = 20
  };
  float vehicle_velocity() const {
    return GetField<float>(VT_VEHICLE_VELOCITY, 0.0f);
  }
  bool mutate_vehicle_velocity(float _vehicle_velocity) {
    return SetField<float>(VT_VEHICLE_VELOCITY, _vehicle_velocity, 0.0f);
  }
  float slope_vehicle_filter() const {
    return GetField<float>(VT_SLOPE_VEHICLE_FILTER, 0.0f);
  }
  bool mutate_slope_vehicle_filter(float _slope_vehicle_filter) {
    return SetField<float>(VT_SLOPE_VEHICLE_FILTER, _slope_vehicle_filter, 0.0f);
  }
  float lat_distance() const {
    return GetField<float>(VT_LAT_DISTANCE, 0.0f);
  }
  bool mutate_lat_distance(float _lat_distance) {
    return SetField<float>(VT_LAT_DISTANCE, _lat_distance, 0.0f);
  }
  float desired_speed() const {
    return GetField<float>(VT_DESIRED_SPEED, 0.0f);
  }
  bool mutate_desired_speed(float _desired_speed) {
    return SetField<float>(VT_DESIRED_SPEED, _desired_speed, 0.0f);
  }
  bool status_switch_flag() const {
    return GetField<uint8_t>(VT_STATUS_SWITCH_FLAG, 0) != 0;
  }
  bool mutate_status_switch_flag(bool _status_switch_flag) {
    return SetField<uint8_t>(VT_STATUS_SWITCH_FLAG, static_cast<uint8_t>(_status_switch_flag), 0);
  }
  bool front_obstacle_timeout_flag() const {
    return GetField<uint8_t>(VT_FRONT_OBSTACLE_TIMEOUT_FLAG, 0) != 0;
  }
  bool mutate_front_obstacle_timeout_flag(bool _front_obstacle_timeout_flag) {
    return SetField<uint8_t>(VT_FRONT_OBSTACLE_TIMEOUT_FLAG, static_cast<uint8_t>(_front_obstacle_timeout_flag), 0);
  }
  bool back_obstacle_timeout_flag() const {
    return GetField<uint8_t>(VT_BACK_OBSTACLE_TIMEOUT_FLAG, 0) != 0;
  }
  bool mutate_back_obstacle_timeout_flag(bool _back_obstacle_timeout_flag) {
    return SetField<uint8_t>(VT_BACK_OBSTACLE_TIMEOUT_FLAG, static_cast<uint8_t>(_back_obstacle_timeout_flag), 0);
  }
  float curve_max() const {
    return GetField<float>(VT_CURVE_MAX, 0.0f);
  }
  bool mutate_curve_max(float _curve_max) {
    return SetField<float>(VT_CURVE_MAX, _curve_max, 0.0f);
  }
  uint8_t full_load_status() const {
    return GetField<uint8_t>(VT_FULL_LOAD_STATUS, 0);
  }
  bool mutate_full_load_status(uint8_t _full_load_status) {
    return SetField<uint8_t>(VT_FULL_LOAD_STATUS, _full_load_status, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_VEHICLE_VELOCITY) &&
           VerifyField<float>(verifier, VT_SLOPE_VEHICLE_FILTER) &&
           VerifyField<float>(verifier, VT_LAT_DISTANCE) &&
           VerifyField<float>(verifier, VT_DESIRED_SPEED) &&
           VerifyField<uint8_t>(verifier, VT_STATUS_SWITCH_FLAG) &&
           VerifyField<uint8_t>(verifier, VT_FRONT_OBSTACLE_TIMEOUT_FLAG) &&
           VerifyField<uint8_t>(verifier, VT_BACK_OBSTACLE_TIMEOUT_FLAG) &&
           VerifyField<float>(verifier, VT_CURVE_MAX) &&
           VerifyField<uint8_t>(verifier, VT_FULL_LOAD_STATUS) &&
           verifier.EndTable();
  }
  ControlOutputForDecisionT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ControlOutputForDecisionT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<ControlOutputForDecision> Pack(flatbuffers::FlatBufferBuilder &_fbb, const ControlOutputForDecisionT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct ControlOutputForDecisionBuilder {
  typedef ControlOutputForDecision Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_vehicle_velocity(float vehicle_velocity) {
    fbb_.AddElement<float>(ControlOutputForDecision::VT_VEHICLE_VELOCITY, vehicle_velocity, 0.0f);
  }
  void add_slope_vehicle_filter(float slope_vehicle_filter) {
    fbb_.AddElement<float>(ControlOutputForDecision::VT_SLOPE_VEHICLE_FILTER, slope_vehicle_filter, 0.0f);
  }
  void add_lat_distance(float lat_distance) {
    fbb_.AddElement<float>(ControlOutputForDecision::VT_LAT_DISTANCE, lat_distance, 0.0f);
  }
  void add_desired_speed(float desired_speed) {
    fbb_.AddElement<float>(ControlOutputForDecision::VT_DESIRED_SPEED, desired_speed, 0.0f);
  }
  void add_status_switch_flag(bool status_switch_flag) {
    fbb_.AddElement<uint8_t>(ControlOutputForDecision::VT_STATUS_SWITCH_FLAG, static_cast<uint8_t>(status_switch_flag), 0);
  }
  void add_front_obstacle_timeout_flag(bool front_obstacle_timeout_flag) {
    fbb_.AddElement<uint8_t>(ControlOutputForDecision::VT_FRONT_OBSTACLE_TIMEOUT_FLAG, static_cast<uint8_t>(front_obstacle_timeout_flag), 0);
  }
  void add_back_obstacle_timeout_flag(bool back_obstacle_timeout_flag) {
    fbb_.AddElement<uint8_t>(ControlOutputForDecision::VT_BACK_OBSTACLE_TIMEOUT_FLAG, static_cast<uint8_t>(back_obstacle_timeout_flag), 0);
  }
  void add_curve_max(float curve_max) {
    fbb_.AddElement<float>(ControlOutputForDecision::VT_CURVE_MAX, curve_max, 0.0f);
  }
  void add_full_load_status(uint8_t full_load_status) {
    fbb_.AddElement<uint8_t>(ControlOutputForDecision::VT_FULL_LOAD_STATUS, full_load_status, 0);
  }
  explicit ControlOutputForDecisionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ControlOutputForDecision> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ControlOutputForDecision>(end);
    return o;
  }
};

inline flatbuffers::Offset<ControlOutputForDecision> CreateControlOutputForDecision(
    flatbuffers::FlatBufferBuilder &_fbb,
    float vehicle_velocity = 0.0f,
    float slope_vehicle_filter = 0.0f,
    float lat_distance = 0.0f,
    float desired_speed = 0.0f,
    bool status_switch_flag = false,
    bool front_obstacle_timeout_flag = false,
    bool back_obstacle_timeout_flag = false,
    float curve_max = 0.0f,
    uint8_t full_load_status = 0) {
  ControlOutputForDecisionBuilder builder_(_fbb);
  builder_.add_curve_max(curve_max);
  builder_.add_desired_speed(desired_speed);
  builder_.add_lat_distance(lat_distance);
  builder_.add_slope_vehicle_filter(slope_vehicle_filter);
  builder_.add_vehicle_velocity(vehicle_velocity);
  builder_.add_full_load_status(full_load_status);
  builder_.add_back_obstacle_timeout_flag(back_obstacle_timeout_flag);
  builder_.add_front_obstacle_timeout_flag(front_obstacle_timeout_flag);
  builder_.add_status_switch_flag(status_switch_flag);
  return builder_.Finish();
}

flatbuffers::Offset<ControlOutputForDecision> CreateControlOutputForDecision(flatbuffers::FlatBufferBuilder &_fbb, const ControlOutputForDecisionT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline ControlOutputForImplementT *ControlOutputForImplement::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  std::unique_ptr<control::ControlOutputForImplementT> _o = std::unique_ptr<control::ControlOutputForImplementT>(new ControlOutputForImplementT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void ControlOutputForImplement::UnPackTo(ControlOutputForImplementT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = vehicle_velocity(); _o->vehicle_velocity = _e; }
  { auto _e = front_wheel_angle(); _o->front_wheel_angle = _e; }
  { auto _e = throttle_pedal(); _o->throttle_pedal = _e; }
  { auto _e = brake_pedal(); _o->brake_pedal = _e; }
  { auto _e = hydraulic_brake(); _o->hydraulic_brake = _e; }
  { auto _e = retarder_ctrl(); _o->retarder_ctrl = _e; }
  { auto _e = park_ctrl(); _o->park_ctrl = _e; }
  { auto _e = exhuast_brake(); _o->exhuast_brake = _e; }
  { auto _e = shift_ctrl(); _o->shift_ctrl = _e; }
  { auto _e = bucket_ctrl(); _o->bucket_ctrl = _e; }
  { auto _e = trumpet_ctrl(); _o->trumpet_ctrl = _e; }
  { auto _e = lamp_ctrl(); _o->lamp_ctrl = _e; }
  { auto _e = shift_limit(); _o->shift_limit = _e; }
  { auto _e = can_send_flag(); _o->can_send_flag = _e; }
  { auto _e = const_speed_downhill_enable(); _o->const_speed_downhill_enable = _e; }
  { auto _e = const_speed_downhill_speed(); _o->const_speed_downhill_speed = _e; }
}

inline flatbuffers::Offset<ControlOutputForImplement> ControlOutputForImplement::Pack(flatbuffers::FlatBufferBuilder &_fbb, const ControlOutputForImplementT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateControlOutputForImplement(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<ControlOutputForImplement> CreateControlOutputForImplement(flatbuffers::FlatBufferBuilder &_fbb, const ControlOutputForImplementT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const ControlOutputForImplementT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _vehicle_velocity = _o->vehicle_velocity;
  auto _front_wheel_angle = _o->front_wheel_angle;
  auto _throttle_pedal = _o->throttle_pedal;
  auto _brake_pedal = _o->brake_pedal;
  auto _hydraulic_brake = _o->hydraulic_brake;
  auto _retarder_ctrl = _o->retarder_ctrl;
  auto _park_ctrl = _o->park_ctrl;
  auto _exhuast_brake = _o->exhuast_brake;
  auto _shift_ctrl = _o->shift_ctrl;
  auto _bucket_ctrl = _o->bucket_ctrl;
  auto _trumpet_ctrl = _o->trumpet_ctrl;
  auto _lamp_ctrl = _o->lamp_ctrl;
  auto _shift_limit = _o->shift_limit;
  auto _can_send_flag = _o->can_send_flag;
  auto _const_speed_downhill_enable = _o->const_speed_downhill_enable;
  auto _const_speed_downhill_speed = _o->const_speed_downhill_speed;
  return control::CreateControlOutputForImplement(
      _fbb,
      _vehicle_velocity,
      _front_wheel_angle,
      _throttle_pedal,
      _brake_pedal,
      _hydraulic_brake,
      _retarder_ctrl,
      _park_ctrl,
      _exhuast_brake,
      _shift_ctrl,
      _bucket_ctrl,
      _trumpet_ctrl,
      _lamp_ctrl,
      _shift_limit,
      _can_send_flag,
      _const_speed_downhill_enable,
      _const_speed_downhill_speed);
}

inline ControlOutputForDecisionT *ControlOutputForDecision::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  std::unique_ptr<control::ControlOutputForDecisionT> _o = std::unique_ptr<control::ControlOutputForDecisionT>(new ControlOutputForDecisionT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void ControlOutputForDecision::UnPackTo(ControlOutputForDecisionT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = vehicle_velocity(); _o->vehicle_velocity = _e; }
  { auto _e = slope_vehicle_filter(); _o->slope_vehicle_filter = _e; }
  { auto _e = lat_distance(); _o->lat_distance = _e; }
  { auto _e = desired_speed(); _o->desired_speed = _e; }
  { auto _e = status_switch_flag(); _o->status_switch_flag = _e; }
  { auto _e = front_obstacle_timeout_flag(); _o->front_obstacle_timeout_flag = _e; }
  { auto _e = back_obstacle_timeout_flag(); _o->back_obstacle_timeout_flag = _e; }
  { auto _e = curve_max(); _o->curve_max = _e; }
  { auto _e = full_load_status(); _o->full_load_status = _e; }
}

inline flatbuffers::Offset<ControlOutputForDecision> ControlOutputForDecision::Pack(flatbuffers::FlatBufferBuilder &_fbb, const ControlOutputForDecisionT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateControlOutputForDecision(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<ControlOutputForDecision> CreateControlOutputForDecision(flatbuffers::FlatBufferBuilder &_fbb, const ControlOutputForDecisionT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const ControlOutputForDecisionT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _vehicle_velocity = _o->vehicle_velocity;
  auto _slope_vehicle_filter = _o->slope_vehicle_filter;
  auto _lat_distance = _o->lat_distance;
  auto _desired_speed = _o->desired_speed;
  auto _status_switch_flag = _o->status_switch_flag;
  auto _front_obstacle_timeout_flag = _o->front_obstacle_timeout_flag;
  auto _back_obstacle_timeout_flag = _o->back_obstacle_timeout_flag;
  auto _curve_max = _o->curve_max;
  auto _full_load_status = _o->full_load_status;
  return control::CreateControlOutputForDecision(
      _fbb,
      _vehicle_velocity,
      _slope_vehicle_filter,
      _lat_distance,
      _desired_speed,
      _status_switch_flag,
      _front_obstacle_timeout_flag,
      _back_obstacle_timeout_flag,
      _curve_max,
      _full_load_status);
}

inline const flatbuffers::TypeTable *ControlOutputForImplementTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_SHORT, 0, -1 },
    { flatbuffers::ET_SHORT, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 }
  };
  static const char * const names[] = {
    "vehicle_velocity",
    "front_wheel_angle",
    "throttle_pedal",
    "brake_pedal",
    "hydraulic_brake",
    "retarder_ctrl",
    "park_ctrl",
    "exhuast_brake",
    "shift_ctrl",
    "bucket_ctrl",
    "trumpet_ctrl",
    "lamp_ctrl",
    "shift_limit",
    "can_send_flag",
    "const_speed_downhill_enable",
    "const_speed_downhill_speed"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 16, type_codes, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *ControlOutputForDecisionTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_FLOAT, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 }
  };
  static const char * const names[] = {
    "vehicle_velocity",
    "slope_vehicle_filter",
    "lat_distance",
    "desired_speed",
    "status_switch_flag",
    "front_obstacle_timeout_flag",
    "back_obstacle_timeout_flag",
    "curve_max",
    "full_load_status"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 9, type_codes, nullptr, nullptr, names
  };
  return &tt;
}

inline const control::ControlOutputForImplement *GetControlOutputForImplement(const void *buf) {
  return flatbuffers::GetRoot<control::ControlOutputForImplement>(buf);
}

inline const control::ControlOutputForImplement *GetSizePrefixedControlOutputForImplement(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<control::ControlOutputForImplement>(buf);
}

inline ControlOutputForImplement *GetMutableControlOutputForImplement(void *buf) {
  return flatbuffers::GetMutableRoot<ControlOutputForImplement>(buf);
}

inline const char *ControlOutputForImplementIdentifier() {
  return "CTIO";
}

inline bool ControlOutputForImplementBufferHasIdentifier(const void *buf) {
  return flatbuffers::BufferHasIdentifier(
      buf, ControlOutputForImplementIdentifier());
}

inline bool VerifyControlOutputForImplementBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<control::ControlOutputForImplement>(ControlOutputForImplementIdentifier());
}

inline bool VerifySizePrefixedControlOutputForImplementBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<control::ControlOutputForImplement>(ControlOutputForImplementIdentifier());
}

inline void FinishControlOutputForImplementBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<control::ControlOutputForImplement> root) {
  fbb.Finish(root, ControlOutputForImplementIdentifier());
}

inline void FinishSizePrefixedControlOutputForImplementBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<control::ControlOutputForImplement> root) {
  fbb.FinishSizePrefixed(root, ControlOutputForImplementIdentifier());
}

inline std::unique_ptr<control::ControlOutputForImplementT> UnPackControlOutputForImplement(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<control::ControlOutputForImplementT>(GetControlOutputForImplement(buf)->UnPack(res));
}

inline std::unique_ptr<control::ControlOutputForImplementT> UnPackSizePrefixedControlOutputForImplement(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<control::ControlOutputForImplementT>(GetSizePrefixedControlOutputForImplement(buf)->UnPack(res));
}

}  // namespace control

#endif  // FLATBUFFERS_GENERATED_CONTROL_CONTROL_H_
