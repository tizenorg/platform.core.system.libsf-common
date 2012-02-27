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




#if !defined(_CSOCK_CLASS_H_)
#define _CSOCK_CLASS_H_
#include <cipc_worker.h>
class csock : public cmutex
{
public:
	enum sock_type_t {
		SOCK_IPC	= 0x01,
		SOCK_NET	= 0x02,
		SOCK_TCP	= 0x04,
		SOCK_UDP	= 0x08,

		SOCK_WORKER	= 0x10,
	};

	struct thread_arg_t {
		csock *sock;
		int client_handle;
		cipc_worker *worker;
		void *client_ctx;
	};

	csock(int handle, int mode = 0x15);
	csock(char *name, int mode, int port, int server = 0);
	virtual ~csock(void);

	bool connect_to_server(void);
	bool wait_for_client(void);

	bool is_connected(void);

	void set_on_close(bool on_close = true);

	void set_worker(void *(*start)(void *data), void *(*running)(void *data), void *(*stop)(void *data));

	void set_client_ctx(void *client_ctx);
	void *client_ctx();

	bool recv(void *buffer, int len);
	bool send(void *buffer, int len);

	int handle(void);
	int mode(void);

	bool disconnect(void);

	static void *thread_tcp_main(void *arg);

	static void *worker_terminate(void *data);

private:
	int m_handle;
	char *m_name;

	sockaddr_un m_addr; 	
	sockaddr_in m_net_addr;	

	void *(*m_start)(void *data);
	void *(*m_running)(void *data);
	void *(*m_stop)(void *data);

	void *m_client_ctx;
	pthread_t m_thid;
	bool m_on_close;
	int m_mode;
};



#endif
//! End of a file
