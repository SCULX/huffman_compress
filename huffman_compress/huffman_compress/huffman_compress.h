#ifndef __HUFFMAN_COMPRESS_H__
#define __HUFFMAN_COMPRESS_H__

#include "huffman_tree.h"								// ����������
#include<ctime>

// �ļ�·����:huffman_compress\huffman_compress.h
 
// �ַ�������
struct BufferType
{	
	char ch;											// �ַ�
	unsigned int bits;									// ʵ�ʱ�����     
};

// ������ѹ����
class HuffmanCompress
{
protected:
//  ������ѹ��������ݳ�Ա:
	HuffmanTree<char, unsigned long> *pHuffmanTree;     //��������
	FILE *infp, *outfp;									// ����/���ļ�
	BufferType buf;										// �ַ�����

//	��������:
	void Write(unsigned int bit);						// ��Ŀ���ļ���д��һ������
	void WriteToOutfp();								// ǿ�н��ַ�����д��Ŀ���ļ�

public:
//  ������ѹ���෽�����������ر���ϵͳĬ�Ϸ�������:
	HuffmanCompress();									// �޲����Ĺ��캯��
	virtual ~HuffmanCompress();							// ��������
	void Compress();									// ѹ���㷨
	void Decompress();									// ��ѹ���㷨
	HuffmanCompress(const HuffmanCompress &source);		// ���ƹ��캯��
	HuffmanCompress &operator=(const HuffmanCompress &source);// ��ֵ�����
	long int CompressTime();                            //ѹ��ʱ��
	long int DecompressTime();                          //��ѹ��ʱ��

public:
	clock_t ticks1, ticks2, ticks3, ticks4;           //���ڱ�ʾ��������ʱ��ı���
};

// ������ѹ�����ʵ�ֲ���
HuffmanCompress::HuffmanCompress()
// �������������һ���յĹ�����ѹ������󡪡��޲����Ĺ��캯��
{
	pHuffmanTree = NULL;								// �չ�������
}

HuffmanCompress::~HuffmanCompress()
// ������������ٹ�����ѹ������󡪡���������
{
	if (pHuffmanTree != NULL) delete []pHuffmanTree;	// �ͷſռ�
}

void HuffmanCompress::Write(unsigned int bit)				
// �����������Ŀ���ļ���д��һ������
{
	buf.bits++;											// �������������1
	buf.ch = (buf.ch << 1) | bit;						// ��bit���뵽�����ַ���
	if (buf.bits == 8)
	{	// ����������,д��Ŀ���ļ�
		fputc(buf.ch, outfp);							// д��Ŀ���ļ�
		buf.bits = 0;									// ��ʼ��bits
		buf.ch = 0;										// ��ʼ��ch
	}
}

void HuffmanCompress::WriteToOutfp()			
// ���������ǿ�н��ַ�����д��Ŀ���ļ�
{
	unsigned int len = buf.bits;						// ����ʵ�ʱ�����
	if (len > 0)
	{	// ����ǿ�, ������ı��ظ������ӵ�8, �Զ�д��Ŀ���ļ�
		for (unsigned int temPos = 0; temPos < 8 - len; temPos++) Write(0);
	}
}

