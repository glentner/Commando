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

	Hello(const int argc, const char **argv): SingleMode(argc, argv,

		"A simple `Hello, world!` program to showcase the CLI::SingleMode framework.\n"
		"Give the -h, --help flag for more information.",

		"Report bugs to: glentner@nd.edu\n"
		"hello.exe home page: <http://github.com/glentner/CLI>"
	){
		Register(&__users, &__verbose, &__computer, &__message, &__version, &__copyright);
	}

	virtual int main();

private:

	CLI::List   __users    {"user", "user names"};

	CLI::Flag   __verbose  {"verbose",  "0",            "show greeting message", "v"};
	CLI::Switch __computer {"computer", "Lisa",         "name of the computer",  "c"};
	CLI::Switch __message  {"message",  "how are you?", "the greeting used",     "m"};

	CLI::Terminator __version {"version", "show version info",
		"hello.exe (1.0.1)", "V"};

	CLI::Terminator __copyright {"copyright", "show copyright info",
		"hello.exe (1.0.1)\nCopyright (c) Geoffrey Lentner 2015. All rights reserved."
		"\nGNU General Public License v3.0\nThis is free software; see the source for"
		" copying conditions.  There is NO\nwarranty; not even for MERCHANTABILITY or"
		" FITNESS FOR A PARTICULAR PURPOSE.", "C"};
};

int Hello::main()
{

	auto verbose  = ArgV.get<bool>("verbose");
	auto computer = ArgV.get<std::string>("computer");
	auto message  = ArgV.get<std::string>("message");
	auto users    = ArgV.get<std::vector<std::string>, CLI::List>("user");

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
