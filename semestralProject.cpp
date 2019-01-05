
#include "iimavlib.h"
#include "iimavlib/WaveSource.h"
#include "iimavlib/WaveSink.h"
#include "iimavlib/Utils.h"
#include "iimavlib/AudioFilter.h"
#include "iimavlib/video_ops.h"
#include "iimavlib/SDLDevice.h"
#include "iimavlib/WaveFile.h"
#include "iimavlib_high_api.h"
#include <string>
#include <cassert>
#include <thread>
#include <cmath>
#ifdef SYSTEM_LINUX
#include <unistd.h>
#include <algorithm>

class rectangle_t;

#endif
namespace iimavlib {

	// ТУТ БУДЕМ ПИСАТЬ ВСПОМОГАТЕЛЬНЫЕ КЛАССЫ

	//	КЛАСС ДОЖДЬ		----М

	// 	КЛАСС БЕЛЫЙ ШУМ		---- Б

	class WhiteNoise : public AudioFilter {
	public:
		WhiteNoise() : AudioFilter(pAudioFilter()) {
			load_file("waw path");
			if (white_noise.size() == 0) throw std::runtime_error("Failed to load drum samples!");
		}


	private:

		std::vector<std::vector<audio_sample_t>> white_noise;

		int index_;

		size_t position_;

		std::mutex position_mutex_;

		//ЭТА ШТУКА ИГРАЕТ ЗВУК (И ВОЗМОЖНО ДЕЛАЕТ ЕЩЕ ЧТО-ТО)
		error_type_t do_process(audio_buffer_t &buffer) override {

			// Get iterator to the data in the buffer
			auto data = buffer.data.begin();

			if (index_ < 0 || (white_noise.size() <= static_cast<size_t>(index_))) {
				// If there's no active drum, we just fill the buffer with zeroes
				std::fill(data, data + buffer.valid_samples, 0);
			}
			else {
				//logger[log_level::info] << "Using " << index_ << " from " << position_;
				std::unique_lock<std::mutex> lock(position_mutex_);
				// Get ref. to the current drum's buffer
				const auto& drum = white_noise[index_];
				size_t samples = white_noise[index_].size();
				size_t remaining = buffer.valid_samples;
				size_t written = 0;
				if (position_ < samples) {
					// We still have some non-copied samples
					const size_t avail = samples - position_; // How many samples are available current drum
					written = (avail >= remaining) ? remaining : avail; // We will copy this count of samples.
					auto first = drum.cbegin() + position_;		// Iterator to first sample to copy
					auto last = (avail >= remaining) ? first + remaining : drum.cend(); // Iterator after the last sample that will be written
					std::copy(first, last, data); // Copy the samples to the buffer
					position_ += written; // Advance the drum's buffer position
					remaining -= written;
				}
				else {
					// We've already copied all the sample, so let's set current drum to none
					index_ = -1;
					position_ = 0;
				}
				// Fill the rest of the buffer (if there's still some space) with zeroes
				std::fill(data + written, data + written + remaining, 0);
			}
			// Update display (because we changed the position_)
			return error_type_t::ok;
		}

		//ЭТА ШТУКА ЗАГРУЖАЕТ ФАЙЛ В КЛАСС
		bool load_file(const std::string filename)
		{
			try {
				WaveFile wav(filename);  // Load wave file
				const audio_params_t params = wav.get_params();
				if (params.rate != sampling_rate_t::rate_44kHz) throw std::runtime_error("Wrong sampling rate. 44kHz expected.");
				//			if (params.format != sampling_format_t::format_16bit_signed) throw std::runtime_error("Wrong sampling format. Signed 16bits expected.");
				//			if (params.num_channels != 2) throw std::runtime_error("Wrong number of channel. Expected stereo file.");
				size_t samples = 44100;
				std::vector<audio_sample_t> data(samples);
				wav.read_data(data, samples);
				data.resize(samples);
				logger[log_level::info] << "Read " << samples << "samples";
				white_noise.push_back(std::move(data));
			}
			catch (std::exception &e) {
				logger[log_level::fatal] << "Failed to load " << filename << " (" << e.what() << ")";
				return false;
			}
			return true;
		}

	};


	//	КЛАСС РОЗОВЫЙ ШУМ		----Б

	//	КЛАСС КОРИЧНЕВЫЙ ШУМ	----Б

	//	КЛАСС ДЕЛЬТА ИЗОХРОНИЧЕСКИЙ РИТМ

	//	КЛАСС АЛЬФА ИЗОХРОНИЧЕСКИЙ РИТМ

