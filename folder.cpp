#include"dir.h"
#include"dirent.h"
#include<iostream>
#include<string>
#include<fstream>
#include"huffman.h"

//Ham chuyen tu chuoi thanh mang char *
void Change(std::string s, char* a){

	for (int i = 0; i < s.size(); i++){
		a[i] = s[i];
	}
	a[s.size()] = NULL;
}


void ReadFolder(char* dir_name, std::ofstream& fo){
	DIR* dir;
	dirent* ent;
	dir = opendir(dir_name);
	int cout = 0;

	while (1){
		if ((ent = readdir(dir)) != NULL){
			cout++;
			std::string temp;
			for (int i = 0; i < strlen(dir_name); i++)
				temp += dir_name[i];
			temp += '/';
			for (int i = 0; i < ent->d_namlen; i++)
				temp += ent->d_name[i];
			Change(temp, ent->d_name);
			std::cout << "Name: " << ent->d_name << "\tType: " << ent->d_type << std::endl;
			fo << temp;

			if (ent->d_type == 16384)
				fo << (char)-2;
			else
				fo << (char)-3;

			if (ent->d_type == 16384 && cout>2){
				ReadFolder(ent->d_name, fo);
			}

		}
		else
			break;
	}

	closedir(dir);
}
//tim vi tri dau tien xuat hien dau theo chieu nguoc lai /
int VitriFolder(std::string temp){
	int size = temp.size();
	for (int i = size-1; i >=0; i--)
	if (temp[i] == '/')
		return i;

	return -1;
}

void ChangeHuffName(std::string& s, int n){
	int vitri = VitriFolder(s);

	std::string temp;

	for (int i = 0; i <= vitri; i++)
		temp.push_back(s[i]);
	
	while (n != 0){
		temp.push_back(n % 10 + 48);
		n = n / 10;
	}
	temp += ".huff";
	s = temp;

}

void ReadFile(char * dir_name,std::string &s,int &n){
	DIR* dir;
	dirent* ent;
	dir = opendir(dir_name);
	int cout = 0;

	while (1){
		if ((ent = readdir(dir)) != NULL){
			cout++;
			std::string temp;
			for (int i = 0; i < strlen(dir_name); i++)
				temp += dir_name[i];
			temp += '/';
			for (int i = 0; i < ent->d_namlen; i++)
				temp += ent->d_name[i];
			Change(temp, ent->d_name);

			if (ent->d_type == 16384 * 2)
			{//Read data from file to file .huff(binary file)
				char* inputname = new char[temp.size()];
				Change(temp, inputname);
				char* huffname = new char[s.size()];
				Change(s, huffname);
				NenHuffman(inputname, huffname);

				std::cout << "\n" << temp << " data: " << s;
				
				ChangeHuffName(s, n);
				n++;
			}

			if (ent->d_type == 16384 && cout>2){
				ReadFile(ent->d_name,s,n);
			}

		}
		else
			break;
	}
	closedir(dir);


}

void NenFolder(std::string nameinfolder, std::string hufffolder){
	char *dir_name = new char[nameinfolder.size()];
	char *folder = new char[hufffolder.size()];
	Change(hufffolder, folder);
	mkdir(folder);
	Change(nameinfolder, dir_name);
	
	std::string hufffile = hufffolder + "/" + "save.huff";
	char *huffname = new char[hufffile.size()];
	Change(hufffile, huffname);

	std::ofstream fo(huffname, std::ios::binary);
	if (fo.fail() == 1)
	{
		std::cout << "\Khong the tao file ";
		return;
	}

	ReadFolder(dir_name, fo);
	fo << (char)-1;
	fo.close();
	
	std::string s = hufffolder + "/" + "0.huff";
	int n = 1;
	ReadFile(dir_name,s,n);
	fo.close();
}



void ChangeNameFolder(std::string& s, int vitri){
	std::string temp1,temp2;
	int size = s.size();
	for (int i = vitri; i < size; i++)
		temp1.push_back(s[i]);

	for (int i = 0; i <= vitri-1; i++)
		temp2.push_back(s[i]);
	temp2.push_back('-');
	temp2.push_back(' ');
	temp2.push_back('C');
	temp2.push_back('o');
	temp2.push_back('p');
	temp2.push_back('y');

	for (int i = 0; i < temp1.size(); i++)
		temp2.push_back(temp1[i]);
	s = temp2;
}

int FindChiSo(std::string hufffolder){
	std::string namehuff = hufffolder + "/" + "save.huff";
	std::ifstream fi(namehuff, std::ios::binary);
	if (fi.fail() == 1)
		return -1;

	std::string temp;
	char c;

	while (1){
		fi.get(c);
		if (c == -2){//Folder
			return temp.size() - 2;
			temp.clear();
		}
		else if (c == -3){//file thuong
		
			temp.clear();

		}
		else
		{
			temp.push_back(c);
		}
		if (c == -1){
			std::cout << temp << std::endl;
			break;
		}

	}

	fi.close();


}

void CreateFolder(std::string hufffolder,int vitri){
	std::string namehuff = hufffolder + "/" + "save.huff";
	std::ifstream fi(namehuff, std::ios::binary);
	if (fi.fail() == 1)
		return;

	std::string temp;
	std::string s = hufffolder + "/" + "0.huff";
	int n = 1;
	char c;

	while (1){
		fi.get(c);
		if (c == -2){//Folder
			ChangeNameFolder(temp, vitri);

			std::cout << temp << std::endl;
			char* nameFolder = new char[temp.size()];
			Change(temp, nameFolder);
			mkdir(nameFolder);
			temp.clear();

		}
		else if (c == -3){//file thuong
		
			ChangeNameFolder(temp, vitri);

			std::cout << temp << std::endl;
			char* namefile = new char[temp.size()];
			Change(temp, namefile);
			char *huffname = new char[s.size()];
			Change(s, huffname);
			std::cout << "File: " << temp << std::endl;
			ReadFile(huffname, namefile);
			
			ChangeHuffName(s, n);
			n++;

			temp.clear();

		}
		else
		{
			temp.push_back(c);
		}
		if (c == -1){
			std::cout << temp << std::endl;
			break;
		}

	}

	fi.close();

}



void GiaiNenFolder( std::string hufffolder){
	int vitri = FindChiSo(hufffolder);
	CreateFolder(hufffolder,vitri);

}





