#include <iostream>						// 编译预处理命令
#include <fstream>						// 文件输入输出
#include <cstring>						// 包含C函数strlen()、strcpy()、strncpy()、strcat()和strcmp()的声明（string.h与cstring是C的头文件）
#include <cstdio>						// 包含EOF的声明（stdio.h与cstdio是C的头文件）
#include <cstdlib>						// 含C函数system()和exit()的声明（stdlib.h与cstdlib是C的头文件）
using namespace std;					// 使用命名空间std 
#include "huffman_compress.h"			// 哈夫曼压缩算法

int main()
{
	HuffmanCompress obj;
	int select = 0;
	while (select != 3)  
	{
		cout << endl << "1. 压缩";
		cout << endl << "2. 解压缩";
		cout << endl << "3. 退出";
		cout << endl << "请根据代号输入您的选择:";
		cin >> select;					// 输入选择
		while (cin.get() != '\n');		// 忽略用户输入的其他字符

		switch (select)
		{
			case 1:   
				obj.Compress();			// 压缩
				cout << "压缩时间：" << obj.CompressTime()<<"ms"<< endl;  //输出压缩所耗时间
				break;
			case 2:
				obj.Decompress();		// 解压缩
				cout << "解压缩时间：" << obj.DecompressTime()<<"ms" <<endl;  //输出解压缩所耗时间
		}
	}

	system("PAUSE");					// 调用库函数system()
	return 0;							// 返回值0, 返回操作系统
}
