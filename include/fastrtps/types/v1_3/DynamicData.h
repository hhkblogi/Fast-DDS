// Copyright 2018 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#ifndef TYPES_1_3_DYNAMIC_DATA_H
#define TYPES_1_3_DYNAMIC_DATA_H

#include <fastrtps/types/TypesBase.h>
#include <fastrtps/types/v1_3/DynamicDataPtr.h>
#include <fastrtps/types/v1_3/MemberId.h>

//#define DYNAMIC_TYPES_CHECKING

namespace eprosima {
namespace fastdds {
namespace dds {
namespace DDSSQLFilter {

class DDSFilterExpression;

}  // namespace DDSSQLFilter
}  // namespace dds
}  // namespace fastdds
}  // namespace eprosima

namespace eprosima {
namespace fastrtps {
namespace types {

class DynamicDataHelper;

namespace v1_3 {

class DynamicType;
class MemberDescriptor;
class DynamicPubSubType;

class DynamicData
{
    friend class DynamicType;

protected:

    explicit DynamicData(
            const DynamicData* pData);
    DynamicData(
            DynamicType_ptr pType);

    ~DynamicData();

    void add_value(
            TypeKind kind,
            MemberId id);

    void create_members(
            DynamicType_ptr pType);

    void create_members(
            const DynamicData* pData);

    void clean();

    void clean_members();

    void* clone_value(
            MemberId id,
            TypeKind kind) const;

    bool compare_values(
            TypeKind kind,
            void* left,
            void* right) const;

    ReturnCode_t insert_array_data(
            MemberId indexId);

    void set_default_value(
            MemberId id);

    void get_value(
            std::string& sOutValue,
            MemberId id = MEMBER_ID_INVALID) const;

    void set_value(
            const std::string& sValue,
            MemberId id = MEMBER_ID_INVALID);

    MemberId get_union_id() const;

    ReturnCode_t set_union_id(
            MemberId id);

    bool has_children() const;

    DynamicType_ptr type_;

#ifdef DYNAMIC_TYPES_CHECKING
    int32_t int32_value_ = 0;
    uint32_t uint32_value_ = 0;
    int16_t int16_value_ = 0;
    uint16_t uint16_value_ = 0;
    int64_t int64_value_ = 0;
    uint64_t uint64_value_ = 0;
    float float32_value_ = 0.0f;
    double float64_value_ = 0.0;
    long double float128_value_ = 0.0;
    char char8_value_ = 0;
    wchar_t char16_value_ = 0;
    octet byte_value_ = 0;
    bool bool_value_ = false;
    std::string string_value_;
    std::wstring wstring_value_;
    std::map<MemberId, DynamicData*> complex_values_;
#else
    std::map<MemberId, void*> values_;
#endif // ifdef DYNAMIC_TYPES_CHECKING
    std::vector<MemberId> loaned_values_;
    bool key_element_ = false;
    DynamicData* default_array_value_ = nullptr;
    MemberId union_id_ = MEMBER_ID_INVALID;

    friend class DynamicDataFactory;
    friend class DynamicPubSubType;
    friend class types::DynamicDataHelper;
    friend class eprosima::fastdds::dds::DDSSQLFilter::DDSFilterExpression;

public:

    /**
     * Retrieve the @ref MemberDescriptor associated to a member
     * @param [out] value @ref MemberDescriptor object to populate
     * @param [in] id identifier of the member to retrieve
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_descriptor(
            MemberDescriptor& value,
            MemberId id);

    /**
     * Clear all memory associated to the object
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t clear_all_values();

    /**
     * Clear all memory not associated to the key
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t clear_nonkey_values();

    /**
     * Clear all memory associated to a specific member
     * @param [in] id identifier of the member to purge
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t clear_value(
            MemberId id);

    /**
     * Compares two @ref DynamicData, equality requires:
     *     - Their respective type definitions are equal
     *     - All contained values are equal and occur in the same order
     *     - If the samples' type is an aggregated type, previous rule shall be amended as follows:
     *          -# Members shall be compared without regard to their order.
     *          -# One of the samples may omit a non-optional member that is present in the other if that
     *             member takes its default value in the latter sample.
     * @param [in] other @ref DynamicData object to compare to
     * @return `true` on equality
     */
    RTPS_DllAPI bool equals(
            const DynamicData* other) const;