void HuffmanCompress::Compress()
// ����������ù���������ѹ���ļ�
{
	char infName[256], outfName[256];					// ����(Դ)/��(Ŀ��)�ļ���
	
	ticks1 = clock();                                   //ѹ����ʼ����ʱ

	cout << "������Դ�ļ���(�ļ�С��4GB):";				// ��ѹ���ļ�С��4GB
	cin >> infName;										// ����Դ�ļ���
	if ((infp = fopen(infName, "rb")) == NULL)
	{	// �����쳣
		cout << "��Դ�ļ�ʧ��!" << endl;				// ��ʾ��Ϣ
		exit(1);										// �˳�����
	}

	fgetc(infp);										// ȡ��Դ�ļ���һ���ַ�
	if (feof(infp))
	{	// �����쳣
		cout << "��Դ�ļ�!" << endl;					// ��ʾ��Ϣ
		exit(2);										// �˳�����
	}

	cout << "������Ŀ���ļ�:";
	cin >> outfName;                  
	if ((outfp = fopen(outfName, "wb")) == NULL)
	{	// �����쳣
		cout << "��Ŀ���ļ�ʧ��!" << endl;			// ��ʾ��Ϣ
		exit(3);										// �˳�����
	}
	
	cout << "���ڴ������Ժ�..." << endl;

	const unsigned long n = 256;						// �ַ�����
    char ch[n];											// �ַ�����
	unsigned long w[n];									// �ַ�����Ƶ��(Ȩ)
	unsigned long size = 0;
	int temPos;
	char cha;

	for (temPos = 0; temPos < n; temPos++)
	{	// ��ʼ��ch[]��w[]
		ch[temPos] = (char)temPos;						// ��ʼ��ch[temPos]
		w[temPos] = 0;									// ��ʼ��w[temPos]
	}

	rewind(infp);										// ʹԴ�ļ�ָ��ָ���ļ���ʼ��
	cha = fgetc(infp);									// ȡ��Դ�ļ���һ���ַ�
	while (!feof(infp))					
	{	// ͳ���ַ�����Ƶ��
		w[(unsigned char)cha]++;						// �ַ�cha����Ƶ���Լ�1
		size++;											// �ļ���С�Լ�1	
		cha=fgetc(infp);								// ȡ��Դ�ļ���һ���ַ�
	}

	if (pHuffmanTree != NULL) delete []pHuffmanTree;	// �ͷſռ�
	pHuffmanTree = new HuffmanTree<char, unsigned long>(ch, w, n); // ���ɹ�������
    rewind(outfp);										// ʹĿ���ļ�ָ��ָ���ļ���ʼ��
	fwrite(&size, sizeof(unsigned long), 1, outfp);		// ��Ŀ���ļ�д��Դ�ļ���С
	for (temPos = 0; temPos < n; temPos++)				
	{	// ��Ŀ���ļ�д���ַ�����Ƶ��
		fwrite(&w[temPos], sizeof(unsigned long), 1, outfp);
	}

	buf.bits = 0;										// ��ʼ��bits
	buf.ch = 0;											// ��ʼ��ch
	rewind(infp);										// ʹԴ�ļ�ָ��ָ���ļ���ʼ��
	cha = fgetc(infp);									// ȡ��Դ�ļ��ĵ�һ���ַ�
    while (!feof(infp))
	{	// ��Դ�ļ��ַ����б��룬��������д��Ŀ���ļ�
		CharString temStr = pHuffmanTree->Encode(cha);		// �ַ�����
		for (temPos = 0; temPos < temStr.Length(); temPos++)
		{	// ��Ŀ���ļ�д�����
			if (temStr[temPos] == '0') Write(0);		// ��Ŀ���ļ�д��0
			else Write(1);								// ��Ŀ���ļ�д��1
		} 
		cha = fgetc(infp);								// ȡ��Դ�ļ�����һ���ַ�
	}
	WriteToOutfp();										// ǿ��д��Ŀ���ļ�
	
	fclose(infp); fclose(outfp);						// �ر��ļ�

	ticks2 = clock();                                  //ѹ���������ʱ

	cout << "�������." << endl;
}

