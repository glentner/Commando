/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  lib/MultiMode.cpp
 *
 *   A MultiMode application has a single `exe`cutable function.
 *   TODO: MultiMode docs
 *
 */

#include <iomanip>
#include <string>
#include <map>


#include "../CLI/MultiMode.hpp"


namespace CLI {




MultiMode::MultiMode()
{
}



MultiMode::MultiMode(const int argc, const char **argv, const std::string& description,
                     const std::string& contact_information)
{
        this -> name = BaseName(argv[0]);
        this -> description = description;
        this -> contact_information = contact_information;
        ArgV = StringVector(argv + 1, argv + argc);

        Register(&help);
}


// clear the pointers
MultiMode::~MultiMode()
{

        for (auto com: Commands)
        {
                if (com.second)
                        delete com.second;

                com.second = nullptr;
        }

}





// pick what command to run from the first argument
void MultiMode::RuntimeConfigure()
{

        if (ArgV.empty()) {

                std::cerr << ShowUsage();
                throw Usage();
        }

        if (ArgV[0][0] == '-')
                InterpretFlag(ArgV[0]);

        if (help.given)
        {
                std::cerr << ShowHelp();
                throw Usage();
        }

        for (auto& flag: AllTerminators) if (flag -> given)
        {
                std::cerr << flag -> information << std::endl;
                throw Usage();
        }


        select = ArgV[0];
        if (Commands.find(select) == Commands.end())
        throw Error("'", select, "' is not an available command. Give the -h or --help "
                "flags to show the available commands or with a command to show its usage.");
}



void MultiMode::InterpretFlag(const std::string& flag)
{
        if (flag.length() < 2)
        throw Error("'-' is not a valid flag!");

        if (flag[1] == '-')
        {
                if (flag.length() < 3)
                throw Error("'--' is not a valid flag!");

                InterpretLongForm(flag.substr(2, flag.length()));
        }

        else InterpretShortForm(flag.substr(1, flag.length()));

}


void MultiMode::InterpretLongForm(const std::string& flag)
{
        bool found = false;
        for (auto& opt: AllTerminators)
        {
                if (opt -> name == flag)
                {
                        found = true;
                        opt -> given = true;
                        break;
                }
        }

        if (not found)
        throw Error("'--", flag, "' does not name a flag!");
}

void MultiMode::InterpretShortForm(const std::string& flag)
{
        bool found = false;
        for (auto& opt: AllTerminators)
        {
                if (opt -> abbrv == flag)
                {
                        found = true;
                        opt -> given = true;
                        break;
                }
        }

        if (not found)
        throw Error("'-", flag, "' does not name a flag!");
}



// execute the appropriate command
int MultiMode::Exe()
{
        try {
                RuntimeConfigure();
                Commands[select] -> FromMultiMode(name, ArgV);
                return Commands[select] -> Exe();

        } catch (const Usage& message) {

                std::cout << message.what();
                return 0;

        } catch (const Error& error) {

                std::cerr << error.what() << std::endl;
                return 1;

        } catch (const std::exception& error) {

                std::cout << error.what() << std::endl;
                return 2;

        } catch (...) {

                std::cerr << "Uncaught exception occured in " << name << "!\n";
                return 3;
        }
}



// show basic usage statement
std::string MultiMode::ShowUsage()
{

        std::string tab(7 + name.length(), ' ');

        int max = 0;
        for (const auto& com: Commands)
        if (com.first.length() > max)
                max = com.first.length();


        std::stringstream message;
        message << "usage: " << name;

        for (const auto& com: Commands)
                message << " " << com.first << std::string(max - com.first.length() + 2, ' ')
                        << "...\n" << tab;

        message << "\n" << description << "\n";
        return message.str();
}


// show more complete help information
std::string MultiMode::ShowHelp()
{
        std::stringstream message;
        message << ShowUsage() << std::endl;

        int spacing = 0;
        for (const auto& flag: AllTerminators)
        if ( (flag -> name).length() > spacing )
                spacing = (flag -> name).length();

        spacing += 10;

        for (const auto& arg: AllTerminators)
                message << arg -> GetHelp(spacing);

        if (!contact_information.empty())
                message << "\n" << contact_information << "\n";

        return message.str();
}


} // namespace CLI
