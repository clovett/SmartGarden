/*
MIT License
Copyright (c) 2019 Atlas Scientific
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE
*/

#ifndef _EZO_UART_H_
#define _EZO_UART_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Ezo_uart{
	public:

		//constructors
		//takes a stream object, which is usually a hardware or software serial port
		Ezo_uart(Stream& Serial_port): name(nullptr), Serial_port(Serial_port){
			response.reserve(bufferlen);
		}

		//takes a stream object, which is usually a hardware or software serial port and a name string
		Ezo_uart(Stream& Serial_port, const char* name): name(name), Serial_port(Serial_port) {
			response.reserve(bufferlen);
		}

		//sends the "R" command to the device and parses the response as a float
		//it returns 0 if no data returned from device.
		float send_read();

		//sends the "RT" command with the temperature converted to as string
		//and parses the response so its
		//this function blocks until a reading is received
		float send_read_with_temp_comp(float temperature);

		//sends a command to the circuit, but doesn't receive the response
		//the reponse can either be later cleared with flush_rx_buffer()
		void send_cmd_no_resp( const String& cmd);

		//sends a command, then blocks until a response.
		//the response is returned as a String.
		String& send_cmd(const String& cmd);

		String& send_cmd_with_num(const char* cmd, float num, uint8_t decimal_amount);
		//sends a command with num appended after as a string, then blocks until a response.
		//When the response is received its placed into char* sensordata_buffer

		//returns a pointer to the name string
		const char* get_name();

		//discards all the data in the module's serial buffer
		void flush_rx_buffer();

		//a passthrough for the serial port's available() function
		uint8_t data_available();

	private:
		const char* name;
		Stream& Serial_port;
		const static uint8_t bufferlen = 32;
		char buffer[bufferlen];  // character array to hold the response data from modules
		String response;

};



#endif
