// System.Data.h
////////////////////////////////////////////////////////
#pragma once
#ifndef __SYSTEM_DATA_H_
#define __SYSTEM_DATA_H_

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

namespace CORE {
	namespace DATA {

		class String {
		public:
			String(){}
			string data;
			int length() const { return data.length(); }
			String(const char* sz) { this->data = sz; }
			String& operator=(char sz[]) { this->data = sz; return *this; }
			operator const char* () { return this->ToCharPtr(); }
			operator const std::string () { return data; }
			String& operator+=(const char ch) { this->data += ch; return *this; }
			bool operator==(const char* sz) const { return data.compare(sz)==0; }
			bool operator==(const DATA::String& str) const { return data.compare(str.ToCharPtr())==0; }
			String& Append(const DATA::String& str) { data.append(str.data); return *this; }
			float ToFloat() const {
#ifdef ANDROID
                return atof(this->data.c_str());
#endif
#ifdef APPLE
                return stof(this->data);
#endif
#ifdef WIN32
                return atof(this->data.c_str());
#endif
            }
			String SubString(int begin, int end) { return String(this->data.substr(begin, end).c_str()); };
			String ReplaceExtension(const DATA::String& str) { return SubString(0, length()-3).Append(str); }
			const char* ToCharPtr() const { return data.c_str(); }
		};

		class Option {
		public:
			void* data;
			operator const bool() { return *((bool*)&data); }
			operator const float() { return *((float*)&data); }
			operator const int() { return *((int*)&data); }
			Option& operator=(bool b) { *((bool*)&data) = b; return *this; }
			Option& operator=(float f) { *((float*)&data) = f; return *this; }
			Option& operator=(int n) { *((int*)&data) = n; return *this; }
		};

		template <class T>
		class Cache {
		private:
#ifdef ANDROID
			std::tr1::unordered_map<std::string, T*> data;
#else
			std::unordered_map<std::string, T*> data;
#endif
		public:
			void Add(std::string key, T* value) { 
				#if (_MSC_VER == 1600) // C++03 Visual Studio 2010
				data.insert(std::make_pair<std::string,T*>(key, value));
				#elif (_MSC_VER == 1700) // C++11 Visual Studio 2012
				data.insert(std::make_pair(key, value));
				#else
                data.insert(std::make_pair(key, value));
				#endif
			}
			bool Contains(std::string key) { return data.count(key) == 1; }
			void Remove(std::string key) { data.erase(key); }
			T* operator[](std::string key) { return data[key]; };
			T* operator[](DATA::String key) { return data[key.data]; }
		};

		class Options {
		public:
			Option operator[](char* name) { return Option(); };
		};

		template <class T>
		class List {
		public:
			vector<T*> collection;
			T* buffer;
			int length;
			List() {
				buffer = 0;
				length = 0;
			}
			bool Contains(const T* ptr) {
				for(int n = 0; n < length; n++) {
					if((*this)[n] == ptr) return true;
				}
				return false;
			}
			T* operator[](int index) {
				length = collection.size();
				if(index < 0) return NULL;
				if(index >= length) return NULL;
				return collection[index];
			}
			int AddPtr(T* item) {
				collection.push_back(item);
				length = collection.size();
				return length-1;
			}
			void RemoveAt(int index) {
				collection.erase(index);
				length = collection.size();
			}
			void RemoveAll() {
				collection.clear();
			}
			T GetValueAt(int index) {
				T zeroValue;
				memset(&zeroValue, 0, sizeof(T));
				T* ptr = (*this)[index];
				if(ptr == 0) return zeroValue;
				else return *ptr;
			}
			T* ToBuffer() {
				if(buffer == 0) {
					buffer = new T[length];
					for(int n = 0; n < length; n++) {
						buffer[n] = GetValueAt(n);
					}
				}
				return buffer;
			}
		};

		template <class T>
		class ValueList {
		public:
			vector<T> collection;
			T* buffer;
			int length;
			ValueList() {
				buffer = 0;
				length = 0;
			}
			T operator[](int index) {
				/*length = collection.size();
				if(index < 0) return NULL;
				if(index >= length) return NULL;
				return collection[index];*/
				// This will crash if the index is invalid,
				// however it ensures a proper datatype T.
				return (T)this->collection[index];
			}
			int AddValue(T item) {
				collection.push_back(item);
				length = collection.size();
				return length-1;
			}
			void RemoveAt(int index) {
				collection.erase(index);
				length = collection.size();
			}
			T GetValueAt(int index) {
				return (*this)[index];
			}
			T* ToBuffer() {
				if(buffer == 0) {
					buffer = new T[length];
					for(int n = 0; n < length; n++) {
						buffer[n] = GetValueAt(n);
					}
				}
				return buffer;
			}
		};

	}
}

#endif
