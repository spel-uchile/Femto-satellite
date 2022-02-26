/**
 * @brief Simple Temperature Library
 */

/*Author: Matias Vidal*/

/*Requiered Libraries*/
#include <Arduino.h>

/**
 * @class TEMP
 * @brief Class for managing the Temperature data
 */

 class TEMP {
    /*Private Members*/

    public:
        /*Public Members*/
        
        // Default constructor
        TEMP(void);
        
        // Overload constructor
        TEMP(uint8_t addr);

        // Mutator functions
        void init(void);
        uint16_t read16(uint8_t reg);
        float updateData(void);
        bool checkValidity(void);

    private:
        uint8_t address;
        float two_el_to_minus4 = pow(2,-4);
 };
