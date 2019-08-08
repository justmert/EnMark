#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <unistd.h>

class FetchWords
{
private:
    static const std::string url;
    static std::string modulePath;
    static std::string Path;
    static void WriteModule(const std::map<std::string,std::string> &,int);
    static void DownloadHtml(const std::vector<int> &);
    static void ParseHtml(std::string &, int);

public:
    static void CheckModules();
    FetchWords()
    {
        char *p = getlogin();
        std::string username = std::string(p);
        modulePath = "/home/"+username+"/.enmark_data/modules";
        Path = "/home/"+username+"/.enmark_data";
    }
};