    RTPS_DllAPI TypeKind get_kind() const;

    /**
     * Provides the @b item @b count of the data and depends on the type of object:
     * @li If the object is of a collection type, returns the number of elements currently in the collection.
     *     In the case of an array type, this value will always be equal to the product of the bounds of all array
     *     dimensions.
     * @li If the object is of a bitmask type, return the number of named flags that are currently set in the bitmask.
     * @li If the object is of a structure or annotation type, return the number of members in the object.
     *     This value may be different than the number of members in the corresponding @ref DynamicType (some optional
     *     members may be omitted.
     * @li If the object is of a union type, return the number of members in the object. This number will be two if the
     *     discriminator value selects a member and one otherwise.
     * @li if the object is of a primitive or enumerated type, it is atomic: return one.
     * @li if the object is of an alias type, return the value appropriate for the alias base type.
     * @return count as defined above
     */
    RTPS_DllAPI uint32_t get_item_count() const;

    RTPS_DllAPI std::string get_name();

    /**
     * Queries members by name
     * @param[in] name string
     * @return MemberId or MEMBER_ID_INVALID on failure
     */
    RTPS_DllAPI MemberId get_member_id_by_name(
            const std::string& name) const;

    /**
     * Queries members by index
     * @param[in] index uint32_t
     * @return MemberId or MEMBER_ID_INVALID on failure
     */
    RTPS_DllAPI MemberId get_member_id_at_index(
            uint32_t index) const;

    /**
     * \b Loans a @ref DynamicData object within the sample
     * @remarks This operation allows applications to visit values without allocating additional
     *         @ref DynamicData objects or copying values.
     * @remarks This loan shall be returned by the @ref DynamicData::return_loaned_value operation
     * @param [in] id identifier of the object to retrieve
     * @return @ref DynamicData object loaned or \b nil on outstanding loaned data
     */
    RTPS_DllAPI DynamicData* loan_value(
            MemberId id);

    /**
     * Returns a loaned retrieved using @ref DynamicData::return_loaned_value
     * @param [in] value @ref DynamicData previously loaned
     */
    RTPS_DllAPI ReturnCode_t return_loaned_value(
            const DynamicData* value);

    RTPS_DllAPI MemberId get_array_index(
            const std::vector<uint32_t>& position);

