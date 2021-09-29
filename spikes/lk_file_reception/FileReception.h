// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_FILE_RECEPTION_H_
#define _LEKA_OS_INTERFACE_FILE_RECEPTION_H_

namespace leka::interface {

class FileReception
{
  public:
	virtual ~FileReception() = default;

	virtual void setFileName(std::string name, std::string path) = 0;
	virtual void setFileSize(size_t size)						 = 0;
	virtual auto setReadyToReceiveFile() -> bool				 = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_FILE_RECEPTION_H_
