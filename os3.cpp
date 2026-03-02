#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct Frame 
{
    int page;
    unsigned char counter;
    bool R;
};

int simulate(const vector<int>& refs, int numFrames) 
{
    vector<Frame> frames;
    int faults = 0;

    for (int ref : refs) 
    {
        bool found = false;

        for (auto& f : frames) 
        {
            if (f.page == ref) 
            {
                f.R = true;
                found = true;
                break;
            }
        }

        if (!found) 
        {
            faults++;

            if ((int)frames.size() < numFrames) 
            {
                frames.push_back({ref, 0, true});
            } 
            else 
            {
                int victim = 0;
                for (int i = 1; i < (int)frames.size(); i++)
                 {
                    if (frames[i].counter < frames[victim].counter)
                        victim = i;
                }
                frames[victim] = {ref, 0, true};
            }
        }

        for (auto& f : frames) 
        {
            f.counter >>= 1;
            if (f.R)
                f.counter |= 0x80;
            f.R = false;
        }
    }

    return faults;
}

int main() {
    ifstream file("input.txt");
    if (!file) 
    {
        cerr << "couldn't open input.txt" << endl;
        return 1;
    }

    vector<int> refs;
    int x;
    while (file >> x)
        refs.push_back(x);

    int maxFrames = 50;

    cout << "Frames, Faults per 1000 references\n";
    for (int n = 1; n <= maxFrames; n++) 
    {
        double rate = (double)simulate(refs, n) / refs.size() * 1000;
        cout << n << ", " << rate << "\n";
    }

    cin.get();
    return 0;
}