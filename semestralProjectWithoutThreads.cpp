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

    class Noize : public SDLDevice, public AudioFilter {
    public:
        static const rgb_t bg;
        Noize(int width, int height) :
                SDLDevice(width, height, "send the help please"),
                AudioFilter(pAudioFilter()),
                data(rectangle_t(0, 0, width, height), bg)
        {

            load_file("/home/boris/mm1semwork/iimavlib-master/data/drum0.wav");
            load_file("/home/boris/mm1semwork/iimavlib-master/data/white.wav");
            load_file("/home/boris/mm1semwork/iimavlib-master/data/drum2.wav");
            // Start the rendering thread
            start();
        };

        ~Noize() {
            // Stop the rendering thread
            stop();
        }
    private:
        /// Video data
        video_buffer_t data;

        std::vector<std::vector<audio_sample_t>> noises;
        /// Next sample to be played for current drum
        size_t position_;
        /// Index of currently playing drum
        int index_;
        /// Mutex to lock @em index_ and @em position_
        std::mutex position_mutex_;

        int mode = 3;
        int t = 0;

        iimavlib::rgb_t random() {

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

        bool do_mouse_button(const int button, const bool pressed, const int x, const int y)
        {
            if (!pressed) return true;
            std::unique_lock<std::mutex> lock(position_mutex_);
            mode++;
            index_ = mode - 1;
            position_ = 0;

            if (mode > 3) mode = 0;
            return true;
        }

        void update_screen() {
            if (mode == 3) {
                update_city();
            }
            else {
                update_noize();
            }
        }

        iimavlib::rgb_t window_color = iimavlib::rgb_t(0, 0, t);

        void rain() {

            iimavlib::rgb_t rain_color = iimavlib::rgb_t(255, 255, 255);

            draw_rectangle(data, iimavlib::rectangle_t(t / 10, t * 3, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 240, t * 3, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 360, t * 3, 4, 15), rain_color);

            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 100, t * 3 + 100, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 260, t * 3+ 180, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 350, t * 3+ 130, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 450, t * 3+ 210, 4, 15), rain_color);

            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 400, t * 3, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 540, t * 3, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 560, t * 3, 4, 15), rain_color);

            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 420, t * 3 + 110, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 580, t * 3 + 310, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 310, t * 3+ 170, 4, 15), rain_color);
            draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 490, t * 3+ 270, 4, 15), rain_color);

        }

        void update_city() {
            t++;
            //draw sky
            draw_rectangle(data, iimavlib::rectangle_t(0, 0, 800, 200), iimavlib::rgb_t(0, 102, 204));
            draw_rectangle(data, iimavlib::rectangle_t(0, 200, 800, 200), iimavlib::rgb_t(0, 128, 255));
            draw_rectangle(data, iimavlib::rectangle_t(0, 400, 800, 200), iimavlib::rgb_t(102, 178, 255));

            //draw houses
            iimavlib::rgb_t houses_color = iimavlib::rgb_t(0, 0, 102);
            draw_rectangle(data, iimavlib::rectangle_t(10, 100, 120, 500), iimavlib::rgb_t(0, 0, 102));
            draw_rectangle(data, iimavlib::rectangle_t(130, 250, 170, 350), iimavlib::rgb_t(0, 0, 32));
            draw_rectangle(data, iimavlib::rectangle_t(300, 150, 120, 450), iimavlib::rgb_t(0, 0, 142));
            draw_rectangle(data, iimavlib::rectangle_t(420, 200, 80, 400), iimavlib::rgb_t(0, 0, 82));

            // draw windows
            window_color = iimavlib::rgb_t(0, 0, t*5);
            draw_rectangle(data, iimavlib::rectangle_t(150, 280, 50, 60), window_color);
            draw_rectangle(data, iimavlib::rectangle_t(210, 280, 50, 60), window_color);
            draw_rectangle(data, iimavlib::rectangle_t(150, 350, 50, 60), window_color);
            draw_rectangle(data, iimavlib::rectangle_t(210, 350, 50, 60), window_color);

            rain();

            blit(data);
        }

        void update_noize() {

            int xy = 100;
            //STROKA 1
            iimavlib::rgb_t  sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(0, 0, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(xy, 0, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(2 * xy, 0, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(3 * xy, 0, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(4 * xy, 0, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(5 * xy, 0, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(6 * xy, 0, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(7 * xy, 0, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(8 * xy, 0, xy, xy), sh);
            sh = random();
            //STROKA 2
            draw_rectangle(data, iimavlib::rectangle_t(0, xy, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(xy, xy, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy, xy, xy), sh);
            sh = random();
            //STROKA 3
            draw_rectangle(data, iimavlib::rectangle_t(0, xy * 2, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 2, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 2, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 2, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 2, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 2, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 2, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 2, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 2, xy, xy), sh);
            sh = random();
            //STROKA 4
            draw_rectangle(data, iimavlib::rectangle_t(0, xy * 3, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 3, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 3, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 3, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 3, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 3, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 3, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 3, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 3, xy, xy), sh);
            sh = random();
            //STROKA 5
            draw_rectangle(data, iimavlib::rectangle_t(0, xy * 4, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 4, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 4, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 4, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 4, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 4, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 4, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 4, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 4, xy, xy), sh);
            sh = random();
            //STROKA 6
            draw_rectangle(data, iimavlib::rectangle_t(0, xy * 5, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 5, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 5, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 5, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 5, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 5, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 5, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 5, xy, xy), sh);
            sh = random();
            draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 5, xy, xy), sh);
            sh = random();

            blit(data);
        }

        error_type_t do_process(audio_buffer_t& buffer)
        {
            t++;
            if (t > 200) t = 0;
            if (is_stopped()) return error_type_t::failed;
            //		const audio_params_t& params = buffer.params;

            // Get iterator to the data in the buffer
            auto data = buffer.data.begin();

            if (index_ < 0 || (noises.size() <= static_cast<size_t>(index_))) {
                // If there's no active drum, we just fill the buffer with zeroes
                std::fill(data, data + buffer.valid_samples, 0);
            }
            else {
                //logger[log_level::info] << "Using " << index_ << " from " << position_;
                std::unique_lock<std::mutex> lock(position_mutex_);
                // Get ref. to the current drum's buffer
                const auto& drum = noises[index_];
                size_t samples = noises[index_].size();
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




            update_screen();
            return error_type_t::ok;
        }

        bool load_file(const std::string filename)
        {
            try {
                WaveFile wav(filename);  // Load wave file
                const audio_params_t params = wav.get_params();
                if (params.rate != sampling_rate_t::rate_44kHz) throw std::runtime_error("Wrong sampling rate. 44kHz expected.");
                //			if (params.format != sampling_format_t::format_16bit_signed) throw std::runtime_error("Wrong sampling format. Signed 16bits expected.");
                //			if (params.num_channels != 2) throw std::runtime_error("Wrong number of channel. Expected stereo file.");
                size_t samples = 944100;
                std::vector<audio_sample_t> data(samples);
                wav.read_data(data, samples);
                data.resize(samples);
                logger[log_level::info] << "Read " << samples << "samples";
                noises.push_back(std::move(data));
            }
            catch (std::exception &e) {
                logger[log_level::fatal] << "Failed to load " << filename << " (" << e.what() << ")";
                return false;
            }
            return true;
        };

    };

    const rgb_t Noize::bg(10, 0, 0);
};




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
        //	update_screen(data, mode);
    };
    sdl.stop();
}

int main()
try
{
    using namespace iimavlib;
    iimavlib::audio_id_t device_id = iimavlib::PlatformDevice::default_device();
    iimavlib::audio_params_t params;
    params.rate = iimavlib::sampling_rate_t::rate_44kHz;
    auto sink = iimavlib::filter_chain<iimavlib::Noize>(800, 600)
            .add<iimavlib::PlatformSink>(device_id)
            .sink();

    //	sink->set_buffers(1,128);
    sink->run();
}
catch (std::exception& e) {
    using namespace iimavlib;
    logger[log_level::fatal] << "The application ended unexpectedly with an error: " << e.what();
}