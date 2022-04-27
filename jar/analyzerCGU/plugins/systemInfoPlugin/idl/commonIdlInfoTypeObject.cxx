// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/*!
 * @file commonIdlInfoTypeObject.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace { char dummy; }
#endif

#include "commonIdlInfo.h"
#include "commonIdlInfoTypeObject.h"
#include <utility>
#include <sstream>
#include <fastrtps/rtps/common/SerializedPayload.h>
#include <fastrtps/utils/md5.h>
#include <fastrtps/types/TypeObjectFactory.h>
#include <fastrtps/types/TypeNamesGenerator.h>
#include <fastrtps/types/AnnotationParameterValue.h>
#include <fastcdr/FastBuffer.h>
#include <fastcdr/Cdr.h>

using namespace eprosima::fastrtps::rtps;

void registercommonIdlInfoTypes()
{
    TypeObjectFactory *factory = TypeObjectFactory::get_instance();
}

const TypeIdentifier* GetBoxTypeIdentifier(bool complete)
{
    const TypeIdentifier* c_identifier = TypeObjectFactory::get_instance()->get_type_identifier("BoxType", complete);
    if (c_identifier != nullptr && (!complete || c_identifier->_d() == EK_COMPLETE))
    {
        return c_identifier;
    }

    GetBoxTypeObject(complete); // Generated inside
    return TypeObjectFactory::get_instance()->get_type_identifier("BoxType", complete);
}

const TypeObject* GetBoxTypeObject(bool complete)
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("BoxType", complete);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }
    else if (complete)
    {
        return GetCompleteBoxTypeObject();
    }
    // else
    return GetMinimalBoxTypeObject();
}

const TypeObject* GetMinimalBoxTypeObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("BoxType", false);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_MINIMAL);
    type_object->minimal()._d(TK_ENUM);

    // No flags apply
    //type_object->minimal().enumerated_type().enum_flags().IS_FINAL(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_APPENDABLE(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_MUTABLE(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_NESTED(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_AUTOID_HASH(false);

    type_object->minimal().enumerated_type().header().common().bit_bound(32); // TODO fixed by IDL, isn't?

    uint32_t value = 0;
    MinimalEnumeratedLiteral mel_ENUM_RESERVE;
    mel_ENUM_RESERVE.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_ENUM_RESERVE.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_ENUM_RESERVE.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_ENUM_RESERVE.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_ENUM_RESERVE.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_ENUM_RESERVE.common().flags().IS_KEY(false); // Doesn't apply
    mel_ENUM_RESERVE.common().flags().IS_DEFAULT(false);
    mel_ENUM_RESERVE.common().value(value++);
    MD5 ENUM_RESERVE_hash("ENUM_RESERVE");
    for(int i = 0; i < 4; ++i)
    {
        mel_ENUM_RESERVE.detail().name_hash()[i] = ENUM_RESERVE_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_ENUM_RESERVE);

    MinimalEnumeratedLiteral mel_ENUM_MASTER_BOX;
    mel_ENUM_MASTER_BOX.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_ENUM_MASTER_BOX.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_ENUM_MASTER_BOX.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_ENUM_MASTER_BOX.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_ENUM_MASTER_BOX.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_ENUM_MASTER_BOX.common().flags().IS_KEY(false); // Doesn't apply
    mel_ENUM_MASTER_BOX.common().flags().IS_DEFAULT(false);
    mel_ENUM_MASTER_BOX.common().value(value++);
    MD5 ENUM_MASTER_BOX_hash("ENUM_MASTER_BOX");
    for(int i = 0; i < 4; ++i)
    {
        mel_ENUM_MASTER_BOX.detail().name_hash()[i] = ENUM_MASTER_BOX_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_ENUM_MASTER_BOX);

    MinimalEnumeratedLiteral mel_ENUM_SLAVE_BOX;
    mel_ENUM_SLAVE_BOX.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_ENUM_SLAVE_BOX.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_ENUM_SLAVE_BOX.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_ENUM_SLAVE_BOX.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_ENUM_SLAVE_BOX.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_ENUM_SLAVE_BOX.common().flags().IS_KEY(false); // Doesn't apply
    mel_ENUM_SLAVE_BOX.common().flags().IS_DEFAULT(false);
    mel_ENUM_SLAVE_BOX.common().value(value++);
    MD5 ENUM_SLAVE_BOX_hash("ENUM_SLAVE_BOX");
    for(int i = 0; i < 4; ++i)
    {
        mel_ENUM_SLAVE_BOX.detail().name_hash()[i] = ENUM_SLAVE_BOX_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_ENUM_SLAVE_BOX);


    TypeIdentifier identifier;
    identifier._d(EK_MINIMAL);

    SerializedPayload_t payload(static_cast<uint32_t>(
        MinimalEnumeratedType::getCdrSerializedSize(type_object->minimal().enumerated_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("BoxType", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("BoxType", false);
}

const TypeObject* GetCompleteBoxTypeObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("BoxType", true);
    if (c_type_object != nullptr && c_type_object->_d() == EK_COMPLETE)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_COMPLETE);
    type_object->complete()._d(TK_ENUM);

    // No flags apply
    //type_object->complete().enumerated_type().enum_flags().IS_FINAL(false);
    //type_object->complete().enumerated_type().enum_flags().IS_APPENDABLE(false);
    //type_object->complete().enumerated_type().enum_flags().IS_MUTABLE(false);
    //type_object->complete().enumerated_type().enum_flags().IS_NESTED(false);
    //type_object->complete().enumerated_type().enum_flags().IS_AUTOID_HASH(false);

    type_object->complete().enumerated_type().header().common().bit_bound(32); // TODO fixed by IDL, isn't?
    type_object->complete().enumerated_type().header().detail().type_name("BoxType");


    uint32_t value = 0;
    CompleteEnumeratedLiteral cel_ENUM_RESERVE;
    cel_ENUM_RESERVE.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_ENUM_RESERVE.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_ENUM_RESERVE.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_ENUM_RESERVE.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_ENUM_RESERVE.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_ENUM_RESERVE.common().flags().IS_KEY(false); // Doesn't apply
    cel_ENUM_RESERVE.common().flags().IS_DEFAULT(false);
    cel_ENUM_RESERVE.common().value(value++);
    cel_ENUM_RESERVE.detail().name("ENUM_RESERVE");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_ENUM_RESERVE);

    CompleteEnumeratedLiteral cel_ENUM_MASTER_BOX;
    cel_ENUM_MASTER_BOX.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_ENUM_MASTER_BOX.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_ENUM_MASTER_BOX.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_ENUM_MASTER_BOX.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_ENUM_MASTER_BOX.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_ENUM_MASTER_BOX.common().flags().IS_KEY(false); // Doesn't apply
    cel_ENUM_MASTER_BOX.common().flags().IS_DEFAULT(false);
    cel_ENUM_MASTER_BOX.common().value(value++);
    cel_ENUM_MASTER_BOX.detail().name("ENUM_MASTER_BOX");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_ENUM_MASTER_BOX);

    CompleteEnumeratedLiteral cel_ENUM_SLAVE_BOX;
    cel_ENUM_SLAVE_BOX.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_ENUM_SLAVE_BOX.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_ENUM_SLAVE_BOX.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_ENUM_SLAVE_BOX.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_ENUM_SLAVE_BOX.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_ENUM_SLAVE_BOX.common().flags().IS_KEY(false); // Doesn't apply
    cel_ENUM_SLAVE_BOX.common().flags().IS_DEFAULT(false);
    cel_ENUM_SLAVE_BOX.common().value(value++);
    cel_ENUM_SLAVE_BOX.detail().name("ENUM_SLAVE_BOX");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_ENUM_SLAVE_BOX);


    TypeIdentifier identifier;
    identifier._d(EK_COMPLETE);

    SerializedPayload_t payload(static_cast<uint32_t>(
        CompleteEnumeratedType::getCdrSerializedSize(type_object->complete().enumerated_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("BoxType", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("BoxType", true);
}

const TypeIdentifier* GetLinkStatusIdentifier(bool complete)
{
    const TypeIdentifier* c_identifier = TypeObjectFactory::get_instance()->get_type_identifier("LinkStatus", complete);
    if (c_identifier != nullptr && (!complete || c_identifier->_d() == EK_COMPLETE))
    {
        return c_identifier;
    }

    GetLinkStatusObject(complete); // Generated inside
    return TypeObjectFactory::get_instance()->get_type_identifier("LinkStatus", complete);
}

const TypeObject* GetLinkStatusObject(bool complete)
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("LinkStatus", complete);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }
    else if (complete)
    {
        return GetCompleteLinkStatusObject();
    }
    // else
    return GetMinimalLinkStatusObject();
}

const TypeObject* GetMinimalLinkStatusObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("LinkStatus", false);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_MINIMAL);
    type_object->minimal()._d(TK_ENUM);

    // No flags apply
    //type_object->minimal().enumerated_type().enum_flags().IS_FINAL(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_APPENDABLE(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_MUTABLE(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_NESTED(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_AUTOID_HASH(false);

    type_object->minimal().enumerated_type().header().common().bit_bound(32); // TODO fixed by IDL, isn't?

    uint32_t value = 0;
    MinimalEnumeratedLiteral mel_ENUM_CONNECT;
    mel_ENUM_CONNECT.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_ENUM_CONNECT.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_ENUM_CONNECT.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_ENUM_CONNECT.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_ENUM_CONNECT.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_ENUM_CONNECT.common().flags().IS_KEY(false); // Doesn't apply
    mel_ENUM_CONNECT.common().flags().IS_DEFAULT(false);
    mel_ENUM_CONNECT.common().value(value++);
    MD5 ENUM_CONNECT_hash("ENUM_CONNECT");
    for(int i = 0; i < 4; ++i)
    {
        mel_ENUM_CONNECT.detail().name_hash()[i] = ENUM_CONNECT_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_ENUM_CONNECT);

    MinimalEnumeratedLiteral mel_ENUM_DISCONNECT;
    mel_ENUM_DISCONNECT.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_ENUM_DISCONNECT.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_ENUM_DISCONNECT.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_ENUM_DISCONNECT.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_ENUM_DISCONNECT.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_ENUM_DISCONNECT.common().flags().IS_KEY(false); // Doesn't apply
    mel_ENUM_DISCONNECT.common().flags().IS_DEFAULT(false);
    mel_ENUM_DISCONNECT.common().value(value++);
    MD5 ENUM_DISCONNECT_hash("ENUM_DISCONNECT");
    for(int i = 0; i < 4; ++i)
    {
        mel_ENUM_DISCONNECT.detail().name_hash()[i] = ENUM_DISCONNECT_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_ENUM_DISCONNECT);


    TypeIdentifier identifier;
    identifier._d(EK_MINIMAL);

    SerializedPayload_t payload(static_cast<uint32_t>(
        MinimalEnumeratedType::getCdrSerializedSize(type_object->minimal().enumerated_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("LinkStatus", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("LinkStatus", false);
}

const TypeObject* GetCompleteLinkStatusObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("LinkStatus", true);
    if (c_type_object != nullptr && c_type_object->_d() == EK_COMPLETE)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_COMPLETE);
    type_object->complete()._d(TK_ENUM);

    // No flags apply
    //type_object->complete().enumerated_type().enum_flags().IS_FINAL(false);
    //type_object->complete().enumerated_type().enum_flags().IS_APPENDABLE(false);
    //type_object->complete().enumerated_type().enum_flags().IS_MUTABLE(false);
    //type_object->complete().enumerated_type().enum_flags().IS_NESTED(false);
    //type_object->complete().enumerated_type().enum_flags().IS_AUTOID_HASH(false);

    type_object->complete().enumerated_type().header().common().bit_bound(32); // TODO fixed by IDL, isn't?
    type_object->complete().enumerated_type().header().detail().type_name("LinkStatus");


    uint32_t value = 0;
    CompleteEnumeratedLiteral cel_ENUM_CONNECT;
    cel_ENUM_CONNECT.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_ENUM_CONNECT.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_ENUM_CONNECT.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_ENUM_CONNECT.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_ENUM_CONNECT.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_ENUM_CONNECT.common().flags().IS_KEY(false); // Doesn't apply
    cel_ENUM_CONNECT.common().flags().IS_DEFAULT(false);
    cel_ENUM_CONNECT.common().value(value++);
    cel_ENUM_CONNECT.detail().name("ENUM_CONNECT");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_ENUM_CONNECT);

    CompleteEnumeratedLiteral cel_ENUM_DISCONNECT;
    cel_ENUM_DISCONNECT.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_ENUM_DISCONNECT.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_ENUM_DISCONNECT.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_ENUM_DISCONNECT.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_ENUM_DISCONNECT.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_ENUM_DISCONNECT.common().flags().IS_KEY(false); // Doesn't apply
    cel_ENUM_DISCONNECT.common().flags().IS_DEFAULT(false);
    cel_ENUM_DISCONNECT.common().value(value++);
    cel_ENUM_DISCONNECT.detail().name("ENUM_DISCONNECT");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_ENUM_DISCONNECT);


    TypeIdentifier identifier;
    identifier._d(EK_COMPLETE);

    SerializedPayload_t payload(static_cast<uint32_t>(
        CompleteEnumeratedType::getCdrSerializedSize(type_object->complete().enumerated_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("LinkStatus", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("LinkStatus", true);
}

const TypeIdentifier* GetmoudleTypeIdentifier(bool complete)
{
    const TypeIdentifier* c_identifier = TypeObjectFactory::get_instance()->get_type_identifier("moudleType", complete);
    if (c_identifier != nullptr && (!complete || c_identifier->_d() == EK_COMPLETE))
    {
        return c_identifier;
    }

    GetmoudleTypeObject(complete); // Generated inside
    return TypeObjectFactory::get_instance()->get_type_identifier("moudleType", complete);
}

const TypeObject* GetmoudleTypeObject(bool complete)
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("moudleType", complete);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }
    else if (complete)
    {
        return GetCompletemoudleTypeObject();
    }
    // else
    return GetMinimalmoudleTypeObject();
}

const TypeObject* GetMinimalmoudleTypeObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("moudleType", false);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_MINIMAL);
    type_object->minimal()._d(TK_ENUM);

    // No flags apply
    //type_object->minimal().enumerated_type().enum_flags().IS_FINAL(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_APPENDABLE(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_MUTABLE(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_NESTED(false);
    //type_object->minimal().enumerated_type().enum_flags().IS_AUTOID_HASH(false);

    type_object->minimal().enumerated_type().header().common().bit_bound(32); // TODO fixed by IDL, isn't?

    uint32_t value = 0;
    MinimalEnumeratedLiteral mel_UnknownModule;
    mel_UnknownModule.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_UnknownModule.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_UnknownModule.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_UnknownModule.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_UnknownModule.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_UnknownModule.common().flags().IS_KEY(false); // Doesn't apply
    mel_UnknownModule.common().flags().IS_DEFAULT(false);
    mel_UnknownModule.common().value(value++);
    MD5 UnknownModule_hash("UnknownModule");
    for(int i = 0; i < 4; ++i)
    {
        mel_UnknownModule.detail().name_hash()[i] = UnknownModule_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_UnknownModule);

    MinimalEnumeratedLiteral mel_CCU;
    mel_CCU.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_CCU.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_CCU.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_CCU.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_CCU.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_CCU.common().flags().IS_KEY(false); // Doesn't apply
    mel_CCU.common().flags().IS_DEFAULT(false);
    mel_CCU.common().value(value++);
    MD5 CCU_hash("CCU");
    for(int i = 0; i < 4; ++i)
    {
        mel_CCU.detail().name_hash()[i] = CCU_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_CCU);

    MinimalEnumeratedLiteral mel_CGU;
    mel_CGU.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_CGU.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_CGU.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_CGU.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_CGU.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_CGU.common().flags().IS_KEY(false); // Doesn't apply
    mel_CGU.common().flags().IS_DEFAULT(false);
    mel_CGU.common().value(value++);
    MD5 CGU_hash("CGU");
    for(int i = 0; i < 4; ++i)
    {
        mel_CGU.detail().name_hash()[i] = CGU_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_CGU);

    MinimalEnumeratedLiteral mel_CGU_TBOX;
    mel_CGU_TBOX.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_CGU_TBOX.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_CGU_TBOX.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_CGU_TBOX.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_CGU_TBOX.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_CGU_TBOX.common().flags().IS_KEY(false); // Doesn't apply
    mel_CGU_TBOX.common().flags().IS_DEFAULT(false);
    mel_CGU_TBOX.common().value(value++);
    MD5 CGU_TBOX_hash("CGU_TBOX");
    for(int i = 0; i < 4; ++i)
    {
        mel_CGU_TBOX.detail().name_hash()[i] = CGU_TBOX_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_CGU_TBOX);

    MinimalEnumeratedLiteral mel_HMI;
    mel_HMI.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_HMI.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_HMI.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_HMI.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_HMI.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_HMI.common().flags().IS_KEY(false); // Doesn't apply
    mel_HMI.common().flags().IS_DEFAULT(false);
    mel_HMI.common().value(value++);
    MD5 HMI_hash("HMI");
    for(int i = 0; i < 4; ++i)
    {
        mel_HMI.detail().name_hash()[i] = HMI_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_HMI);

    MinimalEnumeratedLiteral mel_ICU;
    mel_ICU.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    mel_ICU.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    mel_ICU.common().flags().IS_EXTERNAL(false); // Doesn't apply
    mel_ICU.common().flags().IS_OPTIONAL(false); // Doesn't apply
    mel_ICU.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    mel_ICU.common().flags().IS_KEY(false); // Doesn't apply
    mel_ICU.common().flags().IS_DEFAULT(false);
    mel_ICU.common().value(value++);
    MD5 ICU_hash("ICU");
    for(int i = 0; i < 4; ++i)
    {
        mel_ICU.detail().name_hash()[i] = ICU_hash.digest[i];
    }
    type_object->minimal().enumerated_type().literal_seq().emplace_back(mel_ICU);


    TypeIdentifier identifier;
    identifier._d(EK_MINIMAL);

    SerializedPayload_t payload(static_cast<uint32_t>(
        MinimalEnumeratedType::getCdrSerializedSize(type_object->minimal().enumerated_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("moudleType", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("moudleType", false);
}

const TypeObject* GetCompletemoudleTypeObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("moudleType", true);
    if (c_type_object != nullptr && c_type_object->_d() == EK_COMPLETE)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_COMPLETE);
    type_object->complete()._d(TK_ENUM);

    // No flags apply
    //type_object->complete().enumerated_type().enum_flags().IS_FINAL(false);
    //type_object->complete().enumerated_type().enum_flags().IS_APPENDABLE(false);
    //type_object->complete().enumerated_type().enum_flags().IS_MUTABLE(false);
    //type_object->complete().enumerated_type().enum_flags().IS_NESTED(false);
    //type_object->complete().enumerated_type().enum_flags().IS_AUTOID_HASH(false);

    type_object->complete().enumerated_type().header().common().bit_bound(32); // TODO fixed by IDL, isn't?
    type_object->complete().enumerated_type().header().detail().type_name("moudleType");


    uint32_t value = 0;
    CompleteEnumeratedLiteral cel_UnknownModule;
    cel_UnknownModule.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_UnknownModule.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_UnknownModule.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_UnknownModule.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_UnknownModule.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_UnknownModule.common().flags().IS_KEY(false); // Doesn't apply
    cel_UnknownModule.common().flags().IS_DEFAULT(false);
    cel_UnknownModule.common().value(value++);
    cel_UnknownModule.detail().name("UnknownModule");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_UnknownModule);

    CompleteEnumeratedLiteral cel_CCU;
    cel_CCU.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_CCU.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_CCU.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_CCU.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_CCU.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_CCU.common().flags().IS_KEY(false); // Doesn't apply
    cel_CCU.common().flags().IS_DEFAULT(false);
    cel_CCU.common().value(value++);
    cel_CCU.detail().name("CCU");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_CCU);

    CompleteEnumeratedLiteral cel_CGU;
    cel_CGU.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_CGU.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_CGU.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_CGU.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_CGU.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_CGU.common().flags().IS_KEY(false); // Doesn't apply
    cel_CGU.common().flags().IS_DEFAULT(false);
    cel_CGU.common().value(value++);
    cel_CGU.detail().name("CGU");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_CGU);

    CompleteEnumeratedLiteral cel_CGU_TBOX;
    cel_CGU_TBOX.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_CGU_TBOX.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_CGU_TBOX.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_CGU_TBOX.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_CGU_TBOX.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_CGU_TBOX.common().flags().IS_KEY(false); // Doesn't apply
    cel_CGU_TBOX.common().flags().IS_DEFAULT(false);
    cel_CGU_TBOX.common().value(value++);
    cel_CGU_TBOX.detail().name("CGU_TBOX");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_CGU_TBOX);

    CompleteEnumeratedLiteral cel_HMI;
    cel_HMI.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_HMI.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_HMI.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_HMI.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_HMI.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_HMI.common().flags().IS_KEY(false); // Doesn't apply
    cel_HMI.common().flags().IS_DEFAULT(false);
    cel_HMI.common().value(value++);
    cel_HMI.detail().name("HMI");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_HMI);

    CompleteEnumeratedLiteral cel_ICU;
    cel_ICU.common().flags().TRY_CONSTRUCT1(false); // Doesn't apply
    cel_ICU.common().flags().TRY_CONSTRUCT2(false); // Doesn't apply
    cel_ICU.common().flags().IS_EXTERNAL(false); // Doesn't apply
    cel_ICU.common().flags().IS_OPTIONAL(false); // Doesn't apply
    cel_ICU.common().flags().IS_MUST_UNDERSTAND(false); // Doesn't apply
    cel_ICU.common().flags().IS_KEY(false); // Doesn't apply
    cel_ICU.common().flags().IS_DEFAULT(false);
    cel_ICU.common().value(value++);
    cel_ICU.detail().name("ICU");

    type_object->complete().enumerated_type().literal_seq().emplace_back(cel_ICU);


    TypeIdentifier identifier;
    identifier._d(EK_COMPLETE);

    SerializedPayload_t payload(static_cast<uint32_t>(
        CompleteEnumeratedType::getCdrSerializedSize(type_object->complete().enumerated_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("moudleType", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("moudleType", true);
}