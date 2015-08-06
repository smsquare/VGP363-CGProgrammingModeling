// System.File.cpp
//////////////////////////////////////////////////////////////////////////////
#include "main.h"

using namespace CORE;


namespace CORE {
	namespace File {

        Data::Data() {
            memset(this, 0, sizeof(Data));
            
            text = new TextDataAdapter();
            text->data = this;
            
            binary = new BinaryDataAdapter();
            binary->data = this;
            
            file.data = this;
        }
        
        void* _AssetManager = 0;
		char _AssetFilesDirectory[255] = {0};
		char _LocalFilesDirectory[255] = {0};

		char* GetAssetFilesDirectory(char* fileNameOptional = NULL) {
	#ifdef WIN32
			strncpy_s(_AssetFilesDirectory, "./Assets/", 10);
			if(fileNameOptional) {
				strcat_s(_AssetFilesDirectory, fileNameOptional);
			}
			return _AssetFilesDirectory;
	#endif
	#ifdef APPLE
			char* basePath = getenv("FILESYSTEM");
			int length = strrchr(basePath, '/') - basePath;
			strncpy(_AssetFilesDirectory, basePath, length);
			_AssetFilesDirectory[length+1] = 0;
			strcat(_AssetFilesDirectory, "/Assets/");
			if(fileNameOptional) strcat(_AssetFilesDirectory, fileNameOptional);
			return _AssetFilesDirectory;
	#endif
	#ifdef ANDROID
			// A resource manager object is used to retrieve asset files
			// on Android, so this directory is a relative path...
			strncpy(_AssetFilesDirectory, "/", 2);
			if(fileNameOptional) {
				strcat(_AssetFilesDirectory, fileNameOptional);
			}
			return _AssetFilesDirectory;
	#endif
		}

		char* GetLocalFilesDirectory(char* fileNameOptional = NULL) {
	#ifdef WIN32
			strncpy_s(_LocalFilesDirectory, "", 0);
			//strncpy_s(_LocalFilesDirectory, "./Assets/", 10);
			if(fileNameOptional) {
				strcat_s(_LocalFilesDirectory, fileNameOptional);
			}
			return _LocalFilesDirectory;
	#endif
	#ifdef APPLE
			char* basePath = getenv("APPLICATIONDATADIR");
			strncpy(_LocalFilesDirectory, basePath, strlen(basePath)+1);
			strcat(_LocalFilesDirectory, "/");
			if(fileNameOptional) strcat(_LocalFilesDirectory, fileNameOptional);
			return _LocalFilesDirectory;
	#endif
	#ifdef ANDROID
			// NOTE: Some people online report the "files" directory sometimes doesn't
			// exist and needs to be created... Therefore if there are problems here,
			// some code should be put in to check and create the directory if necessary.
			char* basePath = getenv("APPLICATIONDATADIR");
			strncpy(_LocalFilesDirectory, basePath, strlen(basePath)+1);
			strcat(_LocalFilesDirectory, "/");
			if(fileNameOptional) strcat(_LocalFilesDirectory, fileNameOptional);
			return _LocalFilesDirectory;
	#endif
		}

		char TextDataAdapter::charAt(int index) {
			if(index < 0) return 0;
			if(index >= this->data->size) return 0; 
			return this->data->buffer[index]; 
		}

		char TextDataAdapter::PeekChar() {
			return this->charAt(this->data->position);
		}

		char TextDataAdapter::ReadChar() {
			char result = this->charAt(this->data->position);
			this->data->position += 1;
			return result;
		}

		bool TextDataAdapter::IsWhitespace(char ch) {
			return ch == ' ' || ch == '\t' || ch == '\n';
		}

		void TextDataAdapter::ReadSkipWhitespace() {
			while(this->IsWhitespace(this->PeekChar())) {
				this->ReadChar();
			}
		}

		bool TextDataAdapter::IsDigit(char ch) {
			return ch >= '0' && ch <= '9';
			// When C++11 is supported by Android...
			//return std::regex_match(std::string(1, ch), std::regex("[\\d]"));
		}

		bool TextDataAdapter::IsFloatChar(char ch) {
			return ch == '-' || ch == '.' || this->IsDigit(ch);
			// When C++11 is supported by Android...
			//return std::regex_match(std::string(1, ch), std::regex("[-|\\d|.]"));
		}

		bool TextDataAdapter::IsNumber(const DATA::String& str) {
			const char* num = str.ToCharPtr();
			if(num==NULL) return false;
			if(*num==NULL) return false;
			while(*num) {
				if(this->IsFloatChar(*num)==false) return false;
				num++;
			}
			return true;
		}

		float TextDataAdapter::ParseFloat(const DATA::String& str) {
			return str.ToFloat();
		}

