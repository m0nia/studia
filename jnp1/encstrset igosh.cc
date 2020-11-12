#include "encstrset.h"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#ifdef NDEBUG
static const bool debug = false;
#else
static const bool debug = true;
#endif

using std::cerr;
using std::endl;
using std::hex;
using std::string;
using std::stringstream;
using std::to_string;
using std::unordered_map;
using std::unordered_set;
using std::uppercase;

enum request_type {
    new_set,
    delete_set,
    set_size,
    set_insert,
    set_remove,
    set_test,
    set_clear,
    set_copy
};

enum op_status {
    intro,
    success,
    no_set,
    fail,
    null_value
};

namespace {  // funkcje pomocnicze
static const string EMPTY = "";
const unsigned int OP_COUNT = 8;
const unsigned int MESSAGE_TYPE_COUNT = 5;
const unsigned int MESSAGE_SEGMENT_COUNT = 4;
static const string message[OP_COUNT][MESSAGE_TYPE_COUNT][MESSAGE_SEGMENT_COUNT] =
    {{{"encstrset_new(", ")", "", ""},
      {"encstrset_new: set #", " created", "", ""}},

     {{"encstrset_delete(", ")", "", ""},
      {"encstrset_delete: set #", " deleted", "", ""},
      {"encstrset_delete: set #", " does not exist", "", ""}},

     {{"encstrset_size(", ")", "", ""},
      {"encstrset_size: set #", " contains ", " elements", ""},
      {"encstrset_size: set #", " does not exist", "", ""}},

     {{"encstrset_insert(", ", ", ", ", ")"},
      {"encstrset_insert: set #", ", cypher ", " inserted", ""},
      {"encstrset_insert: set #", " does not exist", "", ""},
      {"encstrset_insert: set #", ", cypher ", " already present", ""},
      {"encstrset_insert: invalid value (NULL)", "", "", ""}},

     {{"encstrset_remove(", ", ", ", ", ")"},
      {"encstrset_remove: set #", ", cypher ", " removed", ""},
      {"encstrset_remove: set #", " does not exist", "", ""},
      {"encstrset_remove: set #", ", cypher ", " was not present", ""},
      {"encstrset_remove: invalid value (NULL)", "", "", ""}},

     {{"encstrset_test(", ", ", ", ", ")"},
      {"encstrset_test: set #", ", cypher ", " is present", ""},
      {"encstrset_test: set #", " does not exist", "", ""},
      {"encstrset_test: set #", ", cypher ", " is not present", ""},
      {"encstrset_test: invalid value (NULL)", "", "", ""}},

     {{"encstrset_clear(", ")", "", ""},
      {"encstrset_clear: set #", " cleared", "", ""},
      {"encstrset_clear: set #", " does not exist", "", ""}},

     {{"encstrset_copy(", ", ", ")", ""},
      {"encstrset_copy: cypher ", " copied from set #", " to set #", ""},
      {"encstrset_copy: set #", " does not exist", "", ""},
      {"encstrset_copy: copied cypher ", " was already present in set #", "", ""}}};

string quote(const string &word) {
    return "\"" + word + "\"";
}

void diagnostic_message(request_type type, op_status status, const string &s1, const string &s2, const string &s3) {
    if (debug)
        cerr << message[type][status][0] << s1
             << message[type][status][1] << s2
             << message[type][status][2] << s3
             << message[type][status][3] << endl;
}

void diagnostic_message(request_type type, op_status status) {
    diagnostic_message(type, status, EMPTY, EMPTY, EMPTY);
}
void diagnostic_message(request_type type, op_status status, const string &s1) {
    diagnostic_message(type, status, s1, EMPTY, EMPTY);
}
void diagnostic_message(request_type type, op_status status, const string &s1, const string &s2) {
    diagnostic_message(type, status, s1, s2, EMPTY);
}

unsigned long id_counter = 0;  // licznik id dla zbiorów
static unordered_map<unsigned long, unordered_set<string>> &encstrmap() {
    static unordered_map<unsigned long, unordered_set<string>> encstrmap;  // mapa przechowująca wszystkie dane ecstrseta
    return encstrmap;
}
string encode(string &value, string &key) {  // koduje wartość za pomocą klucza zgodnie ze specyfikacją zadania
    if (key == "NULL" or key == "")
        return string(value);

    stringstream cypher;
    for (unsigned int i = 0; i < value.length(); i++)
        cypher << char(value[i] xor key[i % key.length()]);

    return cypher.str();
}

string string_to_hex(string src) {  // zwraca src zapisany w systemie szesnastkowym wielkimi literami
    stringstream ss;
    size_t size = src.length();

    for (unsigned int i = 0; i < size; i++)
        ss << hex << uppercase << (int)src[i] << " ";

    ss << char(8);
    return ss.str();
}
bool operation_condition(request_type type, unordered_set<string> &op_set, string &cypher) {
    switch (type) {
        case set_insert:
            return op_set.insert(cypher).second == true;
        case set_remove:
            return op_set.erase(cypher) > 0;
        case set_test:
            return op_set.find(cypher) != op_set.end();
        default:
            return false;
    }
}

bool general_operation(request_type type, unsigned long id, const char *value, const char *key) {
    string value_str = value == NULL ? "NULL" : value;
    string key_str = key == NULL ? "NULL" : key;
    diagnostic_message(type, intro, to_string(id),
                       (value == NULL ? value_str : quote(value_str)),
                       (key == NULL ? key_str : quote(key_str)));

    if (value == NULL) {
        diagnostic_message(type, null_value);
        return false;
    }

    auto queried_set = encstrmap().find(id);
    if (queried_set != encstrmap().end()) {
        auto &operation_set = queried_set->second;
        string cypher = encode(value_str, key_str);

        if (operation_condition(type, operation_set, cypher) == true) {
            diagnostic_message(type, success, to_string(id), quote(string_to_hex(cypher)));
            return true;
        } else {
            diagnostic_message(type, fail, to_string(id), quote(string_to_hex(cypher)));
            return false;
        }
    } else {
        diagnostic_message(type, no_set, to_string(id));
        return false;
    }
}
}  // namespace

