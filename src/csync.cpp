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




#include <common.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <csync.h>

extern int errno;

csync::csync(void)
{
}



csync::~csync(void)
{
}



void csync::wait(void)
{
}



void csync::wait(long t)
{
}



void csync::send_event(void)
{
}



void csync::send_event_all(void)
{
}



// End of a file
