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




#if !defined(_COMMON_H_)
#define _COMMON_H_


#ifdef __cplusplus
extern "C"
{
#endif


enum sf_log_type {
	SF_LOG_PRINT_FILE		= 1,
	SF_LOG_SYSLOG			= 2,
	SF_LOG_DLOG			= 3,
};

enum sf_priority_type {
	SF_LOG_ERR			= 1,
	SF_LOG_DBG			= 2,
	SF_LOG_INFO		= 3,
};

void sf_log(int type , int priority , const char *tag , const char *fmt , ...);


#define MICROSECONDS(tv)        ((tv.tv_sec * 1000000ll) + tv.tv_usec)


//for new log system - dlog
#ifdef LOG_TAG
	#undef LOG_TAG
#endif
#define LOG_TAG	"SensorFW"
//


#if defined(_DEBUG) || defined(USE_FILE_DEBUG) 

#define DbgPrint(fmt, arg...)   do { sf_log(SF_LOG_PRINT_FILE, 0, LOG_TAG , "[SF_MSG_PRT][%s:%d] "fmt"\n",__FILE__, __LINE__, ##arg); } while(0)

#endif

#if defined(USE_SYSLOG_DEBUG)

#define ERR(fmt, arg...) do { sf_log(SF_LOG_SYSLOG, SF_LOG_ERR, LOG_TAG, "[%s:%d] "fmt"\n", __FILE__, __LINE__, ##arg); } while(0)
#define INFO(fmt, arg...) do { sf_log(SF_LOG_SYSLOG, SF_LOG_INFO, LOG_TAG,"[%s:%d] "fmt"\n", __FILE__, __LINE__, ##arg); } while(0)
#define DBG(fmt, arg...) do { sf_log(SF_LOG_SYSLOG, SF_LOG_DBG, LOG_TAG , "[%s:%d] "fmt"\n", __FILE__, __LINE__, ##arg); } while(0)


#elif defined(_DEBUG) || defined(USE_DLOG_DEBUG)

#define ERR(fmt, arg...) do { sf_log(SF_LOG_DLOG, SF_LOG_ERR, LOG_TAG, "[%s:%d] "fmt"\n", __FILE__, __LINE__, ##arg); } while(0)						
#define INFO(fmt, arg...) do { sf_log(SF_LOG_DLOG, SF_LOG_INFO, LOG_TAG, "[%s:%d] "fmt"\n", __FILE__, __LINE__, ##arg); } while(0)					
#define DBG(fmt, arg...) do { sf_log(SF_LOG_DLOG, SF_LOG_DBG, LOG_TAG, "[%s:%d] "fmt"\n", __FILE__, __LINE__, ##arg); } while(0)


#elif defined(USE_FILE_DEBUG) 

#define ERR(fmt, arg...)	do { sf_log(SF_LOG_PRINT_FILE, 0, LOG_TAG ,"[SF_MSG_ERR][%s:%d] "fmt"\n",__FILE__, __LINE__, ##arg); } while(0)
#define DBG(fmt, arg...)	do { sf_log(SF_LOG_PRINT_FILE, 0, LOG_TAG ,"[SF_MSG_DBG][%s:%d] "fmt"\n",__FILE__, __LINE__, ##arg); } while(0)
#define INFO(fmt, arg...)	do { sf_log(SF_LOG_PRINT_FILE, 0, LOG_TAG ,"[SF_MSG_INFO][%s:%d] "fmt"\n",__FILE__, __LINE__, ##arg); } while(0)

#elif defined(USE_DLOG_LOG) 

#define ERR(fmt, arg...) do { sf_log(SF_LOG_DLOG, SF_LOG_ERR, LOG_TAG, "[%s:%d] "fmt"\n", __FILE__, __LINE__, ##arg); } while(0)						
#define INFO(fmt, arg...) do { sf_log(SF_LOG_DLOG, SF_LOG_INFO, LOG_TAG, "[%s:%d] "fmt"\n", __FILE__, __LINE__, ##arg); } while(0)					

#define DBG(...)
#define DbgPrint(...)

#else
#define ERR(fmt, arg...) do { sf_log(SF_LOG_DLOG, SF_LOG_ERR, LOG_TAG, fmt , ##arg); } while(0)
						
#define DbgPrint(...)
#define DBG(...)
#define INFO(...)
#endif


#if defined(_DEBUG)
#  define warn_if(expr, fmt, arg...) do { \
		if(expr) { \
			DBG("(%s) -> "fmt, #expr, ##arg); \
		} \
	} while (0)
#  define ret_if(expr) do { \
		if(expr) { \
			DBG("(%s) -> %s() return", #expr, __FUNCTION__); \
			return; \
		} \
	} while (0)
#  define retv_if(expr, val) do { \
		if(expr) { \
			DBG("(%s) -> %s() return", #expr, __FUNCTION__); \
			return (val); \
		} \
	} while (0)
#  define retm_if(expr, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
			DBG("(%s) -> %s() return", #expr, __FUNCTION__); \
			return; \
		} \
	} while (0)
#  define retvm_if(expr, val, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
			DBG("(%s) -> %s() return", #expr, __FUNCTION__); \
			return (val); \
		} \
	} while (0)

#else
#  define warn_if(expr, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
		} \
	} while (0)
#  define ret_if(expr) do { \
		if(expr) { \
			return; \
		} \
	} while (0)
#  define retv_if(expr, val) do { \
		if(expr) { \
			return (val); \
		} \
	} while (0)
#  define retm_if(expr, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
			return; \
		} \
	} while (0)
#  define retvm_if(expr, val, fmt, arg...) do { \
		if(expr) { \
			ERR(fmt, ##arg); \
			return (val); \
		} \
	} while (0)

#endif


#ifdef __cplusplus
}
#endif

#endif
//! End of a file
