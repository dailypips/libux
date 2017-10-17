/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#ifndef __UX_EVENTS_H__
#define __UX_EVENTS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* events */
/* reminder event */
typedef enum {
    UX_CLOCK_LOCAL  = 0,
    UX_CLOCK_EXCHANGE,
    UX_CLOCK_LAST
}ux_clock_type;

typedef void (*ux_reminder_cb)(uxe_reminder_t *timer);

#define UXE_REMINDER_FIELDS                 \
    ux_clock_type clock_type;               \
    ux_reminder_cb callback;                \
    void *user_data;                        \
    ux_time_t timeout;                      \
    uint64_t repeat;                        \
    /*private*/                             \
    void *ctx;                              \
    void* heap_node[3];                     \
    uint64_t start_id;


/* system */
#define UXE_QUEUE_FIELDS                    \
    void *q;

#define UXE_QUEUE_OPENED_FIELDS             \
    UXE_QUEUE_FIELDS


#define UXE_QUEUE_CLOSED_FIELDS             \
    UXE_QUEUE_FIELDS


#define UXE_SIMULATOR_START_FIELDS          \
    ux_time_t time1;                        \
    ux_time_t time2;                        \
    long long count;

#define UXE_SIMULATOR_STOP_FIELDS

#define UXE_SIMULATOR_PROGRESS_FIELDS       \
    int percent;                            \
    long long count;

#define UXE_OPTIMIZATION_START_FIELDS       \
    ux_time_t time1;                        \
    ux_time_t time2;                        \
    long long count;

#define UXE_OPTIMIZATION_STOP_FIELDS

#define UXE_OPTIMIZATION_PROGRESS_FIELDS    \
    int percent;                            \
    long long count;


#define UXE_EXCEPTION_FIELDS                \
    char *source;


#define UXE_PROVIDER_FIELDS                 \
    int provider_id;                        \
    ux_provider_t *provider;

#define UXE_PROVIDER_ADDED_FIELDS           \
    UXE_PROVIDER_FIELDS


#define UXE_PROVIDER_REMOVED_FIELDS         \
    UXE_PROVIDER_FIELDS


#define UXE_PROVIDER_CONNECTED_FIELDS       \
    UXE_PROVIDER_FIELDS

#define UXE_PROVIDER_DISCONNECTED_FIELDS    \
    UXE_PROVIDER_FIELDS


#define UXE_PROVIDER_STATUS_CHANGED_FIELDS  \
    UXE_PROVIDER_FIELDS



typedef enum {
    UX_PROVIDER_ERROR,
    UX_PROVIDER_WARNING,
    UX_PROVIDER_MESSAGE
}ux_provider_error_type;

#define UXE_PROVIDER_ERROR_FIELDS           \
    UXE_PROVIDER_FIELDS                     \
    int error_id;                           \
    int error_code;                         \
    char *error_text;

#define UX_INSTRUMENT_FIELDS                \
    ux_instrument_t *instrument;

#define UXE_INSTRUMENT_DEF_FIELDS           \
    char *request_id;                       \
    int   provider_id;                      \
    int   total_num;                        \
    ux_instrument_t* instruments[1];

typedef enum {
    UX_REQUEST_RESULT_COMPLETED,
    UX_REQUEST_RESULT_CANCELLED,
    UX_REQUEST_RESULT_ERROR
}ux_request_result;

#define UXE_INSTRUMENT_DEF_END_FIELDS       \
    char *request_id;                       \
    ux_request_result result;               \
    char *text;


#define UXE_INSTRUMENT_ADDED_FIELDS         \
    UX_INSTRUMENT_FIELDS


#define UXE_INSTRUMENT_REMOVED_FIELDS       \
    UX_INSTRUMENT_FIELDS


#define UXE_OUTPUT_FIELDS                   \
    char *text;                             \
    ux_time_t localtime;


#define UXE_GROUP_FIELDS                    \
    int is_cleanr_group_event;              \
    int id;                                 \
    char *name;                             \
    void *fields;                           \
    void* group_events[2];


typedef enum {
    UX_GROUP_FIELD_ADDED,
    UX_GROUP_FIELD_REMOVED,
    UX_GROUP_FIELD_UPDATED
}ux_group_update_type;

#define UXE_GROUP_UPDATE_FIELDS             \
    int group_id;                           \
    char *field_name;                       \
    ux_group_update_type update_type;       \
    int field_type;                         \
    void *value;                            \
    void *old_value;


#define UXE_GROUP_EVENT_FIELDS              \
    uxe_group_t *group;                     \
    ux_event_t *obj;                        \
    int group_id;


#define UX_PORTFOLIO_FIELDS                 \
    ux_portfolio_t *portfolio;

#define UXE_PORTFOLIO_ADDED_FIELDS          \
    UX_PORTFOLIO_FIELDS

#define UXE_PORTFOLIO_REMOVED_FIELDS        \
    UX_PORTFOLIO_FIELDS


#define UXE_PORTFOLIO_PARENT_CHANGED_FIELDS \
    UX_PORTFOLIO_FIELDS


#define UXE_PORTFOLIO_UPDATE_FIELDS         \
    UX_PORTFOLIO_FIELDS                     \
    ux_position_t *position;


#define UXE_HISTORICAL_DATA_FIELDS          \
    char *request_id;                       \
    int total_num;                          \
    void *data;


#define UXE_HISTORICAL_DATA_END_FIELDS      \
    char *request_id;                       \
    ux_request_result result;               \
    char *text;


#define UXE_POSITION_FIELDS                 \
    ux_portfolio_t *portfolio;              \
    ux_position_t *position;

#define UXE_POSITION_OPENED_FIELDS          \
    UXE_POSITION_FIELDS

#define UXE_POSITION_CLOSED_FIELDS          \
    UXE_POSITION_FIELDS

#define UXE_POSITION_CHANGED_FIELDS         \
    UXE_POSITION_FIELDS

#define UXE_COMMAND_FIELDS

#define UXE_USER_COMMAND_FIELDS


#define UXE_STOP_EXECUTED_FIELDS
#define UXE_STOP_CANCELLED_FIELDS
#define UXE_STOP_STATUS_CHANGED_FIELDS

#ifdef __cplusplus
}
#endif

#endif /*__UX_EVENTS_H__*/
