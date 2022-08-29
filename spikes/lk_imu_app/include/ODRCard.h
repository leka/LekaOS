#ifndef _LEKA_OS_SPIKE_ODR_CARD_H_
#define _LEKA_OS_SPIKE_ODR_CARD_H_


#include <chrono>
#include <string>
#include "lsm6dsox_reg.h"

#pragma once

using namespace std::literals::chrono_literals;

namespace leka {

    class ODRCard {

        public:
            ODRCard(auto id_card, std::string name_img, lsm6dsox_odr_xl_t XL_ODR, lsm6dsox_odr_g_t GY_ODR, auto time_odr) {
                this->_id_card                                  = id_card;
                this->_img_path                                 = "/fs/home/img/shapesRecognition/IMU_ODR/" + name_img + ".jpg";
                this->_time_odr                                 = time_odr;

                this->_XL_ODR                                   = XL_ODR; 
                this->_GY_ODR                                   = GY_ODR;
                this->_time_odr                                 = time_odr;
            }

            auto getId() -> uint16_t                            { return this->_id_card; }
            auto getXlODR() -> lsm6dsox_odr_xl_t                { return this->_XL_ODR; }
            auto getGyODR() -> lsm6dsox_odr_g_t                 { return this->_GY_ODR; }
            auto getImgPath() -> std::string                    { return this->_img_path; }
            auto getTimeODR() -> std::chrono::milliseconds      { return this->_time_odr; }
            

		private:
            uint16_t _id_card;
            std::string _img_path;
            lsm6dsox_odr_g_t _GY_ODR;
            lsm6dsox_odr_xl_t _XL_ODR;
            std::chrono::milliseconds _time_odr;

    };

}

void updateODR(auto &videokit, auto &coreimu, std::string img_path, auto XL_ODR, auto GY_ODR, auto &time_odr, auto odr_value) {
	videokit.displayImage(img_path);
	coreimu.setDataRate(XL_ODR, GY_ODR);
	time_odr = odr_value;
}

#endif