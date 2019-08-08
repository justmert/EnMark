#include "FetchWords.h"
#include "curl/curl.h"
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <map>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <iostream>
#include <experimental/filesystem>
#define fs std::experimental::filesystem
const std::string FetchWords::url = "https://www.memrise.com/course/26131/yds-ingilizce-kelime-listesi/";
std::string FetchWords::modulePath;
std::string FetchWords::Path;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string Trim(std::string str)
{
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](char p) { return !isspace(p); }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int p) { return !std::isspace(p); }).base(), str.end());
    return str;
}

void FetchWords::WriteModule(const std::map<std::string, std::string> &mymap,int order)
{
    std::string tempPath = modulePath + "/" + "module"+ std::to_string(order)+ ".txt";
    std::ofstream myfile;
    myfile.open(tempPath,std::ios::out | std::ios::trunc);
    for (auto &&item : mymap)
    {
        myfile << item.first + "=" << item.second << "\n";
    }
}

void FetchWords::ParseHtml(std::string &html,int order)
{
    std::map<std::string, std::string> map;
    size_t found = 0;
    bool which = true;
    std::string first;
    std::string second;
    while (true)
    {
        int i = 0;
        std::string substr = "";
        found = html.find("\"text\">",found+1);
        if (found == std::string::npos)
            break;
        while (html[found + 7 + i] != '<')
        {
            substr += html[found + 7 + i];
            i++;
        }
        substr = Trim(substr);
        std::cout << substr << std::endl;
        if (which)
        {
            first = substr;
            which = false;
        }
        else
        {
            second = substr;
            map.insert({first,second});
            which = true;
        }
    }
    WriteModule(map,order);
}

void FetchWords::DownloadHtml(const std::vector<int> &missingvec)
{
    for (int i = 0; i < 1; i++)
    {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl)
        {
            std::string tempurl = url + std::to_string(missingvec[i]) + "/";
            const char* temp = tempurl.c_str();
            curl_easy_setopt(curl, CURLOPT_URL, temp);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            ParseHtml(readBuffer,missingvec[i]);
        }
    }
}

void FetchWords::CheckModules()
{
    std::vector<int> missinvec;
    if (!fs::exists(Path))
    {
        fs::create_directory(Path);
    }
    if (!fs::exists(modulePath))
    {
        fs::create_directory(modulePath);
    }
    for (int i = 1; i <= 188; i++)
    {
        if (!fs::exists(modulePath + "/module" + std::to_string(i)))
        {
            missinvec.push_back(i);
        }
    }
    if (!missinvec.empty())
    {
        DownloadHtml(missinvec);
    }
}
