#include <iostream>						// ����Ԥ��������
#include <fstream>						// �ļ��������
#include <cstring>						// ����C����strlen()��strcpy()��strncpy()��strcat()��strcmp()��������string.h��cstring��C��ͷ�ļ���
#include <cstdio>						// ����EOF��������stdio.h��cstdio��C��ͷ�ļ���
#include <cstdlib>						// ��C����system()��exit()��������stdlib.h��cstdlib��C��ͷ�ļ���
using namespace std;					// ʹ�������ռ�std 
#include "huffman_compress.h"			// ������ѹ���㷨

int main()
{
	HuffmanCompress obj;
	int select = 0;
	while (select != 3)  
	{
		cout << endl << "1. ѹ��";
		cout << endl << "2. ��ѹ��";
		cout << endl << "3. �˳�";
		cout << endl << "����ݴ�����������ѡ��:";
		cin >> select;					// ����ѡ��
		while (cin.get() != '\n');		// �����û�����������ַ�

		switch (select)
		{
			case 1:   
				obj.Compress();			// ѹ��
				cout << "ѹ��ʱ�䣺" << obj.CompressTime()<<"ms"<< endl;  //���ѹ������ʱ��
				break;
			case 2:
				obj.Decompress();		// ��ѹ��
				cout << "��ѹ��ʱ�䣺" << obj.DecompressTime()<<"ms" <<endl;  //�����ѹ������ʱ��
		}
	}

	system("PAUSE");					// ���ÿ⺯��system()
	return 0;							// ����ֵ0, ���ز���ϵͳ
}
