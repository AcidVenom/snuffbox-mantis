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

	while (std::cin.get() != 'q') {}

	stream.Close();

	return 0;
}