#ifndef DATAX_GLOBAL_H_
#define DATAX_GLOBAL_H_

#include <atomic>
#include "macros.h"
#include "zmq_addon.hpp"

namespace datax {
#define CHANNEL_FILE_NAME "channel.txt"
class DataxGlobal {
 public:
  ~DataxGlobal();
  zmq::context_t& ZmqContext();

#ifdef __VXWORKS__
  std::unordered_map<std::string, std::string>& NodeChannelMap();
  unsigned long ChannelPortNum();

  std::string ChannelNameGet(std::string unix_name);
  void SaveChannelNameToFile(std::fstream& fs);
  std::string DiaGetChannelNameFromFile(std::string channel_name);
#endif

 private:
  zmq::context_t zmq_context_;

#ifdef __VXWORKS__
  std::unordered_map<std::string, std::string> node_channel_map_;
  std::atomic_ulong channel_port_num_;

#endif

  DECLARE_SINGLETON(DataxGlobal);
};

}  // namespace datax
#endif  // DATAX_GLOBAL_H_
