#include "updater.hpp"

#include <cpprest/http_client.h>

#include <cpprest/json.h>

#include <iostream>

#include <windows.h>

#include <shellapi.h>

#include "../../constants.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

using namespace std;

namespace anticheat {
    namespace updater {

        // just simply looks at the latest github release tag and if it doesnt match then they arent up to date obviously
        void CheckForUpdates() {
            // Configure the HTTP client with secure settings
            http_client_config config;
            config.set_validate_certificates(true);

            http_client client(U("https://api.github.com/repos/IlEvelynIl/BO1-AntiCheat/releases/latest"), config);
            http_request request(methods::GET);

            try {
                client.request(request).then([](http_response response) {
                    if (response.status_code() == status_codes::OK) {
                        return response.extract_json();
                    }
                    else {
                        MessageBoxA(NULL, Statuses::COULDNT_CHECK_UPDATES.c_str(), "Error", MB_OK);
                        return pplx::task_from_result(json::value());
                    }
                    }).then([](pplx::task<json::value> jsonTask) {
                        try {
                            json::value json_response = jsonTask.get();

                            if (json_response.has_field(U("tag_name"))) {
                                string tag_name = utility::conversions::to_utf8string(json_response[U("tag_name")].as_string());
                                string description = utility::conversions::to_utf8string(json_response[U("body")].as_string());

                                if (tag_name != Constants::VERSION) {
                                    string updateMessage = Statuses::NEW_UPDATE_AVAILABLE + "\n\nWhat changed:\n" + description;
                                    int result = MessageBoxA(NULL, updateMessage.c_str(), "BO1 Anti Cheat (Updater)", MB_YESNO | MB_ICONINFORMATION);

                                    // when they click yes to wanting to open the download page, open the github release url
                                    if (result == IDYES) {
                                        ShellExecute(NULL, L"open", L"https://github.com/IlEvelynIl/BO1-AntiCheat/releases/latest", NULL, NULL, SW_SHOWNORMAL);
                                    }
                                }
                            }
                        }
                        catch (const exception&) {
                            MessageBoxA(NULL, Statuses::COULDNT_PROCESS_UPDATE.c_str(), "Error", MB_OK);
                        }
                    }).wait();
            }
            catch (const exception&) {
                MessageBoxA(NULL, Statuses::COULDNT_CHECK_UPDATES.c_str(), "Error", MB_OK);
            }
        }

    } // updater
} // anticheat