/*
  MSP430 Emulator
  Copyright (C) 2016 Rudolf Geosits (rgeosits@live.esu.edu)  
                                                                      
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
                                                                   
  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.          
                                                       
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses
*/

#ifndef _EMU_SERVER_H_
#define _EMU_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <libwebsockets.h>

#include "../../main.h"
#include "packet_queue.h"

enum {
  CONTROL_PACKET_OPCODE  = 0x00,
  CONSOLE_PACKET_OPCODE  = 0x01,
  SERIAL_PACKET_OPCODE   = 0x02,
};

enum {
  P1_0_ON_PACKET  = 0x00,
  P1_0_OFF_PACKET = 0x01,

  P1_1_ON_PACKET  = 0x02,
  P1_1_OFF_PACKET = 0x03,

  P1_2_ON_PACKET  = 0x04,
  P1_2_OFF_PACKET = 0x05,

  P1_3_ON_PACKET  = 0x06,
  P1_3_OFF_PACKET = 0x07,

  P1_4_ON_PACKET  = 0x08,
  P1_4_OFF_PACKET = 0x09,

  P1_5_ON_PACKET  = 0x0A,
  P1_5_OFF_PACKET = 0x0B,

  P1_6_ON_PACKET  = 0x0C,
  P1_6_OFF_PACKET = 0x0D,

  P1_7_ON_PACKET  = 0x0E,
  P1_7_OFF_PACKET = 0x0F,
};

int callback_http (struct libwebsocket_context *this,
                          struct libwebsocket *wsi,
                          enum libwebsocket_callback_reasons reason,
                          void *user, void *in, size_t len);

int callback_emu (struct libwebsocket_context *this,
                         struct libwebsocket *wsi,
                         enum libwebsocket_callback_reasons reason,
                         void *user, void *in, size_t len);

static struct libwebsocket_protocols protocols[] = {
  /* first protocol must always be HTTP handler */
  {
    "http-only",   // name
    callback_http, // callback
    0              // per_session_data_size
  },

  {
    "emu-protocol",
    callback_emu,
    0,
    1024 * 4, // rx buffer size
    0,        // id
    0         // user context data
  },

  {
    NULL, NULL, 0   /* End of list */
  }
};

struct Server {
  // Pending Packets Queue  
  Packet *pending_packets_head;
  Packet *pending_packets_tail;
  uint32_t packets_queued;

  // Other  
  bool spin_lock;
};

void print_console (Emulator *emu, char *buf);
void print_serial (Emulator *emu, char *buf);
void send_control (Emulator *emu, uint8_t opcode, 
		   void *data, size_t size);
void *web_server (void *ctxt);

#endif