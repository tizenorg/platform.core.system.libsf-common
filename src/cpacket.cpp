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
#include <errno.h>
#include <string.h>
#include <common.h>

#include <cobject_type.h>
#include <cpacket.h>



cpacket::cpacket(int size)
{
	char *ptr;

	try {
		ptr = new char[size + sizeof(packet_t)];
	} catch (...) {
		throw ENOMEM;
	}

	m_packet = (packet_t*)ptr;
	memset(m_packet, '\0', size + sizeof(packet_t));
	m_packet->size = size;
	m_create = NEW;
}



cpacket::cpacket(void *data)
{
	m_packet = (packet_t*)data;
	m_create = SET;
}



cpacket::~cpacket()
{
	if (m_create == NEW) {
		delete[] (char*)m_packet;
		m_packet = NULL;
	}
}



void cpacket::set_payload_size(int size)
{
	if (!m_packet) {
		ERR("error m_packet null!!");
		return;
	}

	m_packet->size = size;
}



void cpacket::set_version(int version)
{
	if (!m_packet) {
		ERR("error m_packet null!!");
		return;
	}

	m_packet->version = version;
}



int cpacket::version(void)
{
	if (!m_packet) {
		ERR("error m_packet null!!");
		return -1;
	}

	return m_packet->version;
}



void cpacket::set_cmd(int cmd)
{
	if (!m_packet) {
		ERR("error m_packet null!!");
		return;
	}

	m_packet->cmd = cmd;
}



int cpacket::cmd(void)
{
	if (!m_packet) {
		ERR("error m_packet null!!");
		return -1;
	}

	return m_packet->cmd;
}



bool cpacket::copy_data(void *data)
{
	if ( !m_packet ) {
		ERR("error m_packet null!!");
		return false;
	}

	memcpy(m_packet->data, data, m_packet->size);
	return true;
}



void *cpacket::data(void)
{
	if ( !m_packet ) {
		ERR("error m_packet null!!");
		return NULL;
	}

	return m_packet->data;
}



void *cpacket::packet(void)
{
	return (void*)m_packet;
}



int cpacket::header_size(void)
{
	return sizeof(cpacket::packet_t);
}



int cpacket::size(void)
{
	if (!m_packet) {
		ERR("error m_packet null!!");
		return -1;
	}

	return m_packet->size + sizeof(cpacket::packet_t);
}



int cpacket::payload_size(void)
{
	return m_packet->size;
}



// End of a file
