#include "HttpDownloader.h"

//constructor
CHttpDownloader::CHttpDownloader()
{
	curl = curl_easy_init();
	if (!curl)
		std::cout << "libcurl initialized failed!" << std::endl;
}

//desctructor
CHttpDownloader::~CHttpDownloader()
{
	Cleanup();
}

//destroy objects
void CHttpDownloader::Cleanup()
{
	if (curl)
		curl_easy_cleanup(curl);
}

//calback function
size_t CHttpDownloader::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

bool CHttpDownloader::Download(const std::string& url, const std::string& filepath) {
	FILE *out = nullptr;
	fopen_s(&out, filepath.c_str(), "wb");
	if (!out)
	{
		std::cout << "cannot open: " << filepath.c_str() << std::endl;
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	/*libcurl to follow redirection */
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	//curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
	//curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
	/* Perform the request, res will get the return code */
	CURLcode res = curl_easy_perform(curl);
	/* Check for errors */
	if (res != CURLE_OK) {
		fclose(out);
		std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		return false;
	}
	fclose(out);
	return true;
}
