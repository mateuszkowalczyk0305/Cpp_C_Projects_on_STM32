/*
 * oled.hpp
 *
 *  Created on: Oct 11, 2024
 *      Author: user
 */

#ifndef INC_OLED_HPP_
#define INC_OLED_HPP_

#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "ssd1306.h"
#include <vector>

constexpr uint8_t x_default = 100;
constexpr uint8_t y_default = 60;

struct bitmapStruct{
	const unsigned char* bitmap;
	int xpos = 0;
	int ypos = 0;
	int xsize = 0;
	int ysize = 0;
};

enum Bitmaps{
	BMP_READY,
	BMP_GO,
	BMP_1,
	BMP_2,
	BMP_3,
	BMP_4,
	BMP_5
};

class OLED {
public:
	OLED(SPI_HandleTypeDef* _spi_port, GPIO_TypeDef* _cs_port, uint16_t _cs_pin,
		 GPIO_TypeDef* _dc_port, uint16_t _dc_pin, GPIO_TypeDef* _reset_port, uint16_t _reset_pin);

	void init();
	void fill(SSD1306_COLOR color);


	void addSegment(const unsigned char* bmp_name, uint8_t x_position, uint8_t y_position, uint8_t x_size, uint8_t y_size);

	void draw();
	void jumpNext();
	void jumpPrevious();

	void setSegment(uint8_t target_segment);
	int current_segment;

private:
	void drawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t width, uint8_t height, SSD1306_COLOR color);
	void updateScreen();

    SPI_HandleTypeDef* spi_port;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef* dc_port;
    uint16_t dc_pin;
    GPIO_TypeDef* reset_port;
    uint16_t reset_pin;


    std::vector<bitmapStruct> bitmaps;
};


#endif /* INC_OLED_HPP_ */