    RTPS_DllAPI ReturnCode_t insert_sequence_data(
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_int32_value(
            int32_t value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_uint32_value(
            uint32_t value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_int16_value(
            int16_t value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_uint16_value(
            uint16_t value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_int64_value(
            int64_t value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_uint64_value(
            uint64_t value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_float32_value(
            float value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_float64_value(
            double value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_float128_value(
            long double value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_char8_value(
            char value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_char16_value(
            wchar_t value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_byte_value(
            octet value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_bool_value(
            bool value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_string_value(
            const std::string& value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_wstring_value(
            const std::wstring& value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_enum_value(
            const std::string& value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_complex_value(
            const DynamicData* value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_complex_value(
            DynamicData* value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t insert_complex_value(
            DynamicData_ptr value,
            MemberId& outId);

    RTPS_DllAPI ReturnCode_t remove_sequence_data(
            MemberId id);

    RTPS_DllAPI ReturnCode_t clear_data();

    RTPS_DllAPI ReturnCode_t clear_array_data(
            MemberId indexId);

    RTPS_DllAPI ReturnCode_t insert_map_data(
            const DynamicData* key,
            MemberId& outKeyId,
            MemberId& outValueId);

    RTPS_DllAPI ReturnCode_t insert_map_data(
            const DynamicData* key,
            DynamicData* value,
            MemberId& outKey,
            MemberId& outValue);

    RTPS_DllAPI ReturnCode_t insert_map_data(
            const DynamicData* key,
            const DynamicData* value,
            MemberId& outKey,
            MemberId& outValue);

    RTPS_DllAPI ReturnCode_t insert_map_data(
            const DynamicData* key,
            DynamicData_ptr value,
            MemberId& outKey,
            MemberId& outValue);

    RTPS_DllAPI ReturnCode_t remove_map_data(
            MemberId keyId);

    /*
     * Retrieve an \b int32 value associated to an identifier
     * @param [out] value \b int32 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_int32_value(
            int32_t& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b int32 value associated to an identifier
     * @param [in] value \b int32 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_int32_value(
            int32_t value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b uint32 value associated to an identifier
     * @param [out] value \b uint32 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_uint32_value(
            uint32_t& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b uint32 value associated to an identifier
     * @param [in] value \b uint32 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_uint32_value(
            uint32_t value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b int16 value associated to an identifier
     * @param [out] value \b int16 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_int16_value(
            int16_t& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b int16 value associated to an identifier
     * @param [in] value \b int16 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_int16_value(
            int16_t value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b uint16 value associated to an identifier
     * @param [out] value \b uint16 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_uint16_value(
            uint16_t& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b uint16 value associated to an identifier
     * @param [in] value \b uint16 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_uint16_value(
            uint16_t value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b int64 value associated to an identifier
     * @param [out] value \b int64 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_int64_value(
            int64_t& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b int64 value associated to an identifier
     * @param [in] value \b int64 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_int64_value(
            int64_t value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b uint64 value associated to an identifier
     * @param [out] value \b uint64 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_uint64_value(
            uint64_t& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b uint64 value associated to an identifier
     * @param [in] value \b uint64 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_uint64_value(
            uint64_t value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b float32 value associated to an identifier
     * @param [out] value \b float32 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_float32_value(
            float& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b float32 value associated to an identifier
     * @param [in] value \b float32 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_float32_value(
            float value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b float64 value associated to an identifier
     * @param [out] value \b float64 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_float64_value(
            double& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b float64 value associated to an identifier
     * @param [in] value \b float64 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_float64_value(
            double value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b float128 value associated to an identifier
     * @param [out] value \b float128 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @remarks Only available on platforms supporting long double
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_float128_value(
            long double& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b float128 value associated to an identifier
     * @param [in] value \b float128 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_float128_value(
            long double value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b char8 value associated to an identifier
     * @param [out] value \b char8 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_char8_value(
            char& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b char8 value associated to an identifier
     * @param [in] value \b char8 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_char8_value(
            char value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b char16 value associated to an identifier
     * @param [out] value \b char16 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_char16_value(
            wchar_t& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b char16 value associated to an identifier
     * @param [in] value \b char16 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_char16_value(
            wchar_t value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b byte value associated to an identifier
     * @param [out] value \b byte to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_byte_value(
            octet& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b byte value associated to an identifier
     * @param [in] value \b byte to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_byte_value(
            octet value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b int8 value associated to an identifier
     * @param [out] value \b int8 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_int8_value(
            int8_t& value,
            MemberId id = MEMBER_ID_INVALID) const
    {
        octet aux;
        ReturnCode_t result = get_byte_value(aux, id);
        value = static_cast<int8_t>(aux);
        return result;
    }

    /*
     * Set an \b int8 value associated to an identifier
     * @param [in] value \b int8 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_int8_value(
            int8_t value,
            MemberId id = MEMBER_ID_INVALID)
    {
        return set_byte_value(static_cast<octet>(value), id);
    }

    /*
     * Retrieve an \b uint8 value associated to an identifier
     * @param [out] value \b uint8 to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_uint8_value(
            uint8_t& value,
            MemberId id = MEMBER_ID_INVALID) const
    {
        octet aux;
        ReturnCode_t result = get_byte_value(aux, id);
        value = static_cast<uint8_t>(aux);
        return result;
    }

    /*
     * Set an \b uint8 value associated to an identifier
     * @param [in] value \b uint8 to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_uint8_value(
            uint8_t value,
            MemberId id = MEMBER_ID_INVALID)
    {
        return set_byte_value(static_cast<octet>(value), id);
    }

    /*
     * Retrieve an \b bool value associated to an identifier
     * @param [out] value \b bool to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_bool_value(
            bool& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b bool value associated to an identifier
     * @param [in] value \b bool to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_bool_value(
            bool value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Set an \b bool value associated to an identifier
     * @param [in] value \b bool to set
     * @param [in] name bitmask flags can be addressed by name
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_bool_value(
            bool value,
            const std::string& name)
    {
        MemberId id = get_member_id_by_name(name);
        if (id != MEMBER_ID_INVALID)
        {
            return set_bool_value(value, id);
        }
        return ReturnCode_t::RETCODE_BAD_PARAMETER;
    }

    /*
     * Retrieve an \b string value associated to an identifier
     * @param [out] value \b string to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_string_value(
            std::string& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b string value associated to an identifier
     * @param [in] value \b string to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_string_value(
            const std::string& value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b wstring value associated to an identifier
     * @param [out] value \b wstring to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_wstring_value(
            std::wstring& value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set an \b wstring value associated to an identifier
     * @param [in] value \b wstring to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_wstring_value(
            const std::wstring& value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b enum value associated to an identifier
     * @param [out] value string because enumerations can be addressed by name
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_enum_value(
            std::string& value,
            MemberId id = MEMBER_ID_INVALID) const;
    /*
     * Set an \b enum value associated to an identifier
     * @param [in] value string because enumerations can be addressed by name
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_enum_value(
            const std::string& value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve an \b enum value associated to an identifier
     * @param [out] value uin32_t because enums are kept as \b DWORDs.
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_enum_value(
            uint32_t& value,
            MemberId id = MEMBER_ID_INVALID) const;
    /*
     * Set an \b enum value associated to an identifier
     * @param [in] value \b enum to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_enum_value(
            const uint32_t& value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Retrieve a bitmask object \b mask
     * @param [out] value uin64_t because bitmasks are kept as \b QWORDs.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_bitmask_value(
            uint64_t& value) const;

    /*
     * Convenient override to retrieve a bitmask object \b mask
     * @throws \@ref ReturnCode_t on failure
     * @return uint64 representing bitmask mask
     */
    RTPS_DllAPI uint64_t get_bitmask_value() const
    {
        uint64_t value;
        if (get_bitmask_value(value) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Set a \b mask value on a bitmask
     * @param [in] value \b mask to set
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_bitmask_value(
            uint64_t value);

    /*
     * Retrieve a \b complex value associated to an identifier
     * @param [out] value @ref DynamicData reference to populate
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t get_complex_value(
            DynamicData** value,
            MemberId id = MEMBER_ID_INVALID) const;

    /*
     * Set a \b complex value associated to an identifier
     * @param [in] value @ref DynamicData to set
     * @param [in] id identifier of the member to set. \b MEMBER_ID_INVALID for primitives.
     * @return standard DDS @ref ReturnCode_t
     */
    RTPS_DllAPI ReturnCode_t set_complex_value(
            DynamicData* value,
            MemberId id = MEMBER_ID_INVALID);

    /*
     * Convenient override to retrieve an \b int32 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b int32 queried
     */
    RTPS_DllAPI int32_t get_int32_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        int32_t value;
        if (get_int32_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b uint32 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b uint32 queried
     */
    RTPS_DllAPI uint32_t get_uint32_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        uint32_t value;
        if (get_uint32_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b int16 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b int16 queried
     */
    RTPS_DllAPI int16_t get_int16_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        int16_t value;
        if (get_int16_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b uint16 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b uint16 queried
     */
    RTPS_DllAPI uint16_t get_uint16_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        uint16_t value;
        if (get_uint16_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b int64 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b int64 queried
     */
    RTPS_DllAPI int64_t get_int64_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        int64_t value;
        if (get_int64_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b uint64 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b uint64 queried
     */
    RTPS_DllAPI uint64_t get_uint64_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        uint64_t value;
        if (get_uint64_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b float32 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b float32 queried
     */
    RTPS_DllAPI float get_float32_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        float value;
        if (get_float32_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b float64 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b float64 queried
     */
    RTPS_DllAPI double get_float64_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        double value;
        if (get_float64_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b float128 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b float128 queried
     */
    RTPS_DllAPI long double get_float128_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        long double value;
        if (get_float128_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b char8 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b char8 queried
     */
    RTPS_DllAPI char get_char8_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        char value;
        if (get_char8_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b char16 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b char16 queried
     */
    RTPS_DllAPI wchar_t get_char16_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        wchar_t value;
        if (get_char16_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b byte associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b byte queried
     */
    RTPS_DllAPI octet get_byte_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        octet value;
        if (get_byte_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b int8 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b int8 queried
     */
    RTPS_DllAPI int8_t get_int8_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        int8_t value;
        if (get_int8_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b uint8 associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b uint8 queried
     */
    RTPS_DllAPI uint8_t get_uint8_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        uint8_t value;
        if (get_uint8_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b bool associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b bool queried
     */
    RTPS_DllAPI bool get_bool_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        bool value;
        if (get_bool_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b bool associated to an identifier
     * @param [in] name string because \b bitmask can be addressed by name
     * @throws \@ref ReturnCode_t on failure
     * @return \b bool queried
     */
    RTPS_DllAPI bool get_bool_value(
            const std::string& name) const
    {
        MemberId id = get_member_id_by_name(name);
        bool value;
        if (get_bool_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b string associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b string queried
     */
    RTPS_DllAPI std::string get_string_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        std::string value;
        if (get_string_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b wstring associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b wstring queried
     */
    RTPS_DllAPI std::wstring get_wstring_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        std::wstring value;
        if (get_wstring_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    /*
     * Convenient override to retrieve an \b enum associated to an identifier
     * @param [in] id identifier of the member to query. \b MEMBER_ID_INVALID for primitives.
     * @throws \@ref ReturnCode_t on failure
     * @return \b enum queried
     */
    RTPS_DllAPI std::string get_enum_value(
            MemberId id = MEMBER_ID_INVALID) const
    {
        std::string value;
        if (get_enum_value(value, id) != ReturnCode_t::RETCODE_OK)
        {
            throw ReturnCode_t::RETCODE_BAD_PARAMETER;
        }
        return value;
    }

    RTPS_DllAPI ReturnCode_t get_union_label(
            uint64_t& value) const;

    RTPS_DllAPI uint64_t get_union_label() const;

    RTPS_DllAPI MemberId get_discriminator_value() const;

    RTPS_DllAPI ReturnCode_t get_discriminator_value(
            MemberId& id) const noexcept;

    RTPS_DllAPI ReturnCode_t set_discriminator_value(
            MemberId value) noexcept;

    // Serializes and deserializes the Dynamic Data.
    RTPS_DllAPI void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    RTPS_DllAPI bool deserialize(
            eprosima::fastcdr::Cdr& cdr);

    RTPS_DllAPI static size_t getCdrSerializedSize(
            const DynamicData* data,
            size_t current_alignment = 0);

    RTPS_DllAPI static size_t getEmptyCdrSerializedSize(
            const DynamicType* type,
            size_t current_alignment = 0);

    RTPS_DllAPI static size_t getKeyMaxCdrSerializedSize(
            const DynamicType_ptr type,
            size_t current_alignment = 0);

    RTPS_DllAPI static size_t getMaxCdrSerializedSize(
            const DynamicType_ptr type,
            size_t current_alignment = 0);

    RTPS_DllAPI DynamicType_ptr get_type() const;

    void serializeKey(
            eprosima::fastcdr::Cdr& cdr) const;

    // TODO: clone()
};


} // namespace v1_3
} // namespace types
} // namespace fastrtps
} // namespace eprosima

#endif // TYPES_1_3_DYNAMIC_DATA_H
