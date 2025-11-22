#include <httpClient.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include <regex>

std::string getWindowsHostIP()
{
    std::ifstream resolv("/etc/resolv.conf");
    std::string line;
    std::regex ns_regex("^nameserver ([0-9\\.]+)$");

    while (std::getline(resolv, line))
    {
        std::smatch match;
        if (std::regex_match(line, match, ns_regex))
        {
            return match[1];
        }
    }
    return "127.0.0.1";
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

bool checkHealth()
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize curl" << std::endl;
        return false;
    }

    std::string response;
    std::string url = "http://" + getWindowsHostIP() + ":8000/";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    std::cout << "Connecting to FastAPI at: " << url << std::endl;

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        std::cerr << "[CURL Error] " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    std::cout << "HTTP Response Code: " << http_code << "\n";
    std::cout << "Response: " << response << std::endl;

    curl_easy_cleanup(curl);
    return http_code >= 200 && http_code < 300;
}

std::string postJSON(const std::string& endpoint, const std::string& jsonPayload)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize curl for POST request" << std::endl;
        return "";
    }

    std::string response;
    std::string url = "http://" + getWindowsHostIP() + ":8000" + endpoint;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        std::cerr << "[CURL Error] POST to " << endpoint << ": "
                  << curl_easy_strerror(res) << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return "";
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code < 200 || http_code >= 300)
    {
        int message_start = response.find("\"detail\": ") + 2;
        int message_end = response.find("\"", message_start);
        std::cerr
            << "[HTTP Error] POST to " << endpoint << " returned code: "
            << http_code << ". Message: " << response.substr(message_start, message_end) << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return "";
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return response;
}
