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




#if !defined(_CPROCESSOR_CLASS_H_)
#define _CPROCESSOR_CLASS_H_

class cprocessor_module : public cmodule
{
public:
	static const int SF_PLUGIN_PROCESSOR = SF_PLUGIN_BASE + 20;

	cprocessor_module();
	virtual ~cprocessor_module();

	virtual bool add_input(csensor_module *sensor) = 0;
	virtual bool add_input(cfilter_module *filter) = 0;

	virtual cprocessor_module *create_new(void) = 0;
	virtual void destroy(cprocessor_module *module) = 0;

	virtual long value(char *port) = 0;
	virtual long value(int id) = 0;

	virtual bool start(void);
	virtual bool stop(void);

	virtual bool add_event_callback(void  *(*handler)(cprocessor_module *, void *), void *data, bool (*rm_cb_data)(void *) = NULL);
	virtual bool rm_event_callback(void *(*handler)(cprocessor_module *, void*), void *data);

	virtual void set_main(void *(*main)(void *), void *(stopped)(void *), void *arg);
	virtual void wakeup_all_client(void);
	
	virtual bool add_callback_func(cmd_reg_t *param)=0;
	virtual bool remove_callback_func(cmd_reg_t *param)=0;
	virtual bool check_callback_event(cmd_reg_t *param)=0;

	virtual long set_cmd(int type , int property , long input_value) = 0;
	virtual int get_property(unsigned int property_level , void *property_struct ) = 0;
	virtual int get_struct_value(unsigned int struct_type , void *struct_values) = 0;

	void lock(void);
	void unlock(void);

protected:
	struct event_callback_t : public clist
	{
		void *(*handler)(cprocessor_module *inst, void *data);
		void *data;
		cprocessor_module *inst;
		bool (*rm_cb_data)(void *data);
	};

	event_callback_t *m_cb_head;
	event_callback_t *m_cb_tail;

	cworker *m_worker;

	cmutex m_mutex;
};

#endif

//! End of a file
