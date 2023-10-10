#include "names.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <curl/curl.h>
#include "network.hpp"
#include <thread>

NameGenerator *name_generator;

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

std::string placeholder_name(Nation nation, Gender gender, CURLcode code)
{
    std::cout << "Got unexpected CURLcode " << std::string(curl_easy_strerror(code)) << ", using default names" << std::endl;
    return gender == Gender::Male ? "John Doe" : "Jane Doe";
}

std::string romanize_error(std::string romanize, CURLcode res)
{
    std::cout << "error when romanizing " << romanize << ": " << std::string(curl_easy_strerror(res)) << std::endl;
    return romanize;
}

CURLcode make_request(std::string url, std::string post_data, struct MemoryStruct *chunk, bool is_json)
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    struct curl_slist *headers = NULL;

    if (!curl) return CURLE_FAILED_INIT;

    res = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (res) return res;

    res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    if (res) return res;

    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    if (res) return res;

    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
    if (res) return res;

    if (is_json)
    {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        if (res) return res;
    }

    res = curl_easy_perform(curl);
    if (res) return res;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return CURLE_OK;
}

std::string romanize_text(std::string romanize, Nation nation)
{
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    if (nation.api_name == "jp")
    {
        std::string url = "https://japonesbasico.com/furigana/procesa.php";

        std::string post_data_format = "{\"japaneseText\":\"%s\",\"conversion\":\"romaji\",\"lang\":\"en\"}";
        std::string post_data = string_format(post_data_format, romanize.c_str());

        if ((res = make_request(url, post_data, &chunk, true))) return romanize_error(romanize, res);

        chunk.memory[chunk.size] = 0;

        FILE *f = fopen("curl.log", "w");
        fputs(chunk.memory, f);
        fclose(f);

        /* Get our data from the response */
        char *romanized_str = strstr(chunk.memory, "</rt></ruby></td><td>") + 22;
        if (!(*romanized_str)) romanized_str++;
        *strchr(romanized_str, '<') = 0;

        /* Remove the extra space at the end */
        romanized_str[strlen(romanized_str)-1] = 0;
        std::string ret(romanized_str);

        free(chunk.memory);

        return ret;
    }
    else if (nation.api_name == "kr")
    {
        std::string url = "https://asaokitan.net/tools/hangul2yale/web.py";

        std::string post_data_format = "hangul=%s";
        std::string post_data = string_format(post_data_format, romanize.c_str());

        if ((res = make_request(url, post_data, &chunk, false))) return romanize_error(romanize, res);

        chunk.memory[chunk.size - 1] = 0;

        FILE *f = fopen("curl.log", "w");
        fputs(chunk.memory, f);
        fclose(f);

        chunk.memory[0] = toupper(chunk.memory[0]);
        strchr(chunk.memory, ' ')[1] = toupper(strchr(chunk.memory, ' ')[1]);

        std::string ret(chunk.memory);

        free(chunk.memory);

        return ret;
    }
    else
    {
        return romanize;
    }
}

std::string loaded_full_name;
bool used_up_first_name = true;
bool used_up_last_name = true;

std::string get_random_full_name_network(Nation nation, Gender gender, bool use_unicode)
{
    std::string url, gender_str, post_data;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    gender_str = gender == Gender::Male ? "male" : "female";

    url = "https://randomgenerator.name/wp-admin/admin-ajax.php";

    std::string post_data_format = "action=kira_random_online&data[0][name]=gender&data[0][value]=%s&data[1][name]=number&data[1][value]=1&data[2][name]=sets&data[2][value]=1&data[3][name]=surname&data[3][value]=&data[4][name]=randomsurname&data[4][value]=yes&data[5][name]=categories[]&data[5][value]=%s";
    post_data = string_format(post_data_format,
    gender_str.c_str(), nation.api_name.c_str());

    if ((res = make_request(url, post_data, &chunk, false))) return placeholder_name(nation, gender, res);

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

    if (!use_unicode) ret = romanize_text(ret, nation);

    free(full_name);
    free(chunk.memory);

    return ret;
}

Nation home_nation;
void name_idle_generator(std::vector<FullName> *list, bool *should_run, bool *use_unicode) {
    NationGenerator *nation_generator = new NationGenerator;
    while (*should_run)
    {
        FullName new_name;
        if (rand() % 4 == 0)
            new_name.nation = nation_generator->GetRandomNation();
        else
            new_name.nation = home_nation;
        
        if (rand() & 1)
            new_name.gender = Gender::Male;
        else
            new_name.gender = Gender::Female;
        
        new_name.name = get_random_full_name_network(new_name.nation, new_name.gender, *use_unicode);

        list->push_back(new_name);
    }
    delete nation_generator;
}

std::string NameGenerator::get_random_full_name(Nation nation, Gender gender)
{
    used_up_first_name = false;
    used_up_last_name = false;
    for (unsigned long i = 0; i < loaded_full_names.size(); i++)
    {
        if (loaded_full_names[i].gender == gender && loaded_full_names[i].nation.api_name == nation.api_name)
        {
            std::string ret = loaded_full_names[i].name;
            loaded_full_names.erase(loaded_full_names.begin() + i);
            return ret;
        }
    }
    return get_random_full_name_network(nation, gender, can_use_cjk);
}

NameGenerator::NameGenerator(Nation nation)
{
    static bool there_is_another = false;
    if (there_is_another)
    {
        throw new std::runtime_error("NameGenerator Mutex error");
    }
    there_is_another = true;
    InitNetwork();
    can_use_cjk = true;
    home_nation = nation;
    running = true;
    name_generation_thread = std::thread(name_idle_generator, &loaded_full_names, &running, &can_use_cjk);
}

NameGenerator::~NameGenerator()
{
    StopNetwork();
    running = false;
    name_generation_thread.join();
}

void NameGenerator::SetCanUseCJK(bool can)
{
    can_use_cjk = can;
}

std::string NameGenerator::GetRandomFirstName(Nation nation, Gender gender)
{
    std::string full_name = loaded_full_name;
    if (used_up_first_name)
    {
        full_name = get_random_full_name(nation, gender);
        loaded_full_name = full_name;
    }   
    char *str = full_name.data();

    *strchr(str, ' ') = 0;

    used_up_first_name = true;

    return std::string(str);
}

std::string NameGenerator::GetRandomLastName(Nation nation, Gender gender)
{
    std::string full_name = loaded_full_name;
    if (used_up_last_name)
    {
        full_name = get_random_full_name(nation, gender);
        loaded_full_name = full_name;
    }
    char *str = strchr(full_name.data(), ' ') + 1;

    used_up_last_name = true;

    return std::string(str);
}
