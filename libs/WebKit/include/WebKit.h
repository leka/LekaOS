// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <span>

#include "interface/drivers/Network.h"
#include "interface/platform/File.h"

namespace leka {

class WebKit
{
  public:
	struct DownloadableFile {
		const char *url;
		const char *to_path;
	};

	explicit WebKit(interface::Network &network, interface::File &file_handler)
		: _network(network), _file_handle(file_handler)
	{
		// nothing do to
	}

	[[nodiscard]] auto connect(const char *ssid, const char *pass) -> bool;

	void setCertificateStore(std::span<const char *> certificates_path_list);

	auto downloadFile(DownloadableFile const &file) -> bool;

	[[nodiscard]] auto responseHasRedirectionUrl(HttpResponse const &response) const -> bool;
	void getRedirectionUrl(HttpResponse const &response);

	auto updateCertificate(const char *certificate_path) -> bool;

	[[nodiscard]] auto getUrl() const -> const std::string &;

  private:
	interface::Network &_network;
	interface::File &_file_handle;

	std::string _url;

	std::array<const char *, 5> _certificates_path_list {};
	std::array<char, 8192> _certificate {};
};

}	// namespace leka
