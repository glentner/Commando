/*
 *  Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
 *  GNU General Public License v3.0, see LICENSE file.
 *  CLI/examples/calc.cpp
 *
 *  Simple calculator app using the CLI::MultiMode framework.
 */



#include <iostream>

#include "../CLI/SingleMode.hpp"
#include "../CLI/MultiMode.hpp"


class Add : public CLI::SingleMode {
public:

        CLI::Required a {"a", "The LHS value."}, b {"b", "The RHS value."};

        Add(const int argc, const char **argv)
        : SingleMode(argc, argv, "Add two numbers together.")
        {
                Register(&a, &b);
        }

        virtual int main()
        {
                float a = ArgV.get<float>("a");
                float b = ArgV.get<float>("b");

                std::cout << "Answer: " << a + b << std::endl;
                return 0;
        }
};


class Subtract : public CLI::SingleMode {
public:

        CLI::Required a {"a", "The LHS value."}, b {"b", "The RHS value."};

        Subtract(const int argc, const char **argv)
        : SingleMode(argc, argv, "Subtract two numbers.")
        {
                Register(&a, &b);
        }

        virtual int main()
        {
                float a = ArgV.get<float>("a");
                float b = ArgV.get<float>("b");

                std::cout << "Answer: " << a - b << std::endl;
                return 0;
        }
};


class Multiply : public CLI::SingleMode {
public:

        CLI::Required a {"a", "The LHS value."}, b {"b", "The RHS value."};

        Multiply(const int argc, const char **argv)
        : SingleMode(argc, argv, "Multiply two numbers together.")
        {
                Register(&a, &b);
        }

        virtual int main()
        {
                float a = ArgV.get<float>("a");
                float b = ArgV.get<float>("b");

                std::cout << "Answer: " << a * b << std::endl;
                return 0;
        }
};



class Divide : public CLI::SingleMode {
public:

        CLI::Required a {"a", "The LHS value."}, b {"b", "The RHS value."};

        Divide(const int argc, const char **argv)
        : SingleMode(argc, argv, "Divide two numbers.")
        {
                Register(&a, &b);
        }

        virtual int main()
        {
                float a = ArgV.get<float>("a");
                float b = ArgV.get<float>("b");

                std::cout << "Answer: " << a / b << std::endl;
                return 0;
        }
};


class Modulus : public CLI::SingleMode {
public:

        CLI::Required a {"a", "The LHS value."}, b {"b", "The RHS value."};

        Modulus(const int argc, const char **argv)
        : SingleMode(argc, argv, "Take the modulus of two numbers.")
        {
                Register(&a, &b);
        }

        virtual int main()
        {
                float a = ArgV.get<float>("a");
                float b = ArgV.get<float>("b");

                std::cout << "Answer: " << (int)a % (int)b << std::endl;
                return 0;
        }
};


class Calc: public CLI::MultiMode {
public:

        CLI::Terminator version {"version", "show version info",
                "calc.exe (1.0.1)\nCopyright (c) Geoffrey Lentner 2015. All rights reserved."
                "\nGNU General Public License v3.0\nThis is free software; see the source for"
                " copying conditions.  There is NO\nwarranty; not even for MERCHANTABILITY or"
                " FITNESS FOR A PARTICULAR PURPOSE.", "V"};

        CLI::Terminator copyright {"copyright", "show copyright info",
                "calc.exe (1.0.1)\nCopyright (c) Geoffrey Lentner 2015. All rights reserved."
                "\nGNU General Public License v3.0\nThis is free software; see the source for"
                " copying conditions.  There is NO\nwarranty; not even for MERCHANTABILITY or"
                " FITNESS FOR A PARTICULAR PURPOSE.", "C"};

        Calc(const int argc, const char **argv): MultiMode(argc, argv,

                "Operate two numbers on each other. Give one of the above subcommands "
                "with no arguments for their respective usage, or pass the -h, --help flag "
                "for more information alone or with one of the above subcommands.",

                "Report bugs to: glentner@nd.edu\n"
                "calc.exe home page: <http://github.com/glentner/CLI>"
        ){
                Commands["add"]      = new Add(argc, argv);
                Commands["subtract"] = new Subtract(argc, argv);
                Commands["multiply"] = new Multiply(argc, argv);
                Commands["divide"]   = new Divide(argc, argv);
                Commands["mod"]      = new Modulus(argc, argv);

                Register(&version, &copyright);
        }

};



int main(const int argc, const char **argv)
{
        return Calc(argc, argv).Exe();
}
