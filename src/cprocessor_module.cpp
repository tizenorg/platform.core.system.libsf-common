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

cprocessor_module::cprocessor_module()
: m_cb_head(NULL)
, m_cb_tail(NULL)
, m_worker(NULL)
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
	event_callback_t *item;

	try {
		item = new event_callback_t;
	} catch (...) {
		ERR("Failed to allocate memory\n");
		return false;
	}

	item->handler = handler;
	item->inst = this;
	item->data = data;
	item->rm_cb_data = rm_cb_data;

	if (m_cb_head == NULL || m_cb_tail == NULL) {
		m_cb_tail = m_cb_head = item;
	} else {
		item->link(clist::AFTER, m_cb_tail);
		m_cb_tail = item;
	}
	DBG("Event ADDED =======================\n");

	return true;
}



bool cprocessor_module::rm_event_callback(void *(*handler)(cprocessor_module *, void*), void *data)
{
	event_callback_t *iterator;
	event_callback_t *next;
	bool found = false;

	iterator = m_cb_head;
	while (iterator) {
		next = (event_callback_t*)iterator->next();

		if (iterator->handler == handler && iterator->data == data) {
			event_callback_t *prev = (event_callback_t*)iterator->prev();

			iterator->unlink();

			if (iterator == m_cb_tail) {
				m_cb_tail = prev;
			}

			if (iterator == m_cb_head) {
				m_cb_head = next;
			}

			if (iterator->rm_cb_data) {
				iterator->rm_cb_data(iterator->data);
			}

			delete iterator;
			iterator=NULL;
			found = true;
			break;
		}

		iterator = next;
	}

	return found;
}



void cprocessor_module::wakeup_all_client(void)
{
	event_callback_t *iterator;
	event_callback_t *next;
	
	lock();
	iterator = m_cb_head;
	while (iterator) {
		next = (event_callback_t*)iterator->next();

		if (this == iterator->inst) {
			iterator->handler(this, iterator->data);
			rm_event_callback(iterator->handler, iterator->data);
			DBG("Wakeup all client $$$$$$$$$$$$$$$$$$$$$$$$4\n");
		}

		iterator = next;
	}
	unlock();
}

void cprocessor_module::lock(void)
{
	m_mutex.lock();
}



void cprocessor_module::unlock(void)
{
	m_mutex.unlock();
}

//! End of a file
