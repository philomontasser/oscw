#include <iostream>
#include <thread>
#include <utility>
#include <fstream>
#include <array>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <mutex>
#include <map>
#include <algorithm>

using namespace std;


vector<string> parsed_file(string file_name)
{
    ifstream words_file(file_name);
    stringstream file_to_stringstream;
    string file_as_string;
    vector<string> words;

    file_to_stringstream << words_file.rdbuf();
    file_as_string = file_to_stringstream.str();

    stringstream string_to_tokens(file_as_string);
    string token;
    while (string_to_tokens >> token)
    {
        words.push_back(token);
    }
    return words;
}


array<vector<string>, 4> split_words_into_sections(vector<string> words, int word_count)
{
    int section_size_floor = floor(word_count / 4);
    array<vector<string>, 4> sections;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < section_size_floor; j++)
        {
            sections[i].push_back(words[i * section_size_floor + j]);
        }
    }
    for (int i = 3 * section_size_floor; i < word_count; i++)
    {
        sections[3].push_back(words[i]);
    }
    return sections;
}


mutex mtx;

void safe_counting(vector<pair<string,int>> & word_counts, vector<string> section)
{
    for (int i = 0; i < section.size(); i++)
    {
        mtx.lock();
        auto iterator = find_if(word_counts.begin(), word_counts.end(),
                                [&](auto &p){ return p.first == section[i]; });
        if (iterator == word_counts.end())
            word_counts.push_back({section[i], 1});
        else
            word_counts[iterator - word_counts.begin()].second++;
        mtx.unlock();
    }
}



int main()
{
    vector<string> words = parsed_file("words.txt");
    int words_count = words.size();
    array<vector<string>, 4> sections = split_words_into_sections(words, words_count);
    vector<pair<string, int>> sections_counts;


    thread t1(safe_counting, ref(sections_counts), sections[0]);
    thread t2(safe_counting, ref(sections_counts), sections[1]);
    thread t3(safe_counting, ref(sections_counts), sections[2]);
    thread t4(safe_counting, ref(sections_counts), sections[3]);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    for (auto &p : sections_counts)
        cout << p.first << ": " << p.second << endl;


    return 0;
}