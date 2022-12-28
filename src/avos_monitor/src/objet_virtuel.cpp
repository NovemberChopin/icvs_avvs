///
///Classe dérivée de QGraphicsItem. Va servir de classe mère pour tous les objets du dashboardE.lle contient un float Value, et un float valueMax, tous deux en variables
/// protégées
///
///
#include "objet_virtuel.h"

objet_virtuel::objet_virtuel(QGraphicsItem *parent):QGraphicsItem(parent)
{
    for (int i = 0; i < 8; ++i)
    {
        utl_status[i] = 0;
    }
}
///
/// \brief Fonction renvoyant la variable value
/// \return
///
float objet_virtuel::getValue() const
{
	return value;
}

///
/// \brief objet_virtuel::setValue. Permet de modifier la valeur de la variable value
/// \param set_value valeur à attribuer à value
///
void objet_virtuel::setValue(float set_value)
{
	value = set_value;
}
///
/// \brief objet_virtuel::setValue. Permet de modifier la valeur de la variable value
/// \param set_value valeur à attribuer à value
///
void objet_virtuel::setValue(BOARD_SET_TYPE type,float set_value)
{
    switch(type)
    {
        case SIGNAL:
        {
            signal_value = set_value;
        }
        break;
        case LOCATION_STATUS:
        {
            location_status = set_value;
        }
        break;
        case BRAKE_LIGHT:
        {
            braking_value = set_value;
        }   
        break; 
        case BATTERY_VALUE:
        {
            battery_value = set_value;         
        }
        break;
        case VEHICLE_STATUS:
        {
            vehicle_status = set_value;
        }
        break;
        case DOOR_STATUS:
        {
            door_status = set_value;
        }
        break;
        case OVER_TEMP:
        {
             over_temperature = set_value;
        }
        break;
        case BELT_STATUS:
        {
            belt_status = set_value;
        }
        break;
        case LIGHT_STATUS:
        {
            light_status = set_value;
        }
        break;
        case LIDAR_STATUS:
        {
            lidar_status = set_value;
        }
        break;
        case AEB_ENABLE:
        {
            aeb_enable = set_value;
        }
        break;
        case DRIVING_MILEAGE:
        {
            driving_mileage = set_value;
        }
        break;
        case TOTAL_DISTANCE:
        {
            total_distance = set_value;
        }
        break;
        case ON_LINE_STATUS:
        {
            on_line_status = set_value;
        }
        break;
        default:
        {
            value = set_value;
        }
        break;
    }
}

///
/// \brief Fonction renvoyant la variable value
/// \return
///
float objet_virtuel::getValue(BOARD_SET_TYPE type) const
{
    float d_value = 0;

    switch(type)
    {
        case SIGNAL:
        {
        	d_value = signal_value;
        }
        break;
        case LOCATION_STATUS:
        {
        	d_value = location_status;
        }
        break;
        case BRAKE_LIGHT:
        {
        	d_value = braking_value;
        }   
        break; 
        case BATTERY_VALUE:
        {
        	d_value = battery_value;         
        }
        break;
        case VEHICLE_STATUS:
        {
        	d_value = vehicle_status;
        }
        break;
        case DOOR_STATUS:
        {
                d_value = door_status;
        }
        break;
        case OVER_TEMP:
        {
                d_value = over_temperature;
        }
        break;
        case BELT_STATUS:
        {
                d_value = belt_status;
        }
        break;
        case LIGHT_STATUS:
        {
                d_value = light_status;
        }
        break;
        case LIDAR_STATUS:
        {
                d_value = lidar_status;
        }
        break;
        case AEB_ENABLE:
        {
                d_value = aeb_enable;
        }
        break;      
        case DRIVING_MILEAGE:
        {
                d_value = driving_mileage;
        }
        break;
        case TOTAL_DISTANCE:
        {
                d_value = total_distance;
        }
        break;         
        case ON_LINE_STATUS:
        {
            d_value = on_line_status;
        }
        break; 
        default:
        {
                d_value = value;
        }
        break;
    }
    return d_value;
}

int objet_virtuel::getValue(BOARD_SET_TYPE type, int index) const
{
    int value = 0;
    switch(type)
    {
        case ULTRASONIC_STATUS:
        {
            if (index < 8)
            {
                value = utl_status[index];
            }
        }
        break;
        default:
        break;
    }
    return value;
}

void objet_virtuel::setValue(BOARD_SET_TYPE type, int index, int value)
{
    switch(type)
    {        
        case ULTRASONIC_STATUS:
        {
            if (index < 8)
            {
                utl_status[index] = value;;
            }
        }
        break;
        default:
        break;
    }
}
///
/// \brief objet_virtuel::getValueMax Fonction renvoyant la valeur de valueMax
/// \return
///
int objet_virtuel::getValueMax() const
{
	return valueMax;
}
