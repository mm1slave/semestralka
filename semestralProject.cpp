
#include "iimavlib.h"
#include "iimavlib/WaveSource.h"
#include "iimavlib/WaveSink.h"
#include "iimavlib/Utils.h"
#include "iimavlib/AudioFilter.h"
#include "iimavlib/video_ops.h"
#include "iimavlib/SDLDevice.h"
#include <string>
#include <cassert>
#include <cmath>

// ТУТ БУДЕМ ПИСАТЬ ВСПОМОГАТЕЛЬНЫЕ КЛАССЫ

//	КЛАСС ДОЖДЬ

// 	КЛАСС БЕЛЫЙ ШУМ

//	КЛАСС РОЗОВЫЙ ШУМ

//	КЛАСС КОРИЧНЕВЫЙ ШУМ

//	КЛАСС ДЕЛЬТА ИЗОХРОНИЧЕСКИЙ РИТМ

//	КЛАСС АЛЬФА ИЗОХРОНИЧЕСКИЙ РИТМ

//	КЛАСС БЕТА ИЗОХРОНИЧЕСКИЙ РИТМ

//	КЛАСС ГАММА ИЗОХРОНИЧЕСКИЙ РИТМ

int main() {
    iimavlib::SDLDevice sdl(800, 600,
                            "send help"); //задается размер окна и название окна (название отражает реальность)
    iimavlib::rgb_t color(255, 230, 230); //задается цвет c помощью трех значений ргб
    iimavlib::video_buffer_t data(sdl.get_size(), color); //рендер окна и цвета
    sdl.start(); //поехали

    iimavlib::rectangle_t position(data.size.width / 2, data.size.height / 2, 128,
                                   128); //злые формулы задают позицию кружочка
    iimavlib::video_buffer_t bg(data.size, color); //рендер

    while (sdl.blit(data)) {
        draw_circle(data, position, iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
    }

    sdl.stop(); //окно, которое, наверно, закрывается
}


