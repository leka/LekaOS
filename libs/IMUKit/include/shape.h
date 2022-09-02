// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_SHAPE_H_
#define _LEKA_OS_SPIKE_SHAPE_H_



#include "LogKit.h"


#pragma once


typedef typename std::uint8_t TreeOutput;
typedef typename std::uint16_t IDCard;
namespace leka {


	struct AbstractField {

		virtual auto getNameChar() -> char*;
		virtual auto getImageName() -> std::string;
		virtual auto isItemInArray(std::uint8_t value) -> bool;
		virtual auto getIndexOfItem(std::uint8_t item) -> int;
		virtual auto getItemAtIndex(int index) -> std::uint8_t;
		virtual auto getSizeSequence() -> size_t;
		virtual auto getNameStr() -> std::string;
		virtual void setId(int i);
		virtual auto getId() -> std::uint16_t;
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

			Shape(std::string name, std::uint16_t id, std::array<TreeOutput, N> array, std::string name_image) {
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
			auto getId() -> uint16_t							{ return this->_id_card; }
			void setId(int i)									{ this->_id_card = i; }

			
		private:
			std::string _name;
			std::array<std::uint8_t, N> _sequence;
			size_t _size_sequence;
			std::string _name_image;
			std::uint16_t _id_card;

	};



	class OutputClass {

		public:
			OutputClass(std::string name, TreeOutput value, std::string name_image) {
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
			TreeOutput _value;
			std::string _name_image;

	};

}

#endif
