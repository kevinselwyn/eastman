#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

static char *months[13] = {
	"January", "February", "March", "April", "May", "June", "Sol", "July",
	"August", "September", "October", "November", "December"
};

static char *weekdays[7] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

static void date2fixed(struct tm **new_tm, char *date) {
	int mon = 0, mday = 0, year = 0, i = 0, l = 0;
	char *month = NULL;
	struct tm tm = {
		.tm_year = 0,
		.tm_mon = 0,
		.tm_mday = 0
	};

	month = malloc(sizeof(char) * 10);

	if (!month) {
		goto cleanup;
	}

	(void)sscanf(date, "%s %d, %d", month, &mday, &year);

	for (i = 0, l = 13; i < l; i += 1) {
		if (strcmp(month, months[i]) == 0) {
			mon = i;
		}
	}

	if (mon > 5) {
		mon--;
	}

	tm.tm_year = year - 1900;
	tm.tm_mon = mon;
	tm.tm_mday = mday;

	mktime(&tm);
	*new_tm = &tm;

cleanup:
	if (month) {
		free(month);
	}
}

static void usage(char *exec) {
	int length = 0;

	length = (int)strlen(exec) + 7;

	printf("Usage: %s -d [\"<date>\"]\n", exec);
	printf("%*s -h\n", length, " ");
}

int main(int argc, char *argv[]) {
	int rc = 0;
	int yday = 0, year = 0, month = 0, day = 0, weekday = 0;
	int leapyear = 0, leapday = 0, yearday = 0;
	time_t now = 0;
	char *exec = NULL, *action = NULL, *date = NULL;
	struct tm *tm = NULL;

	now = time(0);
	tm = localtime(&now);

	if (argc >= 2) {
		exec = argv[0];
		action = argv[1];

		if (strcmp(action, "-h") == 0 || strcmp(action, "--help") == 0) {
			usage(exec);

			rc = 1;
			goto cleanup;
		} else if (strcmp(action, "-d") == 0 || strcmp(action, "--date") == 0) {
			date = argv[2];
			date2fixed(&tm, date);
		} else {
			printf("Unknown action %s\n", action);

			rc = 1;
			goto cleanup;
		}
	}

	yday = tm->tm_yday;
	year = tm->tm_year + 1900;

	leapyear = year % 4 == 0 ? 1 : 0;
	leapday = yday == 168 ? 1 : 0;

	if (leapyear == 1 && yday > 168) {
		yday--;
	}

	yearday = yday == 364 ? 1 : 0;

	month = (int)floor((double)yday / 28);
	day = yday % 28;
	weekday = day % 7;

	day++;

	if (yearday == 1) {
		printf("Year Day, %d\n", year);
	} else if (leapday == 1) {
		printf("Leap Day, %d\n", year);
	} else {
		printf("%s, %s %d, %d\n", weekdays[weekday], months[month], day, year);
	}

cleanup:
	return rc;
}
