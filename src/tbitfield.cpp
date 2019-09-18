// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw(exception());
	BitLen = len;
	MemLen = (len - 1) / sizeof(TELEM) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; ++i) pMem[i] = 0;

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; ++i) *(pMem+i) = *(bf.pMem+i);
}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{

	return n/(sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n<0 || n>BitLen) throw(exception());
	return 1<<(n&31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n<0 || n>BitLen) throw(exception());
	int MemInd = GetMemIndex(n);
	pMem[MemInd] = pMem[MemInd] | (1 << n % sizeof(TELEM));

}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n<0 || n>BitLen) throw(exception());
	int MemInd = GetMemIndex(n);
	pMem[MemInd] = pMem[MemInd] &(~ (1 << n % sizeof(TELEM)));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n<0 || n>BitLen) throw(exception());
	int MemInd = GetMemIndex(n);
	return  (pMem[MemInd] & (1 <<( n % sizeof(TELEM)))) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (bf.MemLen != MemLen) {
		delete[] pMem;
		MemLen = bf.MemLen;
		pMem= new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; ++i) *(pMem + i) = *(bf.pMem + i);
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	//if(BitLen!=bf.BitLen) throw(exception());;
	for (int i = 0; i < MemLen - 1; ++i) if (pMem[i] != bf.pMem[i]) return 0;
	return (*(pMem + MemLen - 1) >> (MemLen - BitLen * sizeof(TELEM)) == *(bf.pMem + MemLen - 1) >> (MemLen - BitLen * sizeof(TELEM)));
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	//if (BitLen != bf.BitLen) throw(exception());;
	for (int i = 0; i < MemLen - 1; ++i) if (pMem[i]== bf.pMem [i]) return 0;
	return (*(pMem + MemLen - 1) >> (MemLen - BitLen * sizeof(TELEM))) != (*(bf.pMem + MemLen - 1) >> (MemLen - BitLen * sizeof(TELEM)));
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res((bf.BitLen > BitLen)? bf.BitLen : BitLen);
	for (int i = 0; i < res.BitLen; ++i) if (bf.GetBit(i) || GetBit(i)) res.SetBit(i);
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res((bf.BitLen > BitLen) ? bf.BitLen : BitLen);
	for (int i = 0; i < res.BitLen; ++i) if (bf.GetBit(i) && GetBit(i)) res.SetBit(i);
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(*this);
	for (int i = 0; i < BitLen; ++i) {
		if (GetBit(i)) ClrBit(i);
		else SetBit(i);
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; ++i) cout << bf.GetBit(i);
	return ostr;
}
