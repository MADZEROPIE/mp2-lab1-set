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
	MemLen = (len - 1) / TelemBit + 1;
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

	return n/ TelemBit;
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
	pMem[MemInd] = pMem[MemInd] | (1 << n % TelemBit);

}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n<0 || n>BitLen) throw(exception());
	int MemInd = GetMemIndex(n);
	pMem[MemInd] = pMem[MemInd] &(~ (1 << n % TelemBit));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n<0 || n>BitLen) throw(exception());
	int MemInd = GetMemIndex(n);
	return  (pMem[MemInd] & (1 <<( n % TelemBit))) != 0;
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
	if(BitLen!=bf.BitLen) return 0;
	for (int i = 0; i < MemLen - 1; ++i) if (pMem[i] != bf.pMem[i]) return 0;
	return (*(pMem + MemLen - 1) >> (MemLen - BitLen * TelemBit) == *(bf.pMem + MemLen - 1) >> (MemLen - BitLen * TelemBit));
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 1;
	for (int i = 0; i < MemLen - 1; ++i) if (pMem[i]== bf.pMem [i]) return 0;
	return (*(pMem + MemLen - 1) >> (MemLen - BitLen * TelemBit)) != (*(bf.pMem + MemLen - 1) >> (MemLen - BitLen * TelemBit));
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (bf.BitLen > BitLen) {
		TBitField res(bf);
		for (int i = 0; i < BitLen; ++i) if (GetBit(i)) res.SetBit(i);
		return res;
	}
	else{
		TBitField res(*this);
		for (int i = 0; i < bf.BitLen; ++i) if (bf.GetBit(i) ) res.SetBit(i);
		return res;
	}
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (bf.BitLen > BitLen) {
		TBitField res(bf.BitLen);
		for (int i = 0; i < BitLen; ++i) if (GetBit(i)&&bf.GetBit(i)) res.SetBit(i);
		return res;
	}
	else {
		TBitField res(BitLen);
		for (int i = 0; i < bf.BitLen; ++i) if (bf.GetBit(i)&&GetBit(i)) res.SetBit(i);
		return res;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < BitLen; ++i) {
		if (GetBit(i)) res.ClrBit(i);
		else res.SetBit(i);
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
