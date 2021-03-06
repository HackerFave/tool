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
 * @file SignatureSSLTypeObject.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _TG_MSGS_DDS_SIGNATURESSL_TYPE_OBJECT_H_
#define _TG_MSGS_DDS_SIGNATURESSL_TYPE_OBJECT_H_


#include <fastrtps/types/TypeObject.h>
#include <map>

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif
#else
#define eProsima_user_DllExport
#endif

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(SignatureSSL_SOURCE)
#define SignatureSSL_DllAPI __declspec( dllexport )
#else
#define SignatureSSL_DllAPI __declspec( dllimport )
#endif // SignatureSSL_SOURCE
#else
#define SignatureSSL_DllAPI
#endif
#else
#define SignatureSSL_DllAPI
#endif // _WIN32

using namespace eprosima::fastrtps::types;

eProsima_user_DllExport void registerSignatureSSLTypes();


eProsima_user_DllExport const TypeIdentifier* GetFileImportStatusIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetFileImportStatusObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalFileImportStatusObject();
eProsima_user_DllExport const TypeObject* GetCompleteFileImportStatusObject();


eProsima_user_DllExport const TypeIdentifier* GetFileCaStatusIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetFileCaStatusObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalFileCaStatusObject();
eProsima_user_DllExport const TypeObject* GetCompleteFileCaStatusObject();


eProsima_user_DllExport const TypeIdentifier* GetFileDataStreamIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetFileDataStreamObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalFileDataStreamObject();
eProsima_user_DllExport const TypeObject* GetCompleteFileDataStreamObject();


#endif // _TG_MSGS_DDS_SIGNATURESSL_TYPE_OBJECT_H_