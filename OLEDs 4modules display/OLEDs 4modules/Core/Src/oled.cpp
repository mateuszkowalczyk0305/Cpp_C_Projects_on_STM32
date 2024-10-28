/*
 * oled.cpp
 *
 *  Created on: Oct 11, 2024
 *      Author: user
 */

#include "oled.hpp"

OLED::OLED(SPI_HandleTypeDef* _spi_port, GPIO_TypeDef* _cs_port, uint16_t _cs_pin,
	 GPIO_TypeDef* _dc_port, uint16_t _dc_pin, GPIO_TypeDef* _reset_port, uint16_t _reset_pin)
	: spi_port(_spi_port), cs_port(_cs_port), cs_pin(_cs_pin),
	  dc_port(_dc_port), dc_pin(_dc_pin), reset_port(_reset_port), reset_pin(_reset_pin), current_segment(0)
{}

void OLED::addSegment(const unsigned char* bmp_name, uint8_t x_position, uint8_t y_position, uint8_t x_size, uint8_t y_size){
	bitmapStruct bmp;

	bmp.bitmap = bmp_name;
	bmp.xpos = x_position;
	bmp.ypos = y_position;
	bmp.xsize = x_size;
	bmp.ysize = y_size;

	bitmaps.push_back(bmp);
}

void OLED::jumpNext(){
	uint8_t size = static_cast<uint8_t>(bitmaps.size() - 1);
	current_segment++;
	if (current_segment > size){
		current_segment = 0;
	}
	draw();
}

void OLED::jumpPrevious(){
	uint8_t size = static_cast<uint8_t>(bitmaps.size() - 1);

	current_segment--;
	if (current_segment < 0){
		current_segment = size;
	}
	draw();
}

void OLED::setSegment(uint8_t target_segment){
	current_segment = target_segment;
	draw();
};

void OLED::draw(){
	fill(Black);

	drawBitmap(
			bitmaps.at(current_segment).xpos,
			bitmaps.at(current_segment).ypos,
			bitmaps.at(current_segment).bitmap,
			bitmaps.at(current_segment).xsize,
			bitmaps.at(current_segment).ysize,
			White);
}

void OLED::init() {
    ssd1306_Init(spi_port, cs_port, cs_pin, dc_port, dc_pin, reset_port, reset_pin);
    fill(Black);
}

void OLED::fill(SSD1306_COLOR color) {
    ssd1306_Fill(color);
    updateScreen();
}

void OLED::drawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t width, uint8_t height, SSD1306_COLOR color) {
    ssd1306_DrawBitmap(spi_port , cs_port, cs_pin, dc_port, dc_pin, x, y, bitmap, width, height, color);
    updateScreen();
}

void OLED::updateScreen() {
    ssd1306_UpdateScreen(spi_port, cs_port, cs_pin, dc_port, dc_pin);
}