namespace jnp1 {

unsigned long encstrset_new() {
    diagnostic_message(new_set, intro);

    encstrmap().insert(make_pair(id_counter, unordered_set<string>()));
    diagnostic_message(new_set, success, to_string(id_counter));

    return id_counter++;
}

void encstrset_delete(unsigned long id) {
    diagnostic_message(delete_set, intro, to_string(id));

    auto queried_set = encstrmap().find(id);
    if (queried_set != encstrmap().end()) {
        encstrmap().erase(queried_set);
        diagnostic_message(delete_set, success, to_string(id));
    } else
        diagnostic_message(delete_set, no_set, to_string(id));
}

size_t encstrset_size(unsigned long id) {
    diagnostic_message(set_size, intro, to_string(id));

    auto queried_set = encstrmap().find(id);
    if (queried_set != encstrmap().end()) {
        size_t count = queried_set->second.size();

        diagnostic_message(set_size, success, to_string(id), to_string(count));
        return count;
    } else {
        diagnostic_message(set_size, no_set, to_string(id));
        return 0;
    }
}

bool encstrset_insert(unsigned long id, const char *value, const char *key) {
    return general_operation(set_insert, id, value, key);
}

bool encstrset_remove(unsigned long id, const char *value, const char *key) {
    return general_operation(set_remove, id, value, key);
}

bool encstrset_test(unsigned long id, const char *value, const char *key) {
    return general_operation(set_test, id, value, key);
}

void encstrset_clear(unsigned long id) {
    diagnostic_message(set_clear, intro, to_string(id));
    auto queried_set = encstrmap().find(id);
    if (queried_set != encstrmap().end()) {
        queried_set->second.clear();
        diagnostic_message(set_clear, success, to_string(id));
    } else
        diagnostic_message(set_clear, no_set, to_string(id));
}

void encstrset_copy(unsigned long src_id, unsigned long dst_id) {
    diagnostic_message(set_copy, intro, to_string(src_id), to_string(dst_id));
    auto queried_source = encstrmap().find(src_id);
    auto queried_dest = encstrmap().find(dst_id);

    if (queried_source != encstrmap().end()) {
        if (queried_dest != encstrmap().end()) {
            auto &source_set = queried_source->second;
            auto &dest_set = queried_dest->second;

            for (auto elem : source_set) {
                if (dest_set.find(elem) != dest_set.end())
                    diagnostic_message(set_copy, fail, quote(string_to_hex(elem)), to_string(dst_id));
                else {
                    dest_set.insert(elem);
                    diagnostic_message(set_copy, success, quote(string_to_hex(elem)), to_string(dst_id));
                }
            }
        } else
            diagnostic_message(set_copy, no_set, to_string(dst_id));
    } else
        diagnostic_message(set_copy, no_set, to_string(src_id));
}

}  // namespace jnp1
