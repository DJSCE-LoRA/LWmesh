/*
Copyright 2020 Samuel Ramrajkar

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at*/

    //   http://www.apache.org/licenses/LICENSE-2.0

   /*Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "mcc.h"
#include "crc.h"

uint16_t crc16_app(uint8_t* dptr, uint16_t len, uint16_t seed){
    uint8_t* ptr = dptr;
    uint16_t result;
    //Reset the CRC engine
    CRCACCL = seed;
    CRCACCH = seed >> 8;

    CRCCON0bits.EN = 1;
    CRCCON0bits.CRCGO = 1;
    
    for(uint16_t i = 0; i < len; i++ )
    {
        while(1 == CRCCON0bits.FULL){} //Wait if the module is busy
        CRC_8BitDataWrite(*ptr);  
        ptr++;
    }
    //Now wait for the result to be computed
    while(1 == CRCCON0bits.BUSY){} //Wait if the module is busy
    result = CRC_CalculatedResultGet(0,0);
    CRCCON0bits.CRCGO = 0;
    CRCCON0bits.EN = 0;
    return(result);
}
