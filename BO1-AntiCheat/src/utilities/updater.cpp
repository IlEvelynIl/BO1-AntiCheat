#include "updater.hpp"

#include <cpprest/http_client.h>

#include <cpprest/json.h>

#include <iostream>

#include <windows.h>

#include <shellapi.h>

#include "../constants.h"

#include "../display/statuses.h"

#include "../display/display.hpp"

using namespace web;
using namespace web::http;
using namespace web::http::client;

using namespace std;

bool declined_update = false;

void Updater::CheckForUpdates() {
    http_client client(U("https://api.github.com/repos/IlEvelynIl/BO1-AntiCheat/releases/latest"));
    http_request request(methods::GET);

    try {
        client.request(request).then([](http_response response) {
            if (response.status_code() == status_codes::OK) {
                return response.extract_json();
            }
            else {
                MessageBox(NULL, DisplayStatuses::COULDNT_CHECK_UPDATES, L"Error", MB_OK);
                return pplx::task_from_result(json::value());
            }
        }).then([](pplx::task<json::value> jsonTask) {
            try {
                json::value json_response = jsonTask.get();

                if (json_response.has_field(U("tag_name"))) {
                    string tag_name = utility::conversions::to_utf8string(json_response[U("tag_name")].as_string());

                    if (tag_name != Constants::VERSION) {
                        int result = MessageBox(NULL, DisplayStatuses::NEW_UPDATE_AVAILABLE, Constants::TITLE, MB_YESNO | MB_ICONINFORMATION);

                        if (result == IDYES) {
                            ShellExecute(NULL, L"open", L"https://github.com/IlEvelynIl/BO1-AntiCheat/releases", NULL, NULL, SW_SHOWNORMAL);
                        }
                    }
                }
            }
            catch (const exception&) {
                MessageBox(NULL, DisplayStatuses::COULDNT_PROCESS_UPDATE, L"Error", MB_OK);
            }
        }).wait();
    }
    catch (const exception&) {
        MessageBox(NULL, DisplayStatuses::COULDNT_CHECK_UPDATES, L"Error", MB_OK);
    }
}

bool Updater::DeclinedUpdate()
{
    return declined_update;
}