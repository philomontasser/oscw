#include<iostream>
#include<string>
#include<stdio.h>
#include<unistd.h>
#include<vector>

using namespace std;

string read_input()
{
    string input;
    cout << "type ur input: ";
    getline(cin, input);
    return input;
}

void quit()
{
    exit(0);
}

string wd()
{
    char wd_array[256];
    getcwd(wd_array, sizeof(wd_array));
    return string(wd_array);
}


int main()
{
    char wd_array[256];
    getcwd(wd_array, sizeof(wd_array));

    while (true)
    {
        string input = read_input();
        if (input == "quit")
        {
            cout << "quitting..." << endl;
            quit();
        }
        if (input == "cwd" or input == "pwd")
        {
            cout << "current working directory: " << wd() << endl;
        }
        if (input[0] == 'e' && input[1] == 'c' && input[2] == 'h' && input[3] == 'o' && input[4] == ' ')
        {
            cout << input.substr(5) << endl;
        }
        if (input[0] == 's' && input[1] == 'e' && input[2] == 't' && input[3] == ' ')
        {
            string var_name = input.substr(4, input.find(' '));
            string var_value = input.substr(input.find('=') + 1);
        }
    }
    return 0;
}