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




#if !defined(_SCYNC_CLASS_H_)
#define _SCYNC_CLASS_H_



class csync
{
public:
	csync(void);
	virtual ~csync(void);

	void wait(void);
	void wait(long t);
	void send_event(void);
	void send_event_all(void);

private:
	pthread_mutex_t m_lock;
	pthread_cond_t m_cond;
};



#endif
