#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#define NO_NEW_LINE 1

const char *target_user;
const char *target_data;

void print_users(struct json_object *js_f, const char *u)
{
	json_object_object_foreach(js_f, k, v) {
		if (u && strcmp(u, k))
			continue;
		printf("Acount: '%s'\n", k);
		printf("ak: %s\nsk: %s\nregion: %s\n",
		       json_object_to_json_string(
			       json_object_object_get(v, "access_key")),
		       json_object_to_json_string(
			       json_object_object_get(v, "secret_key")),
		       json_object_to_json_string(
			       json_object_object_get(v, "region")));
	}
}

int main(int ac, char **av)
{
	const char *dest = "/.oapi_credentials";
	char buf[1024];
	char *home = getenv("HOME");
	struct json_object *js_f;
	int mode = 0;
	int flag = 0;

	for (int i = 1; i < ac; ++i) {
		if (!strcmp(av[i], "-u")) {
			mode = 1;
			if (i + 1 >= ac) {
				fprintf(stderr, "wrong argument\n");
				return 1;
			}
			target_user = av[i + 1];
			++i;
		} else if (!strcmp(av[i], "-ud")) {
			mode = 2;
			if (i + 2 >= ac) {
				fprintf(stderr, "wrong argument\n");
				return 1;
			}
			target_user = av[i + 1];
			if (!strcmp(av[i+2], "ak") ||
			    !strcmp(av[i+2], "AK")) {
				target_data = "access_key";
			} else if (!strcmp(av[i+2], "sk") ||
				   !strcmp(av[i+2], "SK")) {
				target_data = "secret_key";
			} else {
				target_data = av[i + 2];
			}
			i += 2;
		} else if (!strcmp(av[i], "-n")) {
			flag |= NO_NEW_LINE;
		} else {
			fprintf(stderr, "unknow argument\n");
			return 1;
		}
	}

	strcpy(stpcpy(buf, home), dest);
	js_f = json_object_from_file(buf);
	if (!js_f) {
		fprintf(stderr, "can't open %s\n", buf);
		return 1;
	}

	if (mode == 0 || mode == 1)
		print_users(js_f, target_user);
	else if (mode == 2) {
		printf("%s",
		       json_object_get_string(
			       json_object_object_get(
				       json_object_object_get(js_f,
							      target_user),
				       target_data)));
		if (flag & NO_NEW_LINE)
			fflush(stdout);
		else
			printf("\n");
	}
	return 0;
}
