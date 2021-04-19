#ifndef __CharString_H__
#define __CharString_H__

#include "lk_list.h"													// ��������

// ����
class CharString 
{
protected:
// ���ݳ�Ա:
	mutable char *strVal;												// ��ֵ
	int length;															// ����	

public:                           
// �����������ͷ�������ط������������ر���ϵͳĬ�Ϸ�������:
	CharString();														// ���캯�� 
	virtual ~CharString();												// ��������
	CharString(const CharString &source);								// ���ƹ��캯��
	CharString(const char *source);										// ��C���ת���Ĺ��캯��
	CharString(LinkList<char> &source);									// �����Ա�ת���Ĺ��캯��
	int Length() const;													// �󴮳���			 
	bool Empty() const;													// �жϴ��Ƿ�Ϊ��
	CharString &operator =(const CharString &source);					// ���ظ�ֵ�����
	const char *ToCStr() const { return (const char *)strVal; }			// ����ת����C���
	char &operator [](int pos) const;									// �����±������
};

// ����ز���
CharString Read(istream &input);										// �����������봮
CharString Read(istream &input, char &terminalChar);					// �����������봮������terminalChar���ش������ַ�
void Write(const CharString &s);										// �����
void Concat(CharString &target, const CharString &source);				// ����source���ӵ���target�ĺ���
void Copy(CharString &target, const CharString &source);				// ����source���Ƶ���target
void Copy(CharString &target, const CharString &source, int n);			// ����source����n���ַ�����target 
int Index(const CharString &target, const CharString &pattern, int pos = 0);// ����ģʽ��pattern��һ����Ŀ�괮target�дӵ�pos���ַ���ʼ���ֵ�λ��
CharString SubString(const CharString &s, int pos, int len);		// ��s�ĵ�pos���ַ���ʼ�ĳ���Ϊlen���Ӵ�
bool operator ==(const CharString &first, const CharString &second);	// ���ع�ϵ�����==
bool operator <(const CharString &first, const CharString &second);		// ���ع�ϵ�����<
bool operator >(const CharString &first, const CharString &second);		// ���ع�ϵ�����>
bool operator <=(const CharString &first, const CharString &second);	// ���ع�ϵ�����<=
bool operator >=(const CharString &first, const CharString &second);	// ���ع�ϵ�����>=
bool operator !=(const CharString &first, const CharString &second);	// ���ع�ϵ�����!=

// ���༰��ز�����ʵ�ֲ���
CharString::CharString()
//  �����������ʼ���� 
{
	length = 0;															// ������Ϊ0
	strVal = NULL;														// �մ�
}

CharString::~CharString()
// ������������ٴ����ͷŴ���ռ�ÿռ�
{
	delete []strVal;													// �ͷŴ�strVal
}

CharString::CharString(const CharString &source)
// ����������ɴ�source�����´��������ƹ��캯��
{
	length = strlen(source.ToCStr());									// ����
	strVal = new char[length + 1];										// ����洢�ռ�
	strcpy(strVal, source.ToCStr());									// ���ƴ�ֵ
}

CharString::CharString(const char *source)
// �����������C���ת�������´�����ת�����캯��
{
	length = strlen(source);											// ����
	strVal = new char[length + 1];										// ����洢�ռ� 
	strcpy(strVal, source);												// ���ƴ�ֵ
}

CharString::CharString(LinkList<char> &source)
// ��������������Ա�ת�������´�����ת�����캯��
{
	length = source.Length();											// ����
	strVal = new char[length + 1];										// ����洢�ռ� 
	for (int pos = 0; pos < length; pos++) 
	{	// ���ƴ�ֵ
		source.GetElem(pos + 1, strVal[pos]);
	}
	strVal[length] = '\0';												// ��ֵ��'\0'����	
}

int CharString::Length() const
// ������������ش�����			 
{
	return length;
}

bool CharString::Empty() const
// ��������������Ϊ�գ�����true�����򷵻�false
{
	return length == 0;
}

CharString &CharString::operator =(const CharString &source)
// ������������ظ�ֵ�����
{
	if (&source != this)
	{
		delete []strVal;												// �ͷ�ԭ���洢�ռ�
		length = strlen(source.ToCStr());								// ����
		strVal = new char[length + 1];									// ����洢�ռ� 
		strcpy(strVal, source.ToCStr());								// ���ƴ�ֵ
	}
	return *this;
}

char &CharString::operator [](int pos) const
// ��������������±������
{
	return strVal[pos];
}

CharString Read(istream &input)
// ��������������������봮
{
	LinkList<char> temList;												// ��ʱ���Ա�
	char ch;															// ��ʱ�ַ�
	while ((ch = input.peek()) != EOF &&								// peek()����������ȡһ���ַ�,������ָ�벻��
		(ch = input.get()) != '\n')										// get()����������ȡһ���ַ�,������ָ��ָ����һ�ַ�
	{	// ��������ַ�׷�����Ա���
		temList.Insert(temList.Length() + 1, ch);
	}
	return temList;														// ������temList���ɵĴ�
}

