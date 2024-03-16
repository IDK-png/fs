#ifndef __BLKDEVSIM__H__
#define __BLKDEVSIM__H__

#include <string>

class BlockDeviceSimulator {
public:
	BlockDeviceSimulator(std::string fname);
	~BlockDeviceSimulator();

	void read(int addr, int size, char *ans);
	void write(int addr, int size, const char *data);

	void readROOT(int addr, int size, char *data);
	void writeROOT(int addr, int size, char *data);
	static const int DEVICE_SIZE = 1024 * 1024;
	// 4+4+4

private:
	int fd;
	int fd2;
	unsigned char *filemap;
	unsigned char *secondmap;
};

#endif // __BLKDEVSIM__H__