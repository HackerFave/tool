// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _FASTRTPS_TYPES_HandleDynamicData_HPP_
#define _FASTRTPS_TYPES_HandleDynamicData_HPP_

#include <fastrtps/types/AnnotationDescriptor.h>
#include <fastrtps/types/TypeDescriptor.h>
#include <fastrtps/types/DynamicType.h>
#include <fastrtps/types/DynamicData.h>
#include <fastrtps/types/DynamicDataPtr.h>
#include <fastrtps/types/DynamicTypeMember.h>

#include <iostream>
#include <functional>

namespace eprosima {
namespace fastrtps {
namespace types {

//using namespace eprosima::fastrtps::types;
//using eprosima::fastrtps::types::MemberId;
//using eprosima::fastrtps::types::TypeKind;
//using eprosima::fastrtps::types::DynamicData;
//using eprosima::fastrtps::types::DynamicType;

class HandleDynamicData
{
public:
    HandleDynamicData();
    virtual ~HandleDynamicData();
    void SetDynamicData(const std::string &topic, const DynamicData* dynamic_data, eprosima::fastrtps::types::DynamicType* type);
    void GetJsonString(std::string& json_string);

private:
    const DynamicData* data_;
    eprosima::fastrtps::types::DynamicType* type_;
    std::string json_string_;
    std::string string_;
    std::string topic_name_;

protected:
    void Print();
    void print_basic_element(
            DynamicData* data,
            MemberId id,
            TypeKind kind);

    void print_collection(
            DynamicData* data,
            DynamicType* type,
            const std::string& tabs = "");

    void fill_array_positions(
            const std::vector<uint32_t>& bounds,
            std::vector<std::vector<uint32_t>>& positions);

    void get_index_position(
            uint32_t index,
            const std::vector<uint32_t>& bounds,
            std::vector<uint32_t>& position);

    void aux_index_position(
            uint32_t index,
            uint32_t inner_index,
            const std::vector<uint32_t>& bounds,
            std::vector<uint32_t>& position);

    void print_basic_collection(
            DynamicData* data, DynamicType* type);

    void print_complex_collection(
            DynamicData* data,
            DynamicType* type,
            const std::string& tabs = "");

    void print_complex_element(
            DynamicData* data,
            DynamicType* type,
            MemberId id,
            const std::string& tabs = "");

    void print_member(
            DynamicData* data,
            const DynamicTypeMember* type,
            const std::string& tabs = "");
};


} // namespace types
} // namespace fastrtps
} // namespace eprosima

#endif // _FASTRTPS_TYPES_HandleDynamicData_HPP_
