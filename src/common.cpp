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




#include <syslog.h> 
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <common.h>
#include <dlog.h>
#include <stdarg.h>

#ifndef EXTAPI
#define EXTAPI __attribute__((visibility("default")))
#endif

#define SF_SERVER_MSG_LOG_FILE		"/var/log/messages"
#define FILE_LENGTH 255

static int sf_debug_file_fd;
static char sf_debug_file_buf[FILE_LENGTH];

EXTAPI void sf_log(int type , int priority , const char *tag , const char *fmt , ...)
{
	va_list ap;
	va_start(ap, fmt);
	
	switch (type) {
		case SF_LOG_PRINT_FILE:
			sf_debug_file_fd = open(SF_SERVER_MSG_LOG_FILE, O_WRONLY|O_CREAT|O_APPEND, 0644);
			if (sf_debug_file_fd != -1) {
				vsnprintf(sf_debug_file_buf,255, fmt , ap );
				write(sf_debug_file_fd, sf_debug_file_buf, strlen(sf_debug_file_buf));
				close(sf_debug_file_fd);
			}
			break;

		case SF_LOG_SYSLOG:
			int syslog_prio;
			switch (priority) {
				case SF_LOG_ERR:
					syslog_prio = LOG_ERR|LOG_DAEMON;
					break;
					
				case SF_LOG_DBG:
					syslog_prio = LOG_DEBUG|LOG_DAEMON;
					break;

				case SF_LOG_INFO:
					syslog_prio = LOG_INFO|LOG_DAEMON;
					break;
					
				default:
					syslog_prio = priority;
					break;
			}
			
			vsyslog(syslog_prio, fmt, ap);
			break;

		case SF_LOG_DLOG:
			if (tag) {
				switch (priority) {
					case SF_LOG_ERR:
						SLOG_VA(LOG_ERROR, tag ? tag : "NULL" , fmt ? fmt : "NULL" , ap);
						break;

					case SF_LOG_DBG:
						SLOG_VA(LOG_DEBUG, tag ? tag : "NULL", fmt ? fmt : "NULL" , ap);
						break;

					case SF_LOG_INFO:
						SLOG_VA(LOG_INFO, tag ? tag : "NULL" , fmt ? fmt : "NULL" , ap);
						break;
				}
			}
			break;
	}

	va_end(ap);
}
