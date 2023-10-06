#include "names.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <curl/curl.h>
#include "network.hpp"

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

     mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

NameGenerator::NameGenerator()
{
    InitNetwork();
}

NameGenerator::~NameGenerator()
{
    StopNetwork();
}

std::string placeholder_name(Nation nation, Gender gender, CURLcode code)
{
    std::cout << "Got unexpected CURLcode " << std::string(curl_easy_strerror(code)) << ", using default names" << std::endl;
    return gender == Gender::Male ? "John Doe" : "Jane Doe";
}

std::string get_random_full_name(Nation nation, Gender gender)
{
    CURL *name_curl;
    std::string url, gender_str, post_data;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    name_curl = curl_easy_init();
    if (!name_curl) return placeholder_name(nation, gender, CURLE_OK);

    gender_str = gender == Gender::Male ? "male" : "female";

    url = "https://randomgenerator.name/wp-admin/admin-ajax.php";

    std::string post_data_format = "action=kira_random_online&data[0][name]=gender&data[0][value]=%s&data[1][name]=number&data[1][value]=1&data[2][name]=sets&data[2][value]=1&data[3][name]=surname&data[3][value]=&data[4][name]=randomsurname&data[4][value]=yes&data[5][name]=categories[]&data[5][value]=%s";
    post_data = string_format(post_data_format,
    gender_str.c_str(), nation.api_name.c_str());

    res = curl_easy_setopt(name_curl, CURLOPT_URL, url.c_str());
    if (res != CURLE_OK) return placeholder_name(nation, gender, res);

    res = curl_easy_setopt(name_curl, CURLOPT_POST, 1);
    if (res != CURLE_OK) return placeholder_name(nation, gender, res);

    res = curl_easy_setopt(name_curl, CURLOPT_POSTFIELDS, post_data.c_str());
    if (res != CURLE_OK) return placeholder_name(nation, gender, res);

    res = curl_easy_setopt(name_curl, CURLOPT_POSTFIELDSIZE, post_data.length());
    if (res != CURLE_OK) return placeholder_name(nation, gender, res);

    res = curl_easy_setopt(name_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    if (res != CURLE_OK) return placeholder_name(nation, gender, res);

    res = curl_easy_setopt(name_curl, CURLOPT_WRITEDATA, (void *)&chunk);
    if (res != CURLE_OK) return placeholder_name(nation, gender, res);

    res = curl_easy_perform(name_curl);
    if (res != CURLE_OK) return placeholder_name(nation, gender, res);

    curl_easy_cleanup(name_curl);

    chunk.memory[chunk.size] = 0;

    char *first_name, *last_name, *full_name;

    char *fn_start = strstr(chunk.memory, "<div class=\"box-name\">") + 25;

    for (first_name = fn_start; first_name < chunk.memory + chunk.size; first_name++)
        if (*first_name != 0x20) break;

    last_name = strchr(first_name, 0xd) + 3;

    *strchr(first_name, 0xd) = 0;
    *strchr(last_name, 0xd) = 0;

    int first_name_len = strlen(first_name), last_name_len = strlen(last_name);

    full_name = (char *)malloc(first_name_len + last_name_len + 2);
    strcpy(full_name, first_name);
    full_name[first_name_len] = ' ';
    strcpy(full_name + first_name_len + 1, last_name);

    std::string ret(full_name);

    free(full_name);
    free(chunk.memory);

    return ret;
}

std::string NameGenerator::GetRandomFirstName(Nation nation, Gender gender)
{
    std::string full_name = get_random_full_name(nation, gender);
    char *str = full_name.data();

    *strchr(str, ' ') = 0;

    return std::string(str);
}

std::string NameGenerator::GetRandomLastName(Nation nation, Gender gender)
{
    std::string full_name = get_random_full_name(nation, gender);
    char *str = strchr(full_name.data(), ' ') + 1;

    return std::string(str);
}
