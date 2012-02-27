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




#if !defined(_CQUEUE_CLASS_H_)
#define _CQUEUE_CLASS_H_



class cqueue : public ctype
{
private:
	struct item_t {
		void *data;
	};

	enum {
		PIPE_READ	= 0,
		PIPE_WRITE	= 1,
		PIPE_ALL	= 2,
	};

public:
	cqueue(int size, bool pipe_event = false);
	virtual ~cqueue();

	bool empty();
	bool full();

	bool put(void *data);
	void *get(bool sync = false);
	void *peek(bool sync = false);

	void discard_item(void);

	void delete_item(void *data);

	void set_delete_callback(void (*del)(void *));
	void update_all(bool (*)(void *data, void *item), void *item);

	int get_event_handle(void);

private:
	int m_size;
	int m_current;
	int m_last;

	void (*m_delete)(void *data);

	item_t *m_queue;

	csync m_sync;
	bool m_sync_state;

	int m_pfd[PIPE_ALL];
};



#endif
//! End of a file
