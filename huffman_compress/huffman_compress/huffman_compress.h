#ifndef __HUFFMAN_COMPRESS_H__
#define __HUFFMAN_COMPRESS_H__

#include "huffman_tree.h"								// 哈夫曼树类
#include<ctime>

// 文件路径名:huffman_compress\huffman_compress.h
 
// 字符缓存器
struct BufferType
{	
	char ch;											// 字符
	unsigned int bits;									// 实际比特数     
};

// 哈夫曼压缩类
class HuffmanCompress
{
protected:
//  哈夫曼压缩类的数据成员:
	HuffmanTree<char, unsigned long> *pHuffmanTree;     //哈夫曼树
	FILE *infp, *outfp;									// 输入/出文件
	BufferType buf;										// 字符缓存

//	辅助函数:
	void Write(unsigned int bit);						// 向目标文件中写入一个比特
	void WriteToOutfp();								// 强行将字符缓存写入目标文件

public:
//  哈夫曼压缩类方法声明及重载编译系统默认方法声明:
	HuffmanCompress();									// 无参数的构造函数
	virtual ~HuffmanCompress();							// 析构函数
	void Compress();									// 压缩算法
	void Decompress();									// 解压缩算法
	HuffmanCompress(const HuffmanCompress &source);		// 复制构造函数
	HuffmanCompress &operator=(const HuffmanCompress &source);// 赋值运算符
	long int CompressTime();                            //压缩时间
	long int DecompressTime();                          //解压缩时间

public:
	clock_t ticks1, ticks2, ticks3, ticks4;           //用于表示程序运行时间的变量
};

// 哈夫曼压缩类的实现部分
HuffmanCompress::HuffmanCompress()
// 操作结果：构造一个空的哈夫曼压缩类对象——无参数的构造函数
{
	pHuffmanTree = NULL;								// 空哈夫曼树
}

HuffmanCompress::~HuffmanCompress()
// 操作结果：销毁哈夫曼压缩类对象——析构函数
{
	if (pHuffmanTree != NULL) delete []pHuffmanTree;	// 释放空间
}

void HuffmanCompress::Write(unsigned int bit)				
// 操作结果：向目标文件中写入一个比特
{
	buf.bits++;											// 缓存比特数自增1
	buf.ch = (buf.ch << 1) | bit;						// 将bit加入到缓存字符中
	if (buf.bits == 8)
	{	// 缓存区已满,写入目标文件
		fputc(buf.ch, outfp);							// 写入目标文件
		buf.bits = 0;									// 初始化bits
		buf.ch = 0;										// 初始化ch
	}
}

void HuffmanCompress::WriteToOutfp()			
// 操作结果：强行将字符缓存写入目标文件
{
	unsigned int len = buf.bits;						// 缓存实际比特数
	if (len > 0)
	{	// 缓存非空, 将缓存的比特个数增加到8, 自动写入目标文件
		for (unsigned int temPos = 0; temPos < 8 - len; temPos++) Write(0);
	}
}

void HuffmanCompress::Compress()
// 操作结果：用哈夫曼编码压缩文件
{
	char infName[256], outfName[256];					// 输入(源)/出(目标)文件名
	
	ticks1 = clock();                                   //压缩开始处计时

	cout << "请输入源文件名(文件小于4GB):";				// 被压缩文件小于4GB
	cin >> infName;										// 输入源文件名
	if ((infp = fopen(infName, "rb")) == NULL)
	{	// 出现异常
		cout << "打开源文件失败!" << endl;				// 提示信息
		exit(1);										// 退出程序
	}

	fgetc(infp);										// 取出源文件第一个字符
	if (feof(infp))
	{	// 出现异常
		cout << "空源文件!" << endl;					// 提示信息
		exit(2);										// 退出程序
	}

	cout << "请输入目标文件:";
	cin >> outfName;                  
	if ((outfp = fopen(outfName, "wb")) == NULL)
	{	// 出现异常
		cout << "打开目标文件失败!" << endl;			// 提示信息
		exit(3);										// 退出程序
	}
	
	cout << "正在处理，请稍候..." << endl;

	const unsigned long n = 256;						// 字符个数
    char ch[n];											// 字符数组
	unsigned long w[n];									// 字符出现频度(权)
	unsigned long size = 0;
	int temPos;
	char cha;

	for (temPos = 0; temPos < n; temPos++)
	{	// 初始化ch[]和w[]
		ch[temPos] = (char)temPos;						// 初始化ch[temPos]
		w[temPos] = 0;									// 初始化w[temPos]
	}

	rewind(infp);										// 使源文件指针指向文件开始处
	cha = fgetc(infp);									// 取出源文件第一个字符
	while (!feof(infp))					
	{	// 统计字符出现频度
		w[(unsigned char)cha]++;						// 字符cha出现频度自加1
		size++;											// 文件大小自加1	
		cha=fgetc(infp);								// 取出源文件下一个字符
	}

	if (pHuffmanTree != NULL) delete []pHuffmanTree;	// 释放空间
	pHuffmanTree = new HuffmanTree<char, unsigned long>(ch, w, n); // 生成哈夫曼树
    rewind(outfp);										// 使目标文件指针指向文件开始处
	fwrite(&size, sizeof(unsigned long), 1, outfp);		// 向目标文件写入源文件大小
	for (temPos = 0; temPos < n; temPos++)				
	{	// 向目标文件写入字符出现频度
		fwrite(&w[temPos], sizeof(unsigned long), 1, outfp);
	}

	buf.bits = 0;										// 初始化bits
	buf.ch = 0;											// 初始化ch
	rewind(infp);										// 使源文件指针指向文件开始处
	cha = fgetc(infp);									// 取出源文件的第一个字符
    while (!feof(infp))
	{	// 对源文件字符进行编码，并将编码写入目标文件
		CharString temStr = pHuffmanTree->Encode(cha);		// 字符编码
		for (temPos = 0; temPos < temStr.Length(); temPos++)
		{	// 向目标文件写入编码
			if (temStr[temPos] == '0') Write(0);		// 向目标文件写入0
			else Write(1);								// 向目标文件写入1
		} 
		cha = fgetc(infp);								// 取出源文件的下一个字符
	}
	WriteToOutfp();										// 强行写入目标文件
	
	fclose(infp); fclose(outfp);						// 关闭文件

	ticks2 = clock();                                  //压缩结束后计时

	cout << "处理结束." << endl;
}