CharString Read(istream &input,char &terminalChar)
// ��������������������봮������terminalChar���ش������ַ�
{
	LinkList<char> temList;												// ��ʱ���Ա�
	char ch;															// ��ʱ�ַ�
	while ((ch = input.peek()) != EOF &&								// peek()����������ȡһ���ַ�,������ָ�벻��
		(ch = input.get()) != '\n')										// get()����������ȡһ���ַ�,������ָ��ָ����һ�ַ�
	{	// ��������ַ�׷�����Ա���
		temList.Insert(temList.Length() + 1, ch);
	}
	terminalChar = ch;													// ��terminalChar���ش������ַ�
	return temList;														// ������temList���ɵĴ�
}

void Write(const CharString &s)
// ��������������
{
	cout << s.ToCStr() << endl;											// �����ֵ
}

void Concat(CharString &target, const CharString &source)
// �������������source���ӵ���target�ĺ���
{
	const char *cFirst = target.ToCStr();								// ָ���һ����
	const char *cSecond = source.ToCStr();								// ָ��ڶ�����
	char *cTarget = new char[strlen(cFirst) + strlen(cSecond) + 1];		// ����洢�ռ�
	strcpy(cTarget, cFirst);											// ���Ƶ�һ����
	strcat(cTarget, cSecond);											// ���ӵڶ�����
	target = cTarget;													// ����ֵ
	delete []cTarget;													// �ͷ�cTarget
}

void Copy(CharString &target, const CharString &source)
// �������������source���Ƶ���target
{
	const char *cSource = source.ToCStr();								// ��ʼ��
	char *cTarget = new char[strlen(cSource) + 1];						// ����洢�ռ�
	strcpy(cTarget, cSource);											// ���ƴ�
	target = cTarget;													// ����ֵ
	delete []cTarget;													// �ͷŴ�cTarget
}

void Copy(CharString &target, const CharString &source, int n)
// �������������source����n���ַ�����target 
{
	const char *cSource = source.ToCStr();								// ��ʼ��
	int len = (int)strlen(cSource) < n ? (int)strlen(cSource) : n;		//Ŀ�괮��
	char *cTarget = new char[len + 1];									// ����洢�ռ�
	strncpy(cTarget, cSource, n);										// ���ƴ�
	cTarget[len] = '\0';												// ��ֵ��'\0'����
	target = cTarget;													// ����ֵ
	delete []cTarget;													// �ͷŴ�cTarget
}

int Index(const CharString &target, const CharString &pattern, int pos)
// ������������ƥ��ɹ�������ģʽ��pattern��һ����Ŀ�괮target�дӵ�pos
//	���ַ���ʼ���ֵ�λ��, ���򷵻�-1
{
	const char *cTarget = target.ToCStr();								// Ŀ�괮
	const char *cPattern = pattern.ToCStr();							// ģʽ��
	const char *ptr=strstr(cTarget + pos, cPattern);					// ģʽƥ��
	if (ptr == NULL)
	{	// ƥ��ʧ��	
		return -1; 
	}
	else
	{	// ƥ��ɹ�	
		return ptr - cTarget;
	}
}

CharString SubString(const CharString &s, int pos, int len)
// ��ʼ��������s���ڣ�0 <= pos < s.Length()��0 <= len <= s.Length() - pos
// ������������ش�s�ĵ�pos���ַ���ʼ�ĳ���Ϊlen���Ӵ�
{
	if (0 <= pos && pos < s.Length() && 0 <= len)
	{	// ���ش�s�ĵ�pos���ַ���ʼ�ĳ���Ϊlen���Ӵ�
		len = (len < s.Length() - pos) ? len : (s.Length() - pos);		// �Ӵ���
		char *sub = new char[len + 1];									// ����洢�ռ�
		const char *str = s.ToCStr();									  // ����C���
		strncpy(sub, str + pos, len);									// ���ƴ�
		sub[len] = '\0';												// ��ֵ��'\0'����
		return sub;														// �����Ӵ�sub
	}
	else
	{	// ���ؿմ�
		return "";														// ���ؿմ�
	}
}

bool operator ==(const CharString &first, const CharString &second)
// ������������ع�ϵ�����==
{
	return strcmp(first.ToCStr(), second.ToCStr()) == 0;
}

bool operator <(const CharString &first, const CharString &second)
// ������������ع�ϵ�����<
{
	return strcmp(first.ToCStr(), second.ToCStr()) < 0;
}

bool operator >(const CharString &first, const CharString &second)
// ������������ع�ϵ�����>
{
	return strcmp(first.ToCStr(), second.ToCStr()) > 0;
}

bool operator <=(const CharString &first, const CharString &second)
// ������������ع�ϵ�����<=
{
	return strcmp(first.ToCStr(), second.ToCStr()) <= 0;
}

bool operator >=(const CharString &first, const CharString &second)
// ������������ع�ϵ�����>=
{
	return strcmp(first.ToCStr(), second.ToCStr()) >= 0;
}

bool operator !=(const CharString &first, const CharString &second)
// ������������ع�ϵ�����!=
{
	return strcmp(first.ToCStr(), second.ToCStr()) != 0;
}


#endif