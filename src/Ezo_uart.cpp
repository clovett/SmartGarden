#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Ezo_uart.h>


const char* Ezo_uart::get_name(){
	return this->name;
}

void Ezo_uart::flush_rx_buffer(){
	while(this->Serial_port.available() > 0){
		this->Serial_port.read();
	}
}

uint8_t Ezo_uart::data_available(){
	return this->Serial_port.available();
}

float Ezo_uart::send_read(){
	String& s = send_cmd("r");
	if(s != "*ER") {
		return s.toFloat();
	}
	return 0;
}

float Ezo_uart::send_read_with_temp_comp(float temperature){
	//send_cmd_with_num("rt,", temperature, 3);
	String& s = send_cmd_with_num("rt,", temperature, 3);
	if(s != "*ER") {
		return s.toFloat();
	}
	return false;
}

void Ezo_uart::send_cmd_no_resp( const String& cmd){
	this->Serial_port.print(cmd + "\r");
}

String& Ezo_uart::send_cmd(const String& cmd) {
  this->Serial_port.print(cmd + "\r");
  this->Serial_port.flush();
  uint8_t num_received =  this->Serial_port.readBytesUntil('\r', (uint8_t *)buffer, bufferlen-1);
  response.copy(buffer, num_received);
  return response;
}

String& Ezo_uart::send_cmd_with_num(const char* cmd, float num, uint8_t decimal_amount){
	String temp = String(cmd )+ String(num, decimal_amount);
	const char* pointer = temp.c_str();
	return send_cmd(pointer);
}
