/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/examples/hello.cpp
 *
 *   Basic "hello world" application using the CLI framework.
 */

#include <iostream>
#include "CLI/SingleMode.hpp"


class Hello: public CLI::SingleMode {
public:

	CLI::List   users    {"user", "user names"};
	CLI::Switch verbose  {"verbose", "1", "show greeting message", "v"};
	CLI::Switch computer {"computer", "Lisa", "name of the computer", "c"};
	CLI::Switch message  {"message", "how are you?", "the greeting used", "m"};

	CLI::Terminator version {"version", "show version info",
		"hello.exe (1.0.1)", "V"};

	CLI::Terminator copyright {"copyright", "show copyright info",
		"hello.exe (1.0.1)\nCopyright (c) Geoffrey Lentner 2015. All rights reserved."
		"\nGNU General Public License v3.0\nThis is free software; see the source for"
		" copying conditions.  There is NO\nwarranty; not even for MERCHANTABILITY or"
		" FITNESS FOR A PARTICULAR PURPOSE.", "C"};

	Hello(const int argc, const char **argv): SingleMode(argc, argv,

		"A simple `Hello, world!` program to showcase the CLI::SingleMode framework.\n"
		"  Give the -h, --help flag for more information."
	){
		Register(&users, &verbose, &computer, &message, &version, &copyright);
	}

	virtual int main();
};

int Hello::main()
{

	auto verbose  = ArgV.get<bool>("verbose");
	auto computer = ArgV.get<std::string>("computer");
	auto message  = ArgV.get<std::string>("message");
	auto users    = ArgV.get_arg<CLI::List>("user").value;

	if (verbose)
	{
		std::cout << "Incoming message from '" << computer << "': 'Greetings";
		for (int i = 0; i < users.size(); i++)
		{

			if (i > 0 && users.size() > 2)
				std::cout << ",";

			if (i == users.size() - 1 && users.size() > 1)
				std::cout << " and";

			std::cout << " " << users[i];
		}

		std::cout << "; " << message << "'" << std::endl;
	}

	return 0;
}





int main(const int argc, const char **argv)
{
	return Hello(argc, argv).Exe();
}
