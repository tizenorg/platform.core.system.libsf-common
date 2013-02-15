/*
 *  libsf-common
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: JuHyun Kim <jh8212.kim@samsung.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */ 




#if !defined(_SF_COMMON_H_)
#define _SF_COMMON_H_

#include <sys/types.h>
#include <cpacket.h>
#include <csock.h>

#define DEFAULT_SENSOR_KEY_PREFIX		"memory/private/sensor/"
#define MAX_KEY_LEN		30
#define MAX_DATA_STREAM_SIZE	(sizeof(cmd_get_struct_t) + sizeof(base_data_struct) + 8)	/*always check this size when a new cmd_type struct added*/
#define MAX_VALUE_SIZE 12

static const int SF_PLUGIN_BASE = ctype::UNKNOWN + 0x10000000;



struct cmd_queue_item_t {
	cpacket *packet;
	csock::thread_arg_t *arg;
};

enum data_accuracy {
	ACCURACY_UNDEFINED = -1,
	ACCURACY_BAD = 0,
	ACCURACY_NORMAL =1,
	ACCURACY_GOOD = 2,
	ACCURACY_VERYGOOD = 3
};

enum data_unit_idx_t {
	IDX_UNDEFINED_UNIT,
	IDX_UNIT_METRE_PER_SECOND_SQUARED,
	IDX_UNIT_MICRO_TESLA,
	IDX_UNIT_DEGREE,
	IDX_UNIT_LUX,
	IDX_UNIT_CENTIMETER,	
	IDX_UNIT_LEVEL_1_TO_10,
	IDX_UNIT_STATE_ON_OFF,
	IDX_UNIT_DEGREE_PER_SECOND,
	IDX_UNIT_HECTOPASCAL,
	IDX_UNIT_CELSIUS,
	IDX_UNIT_METER,
	
	IDX_UNIT_VENDOR_UNIT = 100,
	IDX_UNIT_FILTER_CONVERTED,
};

enum sensor_id_t{
	ID_UNKNOWN			= 0x0000,
	ID_ACCEL			= 0x0001,
	ID_GEOMAG			= 0x0002,
	ID_LUMINANT			= 0x0004,	
	ID_PROXI			= 0x0008,
	ID_THERMER			= 0x0010,	
	ID_GYROSCOPE		= 0x0020,
	ID_PRESSURE			= 0x0040,
	ID_MOTION_ENGINE	= 0x0080,
	ID_FUSION			= 0x0100,
};

enum packet_type_t {
	CMD_NONE	= 0x00,
	CMD_HELLO	= 0x01,
	CMD_BYEBYE	= 0x02,
	CMD_GET_VALUE	= 0x03,
	CMD_WAIT_EVENT	= 0x04,
	CMD_DONE	= 0x05,
	CMD_START	= 0x06,
	CMD_STOP	= 0x07,
	CMD_REG	= 0x08,
	CMD_SET_VALUE	= 0x09,
	CMD_GET_PROPERTY = 0x0A,
	CMD_GET_STRUCT = 0x0B,

	CMD_LAST = 0x0C,

	PROTOCOL_VERSION= 0x100,
};

enum reg_type_t {
	REG_DONE	= 0x00,
	REG_ADD 	= 0x01,
	REG_DEL	= 0x02,
	REG_CHK	= 0x03,
	REG_CHG = 0x04,
};

enum poll_value_t {
        POLL_100HZ = 100,
        POLL_50HZ  =  50,
        POLL_25HZ  =  25,
        POLL_20HZ  =  20,
        POLL_10HZ  =  10,
	POLL_5HZ   =   5,
	POLL_1HZ   =   1,
        POLL_100HZ_MS = 10,
        POLL_50HZ_MS  = 20,
        POLL_25HZ_MS  = 40,
        POLL_20HZ_MS  = 50,
        POLL_10HZ_MS  = 100,
	POLL_5HZ_MS   = 200,
	POLL_1HZ_MS   = 1000,
};

typedef struct {
	int id;		//!< Select sensor
	char name[];
} cmd_hello_t;



typedef struct {
} cmd_byebye_t;



typedef struct {
	long value;
} cmd_done_t;



typedef struct {
	int id;
	char port[];
} cmd_get_value_t;

typedef struct {
	unsigned int data_id;
} cmd_get_data_t;

typedef struct {	
	unsigned int get_level;
} cmd_get_property_t;

typedef struct {
	int sensor_unit_idx;
	float sensor_min_range;
	float sensor_max_range;
	float sensor_resolution;
	char sensor_name[MAX_KEY_LEN];
	char sensor_vendor[MAX_KEY_LEN];
} base_property_struct;


typedef struct {
	int state;
	size_t property_struct_size;
	char property_struct[];
} cmd_return_property_t;


typedef struct {
	int data_accuracy;
	int data_unit_idx;
	unsigned long long time_stamp;
	int values_num;
	float values[MAX_VALUE_SIZE];
} base_data_struct;

typedef struct {
	int state;
	size_t data_struct_size;
	char data_struct[];	
} cmd_get_struct_t;



typedef struct {
	long event;
} cmd_wait_event_t;



typedef struct {
	int option;
} cmd_start_t;



typedef struct {
} cmd_stop_t;

typedef struct {
	int type;	
	unsigned int event_type;
	unsigned int interval;
} cmd_reg_t;

typedef struct  {
	int sensor_type;
	int property;	
	long value;
} cmd_set_value_t;

typedef void *(*cmd_func_t)(void *data, void *cb_data);



#endif

//! End of a file
