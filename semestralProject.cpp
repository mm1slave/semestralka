
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

    class WhiteNoise :  public AudioFilter {
    public:
        WhiteNoise(): AudioFilter(pAudioFilter()){
            load_file("/home/boris/mm1semwork/iimavlib-master/data/drum0.wav");
            if (white_noise.size()==0) throw std::runtime_error("Failed to load drum samples!");
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

            if (index_ < 0 || (white_noise.size()<=static_cast<size_t>(index_))) {
                // If there's no active drum, we just fill the buffer with zeroes
                std::fill(data,data+buffer.valid_samples,0);
            } else {
                //logger[log_level::info] << "Using " << index_ << " from " << position_;
                std::unique_lock<std::mutex> lock(position_mutex_);
                // Get ref. to the current drum's buffer
                const auto& drum = white_noise[index_];
                size_t samples = white_noise[index_].size();
                size_t remaining = buffer.valid_samples;
                size_t written = 0;
                if (position_<samples) {
                    // We still have some non-copied samples
                    const size_t avail = samples - position_; // How many samples are available current drum
                    written = (avail>=remaining)?remaining:avail; // We will copy this count of samples.
                    auto first = drum.cbegin()+position_;		// Iterator to first sample to copy
                    auto last = (avail>=remaining)?first+remaining:drum.cend(); // Iterator after the last sample that will be written
                    std::copy(first,last,data); // Copy the samples to the buffer
                    position_+=written; // Advance the drum's buffer position
                    remaining-=written;
                } else {
                    // We've already copied all the sample, so let's set current drum to none
                    index_ = -1;
                    position_ = 0;
                }
                // Fill the rest of the buffer (if there's still some space) with zeroes
                std::fill(data+written, data+written+remaining, 0);
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
                wav.read_data(data,samples);
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


void playNoise() {
    iimavlib::audio_id_t device_id = iimavlib::PlatformDevice::default_device();

    auto sink = iimavlib::filter_chain<iimavlib::WhiteNoise>().add<iimavlib::PlatformSink>(device_id).sink();
    sink->run();

    sink->stop();
}

void openWindow() {
    iimavlib::SDLDevice sdl(800, 600,
                            "send help"); //задается размер окна и название окна (название отражает реальность)
    iimavlib::rgb_t color(255, 230, 230); //задается цвет c помощью трех значений ргб
    iimavlib::video_buffer_t data(sdl.get_size(), color); //рендер окна и цвета
    sdl.start(); //поехали
     iimavlib::rectangle_t position(data.size.width / 2, data.size.height / 2, 4, 58); //злые формулы задают позицию кружочка
    iimavlib::video_buffer_t bg(data.size, color); //рендер

    while (sdl.blit(data)) {
        draw_circle(data, position, iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
    }

    sdl.stop();
}

int main() {
    std::thread first (openWindow);
    std::thread second (playNoise);

    std::cout << "main, foo and bar now execute concurrently...\n";

    // synchronize threads
    first.join();                // pauses until first finishes

//    second.join();  ВОТ КОГДА ЭТА ХЕРНЯ ЗАКОМЕНЧЕНА, ВСЕ ПРЕКРАСНО, НЕ ЗНАЮ ПОЧЕМУ, А ЕСЛИ РАСКОМЕНТИТЬ ТО ПРОГРАММА НЕ ОСТАНОВИТСЯ

    std::cout << "foo and bar completed.\n";
    return 0;

     //окно, которое, наверно, закрывается
}
