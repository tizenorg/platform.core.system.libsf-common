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
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <cobject_type.h>
#include <csync.h>
#include <clist.h>
#include <cmutex.h>
#include <cworker.h>
#include <common.h>

cworker::cworker(void)
: m_state(STOPPED)
, m_context(NULL)
, mutex_lock(PTHREAD_MUTEX_INITIALIZER)
{
	register int i;

	for (i = 0; i < ENUM_LAST; i ++) {
		m_func[i] = NULL;
	}
	
	DBG("processor worker created\n");
}

cworker::~cworker(void)
{
	DBG("----------Processor WORKER TERMINATED--------\n");	
	
	m_state = TERMINATE;

	if (m_func[TERMINATE])
		m_func[TERMINATE](m_context);

	DBG("processor worker terminated\n");
}

bool cworker::start(void)
{
	int ret = 0;

	pthread_mutex_lock(&(mutex_lock));
	if (m_state == START) {
		ERR("Already started\n");
		pthread_mutex_unlock(&(mutex_lock));
		return false;
	}

	m_state = START;
	pthread_mutex_unlock(&(mutex_lock));

	DBG("cworker start\n");

	ret = pthread_create(&m_thid, NULL, started, this);
	
	if(ret != 0)
	{
		pthread_mutex_lock(&(mutex_lock));
		m_state = STOP;
		pthread_mutex_unlock(&(mutex_lock));
		ERR("thread create fail\n");
		return false;
	}
	else
	{
		ret = pthread_detach(m_thid);
		if(ret != 0)
		{
			ERR("thread detach fail\n");
			return false;
		}
		else
		{
			DBG("Thread creation for Processor worker END\n");
		}
	}
	
	return true;
}

bool cworker::terminate(void)
{
	pthread_mutex_lock(&(mutex_lock));
	m_state = TERMINATE;
	pthread_mutex_unlock(&(mutex_lock));
	delete this;
	return true;
}

void *cworker::started(void *data)
{
	cworker *inst = (cworker*)data;
	worker_state_s state;

	do
	{
		state = (worker_state_s)(int)inst->m_func[STARTED](inst->m_context);
		if (state == STOPPED) {
			pthread_mutex_lock(&(inst->mutex_lock));
			inst->m_state = STOP;
			pthread_mutex_unlock(&(inst->mutex_lock));
			ERR("Abnormal Situation: processor_plugin->working() returned STOPPED\n");
			return NULL;
		}
	}while(state == STARTED && inst->m_state == START);

	DBG("\n\n\n#############Processor worker thread END###########\n\n\n");
	
	return NULL;		
}

bool cworker::stop(void)
{
	pthread_mutex_lock(&(mutex_lock));
	if (m_state == STOP) {
		ERR("Already stopped\n");
		pthread_mutex_unlock(&(mutex_lock));
		return false;
	}
	
	m_state = STOP;
	pthread_mutex_unlock(&(mutex_lock));
	DBG("Stop function for Processor worker END ");
	return true;
}

cworker::worker_state_s cworker::state(void)
{
	return m_state;
}

void cworker::set_start(void *(*start)(void *data))
{
	m_func[START] = start;
}

void cworker::set_started(void *(*started)(void *data))
{
	m_func[STARTED] = started;
}

void cworker::set_stop(void *(*stop)(void *data))
{
	m_func[STOP] = stop;
}

void cworker::set_stopped(void *(*stopped)(void *data))
{
	m_func[STOPPED] = stopped;
}

void cworker::set_terminate(void *(*term)(void *data))
{
	m_func[TERMINATE] = term;
}

void cworker::set_context(void *ctx)
{
	m_context = ctx;
}

//! End of a file
