#include "myfs.h"
#include "file.hpp"
#include "root.hpp"
#include <string.h>
#include <iostream>
#include <math.h>
#include <sstream>

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
	while(i<1024)
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

	if(i>=1024)
	{
		return -1;
	}

	return i;
}

int MyFs::findEmptyDataAddress(int size)
{
	int i=1024;
	while(i<3000)
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
	FileNode Inode = FileNode(round(EmptyAddress/20),0,EmptyDataAddress,type, ""); // short inode, int size, int address, char type
	std::string Data = Inode.GetInode()+Inode.GetFsize()+Inode.GetDate()+Inode.GetAddress()+Inode.GetType();
	//std::cout << Data << "    " << round(EmptyAddress/20) << std::endl;

	//blkdevsim->write(EmptyDataAddress,0,""); 
	blkdevsim->write(EmptyAddress,20,Data.c_str());
	root.writeToFile(std::to_string(std::stoi(Inode.GetInode())) + " " + path_str);
	std::cout << root.returnTextByNumber(round(EmptyAddress/20)) << std::endl;
}

std::string MyFs::get_content(std::string path_str) {
	throw std::runtime_error("not implemented");
	return "";
}

void MyFs::set_content(std::string path_str, std::string content) {

	throw std::runtime_error("not implemented");
}

MyFs::dir_list MyFs::list_dir(std::string path_str) {
	dir_list ans;
	throw std::runtime_error("not implemented");
	return ans;
}