		float TextDataAdapter::ReadFloat() {
			this->ReadSkipWhitespace();
			DATA::String str = "";
			while(this->IsFloatChar(this->PeekChar())) {
				str += this->ReadChar();
			}
			str += 0;
			if(!this->IsNumber(str)) {
				// syntax error
				return 0.0f;
			}
			return this->ParseFloat(str);
		}

		int TextDataAdapter::ReadInteger() {
			this->ReadSkipWhitespace();
			DATA::String str = "";
			while(this->IsFloatChar(this->PeekChar())) {
				str += this->ReadChar();
			}
			str += 0;
			if(!this->IsNumber(str)) {
				// syntax error
				return 0;
			}
			return (int)this->ParseFloat(str);
		}

		bool TextDataAdapter::IsEOF(char ch) {
			if(ch == 0) return true;
			else return false;
		}

		void TextDataAdapter::ReadSkipLine() {
			while(true) {
				char ch = this->ReadChar();
				if(!ch) break;
				if(ch == '\n') break;
			}
		}

		DATA::String TextDataAdapter::ReadToEndOfLine() {
			DATA::String str = "";
			while (true) {
				char ch = this->ReadChar();
				if (!ch) break;
				if (ch == '\n') break;
				str += ch;
			}
			return str;
		}

		void AsyncFile::BeginWrite(const DATA::String& fileName) {
	#ifdef WIN32
			char* name = const_cast<char*>(fileName.data.c_str());
			char* fullPath = GetLocalFilesDirectory(name);        
			HANDLE hFile = CreateFile(
				fullPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
				FILE_ATTRIBUTE_NORMAL, NULL
			);
		
			if(hFile == INVALID_HANDLE_VALUE) {
				//Debug::Log("Win32: 002 Error opening file for writing.\n");
				return;
			}
			this->fileHandle = hFile;
			this->data->Allocate(4096);
			return;
	#endif
	#ifdef APPLE
            char* name = const_cast<char*>(fileName.data.c_str());
			char* fullPath = GetLocalFilesDirectory(name);
			FILE* file = fopen(fullPath, "wb");
			if(file == NULL) {
				//Debug::Log("iOS: 002 Error opening file for writing.\n");
				return;
			}
			this->fileHandle = file;
			this->data->Allocate(4096);
			return;
	#endif
	#ifdef ANDROID
            char* name = const_cast<char*>(fileName.data.c_str());
			char* fullPath = GetLocalFilesDirectory(name);
			FILE* file = fopen(fullPath, "wb");
			if(file == NULL) {
				//Debug::Log("Android: 002 Error opening file for writing.\n");
				return;
			}
			this->fileHandle = file;
			this->data->Allocate(4096);
			return;
	#endif
		}

		void AsyncFile::Write(unsigned char* buffer, int numBytes) {
	#ifdef WIN32
			DWORD dwNumWritten = 0;
			WriteFile(this->fileHandle, buffer, numBytes, &dwNumWritten, NULL);
			if(dwNumWritten != numBytes) {
				//Debug::Log("Win32: 002 Error saving file data.\n");
			}
			return;
	#endif
	#ifdef APPLE
			fwrite(buffer, numBytes, 1, (FILE*)this->fileHandle);
			return;
	#endif
	#ifdef ANDROID
			fwrite(buffer, numBytes, 1, (FILE*)this->fileHandle);
			return;
	#endif
		}

		void AsyncFile::EndWrite() {
	#ifdef WIN32
			this->data->Flush();
			CloseHandle((HANDLE)this->fileHandle);
			return;
	#endif
	#ifdef APPLE
			this->data->Flush();
			fclose((FILE*)this->fileHandle);
			return;
	#endif
	#ifdef ANDROID
			this->data->Flush();
			fclose((FILE*)this->fileHandle);
			return;
	#endif
		}

		void Data::Allocate(int nNumBytes) {
			Deallocate();
			this->buffer = new unsigned char[nNumBytes];
			this->size = nNumBytes;
		}

		void Data::Deallocate() {
			if(this->buffer != NULL) {
				delete [] this->buffer;
				this->buffer = NULL;
			}
		}

		void Data::Push(int nNumBytes, char* bytes, bool reverseOrder) {
			if((this->position + nNumBytes) >= this->size) {
				this->Flush();
			}
			if((this->position + nNumBytes) >= this->size) {
				//Debug::Log("ERROR: Data chunk size exceeds buffer limit.");
				return;
			}
			if(reverseOrder) {
				for(int n = nNumBytes-1; n >= 0; n--) {
					this->buffer[this->position] = bytes[n];
					this->position += 1;
				}
			} else {
				for(int n = 0; n < nNumBytes; n++) {
					this->buffer[this->position] = bytes[n];
					this->position += 1;
				}
			}
		}

