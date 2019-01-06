
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

	class Noize : public SDLDevice, public AudioFilter{
	public:
		static const rgb_t black;
		Noize(int width, int height) :
				SDLDevice(width, height, "send the help please"),
				AudioFilter(pAudioFilter()),
				data(rectangle_t(0, 0, width, height), black)
		{
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

		int mode = 1;

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
			mode++;
			if (mode > 2) mode = 0;
			return true;
		}

		void update_screen() {

			int xy = 100;
			//STROKA 1
			iimavlib::rgb_t  sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(0, 0, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(xy, 0, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(2 * xy, 0, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(3 * xy, 0, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(4 * xy, 0, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(5 * xy, 0, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(6 * xy, 0, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(7 * xy, 0, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(8 * xy, 0, xy, xy), sh); //цвет кружочка
			sh = random();
			//STROKA 2
			draw_rectangle(data, iimavlib::rectangle_t(0, xy, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(xy, xy, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy, xy, xy), sh); //цвет кружочка
			sh = random();
			//STROKA 3
			draw_rectangle(data, iimavlib::rectangle_t(0, xy * 2, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 2, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 2, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 2, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 2, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 2, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 2, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 2, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 2, xy, xy), sh); //цвет кружочка
			sh = random();
			//STROKA 4
			draw_rectangle(data, iimavlib::rectangle_t(0, xy * 3, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 3, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 3, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 3, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 3, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 3, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 3, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 3, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 3, xy, xy), sh); //цвет кружочка
			sh = random();
			//STROKA 5
			draw_rectangle(data, iimavlib::rectangle_t(0, xy * 4, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 4, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 4, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 4, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 4, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 4, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 4, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 4, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 4, xy, xy), sh); //цвет кружочка
			sh = random();
			//STROKA 6
			draw_rectangle(data, iimavlib::rectangle_t(0, xy * 5, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(xy, xy * 5, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(2 * xy, xy * 5, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(3 * xy, xy * 5, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(4 * xy, xy * 5, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(5 * xy, xy * 5, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(6 * xy, xy * 5, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(7 * xy, xy * 5, xy, xy), sh); //цвет кружочка
			sh = random();
			draw_rectangle(data, iimavlib::rectangle_t(8 * xy, xy * 5, xy, xy), sh); //цвет кружочка
			sh = random();

			blit(data);

		}

		error_type_t do_process(audio_buffer_t& buffer)
		{
			if (is_stopped()) return error_type_t::failed;
			update_screen();
			return error_type_t::ok;
		}

	};

	const rgb_t Noize::black(10, 0, 0);
};


void rain(iimavlib::video_buffer_t data, int t) {
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 20, t, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 40, t, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 60, t, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка

	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 1, t + 10, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 10, t + 10, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 30, t + 10, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка
	draw_rectangle(data, iimavlib::rectangle_t(t / 10 + 50, t + 10, 4, 15), iimavlib::rgb_t(255, 179, 102)); //цвет кружочка

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
