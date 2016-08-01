#include <iostream>
#include "HttpDownloader.h"
#include "MP4.Parser.h"

using namespace MP4;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " URL" << " Output\n";
		return 0;
	}

	std::string url(argv[1]);
	//CHttpDownloader object to download a file
	CHttpDownloader downloader;
	//download
	if (downloader.Download(url, argv[2]))
	{
		std::cout << "Successfully loaded file " << url.c_str() << std::endl;
		//parse the file and print infos
		Parser mp4Parser(argv[2]);
	}
	else
		std::cout << "Download failed" << std::endl;
	//pause the console screen to see results
	system("PAUSE");
	return 0;
}