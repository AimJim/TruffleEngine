#pragma once

#	define LOGI(...)        \
		printf(__VA_ARGS__); \
		printf("\n")
#	define LOGW(...)        \
		printf(__VA_ARGS__); \
		printf("\n")
#	define LOGE(...)                 \
		fprintf(stderr, __VA_ARGS__); \
		fprintf(stderr, "\n")