#pragma once
#include <iostream>
#include "curl/curl.h"

#pragma comment(lib, "libcurl_a.lib")

//Downloader class
class CHttpDownloader
{
public:
	CHttpDownloader();
	virtual ~CHttpDownloader();

	bool Download(const std::string& url, const std::string& filepath);
	void Cleanup();
protected:
	static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
private:
	CURL* curl;
};

