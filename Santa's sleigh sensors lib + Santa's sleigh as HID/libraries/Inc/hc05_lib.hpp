/*
 * About HC-05:
 *
 *  HC-05 module is an easy to use Bluetooth SPP (Serial Port Protocol) module, designed for transparent wireless serial connection setup.
 *  Serial port Bluetooth module is fully qualified Bluetooth V2.0+EDR (Enhanced Data Rate) 3Mbps Modulation with complete 2.4GHz radio transceiver and baseband.
 *  It uses CSR Bluecore 04-External single chip Bluetooth system with CMOS technology and with AFH(Adaptive Frequency Hopping Feature).
 *
 * HC-05 Bluetooth module library:
 *
 *  - is in AT mode as soon as the power supply is connected (pin outputs sequence inside constructor / initPinConfig() method),
 *  - is working only in default CMODE so he can pair with every device - you shouldn't use binding,
 *  - getBluetoothDevices() method is saving devices around us as an element in std::vector<Slave_info>,
 *  - detailed bluetooth parameters are in Slave_info structure.
 *
 * */

#ifndef INC_HC05_LIB_HPP_
#define INC_HC05_LIB_HPP_

#include "main.h"
#include "string"
#include <vector>
#include <cstring>
#include <string>

// Structure declaration:
struct Slave_info;

// Enums declarations:
enum Role
{
	Role_slave,
	Role_master,
	Role_slaveLoop
};

enum BaudRate
{
    BaudRate_4800 = 4800,
	BaudRate_9600 = 9600,
	BaudRate_19200 = 19200,
	BaudRate_38400 = 38400,
	BaudRate_57600 = 57600,
	BaudRate_115200 = 115200,
	BaudRate_230400 = 230400,
	BaudRate_460800 = 460800,
	BaudRate_921600 = 921600,
	BaudRate_1382400 = 1382400
};

enum StopBit
{
	StopBit_1,
	StopBit_2
};

enum ParityBit
{
	Parity_None,
	Parity_Odd_parity,
	Parity_Even_parity
};

// HC-05 class:
class HC05{
public:
    HC05(UART_HandleTypeDef* _uart, GPIO_TypeDef* _KEY_Port, uint16_t _KEY_Pin, GPIO_TypeDef* _EN_Port, uint16_t _EN_Pin, uint32_t baud_rate);

    // AT Connection check:
    bool isConnectionOk();

    bool restoreToFactoryState(void);

    void disconnectDevices(void);

    void initPinConfig(void);

    // Set connection parameters:
    bool setBaudRate(BaudRate br_speed, StopBit stop_bit, ParityBit parity_bit);

    bool setRole(Role role);

    bool setName(std::string name);

    bool setCustomCmd(std::string at_command);

    // Get device parameters methods:
    std::string getINQM(void);

    std::string getBaudRate(void);

    std::string getState(void);

    std::string getNameFromMAC(std::string mac_addr);

    // Search main methods:
    std::vector<Slave_info> getBluetoothDevices(uint8_t devices, uint8_t inquiring_time);

    std::string getMACAddressByName(std::string search_device_name, const std::vector<Slave_info>&devices_vector);

    std::string connectDevicesViaMACaddr(std::string mac_addr);

    std::string connectDevicesViaName(std::string slave_name, uint8_t devices_to_search, uint8_t inquiring_time_sec);

private:

    // Helpful private methods:
    bool cmdWrite(std::string cmd);

    std::string getAnswer(std::string cmd, uint16_t timeout);

    void setATModeBaudRate(uint16_t baudRate);

    bool checkIsAnswerOK(const std::string& response);

    // Parsing methods:
    std::string findMACaddr(const std::string data);

    std::string findDevType(const std::string data);

    std::string findDevRssi(const std::string data);

    std::string MacConversion(std::string mac_addr);


	UART_HandleTypeDef* huart;

	GPIO_TypeDef* KEY_Port;
	uint16_t KEY_Pin;

	GPIO_TypeDef* EN_Port;
	uint16_t EN_Pin;

	uint32_t userBaudRate;
};

// Slave parameters structure info:
struct Slave_info{
	std::string slave_address;
	std::string slave_name;
	std::string slave_type;
	std::string slave_rssi;
};

#endif
