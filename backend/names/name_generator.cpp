#include "names.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <curl/curl.h>
#include "network.hpp"
#include <thread>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

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
    if (code != CURLE_OPERATION_TIMEDOUT && code != CURLE_COULDNT_RESOLVE_HOST) std::cout << "Got unexpected CURLcode " << std::string(curl_easy_strerror(code)) << ", using default names" << std::endl;
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

    res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
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
        if (!romanized_str || std::string(romanized_str) == "Invalid JSON input")
        {
            std::cerr << "Got invalid response from romanization server. Check curl.log." << std::endl;
            return romanize_error(romanize, CURLE_OK);
        }
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
    if (fn_start == (char *)25)
    {
        std::cerr << "Got invalid response from name generation server. Check curl.log." << std::endl;
        return placeholder_name(nation, gender, CURLE_OK);
    }

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

void init_name_lists(std::vector<FullName> *list)
{
    NationGenerator n_generator;
    std::vector<Nation> nation_list = n_generator.GetNationList();
    std::ifstream name_list_file_fd;
    Nation current_nation;
    Gender current_gender = Gender::Male;
    bool is_in_last_names = false;
    std::string token, line;

    std::map<std::string, std::vector<std::string>> male_first_names, female_first_names, last_names;

    std::cout << "Loading name list..." << std::endl;

    name_list_file_fd.open("name_list.txt");

    while (std::getline(name_list_file_fd, line))
    {
        std::stringstream name_list_file(line);
        name_list_file >> token;
        if (token == "nation")
        {
            name_list_file >> token;
            for (unsigned long i = 0; i < nation_list.size(); i++)
                if (nation_list[i].api_name == token)
                    current_nation = nation_list[i];
        }
        else if (token == "gender")
        {
            is_in_last_names = false;
            name_list_file >> token;
            if (token == "male")
                current_gender = Gender::Male;
            else
                current_gender = Gender::Female;
        }
        else if (token == "last")
        {
            name_list_file >> token;
            is_in_last_names = true;
        }
        else
        {
            std::string result = token;
            while (name_list_file >> token)
                result.append(token);
            if (is_in_last_names)
                last_names[current_nation.api_name].push_back(result);
            else if (current_gender == Gender::Male)
                male_first_names[current_nation.api_name].push_back(result);
            else if (current_gender == Gender::Female)
                female_first_names[current_nation.api_name].push_back(result);
        }
    }

    name_list_file_fd.close();

    std::cout << "Building name tree..." << std::endl;

    for (unsigned long i = 0; i < nation_list.size(); i++)
    {
        Nation nation = nation_list[i];
        for (unsigned long j = 0; j < male_first_names[nation.api_name].size(); j++)
        {
            for (unsigned long k = 0; k < last_names[nation.api_name].size(); k++)
            {
                FullName new_full_name;
                new_full_name.gender = Gender::Male;
                new_full_name.nation = nation;
                new_full_name.name = male_first_names[nation.api_name][j];
                new_full_name.name.append(" ");
                new_full_name.name.append(last_names[nation.api_name][k]);
                list->push_back(new_full_name);
            }
        }
        for (unsigned long j = 0; j < female_first_names[nation.api_name].size(); j++)
        {
            for (unsigned long k = 0; k < last_names[nation.api_name].size(); k++)
            {
                FullName new_full_name;
                new_full_name.gender = Gender::Female;
                new_full_name.nation = nation;
                new_full_name.name = female_first_names[nation.api_name][j];
                new_full_name.name.append(" ");
                new_full_name.name.append(last_names[nation.api_name][k]);
                list->push_back(new_full_name);
            }
        }
    }

    std::cout << "We have " << list->size() << " names in total." << std::endl;
}

void regen_name(std::vector<FullName> *list, bool use_unicode, int i) {
    NationGenerator *nation_generator = new NationGenerator;
    FullName new_name;
    new_name.gender = list->at(i).gender;
    new_name.nation = list->at(i).nation;
    new_name.name = get_random_full_name_network(new_name.nation, new_name.gender, use_unicode);
    if (new_name.name != "John Doe") list->at(i) = new_name;
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
            if (!offline_mode)
                new std::thread(regen_name, &loaded_full_names, can_use_cjk, i);
            return ret;
        }
    }
    std::cerr << "warning: No name found in offline storage. Refresh name list with -r to fix." << std::endl;
    return get_random_full_name_network(nation, gender, can_use_cjk);
}

struct stat statbuf;

NameGenerator::NameGenerator()
{
    static bool there_is_another = false;
    if (there_is_another)
    {
        throw new std::runtime_error("NameGenerator Mutex error");
    }
    there_is_another = true;
    offline_mode = false;
    InitNetwork();
    can_use_cjk = true;
    running = false;
    if (stat(std::string("name_list.txt").c_str(), &statbuf)) NameListFullRefresh();
    init_name_lists(&loaded_full_names);
}

NameGenerator::~NameGenerator()
{
    StopNetwork();
    running = false;
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

const int names_per_gender_per_nation = 5;

void NameGenerator::NameListFullRefresh()
{
    std::ofstream name_list_file;
    NationGenerator temp_generator;
    std::vector<Nation> nation_list = temp_generator.GetNationList();

    if (offline_mode)
    {
        std::cerr << "Error: Can't refresh name list in offline mode." << std::endl;
        return;
    }

    name_list_file.open("name_list.txt");

    for (unsigned long i = 0; i < nation_list.size(); i++)
    {
        std::cout << "nation " << i << " / " << nation_list.size() << std::endl;
        Nation nation = nation_list[i];
        name_list_file << "nation " << nation.api_name << "\n";

        std::vector<std::string> male_names, female_names, last_names;

        /* We can use .reserve() here because we do not expect the size of the vector to change */
        male_names.reserve(names_per_gender_per_nation);
        female_names.reserve(names_per_gender_per_nation);
        last_names.reserve(names_per_gender_per_nation * 2);

        for (int j = 0; j < names_per_gender_per_nation; j++)
        {
            std::cout << "name  " << j << " / " << names_per_gender_per_nation << std::endl;
            std::string male_name = get_random_full_name_network(nation, Gender::Male, can_use_cjk);
            std::string female_name = get_random_full_name_network(nation, Gender::Female, can_use_cjk);

            male_names.push_back(male_name.substr(0, male_name.find(' ')));
            female_names.push_back(female_name.substr(0, female_name.find(' ')));

            last_names.push_back(std::string(strchr(male_name.c_str(), ' ') + 1));
            last_names.push_back(std::string(strchr(female_name.c_str(), ' ') + 1));
        }

        name_list_file << "gender male\n";
        for (unsigned long j = 0; j < male_names.size(); j++)
        {
            name_list_file << male_names[j] << "\n";
        }

        name_list_file << "gender female\n";
        for (unsigned long j = 0; j < female_names.size(); j++)
        {
            name_list_file << female_names[j] << "\n";
        }

        name_list_file << "last names\n";
        for (unsigned long j = 0; j < female_names.size() + male_names.size(); j++)
        {
            name_list_file << last_names[j] << "\n";
        }

        name_list_file.flush();
    }

    name_list_file.close();
}

void NameGenerator::SetOfflineMode(bool offline)
{
    offline_mode = offline;
}
