
#ifndef _LEKA_OS_SPIKE_SHAPE_H_
#define _LEKA_OS_SPIKE_SHAPE_H_

#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"
#include <filesystem>
#include "LogKit.h"

#pragma once

namespace leka {


	struct AbstractField {

		virtual char* getNameChar();
		virtual std::string getImageName();
		virtual bool isItemInArray(std::uint8_t value);
		virtual int getIndexOfItem(std::uint8_t item);
		virtual std::uint8_t getItemAtIndex(int index);
		virtual size_t getSizeSequence();

		//virtual ~AbstractField() = 0;
	};


	template <size_t N>
	class Shape: public AbstractField{
			
		public:
			Shape(){}
			Shape(std::string name, std::array<std::uint8_t, N> array, std::string name_image) {
				this->_name 					= name;
				this->_name_image 				= name_image;
				this->_size_sequence 			= N;
				for(auto i = 0; i < N; ++i)
					this->_sequence[i] 			= array[i];
			}

			Shape(std::string name, uint16_t id, std::array<std::uint8_t, N> array, std::string name_image) {
				this->_name 					= name;
				this->_id_card 					= id;
				this->_name_image 				= name_image;
				this->_size_sequence 			= N;
				for(size_t i = 0; i < N; ++i)
					this->_sequence[i] 			= array[i];
			}


			char* getNameChar(){
				char* c = const_cast<char*>(this->_name.c_str());
				return c;
			}
			bool isItemInArray(std::uint8_t item){
				for(size_t i = 0; i < N; ++i)
					if(item == this->_sequence[i])
						return true;
				return false;
			}

			int getIndexOfItem(std::uint8_t item){
				for(size_t i = 0; i < N; ++i)
					if(item == this->_sequence[i])
						return i;
				return -1;
			}

			auto getNameStr() -> std::string 					{ return this->_name; }
			auto getImageName()-> std::string 					{ return this->_name_image; }
			auto getSequence() -> std::array<std::uint8_t, N>	{ return this->_sequence; }
			auto getItemAtIndex(int index) -> std::uint8_t		{ return this->_sequence[index];}
			auto getSizeSequence() -> size_t					{ return this->_size_sequence; }


		private:
			std::string _name;
			std::array<std::uint8_t, N> _sequence;
			size_t _size_sequence;
			std::string _name_image;
			std::uint16_t _id_card;

	};

	class OutputClass {

		public:
			OutputClass(std::string name, std::uint8_t value, std::string name_image) {
				this->_name 						= name;
				this->_value 						= value;
				this->_name_image 					= name_image;
			}

			auto getNameChar() -> char* {
				char* c = const_cast<char*>(this->_name.c_str());
				return c;
			}

			auto getValue() -> std::uint8_t			{ return this->_value; }
			auto getImageName()	-> std::string 		{ return this->_name_image;}

		private:
			std::string _name;
			std::uint8_t _value;
			std::string _name_image;

	};

}

#endif
