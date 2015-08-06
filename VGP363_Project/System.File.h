// System.File.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _SYSTEM_FILE_H_
#define _SYSTEM_FILE_H_

#include <string.h>
#include "System.Data.h"

namespace CORE {
	namespace File {

        class Data;
        
        ////////////////////////////////////////////////////////////////
        // User Functionality:
        bool DoesLocalFileExist(char* fileName);
        void EnumerateLocalFiles(void (*callback)(const char*), char* pathOptional = 0);
        
        Data* LoadLocal(char* fileName);
        void SaveLocal(char* fileName, Data* data);
        void DeleteLocal(char* fileName);
        
		void UnloadAsset(Data* asset);
        Data* LoadAsset(char* fileName);
        void EnumerateAssetFiles(void (*callback)(const char*), char* pathOptional = 0);
        
        Data* Create(int id, int size);
        
        void Close(Data** data);
        ////////////////////////////////////////////////////////////////
        
		////////////////////////////////////////////////////////////////
		// System Functionality:
        class AsyncFile {
        public:
            void* fileHandle;
            Data* data;
            void BeginWrite(const DATA::String& fileName);
            void Write(unsigned char* buffer, int numBytes);
            void EndWrite();
        };
        
        class TextDataAdapter;
        class BinaryDataAdapter;
        
        class Data {
        public:
            Data();
            ///////////////////////////////////////////////////////////
            AsyncFile file;
            ///////////////////////////////////////////////////////////
            TextDataAdapter* text;
            BinaryDataAdapter* binary;
            ///////////////////////////////////////////////////////////
            unsigned int id, size;
            unsigned int position;
            bool reachedEOF;
            unsigned char* buffer;
            void Allocate(int nNumBytes);
            void Deallocate();
            void Push(int nNumBytes, char* bytes, bool reverseOrder);
            char Peek() { return buffer[position]; }
            char Pull() { return buffer[position++]; }
            void Flush();
        };
        
		class TextDataAdapter {
		public:
			Data* data;
			char PeekChar();
			char ReadChar();
			bool IsWhitespace(char ch);
			void ReadSkipWhitespace();
			bool IsDigit(char ch);
			bool IsFloatChar(char ch);
			bool IsNumber(const CORE::DATA::String& str);
			float ParseFloat(const CORE::DATA::String& str);
			float ReadFloat();
			int ReadInteger();
			bool IsEOF(char ch);
			void ReadSkipLine();
			CORE::DATA::String ReadToEndOfLine();
			char charAt(int index);
		};

		class BinaryDataAdapter {
		public:
			Data* data;
			bool endianMode;
			BinaryDataAdapter() { memset(this, 0, sizeof(BinaryDataAdapter)); }
			
			template<typename T> void Write(T n) {
				this->data->Push(sizeof(T), (char*)&n, this->endianMode);
			}

			template<typename T> void Write(int count, T* items) {
				for(int n = 0; n < count; n++) {
					this->Write<T>(items[n]);
				}
			}

			template<typename T> T Peek() {
				T val; char* ptr = (char*)&val;
				int oldPosition = this->data->position;
				if(this->endianMode) {
					for(int n = sizeof(T)-1; n >= 0; n--) {
						ptr[n] = this->data->Pull();
					}
				} else {
					for(int n = 0; n < sizeof(T); n++) {
						ptr[n] = this->data->Pull();
					}
				}
				this->data->position = oldPosition;
				return val;
			}

			template<typename T> bool Require(T n) {
				if(this->Peek<T>() == n) {
					this->data->position += sizeof(T);
					return true;
				}
				return false;
			}

			template<typename T> void Read(T* ptr) {
				*ptr = this->Peek<T>();
				this->data->position += sizeof(T);
			}

			template<typename T> void Read(int numItems, T* items) {
				for(int n = 0; n < numItems; n++) {
					this->Read<T>(items);
					items += 1;
				}
			}

		};
        
		////////////////////////////////////////////////////////////////
		// System Functionality: 
		void SetAssetManager(void* assetManager);
		////////////////////////////////////////////////////////////////
	}
}
#endif