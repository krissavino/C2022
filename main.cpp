#include <iostream>
#include <windows.h>
#include <fstream>
#include <set>
/*
 * сторонняя библиотека для работа с json в C++, чтобы она заработала, нужно создать папку, куда будем её
 * скачивать, далле запустить git bash и перейти в эту папку посредством команды "cd <Полный путь до созданной папки>"
 * далее прописать команду "git clone https://github.com/nlohmann/json.git"
 * ждать пока скачается и потом подключить библиотеку посредством двух #include директив
*/
#include "D:\GitRepos\OC 2022\json\single_include\nlohmann\json.hpp"
#include "D:\GitRepos\OC 2022\json\single_include\nlohmann\json_fwd.hpp"


using namespace std;
using json = nlohmann::json;

void GetAllTransactionsTypes(const json& document, const std::string& filename) {
    std::set<string> result;

    for (auto& item : document["transactions"sv].items()) {
        for (auto& items : item.value().items()) {
            if (items.key() == "oper_type"sv) {
                result.insert(items.value().get<std::string>());
            }
        }
    }

    std::ofstream res_file(filename, ios_base::trunc);

    bool is_first = true;

    for (auto& item : result) {
        if (!is_first) {
            res_file << '\n';
        }

        is_first = false;

        res_file << item;
    }
}

void DeleteAddOperationsFromSource(const json& document, const string& filename) {
    json result = {
            {"transactions"sv, {}}
    };

    for (auto& item : document["transactions"sv].items()) {
        for (auto& items : item.value().items()) {
            if (items.key() == "oper_type"sv) {
                if (items.value().get<std::string>() == "Пополнение"sv) {
                    continue;
                }

                result["transactions"sv].push_back({item.key(), item.value()});
            }
        }
    }

    std::ofstream res_file(filename, ios_base::trunc);

    res_file << result.dump(4);
}

void GetOnlySuccesOperations(const json& document, const string& filename) {
    json result = {
            {"transactions"sv, {}}
    };

    for (auto& item : document["transactions"sv].items()) {
        bool flag1 = false, flag2 = false;
        for (auto& items : item.value().items()) {

            if (items.key() == "oper_type"sv) {
                if (items.value().get<std::string>() == "Оплата"sv || items.value().get<std::string>() == "Снятие"sv) {
                    flag1 = true;
                }
            } else if (items.key() == "oper_result"sv) {
                if (items.value().get<std::string>() == "Успешно"sv) {
                    flag2 = true;
                }
            }
        }

        if (flag1 && flag2) {
            result["transactions"sv].push_back({item.key(), item.value()});
        }
    }

    std::ofstream res_file(filename, ios_base::trunc);

    res_file << result.dump(4);
}

void SortByKey(const string& sortingKey, const json& document, const string& filename) {
    json result;

    for (auto& item : document["transactions"sv].items()) {
        std::string aa, dd, ee, ff, gg, key;
        std::string bb;
        std::string cc;

        for (auto& items : item.value().items()) {
            if (items.key() == "last_name"sv)   {
                aa = items.value().get<std::string>();
            }
            if (items.key() == "first_name"sv)   {
                bb = items.value().get<std::string>();
            }
            if (items.key() == "patronymic"sv)   {
                cc = items.value().get<std::string>();
            }
            if (items.key() == "passport"sv)   {
                if (items.value().type_name() == "string") {
                    dd = items.value().get<std::string>();
                } else {
                    dd = to_string(items.value().get<int>());
                }
            }
            if (items.key() == "date_of_birth"sv)   {
                ee = items.value().get<std::string>();
            }
            if (items.key() == "phone"sv)   {
                ff = items.value().get<std::string>();
            }
            if (items.key() == sortingKey)   {
                key = to_string(items.value());
            }



        }

        aa += bb;
        aa += cc;
        aa += dd;
        aa += ee;
        aa += ff;

        auto a = item.value();
        a.erase("last_name");
        a.erase("first_name");
        a.erase("patronymic");
        a.erase("phone");
        a.erase("first_name");
        a.erase("passport_valid_to");
        a.erase("passport");
        //a.erase("account");
        a.erase("account_valid_to");
        a.erase("date_of_birth");
        //a.erase("card");
        //a.erase("client");

        result["transactions"sv][(string)key].push_back(a);
    }

    std::ofstream res_file(filename, ios_base::trunc);

        for (auto &item: result["transactions"sv].items()) {
            if (item.value().size() > 1)
                res_file << item.key() << ":" << item.value() << "\n";
        }
}

