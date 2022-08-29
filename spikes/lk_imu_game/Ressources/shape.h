
#ifndef _LEKA_OS_SPIKE_SHAPE_H_
#define _LEKA_OS_SPIKE_SHAPE_H_


#include <cstdint>
#include <array>


#pragma once

namespace leka {

	struct AbstractField {
		virtual char* getNameChar()=0;
		virtual std::string getNameStr() = 0;
		virtual bool isItemInArray(std::uint8_t value)=0;
		virtual int getIndexOfItem(std::uint8_t item)=0;
		virtual std::uint8_t getItemAtIndex(int index)=0;
		virtual size_t getSizeSequence()=0;

		//virtual void getIndexOfId(auto id)=0;
		virtual void setId(int i)=0;
		virtual uint16_t getId() = 0;
		virtual std::string getPicName() = 0;
		virtual char* getPicNameChar() =0;
		//virtual ~AbstractField() = 0;
	};


	template <size_t N>
	class Shape: public AbstractField{
		private:
			std::string name;
			std::array<std::uint8_t, N> sequence;
			size_t sizeSequence;
			std::string pic_name;
			std::uint16_t id_card;

		public:
			Shape(){}
			Shape(std::string name, std::array<std::uint8_t, N> array, std::string pic_name) {
				this->name = name;
				this->sizeSequence = N;
				this->pic_name = pic_name;
				for(size_t i = 0; i < N; ++i)
					this->sequence[i] = array[i];
			}
			Shape(std::string name, uint16_t id, std::array<std::uint8_t, N> array, std::string pic_name) {
				this->name = name;
				this->id_card = id;
				this->pic_name = pic_name;
				this->sizeSequence = N;
				for(size_t i = 0; i < N; ++i)
					this->sequence[i] = array[i];
			}

			std::string getNameStr() {
				return this->name;
			}

			std::string getPicName() {
				return this->pic_name;
			}

			char* getPicNameChar() {
				char* c = const_cast<char*>(this->pic_name.c_str());
				return c;
			}

			char* getNameChar(){
				char* c = const_cast<char*>(this->name.c_str());
				return c;
			}

			std::array<std::uint8_t, N> getSequence(){
				return this->sequence;
			}

			std::uint8_t getItemAtIndex(int index){
				return this->sequence[index];
			}

			bool isItemInArray(std::uint8_t item){
				for(size_t i = 0; i < N; ++i)
					if(item == this->sequence[i])
						return true;
				return false;
			}

			int getIndexOfItem(std::uint8_t item){
				for(size_t i = 0; i < N; ++i)
					if(item == this->sequence[i])
						return i;
				return -1;
			}

			auto getSizeSequence() -> size_t{
				return this->sizeSequence;
			}


			auto getId() -> uint16_t{
				return this->id_card;
			}

			void setId(int i){
				this->id_card = i;
			}

	};



	class Interrupt {
		private:
			std::string name;
			std::uint8_t value;
			std::string name_image;

		public:
			Interrupt(std::string name, std::uint8_t value, std::string name_image) {
				this->name = name;
				this->value = value;
				this->name_image = name_image;
			}

			char* getNameChar(){
				char* c = const_cast<char*>(this->name.c_str());
				return c;
			}

			std::uint8_t getValue() {
				return this->value;
			}

			std::string getImageName(){
				return this->name_image;
			}

	};

}

#endif
