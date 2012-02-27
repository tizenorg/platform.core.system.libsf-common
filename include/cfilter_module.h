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




#if !defined(_CFILTER_MODULE_CLASS_H_)
#define _CFILTER_MODULE_CLASS_H_

class cfilter_module : public cmodule
{
public:
	static const int SF_PLUGIN_FILTER = SF_PLUGIN_BASE + 10;

	cfilter_module();
	virtual ~cfilter_module();

	virtual bool add_input(csensor_module *sensor) = 0;

	virtual bool is_data_ready(bool wait = false) = 0;
	virtual long value(const char *port) = 0;
	virtual long value(int id) = 0;

	virtual void reset(void) = 0;

	virtual bool start(void) = 0;
	virtual bool stop(void) = 0;

	virtual cfilter_module *create_new(void) = 0;
	virtual void destroy(cfilter_module *module) = 0;

	virtual long polling_interval(void) = 0;
	virtual bool update_polling_interval(unsigned long val) = 0;

	virtual int get_sensor_type(void)=0;
	virtual int get_property(unsigned int property_level , void *property_data) = 0;
	virtual int get_struct_value(unsigned int struct_type , void *struct_values) = 0;

	void lock(void);
	void unlock(void);

private:

	cmutex m_mutex;

};

#endif

//! End of a file
