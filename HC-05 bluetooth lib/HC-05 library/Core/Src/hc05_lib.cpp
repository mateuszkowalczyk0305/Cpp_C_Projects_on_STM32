#include "hc05_lib.hpp"

HC05::HC05(UART_HandleTypeDef* _uart, GPIO_TypeDef* _KEY_Port, uint16_t _KEY_Pin, GPIO_TypeDef* _EN_Port, uint16_t _EN_Pin, uint32_t baud_rate)
			:huart(_uart), KEY_Port(_KEY_Port), KEY_Pin(_KEY_Pin), EN_Port(_EN_Port), EN_Pin(_EN_Pin), userBaudRate(baud_rate)
	{
		// Pin default configuration for AT Mode:
	    HAL_GPIO_WritePin(EN_Port, EN_Pin, GPIO_PIN_RESET);
	    HAL_Delay(100);
		HAL_GPIO_WritePin(KEY_Port, KEY_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(EN_Port, EN_Pin, GPIO_PIN_SET);

		// Calculate correct BRR value (OVER8 = 0):
		setATModeBaudRate(baud_rate);
	}

bool HC05::isConnectionOk()
{
	return cmdWrite("AT");
}

bool HC05::restoreToFactoryState(void)
{
	std::string at_command = "AT+ORGL";

	return cmdWrite(at_command);
}

bool HC05::setBaudRate(BaudRate br_speed, StopBit stop_bit, ParityBit parity_bit)
{
	// Int -> String conversion:
	std::string br_string = std::to_string(br_speed);
	std::string sb_string = std::to_string(stop_bit);
	std::string pb_string = std::to_string(parity_bit);

	// Set AT command in appropriate command:
	std::string at_command = "AT+UART=" + br_string + "," + sb_string + "," + pb_string;

	// Return true if communication OK:
	return cmdWrite(at_command);
}

bool HC05::setRole(Role role)
{
	// Int -> String conversion:
	std::string role_string = std::to_string(role);
	std::string at_command = "AT+ROLE=" + role_string;

	// Return true if communication OK:
	return cmdWrite(at_command);
}

bool HC05::setName(std::string name)
{
	// Creating name AT command:
	std::string at_command = "AT+NAME=" + name;

	// Return true if communication OK:
	return cmdWrite(at_command);
}

bool HC05::setCustomCmd(std::string at_command)
{
	// Return true if communication OK:
	return cmdWrite(at_command);
}

std::string HC05::getBaudRate()
{
	// Command:
	std::string response = getAnswer("AT+UART?", 1000);

	// Removing "\r\n" from module answer:
    size_t pos = response.find('\r');
    if (pos != std::string::npos)
    {
        response = response.substr(0, pos);
    }
    return response;
}

std::string HC05::getINQM()
{
	// Command:
	std::string response = getAnswer("AT+INQM?", 1000);

	// Removing "\r\n" from module answer:
    size_t pos = response.find('\r');
    if (pos != std::string::npos)
    {
        response = response.substr(0, pos);
    }
    return response;
}

std::string HC05::getState(void)
{
	// Create command:
	std::string cmd = "AT+STATE?";

	// Send command:
	std::string response = getAnswer(cmd, 3000);

	// Clear command:
	size_t pos = response.find("\r\n");
	std::string clear_response = response.substr(0, pos); // tutaj się klinuje lol

	if(clear_response.empty()) return "Unknown State";

	return clear_response;
}

std::vector<Slave_info> HC05::getBluetoothDevices(uint8_t devices, uint8_t inquiring_time_sec)
{
	// Too much inquiring time error handling:
	if (inquiring_time_sec > 48) inquiring_time_sec = 48;

	// Set master role:
	bool role = setRole(Role_master);
	HAL_Delay(50);
	if (!role) return {};

	// Initialize device searching:
	bool init = cmdWrite("AT+INIT");
	HAL_Delay(50);
	if(!init) return {};

	// Set IAC command for searching all type off devices (with same baud rate):
	bool iac = cmdWrite("AT+IAC=9e8b33");
	HAL_Delay(50);
	if(!iac) return {};

	// Set inquiring parameters:
	std::string devices_string = std::to_string(devices);
	std::string inquiring_time_string = std::to_string(inquiring_time_sec);
	std::string inqm_command = "AT+INQM=0," + devices_string + "," + inquiring_time_string;

	bool inqm = cmdWrite(inqm_command);
	HAL_Delay(50);
	if(!inqm) return {};

	/// Communication data::
	char buff[1000] = {0};
	uint32_t answer_timeout_ms = inquiring_time_sec * 2000; // 2x default timeout
	std::string cmd = "AT+INQ\r\n";

	//UART communication:
	HAL_UART_Transmit(huart, (uint8_t*)cmd.c_str(), cmd.length(), 500);
	HAL_UART_Receive(huart, (uint8_t*)buff, 100, answer_timeout_ms);

	// String conversion:
	std::string response(buff);

	//  Convert to array with devices as an element:
	std::string removing_str = "\r\n";

	size_t inq_pos = 0;
	std::string dev_info[devices];

	for (uint8_t i = 0; i < devices; i++)
	{
		inq_pos = response.find(removing_str);

		if (inq_pos == std::string::npos) break;

		std::string substring = response.substr(0, inq_pos);
		response = response.substr(inq_pos + removing_str.length());

		if (substring.find("OK") != std::string::npos) break;

		dev_info[i] = substring;
	}

	// Init vector for Slave_info structures
	std::vector<Slave_info> devices_info;

	for (uint8_t i = 0; i < devices; i++)
	{
		// Skip operation for empty array elements:
		if (dev_info[i].empty()) break;

		// Find HC-05 specific info:
		std::string mac_address = findMACaddr(dev_info[i]);
		std::string name = getNameFromMAC(mac_address);
		std::string type = findDevType(dev_info[i]);
		std::string rssi = findDevRssi(dev_info[i]);

		// Add info to structure:
		Slave_info device;
		device.slave_address = mac_address;
		device.slave_name = name;
		device.slave_type = type;
		device.slave_rssi = rssi;

		devices_info.push_back(device);
	}

	return devices_info;
}

std::string HC05::getMACAddressByName(std::string search_device_name, const std::vector<Slave_info>&devices_vector)
{
	// Search for search_device_name from all:
	for (const auto& device : devices_vector)
	{
		if (device.slave_name == search_device_name) return device.slave_address;
	}

	return "Can't find MAC address...";
}

std::string HC05::getNameFromMAC(std::string mac_addr)
{
	// Check device name:
	std::string cmd = "AT+RNAME?" + MacConversion(mac_addr);
	std::string answer = getAnswer(cmd, 5000);

	// Separate an answer:
	std::string first_char = ":";
	std::string last_char = "\r\n";

	size_t pos_first = answer.find(first_char);
	size_t pos_second = answer.find(last_char);

	std::string device_name = answer.substr(pos_first + 1, pos_second - (pos_first + 1));

	return device_name;
}

std::string HC05::connectDevicesViaMACaddr(std::string slave_mac_addr)
{
	// Create command:
	std::string cmd = "AT+LINK=" + MacConversion(slave_mac_addr);

	// Communication UART:
	std::string response = getAnswer(cmd, 3000);

	// String clearing (unnecessary at this moment):
	size_t pos = response.find("\r\n");
	std::string substring = response.substr(0, pos);

	return substring;
}

std::string HC05::connectDevicesViaName(std::string slave_name, uint8_t devices_to_search, uint8_t connection_time_sec)
{
	// Iquiring time recalculation because of longer timeout inside getBluetoothDevices():
	uint8_t inquiring_time_sec = connection_time_sec/2;
	if (connection_time_sec < 8) inquiring_time_sec = 4;

	// Search for bluetooth devices and find that one with the same name:
	std::string slave_mac_addr = getMACAddressByName(slave_name, getBluetoothDevices(devices_to_search, inquiring_time_sec));
	// Create command:
	std::string cmd = "AT+LINK=" + MacConversion(slave_mac_addr);

	// Communication UART:
	std::string response = getAnswer(cmd, 3000);

	// String clearing (unnecessary at this moment):
	size_t pos = response.find("\r\n");
	std::string substring = response.substr(0, pos);

	return substring;
}

void HC05::initPinConfig(void)
{
	// Pin default configuration for AT Mode:
    HAL_GPIO_WritePin(EN_Port, EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
	HAL_GPIO_WritePin(KEY_Port, KEY_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(EN_Port, EN_Pin, GPIO_PIN_SET);

	// Calculate correct BRR value (OVER8 = 0):
	setATModeBaudRate(38400);
}

void HC05::disconnectDevices(void)
{
    HAL_GPIO_WritePin(EN_Port, EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
	HAL_GPIO_WritePin(KEY_Port, KEY_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(EN_Port, EN_Pin, GPIO_PIN_SET);

	std::string response = getAnswer("AT+DISC", 1000);
//	return cmdWrite("AT+DISC");
}

std::string HC05::findMACaddr(const std::string data)
{
	// Find MAC address from data response:
	std::string first_char = ":";
	std::string second_char = ",";

	size_t pos_first = data.find(first_char);
	size_t pos_second = data.find(second_char);
	std::string addr = data.substr(pos_first + 1, pos_second - (pos_first + 1));

	return addr;
}

std::string HC05::findDevType(const std::string data)
{
	// Find device type from data response:
	std::string comma = ",";

	size_t pos_first = data.find(comma);
	size_t pos_second = data.find(comma, pos_first + 1);
	std::string type = data.substr(pos_first + 1, pos_second - (pos_first + 1));

	return type;
}

std::string HC05::findDevRssi(const std::string data)
{
	// Find RSSI from data response:
	std::string comma = ",";

	size_t pos_first = data.find(comma);
	size_t pos_second = data.find(comma, pos_first + 1);

	std::string type = data.substr(pos_second + 1);
	//uint32_t type = std::stoul(type_str);

	return type;
}

std::string HC05::MacConversion(std::string mac_addr)
{
	// Change characters ":" -> ",":
	while(1)
	{
		uint32_t pos = mac_addr.find(":");
		if (pos == std::string::npos) break;
		mac_addr.at(pos) = ',';
	}

	return mac_addr;
}

std::string HC05::getAnswer(std::string cmd, uint16_t timeout)
{
	// Set AT mode BRR register value:
	setATModeBaudRate(38400);

	// Fill buffer with '#'
	char response[500] = {0};

	std::string full_command = cmd + "\r\n";

	// UART communication:
	HAL_StatusTypeDef transmit = HAL_UART_Transmit(huart, (uint8_t*)full_command.c_str(), full_command.length(), 1000);
	HAL_StatusTypeDef receive = HAL_UART_Receive(huart, (uint8_t*)response, sizeof(response) - 1, timeout);

	// Check communication and handle errors:
	std::string uart_transmit_error = "UART Transmit error (HAL_ERROR/HAL_BUSY)";
	std::string uart_receive_error = "UART Receive error (HAL_ERROR/HAL_BUSY)";

	if (transmit == HAL_ERROR || transmit == HAL_BUSY) return uart_transmit_error;
	if (receive == HAL_ERROR || receive == HAL_BUSY) return uart_receive_error;

	// Remove unnecessary characters:
	std::string response_str(response);

    size_t pos = response_str.find('\0');
    if (pos != std::string::npos)
    {
        response_str = response_str.substr(0, pos);
    }

    return response_str;
}

bool HC05::checkIsAnswerOK(const std::string& response)
{
	// Return false if communication failed:
	return response.find("OK")!= std::string::npos;
}

bool HC05::cmdWrite(std::string cmd)
{
	// Set AT mode BRR register value:
	setATModeBaudRate(38400);

	// String convert to "+CR +LF" format
	std::string command = cmd;
	std::string full_command = command + "\r\n";

	// UART Communication:
	char response[100] = {0};
	uint16_t timeout = 500;
	HAL_StatusTypeDef transmit = HAL_UART_Transmit(huart, (uint8_t*)full_command.c_str(), full_command.length(), timeout);
	HAL_StatusTypeDef receive = HAL_UART_Receive(huart, (uint8_t*)response, sizeof(response) - 1, timeout);

	// Response string declarations:
	std::string response_str(response);

	// Check communication and handle errors:
	if (transmit == HAL_ERROR || transmit == HAL_BUSY) return false;
	if (receive == HAL_ERROR || receive == HAL_BUSY) return false;

	bool checker = HC05::checkIsAnswerOK(response_str);

	return checker;
}

void HC05::setATModeBaudRate(uint16_t baudRate)
{
	// Calculate and Set AT mode BRR value (OVER8 = 0):
	uint32_t target_baudrate = baudRate;
	uint32_t uart_clk = HAL_RCC_GetPCLK1Freq();
	uint32_t brr = uart_clk/target_baudrate;
	huart->Instance->BRR = brr;
}


