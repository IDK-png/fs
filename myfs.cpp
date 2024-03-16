#include "myfs.h"
#include "file.hpp"
#include "root.hpp"
#include <string.h>
#include <iostream>
#include <math.h>
#include <sstream>
#define TABLE_SIZE 2000
const char *MyFs::MYFS_MAGIC = "MYFS";

MyFs::MyFs(BlockDeviceSimulator *blkdevsim_):blkdevsim(blkdevsim_) {
	struct myfs_header header;
	blkdevsim->read(0, sizeof(header), (char *)&header);

	if (strncmp(header.magic, MYFS_MAGIC, sizeof(header.magic)) != 0 ||
	    (header.version != CURR_VERSION)) {
		std::cout << "Did not find myfs instance on blkdev" << std::endl;
		std::cout << "Creating..." << std::endl;
		format();
		std::cout << "Finished!" << std::endl;
	}
}

void MyFs::format() {

	// put the header in place
	struct myfs_header header;
	strncpy(header.magic, MYFS_MAGIC, sizeof(header.magic));
	header.version = CURR_VERSION;
	blkdevsim->write(0, sizeof(header), (const char*)&header);

	// TODO: put your format code here
}

int MyFs::findEmptyTableAddress()
{
	int i=0;
	while(i<TABLE_SIZE)
	{
		char y[21] = {0};
		blkdevsim->read(i,20,y);
		std::cout << y << std::endl;
		if(strcmp(y,"")==0)
    	{
			//std::cout << i << std::endl;
			break;
     	}
		//std::cout << "Clear" << std::endl;
		i+=20;
	}

	if(i>=TABLE_SIZE)
	{
		return -1;
	}

	return i;
}

int MyFs::findEmptyDataAddress(int size)
{
	int i=TABLE_SIZE;
	while(i<TABLE_SIZE*2)
	{
		char y[size+1] = {0};
		blkdevsim->read(i,size,y);
		if(strcmp(y,"")==0)
		{
			break;
		}
		
		i+=size;
	}

	return i;
}

void MyFs::create_file(std::string path_str, bool directory) {
	int EmptyAddress = findEmptyTableAddress();
	int EmptyDataAddress = findEmptyDataAddress(1);
	char type;
	if(directory)
	{
		type='d';
	}
	else
	{
		type='f';
	}
	ROOT root = ROOT();
	FileNode Inode = FileNode(round(EmptyAddress/20),1,EmptyDataAddress,type, ""); // short inode, int size, int address, char type
	//std::cout << Data << "    " << round(EmptyAddress/20) << std::endl;

	blkdevsim->write(EmptyDataAddress,1,"T"); // Putting placeholder in datablock address 
	blkdevsim->write(EmptyAddress,20,Inode.GetInodeSection().c_str());
	root.writeToFile(std::to_string(std::stoi(Inode.GetInode())) + " " + path_str);
	//std::cout << root.returnTextByNumber(round(EmptyAddress/20)) << std::endl;
	//std::cout << root.returnNumberByText(path_str) << std::endl;
}

std::string MyFs::get_content(std::string path_str) {
	ROOT root = ROOT();
	int InodeNumber = root.returnNumberByText(path_str);
	if(InodeNumber!=0)
	{
		char Inode[21];
		blkdevsim->read(20*InodeNumber,20, Inode);

		FileNode Node(Inode);
		char Data[std::stoi(Node.GetFsize())+1]={0};
		blkdevsim->read(std::stoi(Node.GetAddress()),std::stoi(Node.GetFsize()),Data);

		return Data;
	}
	return "No File Found!";
}

void MyFs::set_content(std::string path_str, std::string content) {
	ROOT root = ROOT();
	int InodeNumber = root.returnNumberByText(path_str);
	if(InodeNumber!=0)
	{
		int EmptyDataAddress = findEmptyDataAddress(content.length());
		char Inode[21];

		blkdevsim->read(20*InodeNumber,20, Inode);

		FileNode Node(Inode);
		Node.SetAddress(EmptyDataAddress);
		Node.SetFSize(content.length());
		
		blkdevsim->write(20*InodeNumber,20,Node.GetInodeSection().c_str());
		blkdevsim->write(std::stoi(Node.GetAddress()), std::stoi(Node.GetFsize()),content.c_str());
	}
}

MyFs::dir_list MyFs::list_dir(std::string path_str) {
	dir_list ans;
	throw std::runtime_error("not implemented");
	return ans;
}