	//	КЛАСС БЕТА ИЗОХРОНИЧЕСКИЙ РИТМ

	//	КЛАСС ГАММА ИЗОХРОНИЧЕСКИЙ РИТМ


}


void rain(iimavlib::video_buffer_t data, int t) {
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 20, t, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 40, t, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 60, t, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка

	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 1, t + 10, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 10, t + 10, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 30, t + 10, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 50, t + 10, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка

}

void playNoise() {
	iimavlib::audio_id_t device_id = iimavlib::PlatformDevice::default_device();

	auto sink = iimavlib::filter_chain<iimavlib::WhiteNoise>().add<iimavlib::PlatformSink>(device_id).sink();
	sink->run();

	sink->stop();
}
iimavlib::rgb_t random(int mode) {

	std::vector<iimavlib::rgb_t> shades_pink = {
		iimavlib::rgb_t(255, 192, 203),
		iimavlib::rgb_t(255,182,193),
		iimavlib::rgb_t(255,105,180),
		iimavlib::rgb_t(255,20,147),
		iimavlib::rgb_t(219,112,147),
		iimavlib::rgb_t(199,21,133)
	};

	std::vector<iimavlib::rgb_t> shades_brown = {
		iimavlib::rgb_t(139,69,19),
		iimavlib::rgb_t(160,82,45),
		iimavlib::rgb_t(128,0,0),
		iimavlib::rgb_t(165,42,42),
		iimavlib::rgb_t(222,184,135),
		iimavlib::rgb_t(210,180,140)
	};

	int index = 0 + (std::rand() % (5 - 0 + 1));

	switch (mode) {
	case 0: {
		int s = 0 + (std::rand() % (255 - 0 + 1));
		return iimavlib::rgb_t(s, s, s);
	};
	case 1:
		return shades_pink[index];
	case 2:
		return shades_brown[index];

	default:
	{return iimavlib::rgb_t(13, 21, 133); };
	}

};

void openWindow() {
	iimavlib::SDLDevice sdl(800, 600,
		"send help"); //задается размер окна и название окна (название отражает реальность)
	iimavlib::rgb_t color(255, 230, 230); //задается цвет c помощью трех значений ргб
	iimavlib::video_buffer_t data(sdl.get_size(), color); // рендер окна и цвета
	sdl.start(); //поехали

	iimavlib::video_buffer_t bg(data.size, color); //рендер

	int t = 0;

	int mode = 2;

	while (sdl.blit(data)) {
		data = bg;



		int xy = 100;



		//STROKA 1
		iimavlib::rgb_t  sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(0, 0, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(xy, 0, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(2 * xy, 0, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(3 * xy, 0, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(4 * xy, 0, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(5 * xy, 0, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(6 * xy, 0, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(7 * xy, 0, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(8 * xy, 0, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		//STROKA 2
		draw_rectangle(data, iimavlib::rectangle_t(0, xy, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(xy, xy, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		//STROKA 3
		draw_rectangle(data, iimavlib::rectangle_t(0, xy * 2, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 2, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 2, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 2, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 2, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 2, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 2, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 2, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 2, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		//STROKA 4
		draw_rectangle(data, iimavlib::rectangle_t(0, xy * 3, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 3, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 3, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 3, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 3, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 3, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 3, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 3, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 3, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		//STROKA 5
		draw_rectangle(data, iimavlib::rectangle_t(0, xy * 4, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 4, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 4, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 4, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 4, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 4, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 4, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 4, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 4, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		//STROKA 6
		draw_rectangle(data, iimavlib::rectangle_t(0, xy * 5, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 5, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 5, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 5, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 5, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 5, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 5, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 5, xy, xy), sh); //цвет кружочка
		sh = random(mode);
		draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 5, xy, xy), sh); //цвет кружочка
		sh = random(mode);

		if (t < 255) { t++; }
		else { t = 0; };

	}

	sdl.stop();
}

int main() {
	std::thread first(openWindow);
	std::thread second(playNoise);

	std::cout << "main, foo and bar now execute concurrently...\n";

	// synchronize threads
	first.join();                // pauses until first finishes

//    second.join();  ВОТ КОГДА ЭТА ХЕРНЯ ЗАКОМЕНЧЕНА, ВСЕ ПРЕКРАСНО, НЕ ЗНАЮ ПОЧЕМУ, А ЕСЛИ РАСКОМЕНТИТЬ ТО ПРОГРАММА НЕ ОСТАНОВИТСЯ

	std::cout << "foo and bar completed.\n";
	return 0;

	//окно, которое, наверно, закрывается
}
