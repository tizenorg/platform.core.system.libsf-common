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




#if !defined(_CPACKET_CLASS_H_)
#define _CPACKET_CLASS_H_



class cpacket
{
public:
	cpacket(int size);
	cpacket(void *data);
	virtual ~cpacket();

	void set_version(int version);
	int version(void);

	void set_cmd(int cmd);
	int cmd(void);

	bool copy_data(void *data);
	void *data(void);

	void *packet(void);

	int size(void);
	int payload_size(void);
	void set_payload_size(int size);

	static int header_size(void);


private:
	enum {
		NEW	= 0x01,
		SET	= 0x02,
	};

	struct packet_t {
		int version;
		int cmd;
		int size;
		char data[0];
	}* m_packet;

	int m_create;
};



#endif
// End of a file
