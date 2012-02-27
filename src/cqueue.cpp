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
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include <cobject_type.h>
#include <csync.h>
#include <cqueue.h>
#include <common.h>



cqueue::cqueue(int size, bool pipe_event)
: ctype(ctype::QUEUE)
, m_size(size)
, m_current(0)
, m_last(0)
, m_delete(NULL)
, m_sync()
, m_sync_state(false)
{
	try {
		m_queue = new item_t[size];
	} catch (...) {
		throw ENOMEM;
	}

	if (pipe_event) {
		if (pipe(m_pfd) == -1) {
			throw EFAULT;
		}
	} else {
		m_pfd[PIPE_READ] = -1;
		m_pfd[PIPE_WRITE] = -1;
	}
}



cqueue::~cqueue()
{
	register int i;

	if (m_sync_state) {
		m_sync.send_event();
	}

	if (m_pfd[PIPE_READ] >= 0) {
		close(m_pfd[PIPE_READ]);
	}

	if (m_pfd[PIPE_WRITE] >= 0) {
		close(m_pfd[PIPE_WRITE]);
	}

	if (m_delete) {
		for (i = m_current; i != m_last; i = (i + 1) % m_size) {
			m_delete(m_queue[i].data);
		}
	}

	delete m_queue;
}



bool cqueue::empty()
{
	return m_current == m_last;
}



bool cqueue::full()
{
	return ((m_last + 1) % m_size) == m_current;
}



bool cqueue::put(void *data)
{
	DBG("Put new item\n");
	if (full()) {
		ERR("Queue FULL error");
		return false;
	}

	m_queue[m_last].data = data;
	m_last = (m_last + 1) % m_size;

	if (m_sync_state) {
		m_sync.send_event();
	}

	if (m_pfd[PIPE_WRITE] >= 0) {
		char dummy = 0xFF;
		if (write(m_pfd[PIPE_WRITE], &dummy, sizeof(char)) != sizeof(char)) {
			ERR("Failed to send an event\n");
			return false;
		}
	}

	return true;
}



void cqueue::update_all(bool (*job)(void *item, void *src), void *src)
{
	int iterator;
	iterator = m_current;

	while (iterator != m_last) {
		if (job) {
			if (job(m_queue[iterator].data, src)) {
				DBG("Updated. queue item is set to NULL\n");
				m_queue[iterator].data = NULL;
			}
		}
		iterator = (iterator + 1) % m_size;
	}
}



void *cqueue::get(bool sync)
{
	void *ret;
	if (empty()) {
		if (sync) {
			m_sync_state = true;
			m_sync.wait();
			m_sync_state = false;
		} else if (m_pfd[PIPE_READ] <0 ){
			return NULL;
		}
	}

	if (m_pfd[PIPE_READ] >= 0) {
		char dummy;
		if (read(m_pfd[PIPE_READ], &dummy, sizeof(char)) != sizeof(char)) {
			ERR("Failed to read event correctly\n");
			return NULL;
			
		}
	}

	ret = m_queue[m_current].data;
	m_current = (m_current + 1) % m_size;
	return ret;
}



int cqueue::get_event_handle(void)
{
	return m_pfd[PIPE_READ];
}



void *cqueue::peek(bool sync)
{
	void *ret;
	if (empty()) {
		if (sync) {
			m_sync_state = true;
			m_sync.wait();
			m_sync_state = false;
		} else {
			return NULL;
		}
	}

	ret = m_queue[m_current].data;
	return ret;
}



void cqueue::discard_item(void)
{
	if (empty()) return;
	m_current = (m_current + 1) % m_size;
}



void cqueue::set_delete_callback(void (*del)(void *))
{
	m_delete = del;
}



void cqueue::delete_item(void *data)
{
	if (m_delete) m_delete(data);
}



//! End of a file
