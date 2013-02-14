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




#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>
#include <pthread.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/time.h>

#include <cobject_type.h>

#include <cmutex.h>
#include <clist.h>
#include <cmodule.h>
#include <common.h>
#include <cpacket.h>

#include <csync.h>
#include <cworker.h>
#include <cipc_worker.h>
#include <csock.h>

#include <sf_common.h>

#include <csensor_module.h>
#include <cfilter_module.h>
#include <cprocessor_module.h>

#define BASE_GATHERING_INTERVAL 1000
#define MS_TO_US 1000

cprocessor_module::cprocessor_module()
: m_worker(NULL)
, m_interval_list_head(NULL)
, m_interval_list_tail(NULL)
{
	ctype::set_type(SF_PLUGIN_PROCESSOR);

	try {
		m_worker = new cworker();
	} catch (...) {
		throw ENOMEM;
	}
}


cprocessor_module::~cprocessor_module()
{
	if(m_worker) {
		m_worker->terminate();
	}
}



void cprocessor_module::set_main(void *(*main)(void *), void*(*stopped)(void*), void *arg)
{
	m_worker->set_stopped(stopped);
	m_worker->set_started(main);
	m_worker->set_context(arg);
}



bool cprocessor_module::start(void)
{
	DBG("START#########################################\n");
	return m_worker->start();
}



bool cprocessor_module::stop(void)
{
	DBG("STOP#########################################\n");
	return m_worker->stop();
}


bool cprocessor_module::add_event_callback(void  *(*handler)(cprocessor_module *, void *), void *data, bool (*rm_cb_data)(void *data))
{
	return true;
}



bool cprocessor_module::rm_event_callback(void *(*handler)(cprocessor_module *, void*), void *data)
{
	return true;
}



void cprocessor_module::wakeup_all_client(void)
{
}

void cprocessor_module::lock(void)
{
	m_mutex.lock();
}



void cprocessor_module::unlock(void)
{
	m_mutex.unlock();
}

int cprocessor_module::add_interval_to_list(int interval, unsigned long polling_interval)
{
        interval_list_t *interval_data = new interval_list_t;
        interval_data->interval = interval;

        if(!m_interval_list_head && !m_interval_list_tail){
                m_interval_list_head = m_interval_list_tail = interval_data;
        } else{
                interval_data->link(clist::AFTER, m_interval_list_tail);
                m_interval_list_tail = interval_data;
        }

	return (polling_interval > interval * MS_TO_US) ?  0 : -1;
}

int cprocessor_module::del_interval_to_list(int interval, unsigned long polling_interval)
{
        interval_list_t *iterator = m_interval_list_head;
        unsigned int min_interval = BASE_GATHERING_INTERVAL;

        //find interval passed by param and remove
        while(iterator){
                if(interval == iterator->interval)
                {
                        if(iterator->next() == NULL)
                                m_interval_list_tail = (interval_list_t*)iterator->prev();
                        if(iterator->prev() == NULL)
                                m_interval_list_head = (interval_list_t*)iterator->next();
                        iterator->unlink();
                        delete iterator;
                        break;
                }
                iterator = (interval_list_t*)iterator->next();
        }

        //find min interval value in list
        interval_list_t *iterator_min_search = m_interval_list_head;

        while(iterator_min_search){
                if(iterator_min_search->interval <= min_interval){
                        min_interval = iterator_min_search->interval;
                }
                iterator_min_search = (interval_list_t*)iterator_min_search->next();
        }
	
	return min_interval;
}


int cprocessor_module::check_hz(int time_ms)
{
	if(time_ms >= POLL_1HZ_MS)
		return POLL_1HZ;
	else if (time_ms < POLL_1HZ_MS  && time_ms >= POLL_5HZ_MS)
		return POLL_5HZ;
	else if (time_ms < POLL_5HZ_MS  && time_ms >= POLL_10HZ_MS)
                return POLL_10HZ;
        else if (time_ms < POLL_10HZ_MS && time_ms >= POLL_20HZ_MS)
                return POLL_20HZ;
        else if (time_ms < POLL_20HZ_MS && time_ms >= POLL_25HZ_MS)
                return POLL_25HZ;
        else if (time_ms < POLL_25HZ_MS && time_ms >= POLL_50HZ_MS)
                return POLL_50HZ;
        else if (time_ms < POLL_50HZ_MS && time_ms >= POLL_100HZ_MS)
                return POLL_100HZ;
        else
        {
                DBG("Cannot support input time [%d]",time_ms);
                return -1;
        }
}

int cprocessor_module::norm_interval(int time_ms)
{
        if(time_ms >= POLL_1HZ_MS)
                return POLL_1HZ_MS;  //1000ms
        else if (time_ms < POLL_1HZ_MS  && time_ms >= POLL_5HZ_MS)
                return POLL_5HZ_MS;  //200ms
        else if (time_ms < POLL_5HZ_MS  && time_ms >= POLL_10HZ_MS)
                return POLL_10HZ_MS; //100ms
        else if (time_ms < POLL_10HZ_MS && time_ms >= POLL_20HZ_MS)
                return POLL_20HZ_MS; //50ms
        else if (time_ms < POLL_20HZ_MS && time_ms >= POLL_25HZ_MS)
                return POLL_25HZ_MS; //40ms
        else if (time_ms < POLL_25HZ_MS && time_ms >= POLL_50HZ_MS)
                return POLL_50HZ_MS; // 20ms
        else if (time_ms < POLL_50HZ_MS && time_ms >= POLL_100HZ_MS)
                return POLL_100HZ_MS;//10ms
        else
        {
                DBG("Cannot support input time [%d]",time_ms);
                return -1;
        }
}
//! End of a file
