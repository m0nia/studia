#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <codecvt>
#include <vector>
#include <thread>
#include <future>

/*
 * W rozwiązaniu ustalam stałą pulę wątków i pliki do przetworzenia rozdzielam po równo
 * pomiędzy te wątki. Nie jestem pewna czy dobrze zinterpretowałam polecenie
 * ("wątki powinny statycznie podzielić się pracą"), ale też nie wiedziałam, jak
 * zrobić to efektywniej bez globalnych struktur i ich ochrony, a tych mechanizmów
 * jeszcze nie omawialiśmy.
 *
 */
constexpr int POOL = 4;

using std::vector;
using std::string;
using std::promise;
using std::future;

int grep_seq(std::string& filename, std::wstring& word) {
    std::locale loc("pl_PL.UTF-8");
    std::wfstream file(filename);
    file.imbue(loc);
    std::wstring line;
    unsigned int count = 0;
    while (getline(file, line)) {
        for (auto pos = line.find(word,0);
             pos != std::string::npos;
             pos = line.find(word, pos+1))
            count++;
    }
    return count;
}

void grep(vector<string>& filenames, std::wstring word, promise<unsigned int>& promise) {
    unsigned int sum = 0;
    for(string& s: filenames){
        sum += grep_seq(s, word);
    }
    promise.set_value(sum);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::locale loc("pl_PL.UTF-8");
    std::wcout.imbue(loc);
    std::wcin.imbue(loc);

    std::wstring word;
    std::getline(std::wcin, word);

    std::wstring s_file_count;
    std::getline(std::wcin, s_file_count);
    int file_count = std::stoi(s_file_count);
    // Rozdzielam pomiędzy 4 wątki pliki - każdy dostaje file_count/POOL
    vector<vector<string>> threads_filenames(POOL, vector<string>());

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    for (int file_num = 0; file_num < file_count; file_num++) {
        std::wstring w_filename;
        std::getline(std::wcin, w_filename);
        std::string s_filename = converter.to_bytes(w_filename);
        int thread_nr = file_num % POOL;
        threads_filenames[thread_nr].push_back(s_filename);
    }
    promise<unsigned int> promises[POOL];
    future<unsigned int> futures[POOL];

    for(int i = 0; i < POOL; i++){
        promises[i] = promise<unsigned int>();
        futures[i] = promises[i].get_future();
    }

    std::thread threads[POOL];
    for(int i = 0; i < POOL; i++)
        threads[i] = std::thread{grep, std::ref(threads_filenames[i]), word, std::ref(promises[i])};

    unsigned int count = 0;

    for (int i = 0; i < POOL; i++) {
        count += futures[i].get();
    }

    for(auto & thread : threads) thread.join();
    std::wcout << count << std::endl;
}