		void Data::Flush() {
			if(this->file.fileHandle != NULL && this->position > 0) {
				this->file.Write(this->buffer, this->position);
			}
			this->position = 0;
		}

		void SetAssetManager(void* assetManager) {
	#ifdef ANDROID
			_AssetManager = assetManager;
	#endif
		}

		void Close(Data** data) {
			if(data == NULL) return;
			if(*data == NULL) return;
			if((*data)->buffer) {
				delete [] (*data)->buffer;
			}
			(*data)->id = -1;
			(*data)->size = 0;
			(*data)->buffer = 0;
			delete *data;
			*data = NULL;
		}

		Data* Create(int id, int size) {
			Data* data = new Data();
			data->size = size;
			data->buffer = new unsigned char[size+1];
			memset(data->buffer, 0, size+1);
			data->id = id;
			return data;
		}

		void DeleteLocal(char* fileName) {
	#ifdef WIN32
			DeleteFile(fileName);
	#endif
	#ifdef APPLE
			char* fullPath = GetLocalFilesDirectory(fileName);
			remove(fullPath);
	#endif
	#ifdef ANDROID
			char* fullPath = GetLocalFilesDirectory(fileName);
			remove(fullPath);
	#endif
		}

		void EnumerateLocalFiles(void (*callback)(const char*), char* pathOptional /* = NULL */) {
	#ifdef WIN32
			WIN32_FIND_DATA findData;
			char* fullPath = GetLocalFilesDirectory(pathOptional);
			strcat_s(fullPath, 255, "/*");
			HANDLE hFind = FindFirstFile(fullPath, &findData);
			if (INVALID_HANDLE_VALUE == hFind) {
				//Debug::Log("Win32: Error enumerating local files.");
			} 
			do { callback(findData.cFileName); }
			while (FindNextFile(hFind, &findData) != 0);
	#endif
	#ifdef APPLE
			char* fullPath = GetLocalFilesDirectory(pathOptional);
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir(fullPath)) != NULL) {
				while ((ent = readdir(dir)) != NULL) {
					callback(ent->d_name);
				}
				closedir(dir);
			} else {
				//Debug::Log("Apple: Error: Enumeration of local files failed.");
			}
			return;
	#endif
	#ifdef ANDROID
			char* fullPath = GetLocalFilesDirectory(pathOptional);
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir(fullPath)) != NULL) {
				while ((ent = readdir(dir)) != NULL) {
					callback(ent->d_name);
				}
				closedir(dir);
			} else {
				//Debug::Log("Android: Error: Enumeration of local files failed.");
			}
			return;
	#endif
		}

		bool DoesLocalFileExist(char* fileName) {
	#ifdef WIN32
			WIN32_FIND_DATA findData;
			HANDLE hFound = FindFirstFile(fileName, &findData) ;
			if(hFound != INVALID_HANDLE_VALUE) 
			{
				FindClose(hFound);
				return true;
			}
			return false;
	#endif
	#ifdef APPLE
			char* fullPath = GetLocalFilesDirectory(fileName);
			FILE* file = fopen(fullPath, "r");
			if(file == NULL) return false;
			fclose(file);
			return true;
	#endif
	#ifdef ANDROID
			char* fullPath = GetLocalFilesDirectory(fileName);
			FILE* file = fopen(fullPath, "r");
			if(file == NULL) return false;
			fclose(file);
			return true;
	#endif
		}

		void SaveLocal(char* fileName, Data* data) {
	#ifdef WIN32
			char* fullPath = GetLocalFilesDirectory(fileName);        
			HANDLE hFile = CreateFile(
				fullPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
				FILE_ATTRIBUTE_NORMAL, NULL
			);
		
			if(hFile == INVALID_HANDLE_VALUE) {
				//Debug::Log("Win32: Error opening file for writing.\n");
				return;
			}

			DWORD dwNumWritten = 0;
			WriteFile(hFile, data->buffer, data->size, &dwNumWritten, NULL);
			if(dwNumWritten != data->size) {
				//Debug::Log("Win32: Error saving file data.\n");
			}

			CloseHandle(hFile);
			return;
	#endif
	#ifdef APPLE
			char* fullPath = GetLocalFilesDirectory(fileName);        
			FILE* file = fopen(fullPath, "wb");
		
			if(file == NULL) return;
			fwrite(data->buffer, data->size, 1, file);
		
			fclose(file);
			return;
	#endif
	#ifdef ANDROID
			char* fullPath = GetLocalFilesDirectory(fileName);        
			FILE* file = fopen(fullPath, "wb");
		
			if(file == NULL) return;
			fwrite(data->buffer, data->size, 1, file);
		
			fclose(file);
			return;
	#endif
		}

		Data* LoadLocal(char* fileName) {
	#ifdef WIN32
			char* fullPath = GetLocalFilesDirectory(fileName);        
			HANDLE hFile = CreateFile(
				fullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, NULL
			);

			if(hFile == INVALID_HANDLE_VALUE) {
				//Debug::Log("Win32: Error opening file for reading.\n");
				return NULL;
			}

			DWORD dwFileSize = GetFileSize(hFile, NULL);
			Data* data = File::Create((int)fileName, dwFileSize);

			DWORD dwNumRead = 0;
			ReadFile(hFile, data->buffer, dwFileSize, &dwNumRead, NULL);
			data->buffer[data->size] = 0;
			if(dwNumRead != dwFileSize) {
				//Debug::Log("Win32: ERROR: File was not read completely.\n");
			}

			CloseHandle(hFile);
			return data;
	#endif
	#ifdef APPLE
			char* fullPath = GetLocalFilesDirectory(fileName);        
			FILE* file = fopen(fullPath, "rb");
		
			if(file == NULL) return NULL;
		
			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			fseek(file, 0, SEEK_SET);
		
            File::Data *data = File::Create(0, size);
			fread(data->buffer, data->size, 1, file);
			data->buffer[data->size] = 0;
		
			fclose(file);
		
			return data;
	#endif
	#ifdef ANDROID
			char* fullPath = GetLocalFilesDirectory(fileName);        
			FILE* file = fopen(fullPath, "rb");
		
			if(file == NULL) return NULL;
		
			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			fseek(file, 0, SEEK_SET);
		
			File::Data *data = File::Create(0, size);
			fread(data->buffer, data->size, 1, file);
			data->buffer[data->size] = 0;
		
			fclose(file);
		
			return data;
	#endif
		}

		void EnumerateAssetFiles(void (*callback)(const char*), char* pathOptional /*= NULL*/) {
	#ifdef WIN32
			return EnumerateLocalFiles(callback, pathOptional);
	#endif
	#ifdef APPLE
			char* fullPath = GetAssetFilesDirectory(pathOptional);
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir(fullPath)) != NULL) {
				while ((ent = readdir(dir)) != NULL) {
					callback(ent->d_name);
				}
				closedir(dir);
			}  else {
				//Debug::Log("Apple: Error: Enumeration of asset files failed.");
			}
			return;
	#endif
	#ifdef ANDROID
			// NOTE: Android lists the wrong set of files when attempting
			// to enumerate the resources directory... As this is intended
			// for debugging purposes, the functionality is not used.
			if(_AssetManager == NULL) return;
			AAssetManager *mgr = (AAssetManager*)_AssetManager;
			char* fullPath = GetAssetFilesDirectory(pathOptional);
			AAssetDir* assetDir = AAssetManager_openDir(mgr, fullPath);
			if(assetDir == NULL) {
				//Debug::Log("Android: Error: Enumeration of asset files failed.");
				return;
			}
			const char* fileName = NULL;
			do {
				fileName = AAssetDir_getNextFileName(assetDir);
				if(fileName == NULL) break;
				callback(fileName);
			} while(true);
			AAssetDir_close(assetDir);
			return;
	#endif
		}

		void UnloadAsset(Data* asset) {
			CORE::File::Close(&asset);
		}

		Data* LoadAsset(char* fileName) {
	#ifdef WIN32 
			return LoadLocal(fileName);
	#endif
	#ifdef APPLE
			char* fullPath = GetAssetFilesDirectory(fileName);
			FILE* file = fopen(fullPath, "rb");
		
			if(file == NULL) return NULL;
		
			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			fseek(file, 0, SEEK_SET);
		
            File::Data *data = File::Create(0, size);
			fread(data->buffer, data->size, 1, file);
			data->buffer[data->size] = 0;
		
			fclose(file);
		
			return data;
	#endif
	#ifdef ANDROID
			if(_AssetManager == 0) return NULL;
			AAssetManager *mgr = (AAssetManager*)_AssetManager;
			AAsset* asset = AAssetManager_open(mgr, fileName, AASSET_MODE_UNKNOWN);
			if (asset) {
				int size = AAsset_getLength(asset);
				File::Data *data = File::Create((int)fileName, size);
				int bytesread = AAsset_read(asset, data->buffer, size);
				data->buffer[data->size] = 0;
				if (bytesread != size) {
					//Debug::Log("Android: ERROR: File was not read completely.\n");
				}
				AAsset_close(asset);
				return data;
			}
			return NULL;
	#endif
		}
	}
} // namespace File
