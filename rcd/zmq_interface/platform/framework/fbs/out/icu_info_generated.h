// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ICUINFO_ICU_H_
#define FLATBUFFERS_GENERATED_ICUINFO_ICU_H_

#include "flatbuffers/flatbuffers.h"

namespace icu {

struct MsgIcuVersion;
struct MsgIcuVersionBuilder;
struct MsgIcuVersionT;

bool operator==(const MsgIcuVersionT &lhs, const MsgIcuVersionT &rhs);
bool operator!=(const MsgIcuVersionT &lhs, const MsgIcuVersionT &rhs);

inline const flatbuffers::TypeTable *MsgIcuVersionTypeTable();

struct MsgIcuVersionT : public flatbuffers::NativeTable {
  typedef MsgIcuVersion TableType;
  uint8_t product_type;
  uint8_t vehicle_type;
  uint8_t module_type;
  uint8_t func_vary_version;
  uint8_t func_perf_version;
  uint8_t bug_fix_version;
  MsgIcuVersionT()
      : product_type(0),
        vehicle_type(0),
        module_type(0),
        func_vary_version(0),
        func_perf_version(0),
        bug_fix_version(0) {
  }
};

inline bool operator==(const MsgIcuVersionT &lhs, const MsgIcuVersionT &rhs) {
  return
      (lhs.product_type == rhs.product_type) &&
      (lhs.vehicle_type == rhs.vehicle_type) &&
      (lhs.module_type == rhs.module_type) &&
      (lhs.func_vary_version == rhs.func_vary_version) &&
      (lhs.func_perf_version == rhs.func_perf_version) &&
      (lhs.bug_fix_version == rhs.bug_fix_version);
}

inline bool operator!=(const MsgIcuVersionT &lhs, const MsgIcuVersionT &rhs) {
    return !(lhs == rhs);
}


struct MsgIcuVersion FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MsgIcuVersionT NativeTableType;
  typedef MsgIcuVersionBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return MsgIcuVersionTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PRODUCT_TYPE = 4,
    VT_VEHICLE_TYPE = 6,
    VT_MODULE_TYPE = 8,
    VT_FUNC_VARY_VERSION = 10,
    VT_FUNC_PERF_VERSION = 12,
    VT_BUG_FIX_VERSION = 14
  };
  uint8_t product_type() const {
    return GetField<uint8_t>(VT_PRODUCT_TYPE, 0);
  }
  bool mutate_product_type(uint8_t _product_type) {
    return SetField<uint8_t>(VT_PRODUCT_TYPE, _product_type, 0);
  }
  uint8_t vehicle_type() const {
    return GetField<uint8_t>(VT_VEHICLE_TYPE, 0);
  }
  bool mutate_vehicle_type(uint8_t _vehicle_type) {
    return SetField<uint8_t>(VT_VEHICLE_TYPE, _vehicle_type, 0);
  }
  uint8_t module_type() const {
    return GetField<uint8_t>(VT_MODULE_TYPE, 0);
  }
  bool mutate_module_type(uint8_t _module_type) {
    return SetField<uint8_t>(VT_MODULE_TYPE, _module_type, 0);
  }
  uint8_t func_vary_version() const {
    return GetField<uint8_t>(VT_FUNC_VARY_VERSION, 0);
  }
  bool mutate_func_vary_version(uint8_t _func_vary_version) {
    return SetField<uint8_t>(VT_FUNC_VARY_VERSION, _func_vary_version, 0);
  }
  uint8_t func_perf_version() const {
    return GetField<uint8_t>(VT_FUNC_PERF_VERSION, 0);
  }
  bool mutate_func_perf_version(uint8_t _func_perf_version) {
    return SetField<uint8_t>(VT_FUNC_PERF_VERSION, _func_perf_version, 0);
  }
  uint8_t bug_fix_version() const {
    return GetField<uint8_t>(VT_BUG_FIX_VERSION, 0);
  }
  bool mutate_bug_fix_version(uint8_t _bug_fix_version) {
    return SetField<uint8_t>(VT_BUG_FIX_VERSION, _bug_fix_version, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_PRODUCT_TYPE) &&
           VerifyField<uint8_t>(verifier, VT_VEHICLE_TYPE) &&
           VerifyField<uint8_t>(verifier, VT_MODULE_TYPE) &&
           VerifyField<uint8_t>(verifier, VT_FUNC_VARY_VERSION) &&
           VerifyField<uint8_t>(verifier, VT_FUNC_PERF_VERSION) &&
           VerifyField<uint8_t>(verifier, VT_BUG_FIX_VERSION) &&
           verifier.EndTable();
  }
  MsgIcuVersionT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(MsgIcuVersionT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<MsgIcuVersion> Pack(flatbuffers::FlatBufferBuilder &_fbb, const MsgIcuVersionT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct MsgIcuVersionBuilder {
  typedef MsgIcuVersion Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_product_type(uint8_t product_type) {
    fbb_.AddElement<uint8_t>(MsgIcuVersion::VT_PRODUCT_TYPE, product_type, 0);
  }
  void add_vehicle_type(uint8_t vehicle_type) {
    fbb_.AddElement<uint8_t>(MsgIcuVersion::VT_VEHICLE_TYPE, vehicle_type, 0);
  }
  void add_module_type(uint8_t module_type) {
    fbb_.AddElement<uint8_t>(MsgIcuVersion::VT_MODULE_TYPE, module_type, 0);
  }
  void add_func_vary_version(uint8_t func_vary_version) {
    fbb_.AddElement<uint8_t>(MsgIcuVersion::VT_FUNC_VARY_VERSION, func_vary_version, 0);
  }
  void add_func_perf_version(uint8_t func_perf_version) {
    fbb_.AddElement<uint8_t>(MsgIcuVersion::VT_FUNC_PERF_VERSION, func_perf_version, 0);
  }
  void add_bug_fix_version(uint8_t bug_fix_version) {
    fbb_.AddElement<uint8_t>(MsgIcuVersion::VT_BUG_FIX_VERSION, bug_fix_version, 0);
  }
  explicit MsgIcuVersionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MsgIcuVersionBuilder &operator=(const MsgIcuVersionBuilder &);
  flatbuffers::Offset<MsgIcuVersion> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MsgIcuVersion>(end);
    return o;
  }
};

inline flatbuffers::Offset<MsgIcuVersion> CreateMsgIcuVersion(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t product_type = 0,
    uint8_t vehicle_type = 0,
    uint8_t module_type = 0,
    uint8_t func_vary_version = 0,
    uint8_t func_perf_version = 0,
    uint8_t bug_fix_version = 0) {
  MsgIcuVersionBuilder builder_(_fbb);
  builder_.add_bug_fix_version(bug_fix_version);
  builder_.add_func_perf_version(func_perf_version);
  builder_.add_func_vary_version(func_vary_version);
  builder_.add_module_type(module_type);
  builder_.add_vehicle_type(vehicle_type);
  builder_.add_product_type(product_type);
  return builder_.Finish();
}

flatbuffers::Offset<MsgIcuVersion> CreateMsgIcuVersion(flatbuffers::FlatBufferBuilder &_fbb, const MsgIcuVersionT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline MsgIcuVersionT *MsgIcuVersion::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  std::unique_ptr<icu::MsgIcuVersionT> _o = std::unique_ptr<icu::MsgIcuVersionT>(new MsgIcuVersionT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void MsgIcuVersion::UnPackTo(MsgIcuVersionT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = product_type(); _o->product_type = _e; }
  { auto _e = vehicle_type(); _o->vehicle_type = _e; }
  { auto _e = module_type(); _o->module_type = _e; }
  { auto _e = func_vary_version(); _o->func_vary_version = _e; }
  { auto _e = func_perf_version(); _o->func_perf_version = _e; }
  { auto _e = bug_fix_version(); _o->bug_fix_version = _e; }
}

inline flatbuffers::Offset<MsgIcuVersion> MsgIcuVersion::Pack(flatbuffers::FlatBufferBuilder &_fbb, const MsgIcuVersionT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateMsgIcuVersion(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<MsgIcuVersion> CreateMsgIcuVersion(flatbuffers::FlatBufferBuilder &_fbb, const MsgIcuVersionT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const MsgIcuVersionT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _product_type = _o->product_type;
  auto _vehicle_type = _o->vehicle_type;
  auto _module_type = _o->module_type;
  auto _func_vary_version = _o->func_vary_version;
  auto _func_perf_version = _o->func_perf_version;
  auto _bug_fix_version = _o->bug_fix_version;
  return icu::CreateMsgIcuVersion(
      _fbb,
      _product_type,
      _vehicle_type,
      _module_type,
      _func_vary_version,
      _func_perf_version,
      _bug_fix_version);
}

inline const flatbuffers::TypeTable *MsgIcuVersionTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 },
    { flatbuffers::ET_UCHAR, 0, -1 }
  };
  static const char * const names[] = {
    "product_type",
    "vehicle_type",
    "module_type",
    "func_vary_version",
    "func_perf_version",
    "bug_fix_version"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 6, type_codes, nullptr, nullptr, names
  };
  return &tt;
}

}  // namespace icu

#endif  // FLATBUFFERS_GENERATED_ICUINFO_ICU_H_
