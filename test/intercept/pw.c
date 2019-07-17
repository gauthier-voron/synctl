#define _GNU_SOURCE


#include <dlfcn.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define offsetof(__s, __f) \
	((size_t) (&((__s *) 0)->__f))

#define PARSE_FIELD_NAME     0
#define PARSE_FIELD_PASSWD   1
#define PARSE_FIELD_UID      2
#define PARSE_FIELD_GID      3
#define PARSE_FIELD_GECOS    4
#define PARSE_FIELD_DIR      5
#define PARSE_FIELD_SHELL    6
#define PARSE_FIELD_NOMORE   7

#define PARSE_TYPE_STR       0
#define PARSE_TYPE_NUM       1

#define PARSE_FIELD_NEXT(__f)  ((__f) + 1)

#define PARSE_FIELD(__s, __f, __t)  \
	(*((__t *) (((char *) (__s)) + PARSE_TYPE_OFFSET(__f))))

#define READ_BUFLEN  1024

#define STORE_BUFLEN 1024


static inline int PARSE_TYPE_CODE(int field) {
	switch (field) {
	case PARSE_FIELD_UID:
	case PARSE_FIELD_GID:
		return PARSE_TYPE_NUM;
	default:
		return PARSE_TYPE_STR;
	}
}

static inline size_t PARSE_TYPE_OFFSET(int field) {
	switch (field) {
	case PARSE_FIELD_NAME:
		return offsetof(struct passwd, pw_name);
	case PARSE_FIELD_PASSWD:
		return offsetof(struct passwd, pw_passwd);
	case PARSE_FIELD_UID:
		return offsetof(struct passwd, pw_uid);
	case PARSE_FIELD_GID:
		return offsetof(struct passwd, pw_gid);
	case PARSE_FIELD_GECOS:
		return offsetof(struct passwd, pw_gecos);
	case PARSE_FIELD_DIR:
		return offsetof(struct passwd, pw_dir);
	case PARSE_FIELD_SHELL:
		return offsetof(struct passwd, pw_shell);
	default:
		return ((size_t) -1);
	}
}

static inline char *__create_command(const char *base, const char *fmt,
				     va_list ap)
{
	char *true_fmt, *command;
	size_t blen, flen;
	va_list cpy;
	int ret;

	blen = strlen(base);
	flen = strlen(fmt);
	true_fmt = alloca(blen + 1 + flen + 1);

	strcpy(true_fmt, base);
	true_fmt[blen] = ' ';
	strcpy(true_fmt + blen + 1, fmt);

	va_copy(cpy, ap);
	ret = vsnprintf(NULL, 0, true_fmt, cpy);

	if (ret < 0)
		return NULL;

	command = malloc(ret + 1);

	if (command == NULL)
		return NULL;

	vsnprintf(command, ret + 1, true_fmt, ap);

	return command;
}

static inline char *__invoke(const char *env, int *ecode, const char *fmt, ...)
{
	char *cmd, *buf, *nbuf;
	size_t cap, len;
	int pipefd[2];
	ssize_t ssize;
	va_list ap;
	pid_t pid;

	cmd = getenv(env);
	buf = NULL;

	if (cmd == NULL)
		goto err;
	if (pipe(pipefd) != 0)
		goto err;

	if ((pid = fork()) == 0) {
		va_start(ap, fmt);
		cmd = __create_command(cmd, fmt, ap);
		va_end(ap);

		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);

		execlp("sh", "sh", "-c", cmd, NULL);
		exit(EXIT_FAILURE);
	}

	close(pipefd[1]);

	cap = len = 0;

	do {
		if (cap < (len + READ_BUFLEN)) {
			cap = len + READ_BUFLEN;
			nbuf = realloc(buf, cap);

			if (nbuf == NULL)
				goto err;

			buf = nbuf;
		}

		ssize = read(pipefd[0], buf + len, READ_BUFLEN - 1);

		if (ssize < 0)
			goto err;

		len += (size_t) ssize;
	} while (ssize > 0);

	buf[len++] = '\0';

	waitpid(pid, ecode, 0);
	*ecode = WEXITSTATUS(*ecode);
	return buf;
 err:
	free(buf);
	*ecode = -1;
	return NULL;
}

