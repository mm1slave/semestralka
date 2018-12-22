
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

using namespace iimavlib;

namespace {
}


// ТУТ БУДЕМ ПИСАТЬ ВСПОМОГАТЕЛЬНЫЕ КЛАССЫ

//	КЛАСС ДОЖДЬ

// 	КЛАСС БЕЛЫЙ ШУМ

//	КЛАСС РОЗОВЫЙ ШУМ

//	КЛАСС КОРИЧНЕВЫЙ ШУМ

//	КЛАСС ДЕЛЬТА ИЗОХРОНИЧЕСКИЙ РИТМ

//	КЛАСС АЛЬФА ИЗОХРОНИЧЕСКИЙ РИТМ

//	КЛАСС БЕТА ИЗОХРОНИЧЕСКИЙ РИТМ

//	КЛАСС ГАММА ИЗОХРОНИЧЕСКИЙ РИТМ




int main() try
{
	//СОЗДАЛИ ПУСТОЕ ЧЕРНОЕ ОКНО
	iimavlib::SDLDevice sdl(800, 600, "Semestralka!");
 	iimavlib::rgb_t black (0,0,0);
 	iimavlib::video_buffer_t data(sdl.get_size(),black);  //ХЗ ЧТО ЭТО И ЗАЧЕМ ???
 	sdl.start(); 	//ЗАПУСТИЛИ ПУСТОЕ ЧЕРНОЕ ОКНО

	while (true){
		continue;
		}



	sdl.stop(); // ЗАКРЫЛИ ЧЕРНОЕ ОКНО (но оно мгновенно закрывается само)


}
catch (std::exception& e)
{
	logger[log_level::fatal] << "ERROR: An error occurred during program run: " << e.what();
}
