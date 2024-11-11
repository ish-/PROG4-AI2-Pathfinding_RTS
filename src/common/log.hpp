#pragma once
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmacro-redefined"

#define COUT(...)
#define LOG(...)
#define TEST(...)

#ifndef COUT_DISABLE
	#define CRED(msg) "\033[31m" msg "\033[0m"
	#define CGREEN(msg) "\033[32m" msg "\033[0m"
	#define CYELLOW(msg) "\033[33m" msg "\033[0m"
	#define CBOLD(msg) "\033[1m" msg "\033[0m"

	#define COUT(...) _COUT(__VA_ARGS__)

	template <typename... T>
	inline void _COUT (T&&... args) {
		((std::cout << args << " "), ...);
		std::cout << '\n';
	}

	#ifndef LOG_DISABLE
		#define LOG(...) _LOG(__VA_ARGS__)

		template <typename... T>
		inline void _LOG (const char* title, T&&... args) {
			if constexpr (sizeof...(args) > 0)
				_COUT( CYELLOW("[LOG]"), title, "->", args... );
			else
				_COUT( CYELLOW("[LOG]"), title);
		}
	#endif

	#ifndef TEST_DISABLE
		#define TEST(...) _TEST(__VA_ARGS__)

		template <typename T>
		bool eqByRef (T& a, T& b) { return &a == &b; }
		template <typename T>
		bool eqByVal (T& a, T& b) { return a == b; }

		template <typename T>
		void _TEST (const char* name, T res, std::type_identity_t<T> exp, bool (*eq)(T&, T&) = eqByVal) {
			bool passed = eq(res, exp);
			if (passed)
				_COUT(CBOLD(CGREEN("[TEST] Pass")), name, "=", res);
			else
				_COUT(CBOLD(CRED("[TEST] FAIL")), name, res, "!=", exp);
			#ifdef TEST_THROW
				throw std::runtime_error("Test failed");
			#endif
		}
	#endif
#endif

#pragma GCC diagnostic pop
