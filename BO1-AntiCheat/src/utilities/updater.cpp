#include "updater.hpp"

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>
#include <windows.h>
#include <shellapi.h>

#include "../constants.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

using namespace std;

void Updater::CheckForUpdates() {
    http_client client(U("https://api.github.com/repos/IlEvelynIl/BO1-AntiCheat/releases/latest"));
    http_request request(methods::GET);

    try {
        client.request(request).then([](http_response response) {
            if (response.status_code() == status_codes::OK) {
                return response.extract_json();
            }
            MessageBox(NULL, L"There was a problem checking for updates.", L"Error", MB_OK);
            }).then([](json::value jsonResponse) {
                if (jsonResponse.has_field(U("tag_name"))) {
                    utility::string_t tagName = jsonResponse[U("tag_name")].as_string();
                    string tagNameStr = utility::conversions::to_utf8string(tagName);

                    if (tagNameStr != Constants::VERSION) {
                        int result = MessageBox(NULL, L"A new update is available!\nWould you like to open the release page?", Constants::TITLE, MB_YESNO | MB_ICONINFORMATION);

                        if (result == IDYES) {
                            ShellExecute(NULL, L"open", L"https://github.com/IlEvelynIl/BO1-AntiCheat/releases", NULL, NULL, SW_SHOWNORMAL);
                        }
                    }
                }
                }).wait();
    }
    catch (const exception& e) {
        MessageBox(NULL, L"There was a problem checking for updates.", L"Error", MB_OK);
    }
}
