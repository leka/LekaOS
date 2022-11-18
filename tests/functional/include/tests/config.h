// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LogKit.h"
#include "boost/ut.hpp"

namespace ut = boost::ut;

namespace cfg {

template <class TPrinter = ut::printer>
class reporter : public ut::reporter<ut::printer>
{
  public:
	auto on(ut::events::test_begin test_begin) -> void
	{
		printer_ << "Running \"" << test_begin.name << "\"...";
		fails_ = asserts_.fail;
	}

	auto on(ut::events::test_run test_run) -> void { printer_ << "\n \"" << test_run.name << "\"..."; }

	auto on(ut::events::test_skip test_skip) -> void
	{
		printer_ << test_skip.name << "...SKIPPED\n";
		++tests_.skip;
	}

	auto on(ut::events::test_end test_end) -> void
	{
		if (asserts_.fail > fails_) {
			++tests_.fail;
			printer_ << '\n';
			printer_ << "Running \"" << test_end.name << "\"...";
			printer_ << printer_.colors().fail << "FAILED" << printer_.colors().none << '\n';
		} else {
			++tests_.pass;
			printer_ << '\n';
			printer_ << "Running \"" << test_end.name << "\"...";
			printer_ << printer_.colors().pass << "PASSED" << printer_.colors().none << '\n';
		}
	}

	template <class TMsg>
	auto on(ut::events::log<TMsg> l) -> void
	{
		printer_ << l.msg;
	}

	auto on(ut::events::exception exception) -> void
	{
		printer_ << '\n'
				 << "  " << printer_.colors().fail << "Unexpected exception with message:\n"
				 << exception.what() << printer_.colors().none;
		++asserts_.fail;
	}

	template <class TExpr>
	auto on(ut::events::assertion_pass<TExpr> assertion) -> void
	{
		++asserts_.pass;
	}

	template <class TExpr>
	auto on(ut::events::assertion_fail<TExpr> assertion) -> void
	{
		constexpr auto short_name = [](std::string_view name) {
			return name.rfind('/') != std::string_view::npos ? name.substr(name.rfind('/') + 1) : name;
		};

		printer_ << '\n'
				 << "  " << short_name(assertion.location.file_name()) << ':' << assertion.location.line() << ':'
				 << " [" << std::boolalpha << assertion.expr << printer_.colors().none << ']';

		++asserts_.fail;
	}

	auto on(ut::events::fatal_assertion) -> void {}

	auto on(ut::events::summary) -> void
	{
		printer_ << "\n========================================================"
					"=======================\n";

		if (tests_.fail or asserts_.fail) {
			printer_ << "tests:   " << (tests_.pass + tests_.fail) << " | " << printer_.colors().fail << tests_.fail
					 << " failed" << printer_.colors().none << '\n';
			printer_ << "asserts: " << (asserts_.pass + asserts_.fail) << " | " << asserts_.pass << " passed"
					 << " | " << printer_.colors().fail << asserts_.fail << " failed" << printer_.colors().none << '\n';
		} else {
			printer_ << printer_.colors().pass << "All tests passed" << printer_.colors().none << " (" << asserts_.pass
					 << " asserts in " << tests_.pass << " tests)\n";

			if (tests_.skip) {
				printer_ << tests_.skip << " tests skipped\n";
			}
		}

		log_ll(printer_.str().data(), printer_.str().size());
	}
};

}	// namespace cfg

template <>
inline auto ut::cfg<ut::override> = ut::runner<cfg::reporter<ut::printer>> {};
