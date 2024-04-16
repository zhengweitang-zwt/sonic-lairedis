#pragma once

extern "C" {
#include "otai.h"
}

#include "swss/table.h"

#include "otairedis.h" // for notify enum

#include <string>
#include <fstream>
#include <vector>

#define OTAI_REDIS_RECORDER_DECLARE_RECORD_REMOVE(ot)    \
    void recordRemove(                                  \
            _In_ const otai_ ## ot ## _t* ot);

#define OTAI_REDIS_RECORDER_DECLARE_RECORD_CREATE(ot)    \
    void recordCreate(                                  \
            _In_ const otai_ ## ot ## _t* ot,            \
            _In_ uint32_t attr_count,                   \
            _In_ const otai_attribute_t *attr_list);

#define OTAI_REDIS_RECORDER_DECLARE_RECORD_SET(ot)       \
    void recordSet(                                     \
            _In_ const otai_ ## ot ## _t* ot,            \
            _In_ const otai_attribute_t *attr);

#define OTAI_REDIS_RECORDER_DECLARE_RECORD_GET(ot)       \
    void recordGet(                                     \
            _In_ const otai_ ## ot ## _t* ot,            \
            _In_ uint32_t attr_count,                   \
            _In_ const otai_attribute_t *attr_list);

namespace otairedis
{
    class Recorder
    {
        public:

            Recorder();

            virtual ~Recorder();

        public: // OTAI API

            // Ideally we would have each record api here with exact the same
            // attributes as OTAI apis, but since we use serialize methods for
            // attributes and the same format is used in recorder and in REDIS
            // database in message queue, it make sense to reuse those
            // serialized values to not double serialize the same attributes.

            // generic create, remove, set, get
            // generic bulk apis
            //
            // otai_flush_fdb_entries
            // otai_remove_all_neighbor_entries
            // otai_clear_port_all_stats
            //
            // otai_api_query
            // otai_log_set
            // otai_api_initialize
            // otai_api_uninitialize
            // otai_dbg_generate_dump
            // otai_object_type_get_availability
            // otai_get_maximum_attribute_count
            // otai_get_object_key
            // otai_get_object_count
            // otai_query_attribute_capability
            // otai_query_attribute_enum_values_capability
            // otai_bulk_get_attribute
            // otai_tam_telemetry_get_data

        public: // OTAI quad API

            /**
             * @brief Record generic Create API.
             *
             * NOTE: recordCreate should log (object_type, linecard_id,
             * attr_count, attr_list) which we want to create new object id.
             * We omit object id since we assume that object id was already
             * allocated/generated by syncd, but this approach will not work in
             * asynchronous mode, so we are allocating new object id before
             * calling OTAI create API by using VirtualObjectIdManager.  That's
             * why record interface here is different than OTAI interface.
             *
             * Also currently in otai player response for create is not expected.
             */
            void recordGenericCreate(
                    _In_ otai_object_type_t objectType,
                    _In_ otai_object_id_t objectId, // already allocated
                    _In_ uint32_t attr_count,
                    _In_ const otai_attribute_t *attr_list);

            // TODO to be private
            void recordGenericCreate(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            /**
             * @brief Record generic Create API response.
             *
             * @param status Returned status from OTAI implementation.
             * @param objectId Newly allocated object ID if call to create succeeded.
             */
            void recordGenericCreateResponse(
                    _In_ otai_status_t status,
                    _In_ otai_object_id_t objectId);

            void recordGenericCreateResponse(
                    _In_ otai_status_t status);

            void recordGenericRemove(
                    _In_ otai_object_type_t objectType,
                    _In_ otai_object_id_t objectId);

            // TODO to be private
            void recordGenericRemove(
                    _In_ const std::string& key);

            void recordGenericRemoveResponse(
                    _In_ otai_status_t status);

            void recordGenericSet(
                    _In_ otai_object_type_t objectType,
                    _In_ otai_object_id_t objectId,
                    _In_ const otai_attribute_t *attr);

            // TODO to be private
            void recordGenericSet(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordGenericSetResponse(
                    _In_ otai_status_t status);

            void recordGenericGet(
                    _In_ otai_object_type_t objectType,
                    _In_ otai_object_id_t objectId,
                    _In_ uint32_t attr_count,
                    _In_ const otai_attribute_t *attr_list);

            // TODO to be private
            void recordGenericGet(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordGenericGetResponse(
                    _In_ otai_status_t status,
                    _In_ otai_object_type_t objectType,
                    _In_ uint32_t attr_count,
                    _In_ const otai_attribute_t *attr_list);

            // TODO to be private
            void recordGenericGetResponse(
                    _In_ otai_status_t status,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);


        public: // OTAI stats API

            void recordGenericGetStats(
                    _In_ otai_object_type_t object_type,
                    _In_ otai_object_id_t object_id,
                    _In_ uint32_t number_of_counters,
                    _In_ const otai_stat_id_t *counter_ids);

            // TODO to private
            void recordGenericGetStats(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordGenericGetStatsResponse(
                    _In_ otai_status_t status,
                    _In_ uint32_t count,
                    _In_ const uint64_t *counters);

            void recordGenericClearStats(
                    _In_ otai_object_type_t object_type,
                    _In_ otai_object_id_t object_id,
                    _In_ uint32_t number_of_counters,
                    _In_ const otai_stat_id_t *counter_ids);

            // TODO to private
            void recordGenericClearStats(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordGenericClearStatsResponse(
                    _In_ otai_status_t status);

        public: // OTAI bulk API

            void recordBulkGenericCreate(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordBulkGenericCreateResponse(
                    _In_ otai_status_t status,
                    _In_ uint32_t objectCount,
                    _In_ const otai_status_t *objectStatuses);

            void recordBulkGenericRemove(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordBulkGenericRemoveResponse(
                    _In_ otai_status_t status,
                    _In_ uint32_t objectCount,
                    _In_ const otai_status_t *objectStatuses);

            void recordBulkGenericSet(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordBulkGenericSetResponse(
                    _In_ otai_status_t status,
                    _In_ uint32_t objectCount,
                    _In_ const otai_status_t *objectStatuses);

        public: // OTAI global interface API

            void recordObjectTypeGetAvailability(
                    _In_ otai_object_id_t switchId,
                    _In_ otai_object_type_t objectType,
                    _In_ uint32_t attrCount,
                    _In_ const otai_attribute_t *attrList);

            void recordObjectTypeGetAvailabilityResponse(
                    _In_ otai_status_t status,
                    _In_ const uint64_t *count);

            void recordQueryAttributeCapability(
                    _In_ otai_object_id_t linecard_id,
                    _In_ otai_object_type_t object_type,
                    _In_ otai_attr_id_t attr_id,
                    _Out_ otai_attr_capability_t *capability);

            void recordQueryAttributeCapabilityResponse(
                    _In_ otai_status_t status,
                    _In_ otai_object_type_t objectType,
                    _In_ otai_attr_id_t attrId,
                    _In_ const otai_attr_capability_t* capability);

            void recordQueryAattributeEnumValuesCapability(
                    _In_ otai_object_id_t linecard_id,
                    _In_ otai_object_type_t object_type,
                    _In_ otai_attr_id_t attr_id,
                    _Inout_ otai_s32_list_t *enum_values_capability);

            void recordQueryAattributeEnumValuesCapabilityResponse(
                    _In_ otai_status_t status,
                    _In_ otai_object_type_t objectType,
                    _In_ otai_attr_id_t attrId,
                    _In_ const otai_s32_list_t* enumValuesCapability);

            // TODO move to private
            void recordQueryAttributeCapability(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordQueryAttributeCapabilityResponse(
                    _In_ otai_status_t status,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordQueryAttributeEnumValuesCapability(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordQueryAttributeEnumValuesCapabilityResponse(
                    _In_ otai_status_t status,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordObjectTypeGetAvailability(
                    _In_ const std::string& key,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

            void recordObjectTypeGetAvailabilityResponse(
                    _In_ otai_status_t status,
                    _In_ const std::vector<swss::FieldValueTuple>& arguments);

        public: // OTAI notifications

            void recordNotification(
                    _In_ const std::string &name,
                    _In_ const std::string &serializedNotification,
                    _In_ const std::vector<swss::FieldValueTuple> &values);

        public: // otairedis/syncd internal API

            void recordNotifySyncd(
                    _In_ otai_object_id_t switchId,
                    _In_ otai_redis_notify_syncd_t redisNotifySyncd);

            void recordNotifySyncd(
                    _In_ const std::string& key);

            void recordNotifySyncdResponse(
                    _In_ otai_status_t status);

        public: // Recorder API

            void enableRecording(
                    _In_ bool enabled);

            bool setRecordingOutputDirectory(
                    _In_ const otai_attribute_t &attr);

            bool setRecordingFilename(
                    _In_ const otai_attribute_t &attr);

            void requestLogRotate();

        public: // static helper functions

            static std::string getTimestamp();

            void recordStats(
                _In_ bool enable);

            void recordAlarms(
                _In_ bool enable);

        private: // recording helpers

            void recordCreate(
                    _In_ otai_object_type_t objectType,
                    _In_ const std::string& serializedObjectId,
                    _In_ uint32_t attr_count,
                    _In_ const otai_attribute_t *attr_list);

            void recordRemove(
                    _In_ otai_object_type_t objectType,
                    _In_ const std::string& serializedObjectId);

            void recordSet(
                    _In_ otai_object_type_t objectType,
                    _In_ const std::string& serializedObjectId,
                    _In_ const otai_attribute_t *attr);

            void recordGet(
                    _In_ otai_object_type_t object_type,
                    _In_ const std::string &serializedObjectId,
                    _In_ uint32_t attr_count,
                    _In_ const otai_attribute_t *attr_list);

        private:

            void recordingFileReopen();

            void startRecording();

            void stopRecording();

            void recordLine(
                    _In_ const std::string& line);

        private:

            bool m_performLogRotate;

            bool m_enabled;

            bool m_recordStats;

            bool m_recordAlarms;

            std::string m_recordingOutputDirectory;

            std::string m_recordingFileName;

            std::string m_recordingFile;

            std::ofstream m_ofstream;

            std::mutex m_mutex;
    };
}