static void __log(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	fprintf(stderr, "intercept (%d): ", getpid());
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
}

static int __parse_entry(struct passwd *dest, char *buf, size_t buflen,
			 const char *entry)
{
	int field = PARSE_FIELD_NAME;
	const char *ptr = entry;
	size_t len;
	char *err;

	ptr -= 1;
	do {
		ptr++;

		if ((*ptr != ':') && (*ptr != '\n') && (*ptr != '\0'))
			continue;

		len = ptr - entry;

		switch (PARSE_TYPE_CODE(field)) {
		case PARSE_TYPE_STR:
			if (len + 1 > buflen)
				return -1;

			strncpy(buf, entry, len);
			buf[len] = '\0';

			PARSE_FIELD(dest, field, char *) = buf;

			buf += len + 1;
			buflen -= len + 1;
			break;
		case PARSE_TYPE_NUM:
			PARSE_FIELD(dest,field,uid_t) = strtol(entry,&err,10);

			if (err != ptr)
				return -1;

			break;
		}

		entry = ptr + 1;
		field = PARSE_FIELD_NEXT(field);
	} while ((*ptr != '\n') && (*ptr != '\0'));

	if (field == PARSE_FIELD_NOMORE)
		return 0;

	return -1;
}


struct passwd *getpwnam(const char *name)
{
	static struct passwd *(*defunc)(const char *) = NULL;
	static char storbuf[STORE_BUFLEN];
	static struct passwd storage;
	struct passwd *ret = NULL;
	char *stdout;
	int ecode;

	stdout = __invoke("INTERCEPT_GETPWNAM", &ecode, "%s", name);

	if (ecode != 0)
		goto deflt;

	if (__parse_entry(&storage, storbuf, sizeof (storbuf), stdout) == 0)
		ret = &storage;

	if (ret != NULL)
		__log("getpwnam('%s') = { name = '%s', passwd = '%s', "
		      "uid = %u, gid = %u, gecos = '%s', dir = '%s', "
		      "shell = '%s' }", name, ret->pw_name, ret->pw_passwd,
		      ret->pw_uid, ret->pw_gid, ret->pw_gecos, ret->pw_dir,
		      ret->pw_shell);
	else
		__log("getpwnam('%s') = NULL", name);
 out:
	if (stdout != NULL)
		free(stdout);
	return ret;
 deflt:
	if (defunc == NULL)
		defunc = dlsym(RTLD_NEXT, "getpwnam");
	ret = defunc(name);
	goto out;
}

struct passwd *getpwuid(uid_t uid)
{
	static struct passwd *(*defunc)(uid_t) = NULL;
	static char storbuf[STORE_BUFLEN];
	static struct passwd storage;
	struct passwd *ret = NULL;
	char *stdout;
	int ecode;

	stdout = __invoke("INTERCEPT_GETPWUID", &ecode, "%u", uid);

	if (ecode != 0)
		goto deflt;

	if (__parse_entry(&storage, storbuf, sizeof (storbuf), stdout) == 0)
		ret = &storage;

	if (ret != NULL)
		__log("getpwuid(%u) = { name = '%s', passwd = '%s', "
		      "uid = %u, gid = %u, gecos = '%s', dir = '%s', "
		      "shell = '%s' }", uid, ret->pw_name, ret->pw_passwd,
		      ret->pw_uid, ret->pw_gid, ret->pw_gecos, ret->pw_dir,
		      ret->pw_shell);
	else
		__log("getpwuid(%u) = NULL", uid);
 out:
	if (stdout != NULL)
		free(stdout);
	return ret;
 deflt:
	if (defunc == NULL)
		defunc = dlsym(RTLD_NEXT, "getpwuid");
	ret = defunc(uid);
	goto out;
}