void HuffmanCompress::Decompress()
// �����������ѹ���ù���������ѹ�����ļ�
{
	char infName[256], outfName[256];					// ����(ѹ��)/��(Ŀ��)�ļ���

	ticks3 = clock();                                  //��ѹ����ʼ����ʱ
	
	cout << "������ѹ���ļ���:";
	cin >> infName;                  
	if ((infp = fopen(infName, "rb")) == NULL)
	{	// �����쳣
		cout << "��ѹ���ļ�ʧ��!" << endl;			// ��ʾ��Ϣ
		exit(4);										// �˳�����
	}

    fgetc(infp);										// ȡ��ѹ���ļ���һ���ַ�
	if (feof(infp))
	{	// �����쳣
		cout << "ѹ���ļ�Ϊ��!" << endl;				// ��ʾ��Ϣ
		exit(5);										// �˳�����
	}

	cout << "������Ŀ���ļ���:";
	cin >> outfName;                  
	if ((outfp = fopen(outfName, "wb")) == NULL)
	{	// �����쳣
		cout << "��Ŀ���ļ�ʧ��!" << endl;			// ��ʾ��Ϣ
		exit(6);										// �˳�����
	}
	
	cout << "���ڴ������Ժ�..." << endl;

	const unsigned long n = 256;						// �ַ�����
	char ch[n];											// �ַ�����
	unsigned long w[n];									// Ȩ
	unsigned long size = 0;
	int temPos;
	char cha;

	rewind(infp);										// ʹԴ�ļ�ָ��ָ���ļ���ʼ��
	fread(&size, sizeof(unsigned long), 1, infp);		// ��ȡĿ���ļ��Ĵ�С	
	for (temPos = 0; temPos < n; temPos++)
	{
		ch[temPos] = (char)temPos;						// ��ʼ��ch[temPos]
		fread(&w[temPos], sizeof(unsigned long), 1, infp);	// ��ȡ�ַ�Ƶ��
	}
	if (pHuffmanTree != NULL) delete []pHuffmanTree;	// �ͷſռ�
	pHuffmanTree = new HuffmanTree<char, unsigned long>(ch, w, n); // ���ɹ�������

	unsigned long len = 0;								// ��ѹ���ַ���
	cha = fgetc(infp);									// ȡ��Դ�ļ��ĵ�һ���ַ�
	while (!feof(infp))
	{	// ��ѹ���ļ��ַ����н��룬����������ַ�д��Ŀ���ļ�
		CharString temStr = "";							// ��chaת����������ʽ�Ĵ�
		unsigned char c = (unsigned char)cha;			// ��chaת����unsigned char����
		for (temPos = 0; temPos < 8; temPos++)
		{	// ��cת���ɶ����ƴ�
			if (c < 128) Concat(temStr, "0");			// ���λΪ0
			else Concat(temStr, "1");					// ���λΪ1
			c = c << 1;									// ����һλ
		}

		LinkList<char> lkText = pHuffmanTree->Decode(temStr);// ����
		CharString curStr(lkText);
		for (temPos = 1; temPos <= curStr.Length(); temPos++)
		{	// ��Ŀ���ļ�д���ַ�
			len++;										// Ŀ���ļ������Լ�1
			fputc(curStr[temPos - 1], outfp);			// ���ַ�д��Ŀ���ļ���
			if (len == size) break;						// ��ѹ����˳���ѭ��
		} 
		if (len == size) break;							// ��ѹ����˳���ѭ��
		cha = fgetc(infp);								// ȡ��Դ�ļ�����һ���ַ�
	}

	fclose(infp); fclose(outfp);						// �ر��ļ�

	ticks4 = clock();                                   //��ѹ����������ʱ

	cout << "�������." << endl;
}

HuffmanCompress::HuffmanCompress(const HuffmanCompress &source)
// ����������ɹ�����ѹ�������source�����¹�����ѹ������󡪡����ƹ��캯��
{
	if (source.pHuffmanTree != NULL)
	{	// sourceΪ�ǿչ�����ѹ�������
		pHuffmanTree = new HuffmanTree<char, unsigned long>(*source.pHuffmanTree);	// ���ɹ�������
	}
	else pHuffmanTree =  NULL;							// ���ɿչ�����ѹ�������
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
// �����������������ѹ�������source��ֵ����ǰ������ѹ������󡪡���ֵ�����
{
	if (&source != this)
	{
		if (pHuffmanTree != NULL) delete []pHuffmanTree;// �ͷſռ�
		if (source.pHuffmanTree != NULL)
		{	// sourceΪ�ǿչ�����ѹ�������
			pHuffmanTree = new HuffmanTree<char, unsigned long>(*source.pHuffmanTree);	// ���ɹ�������
		}
		else pHuffmanTree =  NULL;						// ���ɿչ�����ѹ�������
	}
	return *this;
}

#endif
