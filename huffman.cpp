#include"huffman.h"
#include <stdio.h>
#include<iostream>
#include<fstream>
#include<string>


struct NODE {
	unsigned char	c;		// ky tu
	int		freq;	// so lan xuat hien
	bool	used;	// danh dau node da xu ly chua 
	int		nLeft;	// chi so nut con nam ben trai
	int		nRight;	// chi so nut con nam ben phai
};

struct MABIT {
	char*	bits;
	int		soBit;
};

const int MAX_NODE = 256 * 9;
const int MAX_BIT_LEN = 10000;
NODE	huffTree[MAX_NODE];
MABIT	bangMaBit[256];



void KhoiTao() {
	for (int i = 0; i < MAX_NODE; i++) {
		huffTree[i].c = i;
		huffTree[i].freq = 0;
		huffTree[i].used = false;
		huffTree[i].nLeft = -1;
		huffTree[i].nRight = -1;
	}
}

void ThongKeTanSoXuatHien(char* tenFile, size_t& size) {
	FILE* fi = fopen(tenFile, "rt");
	size = 0;
	if (fi == NULL){
		std::cout << "\nKhong tim thay file.";
		return;
	}
	unsigned char c;

	while (1)	{
		fscanf(fi, "%c", &c);
		if (feof(fi)) {
			break;
		}
		huffTree[c].freq++;
		size++;
	}

	fclose(fi);
}

void XuatBangThongKe() {
	printf("Bang thong ke tan suat: \n");
	for (int i = 0; i < 256; i++) {
		if (huffTree[i].freq > 0) {
			printf("%c: %d\n", i, huffTree[i].freq);
		}
	}
}

bool Tim2PhanTuMin(int &i, int &j, int nNode) {
	i = -1;
	j = -1;

	// tim 2 phan tu co trong so nho nhat
	for (int k = 0; k < nNode; k++)
	if (huffTree[k].used == false && huffTree[k].freq > 0)		{
		if (i == -1) {
			i = k;
		}
		else if (j == -1) {
			j = k;
		}
		else {
			if (huffTree[i].freq > huffTree[j].freq) {
				if (huffTree[k].freq < huffTree[i].freq) {
					i = k;
				}
			}
			else {
				if (huffTree[k].freq < huffTree[j].freq) {
					j = k;
				}
			}
		}
	}
	// sap xep lai 2 phan tu de co i: phan tu co trong so nho nhat, j: phan tu co trong so nho nhi	
	// co 2 truong hop can doi i,j:
	// huffTree[i].freq > huffTree[j].freq
	// huffTree[i].freq == huffTree[j].freq va (huffTree[i].c > huffTree[j].c)
	if (i != -1 && j != -1) {
		if ((huffTree[i].freq > huffTree[j].freq) || ((huffTree[i].freq >= huffTree[j].freq) && (huffTree[i].c > huffTree[j].c))) {
			int t = i;
			i = j;
			j = t;
		}
		return true;
	}
	else {
		return false;
	}
}


int TaoCayHuffman() {
	int nNode = 256;
	int i, j;
	bool timThay = false;
	while (true)	{
		timThay = Tim2PhanTuMin(i, j, nNode);
		if (!timThay) {
			break;
		}

		huffTree[nNode].c = (huffTree[i].c < huffTree[j].c) ? huffTree[i].c : huffTree[j].c;
		huffTree[nNode].freq = huffTree[i].freq + huffTree[j].freq;
		huffTree[nNode].nLeft = i;
		huffTree[nNode].nRight = j;

		huffTree[i].used = true;
		huffTree[j].used = true;


		huffTree[nNode].used = false;
		nNode++;

	}
	return nNode - 1;

}

void XuatCayHuffman(int node, int tab) {
	if (node == -1) {
		return;
	}
	for (int i = 0; i < tab; i++) {
		printf("\t");
	}
	if (huffTree[node].nLeft == -1 && huffTree[node].nRight == -1)	{
		printf("%c\n", huffTree[node].c);
	}
	else	{
		printf("%c..\n", huffTree[node].c);
		XuatCayHuffman(huffTree[node].nLeft, tab + 1);
		XuatCayHuffman(huffTree[node].nRight, tab + 1);
	}
}

void DuyetCayHuffman(int node, char maBit[], int nMaBit) {
	if (node == -1) {
		return;
		//Dung cho th co 1 con
	}
	if (huffTree[node].nLeft == -1 && huffTree[node].nRight == -1) {
		bangMaBit[node].soBit = nMaBit;
		bangMaBit[node].bits = new char[nMaBit];
		for (int i = 0; i < nMaBit; i++) {
			bangMaBit[node].bits[i] = maBit[i];
		}
		return;
	}
	else {
		//Ghi chú: turn left = 0
		maBit[nMaBit] = 0;
		DuyetCayHuffman(huffTree[node].nLeft, maBit, nMaBit + 1);

		//Ghi chú: turn right = 1
		maBit[nMaBit] = 1;
		DuyetCayHuffman(huffTree[node].nRight, maBit, nMaBit + 1);
	}
}

