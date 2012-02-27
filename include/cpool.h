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




#if !defined(_CPOOL_CLASS_H_)
#define _CPOOL_CLASS_H_



class cpool : public cmutex
{
public:
	static cpool *get_instance();
	static cpool *current_instance();

	bool put(char *name, void *data, void (*cb_del)(void *data) = NULL);
	void *get(char *name);
	bool del(char *name, bool remove = true);

	void release_instance(void);

private:
	cpool();
	virtual ~cpool();

	struct item_t : public clist {
		char *name;
		unsigned long name_hash;
		void *data;
		void (*cb_del)(void *data);
	};

	item_t *m_head;
	item_t *m_tail;

	static cpool *m_instance;
	static int m_ref_count;
	static cmutex m_ref_lock;

	item_t *search_item(char *name);
};



#endif
//! End of a file
