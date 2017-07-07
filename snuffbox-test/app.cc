#include "app.h"
#include <cmath>
#include <snuffbox-engine/core/eastl.h>

namespace snuffbox
{
	namespace test
	{
		//-----------------------------------------------------------------------------------------------
		App::App() :
			Application(static_cast<size_t>(4294967296)),
			offset_(0.0f)
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		void App::OnUpdate()
		{
			unsigned char fg[3];
			unsigned char bg[3];

			offset_ += 0.05f;
			HSLToRGB(0.0f, 0.0f, 0.0f, fg);
			HSLToRGB(std::fmodf(offset_, 1.0f), 1.0f, 0.5f, bg);

			engine::String bleh = "This is some random line from the engine, aye";
			stream_.Log(console::LogSeverity::kRGB, bleh.c_str(), static_cast<int>(bleh.size()), fg, bg);
		}

		//-----------------------------------------------------------------------------------------------
		void App::HSLToRGB(float h, float s, float l, unsigned char* converted)
		{
			auto HueToRGB = [](float p, float q, float t)
			{
				if (t < 0.0f) t += 1.0f;
				if (t > 1.0f) t -= 1.0f;
				if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
				if (t < 1.0f / 2.0f) return q;
				if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
				return p;
			};

			h = std::fmaxf(0.0f, std::fminf(1.0f, h));
			s = std::fmaxf(0.0f, std::fminf(1.0f, s));
			l = std::fmaxf(0.0f, std::fminf(1.0f, l));

			float rgb[3];

			if (s == 0.0f)
			{
				rgb[0] = rgb[1] = rgb[2] = l;
			}
			else
			{
				float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
				float p = 2.0f * l - q;
				rgb[0] = HueToRGB(p, q, h + 1.0f / 3.0f);
				rgb[1] = HueToRGB(p, q, h);
				rgb[2] = HueToRGB(p, q, h - 1.0f / 3.0f);
			}

			converted[0] = static_cast<unsigned char>(rgb[0] * 255.0f);
			converted[1] = static_cast<unsigned char>(rgb[1] * 255.0f);
			converted[2] = static_cast<unsigned char>(rgb[2] * 255.0f);
		};
	}
}