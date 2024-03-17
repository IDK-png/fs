#ifndef __ROOT_H__
#define __ROOT_H__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ROOT
{
    public:
    ROOT();
    static std::string returnTextByNumber(int);
    static void writeToFile(std::string);
    static int returnNumberByText(std::string);
};

inline ROOT::ROOT()
{

}

// This function searches for a given text (FName) in root file and returns the number associated with the given text.
inline int ROOT::returnNumberByText(std::string FName)
{
    std::ifstream File("rootFile");
    std::string line;
    if(File.is_open())
    {
        while(File)
        {
            std::getline(File, line);
            if(FName.compare(line.substr(line.find(' ')+1))==0)
            {
                return std::stoi(line.substr(0,line.find(' ')));
            }
        }
    }
    return 0;
}

// Returns the text associated with a given number from root file.
inline std::string ROOT::returnTextByNumber(int Inode)
{
    std::ifstream File("rootFile");
    std::string line;
    if(File.is_open())
    {
        while(File)
        {
            std::getline(File, line);
            if(std::stoi(line.substr(0,line.find(' ')))==Inode)
            {
                return line;
            }
        }
    }
    return "";
}

// Appends data to a root file
inline void ROOT::writeToFile(std::string Data)
{
    std::ofstream File("rootFile", std::ios::app);
    if(File.is_open())
    {
        File << Data << std::endl;
        File.close();
    }
}
#endif //__ROOT_H__