void HuffmanCompress::Decompress()
// 操作结果：解压缩用哈夫曼编码压缩的文件
{
	char infName[256], outfName[256];					// 输入(压缩)/出(目标)文件名

	ticks3 = clock();                                  //解压缩开始处计时
	
	cout << "请输入压缩文件名:";
	cin >> infName;                  
	if ((infp = fopen(infName, "rb")) == NULL)
	{	// 出现异常
		cout << "打开压缩文件失败!" << endl;			// 提示信息
		exit(4);										// 退出程序
	}

    fgetc(infp);										// 取出压缩文件第一个字符
	if (feof(infp))
	{	// 出现异常
		cout << "压缩文件为空!" << endl;				// 提示信息
		exit(5);										// 退出程序
	}

	cout << "请输入目标文件名:";
	cin >> outfName;                  
	if ((outfp = fopen(outfName, "wb")) == NULL)
	{	// 出现异常
		cout << "打开目标文件失败!" << endl;			// 提示信息
		exit(6);										// 退出程序
	}
	
	cout << "正在处理，请稍候..." << endl;

	const unsigned long n = 256;						// 字符个数
	char ch[n];											// 字符数组
	unsigned long w[n];									// 权
	unsigned long size = 0;
	int temPos;
	char cha;

	rewind(infp);										// 使源文件指针指向文件开始处
	fread(&size, sizeof(unsigned long), 1, infp);		// 读取目标文件的大小	
	for (temPos = 0; temPos < n; temPos++)
	{
		ch[temPos] = (char)temPos;						// 初始化ch[temPos]
		fread(&w[temPos], sizeof(unsigned long), 1, infp);	// 读取字符频度
	}
	if (pHuffmanTree != NULL) delete []pHuffmanTree;	// 释放空间
	pHuffmanTree = new HuffmanTree<char, unsigned long>(ch, w, n); // 生成哈夫曼树

	unsigned long len = 0;								// 解压的字符数
	cha = fgetc(infp);									// 取出源文件的第一个字符
	while (!feof(infp))
	{	// 对压缩文件字符进行解码，并将解码的字符写入目标文件
		CharString temStr = "";							// 将cha转换二进制形式的串
		unsigned char c = (unsigned char)cha;			// 将cha转换成unsigned char类型
		for (temPos = 0; temPos < 8; temPos++)
		{	// 将c转换成二进制串
			if (c < 128) Concat(temStr, "0");			// 最高位为0
			else Concat(temStr, "1");					// 最高位为1
			c = c << 1;									// 左移一位
		}

		LinkList<char> lkText = pHuffmanTree->Decode(temStr);// 译码
		CharString curStr(lkText);
		for (temPos = 1; temPos <= curStr.Length(); temPos++)
		{	// 向目标文件写入字符
			len++;										// 目标文件长度自加1
			fputc(curStr[temPos - 1], outfp);			// 将字符写入目标文件中
			if (len == size) break;						// 解压完毕退出内循环
		} 
		if (len == size) break;							// 解压完毕退出外循环
		cha = fgetc(infp);								// 取出源文件的下一个字符
	}

	fclose(infp); fclose(outfp);						// 关闭文件

	ticks4 = clock();                                   //解压缩结束处计时

	cout << "处理结束." << endl;
}

HuffmanCompress::HuffmanCompress(const HuffmanCompress &source)
// 操作结果：由哈夫曼压缩类对象source构造新哈夫曼压缩类对象——复制构造函数
{
	if (source.pHuffmanTree != NULL)
	{	// source为非空哈夫曼压缩类对象
		pHuffmanTree = new HuffmanTree<char, unsigned long>(*source.pHuffmanTree);	// 生成哈夫曼树
	}
	else pHuffmanTree =  NULL;							// 生成空哈夫曼压缩类对象
}


long int HuffmanCompress::CompressTime()
{
	return ticks2 - ticks1;
}




long int HuffmanCompress::DecompressTime()
{
	return ticks4 - ticks3;
}


HuffmanCompress &HuffmanCompress::operator=(const HuffmanCompress &source)
// 操作结果：将哈夫曼压缩类对象source赋值给当前哈夫曼压缩类对象——赋值运算符
{
	if (&source != this)
	{
		if (pHuffmanTree != NULL) delete []pHuffmanTree;// 释放空间
		if (source.pHuffmanTree != NULL)
		{	// source为非空哈夫曼压缩类对象
			pHuffmanTree = new HuffmanTree<char, unsigned long>(*source.pHuffmanTree);	// 生成哈夫曼树
		}
		else pHuffmanTree =  NULL;						// 生成空哈夫曼压缩类对象
	}
	return *this;
}

#endif
