#include <iostream>
#include <string>
#define num 7
using namespace std;
int main()
{
    string command[num] = {"cat /proc/cpuinfo", "cat /proc/meminfo", "lsblk", "cat /proc/ioports", "ps -aux", "cat /proc/filesystems", "cat /proc/devices"};
    for (int i = 0; i < num; i++)
    {
        system((command[i] + " >> result.txt").c_str());
    }
    return 0;
}