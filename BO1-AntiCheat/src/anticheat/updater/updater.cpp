#include "updater.hpp"

#include <cpprest/http_client.h>

#include <cpprest/json.h>

#include <iostream>

#include <windows.h>

#include <shellapi.h>

#include "../../constants.h"

#include <future>

using namespace web;
using namespace web::http;
using namespace web::http::client;

using namespace std;

namespace anticheat {
	namespace updater {

		// just simply looks at the latest github release tag and if it doesnt match then they arent up to date obviously
		std::string GetAvailableUpdates() {
			try {
				// Configure the HTTP client with secure settings
				http_client_config config;
				config.set_validate_certificates(true);

				http_client client(U("https://api.github.com/repos/IlEvelynIl/BO1-AntiCheat/releases/latest"), config);
				http_request request(methods::GET);

				// Use promise/future to return the result synchronously
				std::promise<std::string> updatePromise;
				auto updateFuture = updatePromise.get_future();

				client.request(request).then([&updatePromise](http_response response) {
					if (response.status_code() == status_codes::OK) {
						return response.extract_json();
					}
					else {
						MessageBoxA(NULL, Statuses::COULDNT_CHECK_UPDATES.c_str(), "Error", MB_OK);
						updatePromise.set_value("");
						return pplx::task_from_result(json::value());
					}
					}).then([&updatePromise](pplx::task<json::value> jsonTask) {
						try {
							json::value json_response = jsonTask.get();

							if (json_response.has_field(U("tag_name"))) {
								std::string tag_name = utility::conversions::to_utf8string(json_response[U("tag_name")].as_string());
								std::string description = utility::conversions::to_utf8string(json_response[U("body")].as_string());
								if (tag_name != Constants::VERSION) {
									std::string available_updates = description;
									updatePromise.set_value("v" + tag_name + ":\n" + available_updates);
									return;
								}
							}
							updatePromise.set_value(""); // No updates available
						}
						catch (const std::exception&) {
							MessageBoxA(NULL, Statuses::COULDNT_PROCESS_UPDATE.c_str(), "Error", MB_OK);
							updatePromise.set_value("");
						}
					});

					return updateFuture.get(); // Wait for promise result
			}
			catch (const std::exception&) {
				MessageBoxA(NULL, Statuses::COULDNT_CHECK_UPDATES.c_str(), "Error", MB_OK);
				return "";
			}
		}

	} // updater
} // anticheat