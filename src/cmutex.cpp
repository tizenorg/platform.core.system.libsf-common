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




#include <pthread.h>
#include <cobject_type.h>
#include <cmutex.h>


cmutex::cmutex()
{
	pthread_mutex_init(&m_mutex, NULL);
}



cmutex::~cmutex()
{
	pthread_mutex_destroy(&m_mutex);
}



void cmutex::lock()
{
	pthread_mutex_lock(&m_mutex);
}



void cmutex::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}



// End of a file
