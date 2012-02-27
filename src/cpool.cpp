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
#include <pthread.h>

#include <cobject_type.h>
#include <cutil.h>
#include <cmutex.h>
#include <clist.h>
#include <cpool.h>



cpool *cpool::m_instance = NULL;
int cpool::m_ref_count = 0;
cmutex cpool::m_ref_lock;



cpool::cpool()
: m_head(NULL)
, m_tail(NULL)
{
	ctype::set_type(ctype::POOL);
}



cpool::~cpool()
{
	item_t *item;
	item_t *next;
	item = m_head;
	while (item) {
		next = (item_t*)item->next();

		if (item->cb_del) {
			item->cb_del(item->data);
		}

		delete item;
		item = next;
	}

	cpool::m_instance = NULL;
}


cpool *cpool::current_instance()
{
	return cpool::m_instance;
}

cpool *cpool::get_instance()
{
	m_ref_lock.lock();
	if (!cpool::m_instance) {
		try {
			cpool::m_instance = new cpool;
		} catch (...) {
			m_ref_lock.unlock();
			perror(__func__);
			return NULL;
		}
	}
	m_ref_count ++;
	m_ref_lock.unlock();
	return cpool::m_instance;
}



bool cpool::put(char *name, void *data, void (*cb_del)(void *data))
{
	item_t *item;

	try {
		item = new cpool::item_t;
	} catch (...) {
		return false;
	}

	item->name = strdup(name);
	if(item->name == NULL)
		return false;

	item->name_hash = cutil::string_hash(item->name);
	item->data = data;
	item->cb_del = cb_del;

	lock();
	if (m_tail) {
		item->link(clist::AFTER, m_tail);
	} else {
		m_head = item;
	}
	m_tail = item;
	unlock();
	return true;
}



void *cpool::get(char *name)
{
	item_t *item;
	void *data;

	item = search_item(name);
	data = item ? item->data : NULL;

	return data;
}



bool cpool::del(char *name, bool remove)
{
	item_t *item;

	item = search_item(name);
	if (!item) {
		return false;
	}

	lock();
	item->unlink();

	if (item->cb_del && remove) {
		item->cb_del(item->data);
	}
	unlock();

	delete item;
	return true;
}



cpool::item_t *cpool::search_item(char *name)
{
	item_t *item;
	unsigned long name_hash;

	name_hash = cutil::string_hash(name);

	lock();
	item = m_head;
	while (item) {
		if (name_hash == item->name_hash) {
			if (!strcmp(name, item->name)) {
				break;
			}
		}

		item = (item_t*)item->next();
	}
	unlock();

	return item;
}



void cpool::release_instance(void)
{
	m_ref_lock.lock();
	m_ref_count --;

	if (!m_ref_count)
		delete this;

	m_ref_lock.unlock();
}



//! End of a file