void SearchForMaxUsers(const string& filename) {
    ifstream ifs(filename);
    json data = json::parse(ifs);

    map<string, int> check;
    for (auto& item : data["transactions"sv].items()) {
        check[item.key()] = item.value().size();
    }
    std::set<std::pair<int, std::string>> s;  // The new (temporary) container.

    for (auto const &kv : check)
        s.emplace(kv.second, kv.first);  // Flip the pairs.

    ofstream ofs("res2_2.txt");

    for (auto it = --s.end(); it != s.begin(); --it) {
        ofs << it->second << ":" << it->first << endl;
    }
    ofs << s.begin()->second << ":" << s.begin()->first << endl;
}

void SearchForMaxUsersAndUniqueCards(const string& filename) {
    ifstream ifs(filename);
    json data = json::parse(ifs);

    map<string, std::pair<int, set<string>>> check;

    for (auto& item : data["transactions"sv].items()) {
        check[item.key()].first = item.value().size();
    }

    for (auto& item : data["transactions"sv].items()) {
        for (auto& item_2 : item.value().items()) {
            for (auto& item_3 : item_2.value().items()){
                if (item_3.key() == "card") {
                    check[item.key()].second.insert(item_3.value());
                }
            }

            //cout << item_2.value()["account"] << endl;

            //cout << item_2.value() << endl;
        }
    }


    std::set<std::pair<int, std::pair<int, std::string>>> s;  // The new (temporary) container.

    for (auto const &kv : check)
        s.insert({kv.second.second.size(), {kv.second.first, kv.first}});  // Flip the pairs.

    ofstream ofs("res2_3.txt");

    for (auto it = --s.end(); it != s.begin(); --it) {
        ofs << it->second.second << ":" << it->second.first << ":" << it->first << endl;
    }

    ofs << s.begin()->second.second << ":" << s.begin()->first << ":" << s.begin()->second.first << endl;

}

void searchExpiredTransactions(const string& filename) {
    ifstream ifs(filename);
    json data = json::parse(ifs);


    map<string, string> check;

    for (auto& item : data["transactions"sv].items()) {
        string accountValidTo = "";
        for (auto &item_2: item.value().items())
        {
            if(item_2.key() == "account_valid_to") {
                accountValidTo = item_2.value();
            }
            if(item_2.key() == "date") {
                for (int i = 0; i < 10; i++)
                    if (accountValidTo < ((string) item_2.value())) {
                        //cout << accountValidTo + " " + (string) item_2.value() << endl;
                        check[item.key()] = accountValidTo + " " + (string) item_2.value();
                        continue;
                    }
            }
        }
    }

    ofstream ofs("expiredTransactions.txt");

    for (auto it = check.begin(); it != check.end(); ++it) {

        ofs << it->first << ":" << it->second << endl;
    }
}

void searchRepeatAmount(const string& filename) {
    ifstream ifs(filename);
    json data = json::parse(ifs);


    map<string, string> check;

    for (auto& item : data["transactions"sv].items()) {
        string accountValidTo = "";
        for (auto &item_2: item.value().items())
        {
            if(item_2.key() == "account_valid_to") {
                accountValidTo = item_2.value();
            }
            if(item_2.key() == "date") {
                for (int i = 0; i < 10; i++)
                    if (accountValidTo < ((string) item_2.value())) {
                        //cout << accountValidTo + " " + (string) item_2.value() << endl;
                        check[item.key()] = accountValidTo + " " + (string) item_2.value();
                        continue;
                    }
            }
        }
    }

    ofstream ofs("repeatAmount.txt");

    for (auto it = check.begin(); it != check.end(); ++it) {

        ofs << it->first << ":" << it->second << endl;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    string input_file_name = "transactions.json";

    std::ifstream input_file(input_file_name);
    json data = json::parse(input_file);

    //searchExpiredTransactions(input_file_name);
    SortByKey("client", data, "sorted_by_key.json");

   /* GetAllTransactionsTypes(data, "TransactionsTypes.txt");
    DeleteAddOperationsFromSource(data, "result_after_deleting_add.json");
    GetOnlySuccesOperations(data, "OnlySuccesOperations.json");
    SortByUser(data, "sorted_2.json");
    SearchForMaxUsers("sorted_2.json");
    SearchForMaxUsersAndUniqueCards("sorted_2.json");*/
}


