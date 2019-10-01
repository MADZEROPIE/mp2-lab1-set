// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw "Incorrect lenght"; // Можно выкидывать что-то попроще (?)
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
	if (n<0 || n>BitLen) throw(exception());
	return n/TelemBit;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n<0 || n>BitLen) throw(exception()); // Можно выкидывать что-то попроще (?)
	return one<<(n&(TelemBit-1)); //one = TELEM(1)
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n<0 || n>BitLen) throw(exception()); // Можно выкидывать что-то попроще (?)
	int MemInd = GetMemIndex(n);
	pMem[MemInd] = pMem[MemInd] | (one << (n % TelemBit));

}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n<0 || n>BitLen) throw(exception()); // Можно выкидывать что-то попроще (?)
	int MemInd = GetMemIndex(n);
	pMem[MemInd] = pMem[MemInd] &(~ (one << n % TelemBit));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n<0 || n>BitLen) throw(exception());
	int MemInd = GetMemIndex(n);
	return  (pMem[MemInd] & (one <<(n % TelemBit))) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (bf.MemLen != MemLen) { //Не нужно проверять на присваивание самому себе
		delete[] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; ++i) *(pMem + i) = *(bf.pMem + i);
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if(BitLen!=bf.BitLen) return 0;
	for (int i = 0; i < MemLen - 1; ++i) if (pMem[i] != bf.pMem[i]) return 0;
	for (int i = (MemLen - 1) * TelemBit; i < BitLen; ++i) if (GetBit(i) != bf.GetBit(i)) return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res(bf);
	int len = MemLen,minbtlen=BitLen;
	if (bf.BitLen < BitLen) {
		len = bf.MemLen;
		minbtlen = bf.BitLen;
		res = *this;
	}
	for (int i = 0; i < len-1; ++i) res.pMem[i] = bf.pMem[i] | pMem[i];
	for (int i = (len - 1) * TelemBit; i < minbtlen; ++i) if (bf.GetBit(i) || GetBit(i)) res.SetBit(i); //На случай, если в "хвосте" лежит мусор, чего быть не должно
	return res;
		
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int len = MemLen,maxbtlen=bf.BitLen,minbtlen = BitLen;
	if (bf.BitLen < BitLen) {
		len = bf.MemLen;
		maxbtlen = BitLen; minbtlen = bf.BitLen;
	}
	TBitField res(maxbtlen);
	for(int i=0;i<len-1;++i) res.pMem[i] = bf.pMem[i] & pMem[i];
	for (int i = (len - 1) * TelemBit; i < minbtlen; ++i) if (bf.GetBit(i) && GetBit(i)) res.SetBit(i); //На случай, если в "хвосте" лежит мусор, чего быть не должно
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(*this);
	for (int i = 0; i < MemLen-1; ++i) {
		res.pMem[i] = ~pMem[i];
	}
	for(int i = (MemLen-1)*TelemBit; i < BitLen; ++i){
		if (GetBit(i)) res.ClrBit(i);
		else res.SetBit(i);
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string arr;
	istr >> arr;
	delete[] bf.pMem;
	bf.BitLen = arr.size();
	bf.MemLen = (bf.BitLen - 1) / TelemBit + 1;
	bf.pMem = new TELEM[bf.MemLen];
	for (int i = 0; i < bf.BitLen; ++i) {
		if (arr[i] == 1) bf.SetBit(i); 
		else bf.ClrBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; ++i) ostr << bf.GetBit(i);
	return ostr;
}
