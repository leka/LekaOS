#ifndef __CORE_CM7_H__
#define __CORE_CM7_H__

#ifdef __cplusplus
	#define __I volatile /*!< Defines 'read only' permissions */
#else
	#define __I volatile const /*!< Defines 'read only' permissions */
#endif
#define __O	 volatile /*!< Defines 'write only' permissions */
#define __IO volatile /*!< Defines 'read / write' permissions */

#endif
