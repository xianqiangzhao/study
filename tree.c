/******************************************  
 * 参照 http://blog.csdn.net/blogofshuguang/article/details/17040915
 * 增加了 目录文件分层次显示
 * 输入：./a.out <dirPaht>  比如：列出home下的所有
 * 文件，只需要输入./a.out /home
 * 主要使用了系统函数  opendir   readdir closedir  目录判断函数 lstat 
 *  文件结构 struct dirent  目录结果 DIR    是不是目录判断宏 S_ISDIR    目录判断返回结构体 struct stat  
 ******************************************/
#include <stdio.h>  
#include <unistd.h>  
#include <dirent.h>  
#include <stdlib.h>  
#include <sys/stat.h>  
#include <string.h>  
#include <assert.h>  

#define MAX_PATH_LEN 512  

int count = 0;
char dirPath[MAX_PATH_LEN];

/*
 space insert
 */
void spaceInsert(int depth) {
	int i = 0;
	for (; i < depth; i++)
		printf("  ");
}
/*
 show files 
 */
void listAllFiles(char *dirname, int depth) {
	assert(dirname != NULL);

	char path[512];
	struct dirent *filename; //readdir 的返回类型
	DIR *dir;
	dir = opendir(dirname);
	if (dir == NULL) {
		printf("open dir %s error!\n", dirname);
		exit(1);
	}

	while ((filename = readdir(dir)) != NULL) {
		//目录结构下面问什么会有两个.和..的目录？ 跳过着两个目录
		if (!strcmp(filename->d_name, ".") || !strcmp(filename->d_name, ".."))
			continue;
		sprintf(path, "%s/%s", dirname, filename->d_name);

		struct stat s;
		lstat(path, &s);
		if (S_ISDIR(s.st_mode)) {
			spaceInsert(depth);
			printf("+%s\n", filename->d_name);
			depth++;
			listAllFiles(path, depth);
		} else {
			spaceInsert(depth);
			printf("|-%s\n", filename->d_name);
			++count;

		}
	}
	closedir(dir);
}
/**
 * main 
 */
int main(int argc, char **argv) {

	if (argc != 2) {
		printf("one dir required!(for eample: ./a.out /home/myFolder)\n");
		exit(1);
	}
	strcpy(dirPath, argv[1]);
	listAllFiles(dirPath, 0);
	printf("total files:%d\n", count);
	return 0;
}
