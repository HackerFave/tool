/* datax_global.cpp - DataX global resources configuration */
/*
 * Copyright (c) 2020
 * TAGE IDRIVER Co.,Ltd.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable TAGE IDRIVER license agreement.
 */

/*
modification history
--------------------
31aug20,jlj  written
*/

#include <unordered_map>
#include "datax_global.h"

namespace datax {

DataxGlobal::DataxGlobal() {
#ifdef __VXWORKS__
  channel_port_num_ = 50010;
#endif
}

DataxGlobal::~DataxGlobal() { DataxGlobal::Instance()->ZmqContext().close(); }

zmq::context_t& DataxGlobal::ZmqContext() { return zmq_context_; }

#ifdef __VXWORKS__
std::unordered_map<std::string, std::string>& DataxGlobal::NodeChannelMap() {
  return node_channel_map_;
}

unsigned long DataxGlobal::ChannelPortNum() {
  channel_port_num_.fetch_add(1, std::memory_order_relaxed);
  return channel_port_num_;
}

// This routine is used to retrieve a TCP based channel name from a unix channel name.
// It is not thread-safe, that is user shall take care of the internal map data
// when calling this routine.
// Return value:
// A valid channel string if the query is success, or a nullptr if fail.
std::string DataxGlobal::ChannelNameGet(std::string unix_name) {
  const auto it = node_channel_map_.find(unix_name);
  if (it != node_channel_map_.end()) {
    return it->second;
  }
  return nullptr;
}

void  DataxGlobal::SaveChannelNameToFile(std::fstream &fs){
	for(auto it : node_channel_map_){
		fs<<it.first<<"--->"<<it.second<<'\n';
	}
	fs.close();
}
std::string DataxGlobal::DiaGetChannelNameFromFile(std::string channel_name)
{
	static bool get_map_flag = false;
  	static std::unordered_map<std::string, std::string> channel_map_;
	if(!get_map_flag){
		std::ifstream channel_file(CHANNEL_FILE_NAME);
		if(channel_file.is_open()){
			while(!channel_file.eof()){ //read file channel to map
				std::string line;
				std::getline(channel_file,line);	
				if( line.size() < 5 )
					continue;
				std::string key(line,0,line.find("--->"));
				std::string value(line,line.find("tcp://"),line.size()-line.find("tcp://"));	
				channel_map_.emplace(key,value);
			}
			get_map_flag = true;
		}	
		channel_file.close();
	}
	if(get_map_flag){
#if 0		
	for(auto it : channel_map_)
		std::cout <<"+++"<<it.first<<"--->"<<it.second<<'\n';
	printf("%s--->%d\n",__func__,__LINE__);
#endif
  	auto it = channel_map_.find(channel_name);
  		if (it != channel_map_.end()) {
    		return it->second;
  		}	

	}
	return "error";
}

#endif

}  // namespace datax
