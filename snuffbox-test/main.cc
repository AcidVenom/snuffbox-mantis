#include <memory/allocators.h>
#include <stdio.h>
#include <EASTL/string.h>
#include <iostream>

#define SNUFF_INCLUDE_WINSOCK
#include <logging_stream.h>
#include <connection/logging_client.h>

class A
{
public:

	A() {}

	A(const int& f, const bool& b, const size_t& s, const int& argc, char** argv) :
		foo(f),
		bar(b),
		some_value(s)
	{
		for (int i = 0; i < argc; ++i)
		{
			printf("%s\n", argv[i]);
		}
	}

	int foo;
	bool bar;
	size_t some_value;
};

class Client : public snuffbox::LoggingClient
{
	void OnConnect(const bool& stream_quit) const override
	{
		printf("CLIENT CONNECTED\n");
	}

	void OnDisconnect(const bool& stream_quit) const override
	{
		printf("CLIENT DISCONNECTED\n");
	}
};

int main(int argc, char** argv)
{
	snuffbox::MallocAllocator alloc(1024 * 1024 * 4);
	eastl::basic_string<char, snuffbox::EASTLAllocator<snuffbox::MallocAllocator>> testString = "This is a test string.";
	printf("EASTL string: %s\n", testString.c_str());
	testString.at(2) = '!';
	printf("EASTL string (change i to !): %s\n", testString.c_str());
	testString.erase(testString.begin() + testString.size() - 1);
	printf("EASTL string (remove last character): %s\n", testString.c_str());

	void* ptr = alloc.Malloc(4000);
	alloc.Free(ptr);

	A* test = alloc.Construct<A>(5, false, 1283917, argc, argv);

	printf("foo: %i, bar: %s, some_value: %zu\n", test->foo, test->bar == true ? "true" : "false", test->some_value);

	alloc.Destruct(test);

	snuffbox::LoggingStream stream;
	Client client;
	stream.Open(&client);

	auto HSLToRGB = [](float h, float s, float l, unsigned char* converted)
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

	float offset = 0.0f;
	unsigned char fg[3];
	unsigned char bg[3];

	while (std::cin.get() != 'q') 
	{
		offset += 0.05f;
		HSLToRGB(0.0f, 0.0f, 0.0f, fg);
		HSLToRGB(std::fmodf(offset, 1.0f), 1.0f, 0.5f, bg);

		std::string bleh = "This is some random line from the engine, aye";
		stream.Log(snuffbox::LogSeverity::kRGB, bleh.c_str(), static_cast<int>(bleh.size()), fg, bg);
	}

	stream.Close();

	return 0;
}