void PhatSinhMaBit(int nRoot) { // Ghi chú: nRoot la so node cua bang thong ke
	for (int i = 0; i < 256; i++) {
		bangMaBit[i].soBit = 0;
		bangMaBit[i].bits = NULL;
	};
	char maBit[MAX_BIT_LEN / 8];//1250 bit 
	int nMaBit = 0;

	DuyetCayHuffman(nRoot, maBit, nMaBit);
}

void XuatBangMaBit() {
	for (int i = 0; i < 256; i++)
	if (bangMaBit[i].soBit > 0)	{
		printf("%c: ", i);
		for (int j = 0; j < bangMaBit[i].soBit; j++)
			printf("%d", bangMaBit[i].bits[j]);
		printf("\n");
	}
}

void XuatByte(unsigned char out, int soBitCoNghia, FILE* f) {

	for (int i = 7; i >= 7 - soBitCoNghia + 1; i--) {
		if ((out >> i) & 1) {
			printf("1");
		}
		else {
			printf("0");
		}
	}
	printf(" ");
}

void MaHoa1KyTu(unsigned char c, unsigned char &out, unsigned char &viTriBit, FILE* fo) {
	for (int i = 0; i < bangMaBit[c].soBit; i++) {
		if (bangMaBit[c].bits[i] == 1) {
			out = out | (1 << viTriBit);
		}
		if (viTriBit == 0) { // da du 1 byte, ghi byte do ra file
			viTriBit = 7;
			fwrite(&out, sizeof(c), 1, fo);

			/*XuatByte(out, 8,f);*/
			out = 0;
		}
		else {
			viTriBit--;
		}
	}
}


void NenHuffman(char* inputFile, char* huffname) {
	// BUOC 1: thong ke tan suat xuat hien cua cac ki tu
	KhoiTao();
	size_t size;
	ThongKeTanSoXuatHien(inputFile, size);

	//XuatBangThongKe();

	// BUOC 2: tao cay Huffman

	int nRoot = TaoCayHuffman();

	/*XuatCayHuffman(nRoot, 0);*/

	// BUOC 3: phat sinh ma bit

	PhatSinhMaBit(nRoot);

	//XuatBangMaBit();

	// BUOC 4: thay the cac ky tu bang ma bit tuong ung
	unsigned char out = 0;				// ky tu se xuat ra
	unsigned char soBitCoNghia = 0;		// byte cuoi co the ko su dung het cac bit nen can luu so bit co nghia cua byte cuoi

	unsigned char c;
	unsigned char viTriBit = 7;			//Ghi chú: ý nghĩa của biến viTriBit?
	FILE* fi = fopen(inputFile, "rt");
	FILE* f = fopen(huffname, "wb");

	fwrite(&size, sizeof(size), 1, f);
	fwrite(&nRoot, sizeof(nRoot), 1, f);
	for (int i = 0; i <= nRoot; i++)
		fwrite(&huffTree[i], sizeof(huffTree[i]), 1, f);

	while (true) {
		fscanf(fi, "%c", &c);
		if (feof(fi)) {
			break;
		}
		MaHoa1KyTu(c, out, viTriBit, f);
	}
	fwrite(&out, sizeof(out), 1, f);

	fclose(f);
	fclose(fi);
}


bool DuyetTrai(int &index, char &c, int nRoot){
	index = huffTree[index].nLeft;
	if (huffTree[index].nLeft == -1 && huffTree[index].nRight == -1)
	{
		c = huffTree[index].c;
		index = nRoot;
		return true;
	}

	return false;
}

bool DuyetPhai(int &index, char &c, int nRoot){
	index = huffTree[index].nRight;
	if (huffTree[index].nLeft == -1 && huffTree[index].nRight == -1)
	{
		c = huffTree[index].c;
		index = nRoot;
		return true;
	}
	return false;
}





void ReadFile(char* huffname, char* fileout){
	FILE *f = fopen(huffname, "rb");
	FILE *fi = fopen(fileout, "w");
	if (f == NULL)
	{
		printf("open error!");
		return;
	}
	size_t size;
	int nRoot;
	fread(&size, sizeof(size), 1, f);
	//	std::cout << "\nSize: " << size;
	fread(&nRoot, sizeof(nRoot), 1, f);
	for (int i = 0; i <= nRoot; i++)
		fread(&huffTree[i], sizeof(huffTree[i]), 1, f);

	/*XuatCayHuffman(nRoot, 0);*/
	int index = nRoot;
	char c = 0;

	unsigned char out;
	size_t cout = 0;
	while (1){

		fread(&out, sizeof(out), 1, f);
		if (feof(f))
			break;

		for (int i = 7; i >= 0; i--)
		{
			if (cout == size)
				break;
			if ((out >> i) & 1) {
				if (DuyetPhai(index, c, nRoot) == true){
					cout++;
					fprintf(fi, "%c", c);
				}
			}
			else {
				if (DuyetTrai(index, c, nRoot) == true){
					cout++;
					fprintf(fi, "%c", c);
				}
			}

		}
	}



	fclose(f);
	fclose(fi);
}


