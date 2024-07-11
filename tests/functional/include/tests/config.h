// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <iomanip>

#include "./utils.h"
#include "LogKit.h"
#include "boost/ut.hpp"

namespace ut = boost::ut;

namespace cfg {

using namespace std::literals;
using namespace leka;

namespace output {

	inline constexpr auto RUN		  = std::string_view {"\033[32m[ RUN      ]\033[00m "};
	inline constexpr auto OKAY		  = std::string_view {"\033[32m[       OK ]\033[00m "};
	inline constexpr auto SKIP		  = std::string_view {"\033[33m[ SKIP     ]\033[00m "};
	inline constexpr auto FAILED	  = std::string_view {"\033[31m[  FAILED  ]\033[00m "};
	inline constexpr auto PASSED	  = std::string_view {"\033[32m[  PASSED  ]\033[00m "};
	inline constexpr auto SKIPPED	  = std::string_view {"\033[33m[   SKIP   ]\033[00m "};
	inline constexpr auto SINGLE_LINE = std::string_view {"\033[32m[----------]\033[00m "};
	inline constexpr auto DOUBLE_LINE = std::string_view {"\033[32m[==========]\033[00m "};

}	// namespace output

template <class TPrinter = ut::printer>
class reporter : public ut::reporter<ut::printer>
{
	bool is_first_test = true;

  public:
	auto on(ut::events::test_begin test_begin) -> void
	{
		if (is_first_test) {
			is_first_test = false;

			printer_ << '\n'
					 << output::DOUBLE_LINE << "Running " << TARGET << " tests." << '\n'
					 << output::SINGLE_LINE << '\n';

			utils::time::main::start = rtos::Kernel::Clock::now();
		}

		printer_ << '\n' << output::RUN << test_begin.name;
		fails_					 = asserts_.fail;
		utils::time::test::start = rtos::Kernel::Clock::now();
	}

	auto on(ut::events::test_run test_run) -> void { printer_ << '\n' << "  \"" << test_run.name << "\"..."; }

	auto on(ut::events::test_skip test_skip) -> void
	{
		printer_ << '\n' << output::SKIP << test_skip.name;
		++tests_.skip;
	}

	auto on(ut::events::test_end test_end) -> void
	{
		utils::time::test::stop = rtos::Kernel::Clock::now();
		if (asserts_.fail > fails_) {
			++tests_.fail;
			printer_ << '\n' << output::FAILED << test_end.name;
		} else {
			++tests_.pass;
			printer_ << '\n' << output::OKAY << test_end.name << " (" << utils::time::test::delta() << " ms)";
		}
	}

	template <class TMsg>
	auto on(ut::events::log<TMsg> log) -> void
	{
		printer_ << log.msg;
	}

	auto on(ut::events::exception exception) -> void {}

	template <class TExpr>
	auto on(ut::events::assertion_pass<TExpr> assertion) -> void
	{
		++asserts_.pass;
	}

	template <class TExpr>
	auto on(ut::events::assertion_fail<TExpr> assertion) -> void
	{
		printer_ << '\n'
				 << assertion.location.file_name() << ':' << assertion.location.line() << ':' << printer_.colors().fail
				 << " Failure" << printer_.colors().none << '\n'
				 << "  " << std::boolalpha << assertion.expr << printer_.colors().none;

		++asserts_.fail;
	}

	auto on(ut::events::fatal_assertion fatal) -> void {}

	auto on(ut::events::summary summary) -> void
	{
		utils::time::main::stop = rtos::Kernel::Clock::now();

		auto total_asserts = (asserts_.pass + asserts_.fail);
		auto total_tests   = (tests_.pass + tests_.fail);

		auto width_asserts = total_asserts >= 100 ? 3 : total_asserts >= 10 ? 2 : 1;
		auto width_tests   = total_tests >= 100 ? 3 : total_tests >= 10 ? 2 : 1;

		printer_ << '\n'
				 << '\n'
				 << output::SINGLE_LINE << "Ran " << TARGET << " tests." << '\n'
				 << output::DOUBLE_LINE << std::right << std::setw(width_asserts) << total_asserts << " asserts in "
				 << std::right << std::setw(width_tests) << total_tests << " tests ran. (" << utils::time::main::delta()
				 << " ms total)" << '\n';

		if (tests_.fail or asserts_.fail) {
			printer_ << output::PASSED << std::right << std::setw(width_asserts) << asserts_.pass << " asserts in "
					 << std::right << std::setw(width_tests) << tests_.pass << " tests." << '\n';
			printer_ << output::FAILED << std::right << std::setw(width_asserts) << asserts_.fail << " asserts in "
					 << std::right << std::setw(width_tests) << tests_.fail << " tests." << '\n';
		} else {
			printer_ << output::PASSED << "All tests passed!" << '\n';
		}

		if (tests_.skip) {
			printer_ << output::SKIPPED << std::right << std::setw(width_asserts) << tests_.skip << " tests." << '\n';
		}

		printer_ << '\n';

		log_ll(printer_.str().data(), printer_.str().size());
	}
};

}	// namespace cfg

template <>
inline auto ut::cfg<ut::override> = ut::runner<cfg::reporter<ut::printer>> {};
