#ifndef __FILE_H__
#define __FILE_H__
//pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <ctime>
#include <chrono>
#include <string>
#include <cstring>

class FileNode 
{
public:
/*
short inode, int size, int address, char type
*/
FileNode(short inode, int size, int address, char type, std::string Fdata);
FileNode(std::string inodeString);
std::string GetInode();
std::string GetFsize();
std::string GetDate();
std::string GetAddress();
std::string GetData();
std::string GetInodeSection();
void SetFSize(int);
void SetAddress(int);
char GetType();

private:              // Docs                      | Example                 | Size(20 bytes)
                      // __________________________|_________________________|_____________________
    short InodeNum;   // Inode Number.             | 1,2,3,4,5...            | 2 bytes
    int FSize;        // File Size                 | 1000, 1400, 0010        | 4 bytes
    char Date_Time[9];// Date/Time File was created| 15/03/24, 28/12/22      | 9 bytes
    int Address;      // Address Of Data           | ...                     | 4 bytes
    char FType;       // File Type                 | F(File),D(Dir),E(Empty) | 1 bytes 
    std::string data; // Data of the file          | any text                | depends             
};

// Address setter
inline void FileNode::SetAddress(int newAddress)
{
    Address = newAddress;
}

// File size(FSize) setter
inline void FileNode::SetFSize(int newSize)
{
    FSize = newSize;
}

// Get string in format: Inode number|File size|Date file created|Address of data|Type of Inode
inline std::string FileNode::GetInodeSection()
{
    std::string Data = this->GetInode()+this->GetFsize()+this->GetDate()+this->GetAddress()+this->GetType();
    return Data;
}

// Constructor && generates date for Date_Time variable
inline FileNode::FileNode(short inode, int size, int address, char type, std::string Fdata) : InodeNum(inode), FSize(size), Address(address), FType(type), data(Fdata)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm* local_now = std::localtime(&now_c);

    std::strftime(Date_Time, 9, "%d/%m/%y", local_now);

    //std::cout << sizeof(InodeNum) + sizeof(FSize) + sizeof(Date_Time) + sizeof(Address) + sizeof(FType) << std::endl;
    //std::cout << Date_Time << std::endl;
}

// Constructs a FileNode object from a inode string
inline FileNode::FileNode(std::string inodeString)
{
    InodeNum=std::stoi(inodeString.substr(0,2));
    FSize=std::stoi(inodeString.substr(2,4));
    strcpy(Date_Time, inodeString.substr(6,8).c_str());
    Address = std::stoi(inodeString.substr(14,4));
    FType = inodeString[19];
}

// Returns the inode number as a formatted string
inline std::string FileNode::GetInode()
{
    std::string empty = "00";
    std::string num = std::to_string(InodeNum);
    
    for(int i = num.length(); i>0; i--)
    {
        empty[empty.length()-i] = num[num.length()-i];
    }
    return empty;
}

// Returns the file size as a formatted string.
inline std::string FileNode::GetFsize()
{
    std::string empty = "0000";
    std::string num = std::to_string(FSize);
    
    for(int i = num.length(); i>0; i--)
    {
        empty[empty.length()-i] = num[num.length()-i];
    }
    return empty;
}

// Date_Time getter
inline std::string FileNode::GetDate()
{
    return Date_Time;
}

// Address getter
inline std::string FileNode::GetAddress()
{
    return std::to_string(Address);
}

// data varaible getter
inline std::string FileNode::GetData()
{
    return data;
}

// FType getter
inline char FileNode::GetType()
{
    return FType;
}
#endif //__FILE_H__