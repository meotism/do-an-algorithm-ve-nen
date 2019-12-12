#include"folder.h"
#include"dirent.h"
#include"huffman.h"
#include<iostream>
#include"dir.h"
#include<string>
#include<ctime>
int main()
{
	int menu = 0;
	do{
		system("cls");
		int thaotac = 0;
		std::cout << "\nNhap thao tac muon dung: " << std::endl;
		std::cout << "1.Nen file. " << std::endl;
		std::cout << "2.Giai nen file huffman." << std::endl;
		std::cout << "3.Nen folder. " << std::endl;
		std::cout << "4.Giai nen folder." << std::endl;
		std::cin >> thaotac;
		std::clock_t start;
		start = clock();
		
		std::string nameinfolder = "T1";
		std::string namehuff = "Tao";
		std::string namefile = "file";
		std::string filehuff = "huff";
		if (thaotac == 1){
			//nenfile
			std::cin.ignore();
			std::cout << "\nNhap ten file can nen: " << std::endl;
			std::getline(std::cin, namefile);
			std::cout << "\nNhap ten file huff: " << std::endl;
			std::getline(std::cin, filehuff);
			char* huff = new char[filehuff.size()];
			char* ifile = new char[namefile.size()];
			Change(namefile, ifile);
			Change(filehuff, huff);
			NenHuffman(ifile,huff);
			start = clock() - start;
			std::cout << "\nThoi gian nen: " << start;
		}
		if (thaotac == 2){
			//Giainenfile
			std::cin.ignore();
			std::cout << "\nNhap ten file huff: " << std::endl;
			std::getline(std::cin, filehuff);
			std::cout << "\n Nhap ten file sau khi giai nen: " << std::endl;
			std::getline(std::cin, namefile);
			char* huff = new char[filehuff.size()];
			char* ifile = new char[namefile.size()];
			Change(namefile, ifile);
			Change(filehuff, huff);
			ReadFile(huff, ifile);
			start = clock() - start;
			std::cout << "\nThoi gian giai nen: " << start;
		}
		if (thaotac == 3){
			//nenfolder
			std::cin.ignore();
			std::cout << "\n Nhap ten folder can nen: " << std::endl;
			std::getline(std::cin, nameinfolder);
			std::cout << "\nNhap ten folder huff: " << std::endl;
			std::getline(std::cin, namehuff);

			char* namefolder = new char[nameinfolder.size()];
			Change(nameinfolder, namefolder);
			char* hufffloder = new char[namehuff.size()];
			Change(namehuff, hufffloder);

			if (mkdir(namefolder) != -1){
				std::cout << "\nDu lieu khong hop le.";
				std::cout << "\nNhap 0 de thuc hien lai. Nhap -1 de thoat." << std::endl;
				std::cin >> menu;
			}
			NenFolder(nameinfolder, namehuff);
			start = clock() - start;
			std::cout << "\nThoi gian nen: " << start;
		}
		if (thaotac == 4){
			//Giainenfolder
			std::cin.ignore();
			std::cout << "\nNhap ten folder huff: " << std::endl;
			std::getline(std::cin, namehuff);
			char* hufffolder = new char[namehuff.size()];
			Change(namehuff, hufffolder);
			if (mkdir(hufffolder) != -1){
				std::cout << "\nDu lieu khong hop le.";
				std::cout << "\nNhap 0 de thuc hien lai. Nhap -1 de thoat." << std::endl;
				std::cin >> menu;
			}
			GiaiNenFolder(namehuff);
			start = clock() - start;
			std::cout << "\nThoi gian nen: " << start;
		}
		std::cout << "\nNhap 0 de thuc hien lai. Nhap -1 de thoat." << std::endl;
		std::cin >> menu;
	} while (menu == 0);


	return 0;
}