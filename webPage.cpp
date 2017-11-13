// //#include <stdio.h>
// #include <iostream>
// #include <curl/curl.h>
// #include <fstream>

// using namespace std;

// int main(void)
// {
//   CURL *curl;
//   CURLcode res;

//   curl = curl_easy_init();
  
//   if(curl) {
//     fstream ifs;
//     ifs.open("words.txt");
//     curl_easy_setopt(curl, CURLOPT_URL, "https://raw.githubusercontent.com/dwyl/english-words/master/words.txt");
//     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ifs);
//     res = curl_easy_perform(curl);
//       /* cleanup */
//     curl_easy_cleanup(curl);
//     ifs.close();
//   }
//   return 0;
// }

#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char const *url = "https://raw.githubusercontent.com/dwyl/english-words/master/words.txt";
    char outfilename[FILENAME_MAX] = "words.txt";
    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}